// Self-sustaining-core attack: q(w), danger profiles, forced runs,
// suffix k-mer graphs, Up-and-Down period blocks, random-walk survival.
// Does NOT rediscover unique-vote / B(m) / Krylov. Uses unique-vote only
// as the already-proved O(n) way to compute F_w.
#include "acf.hpp"
#include "search.hpp"
#include <algorithm>
#include <cmath>
#include <fstream>
#include <functional>
#include <iomanip>
#include <iostream>
#include <map>
#include <random>
#include <set>
#include <sstream>
#include <unordered_map>
#include <unordered_set>
using namespace acf;

static int vote_x(const std::vector<i64>& S, int n, int d) {
    // n = |w|, hypothetical append makes N=n+1.
    const int N = n + 1;
    const int i1 = N - 3 * d, i2 = N - 2 * d, i3 = N - d;
    if (i1 < 0) return -99;
    i64 s1 = S[i2] - S[i1];
    i64 s2 = S[i3] - S[i2];
    if (s1 != s2) return -99;  // inactive
    i64 last = S[n] - S[i3];
    i64 x = s1 - last;
    if (x < 0 || x > 3) return -1;  // vote off alphabet
    return (int)x;
}

static int forbidden_mask(const std::vector<i64>& S, int n) {
    int mask = 0;
    int dmax = (n + 1) / 3;
    for (int d = 1; d <= dmax; ++d) {
        int x = vote_x(S, n, d);
        if (x >= 0) mask |= (1 << x);
    }
    return mask;
}

static int pop4(int m) {
    return (m & 1) + ((m >> 1) & 1) + ((m >> 2) & 1) + ((m >> 3) & 1);
}

static int q_of(const std::vector<i64>& S, int n) { return 4 - pop4(forbidden_mask(S, n)); }

static std::vector<u8> load_word(const char* path) {
    std::ifstream in(path);
    if (!in) {
        std::cerr << "cannot open " << path << "\n";
        std::exit(1);
    }
    std::string s((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
    return parse_digits(s);
}

static void mode_word(const std::vector<u8>& w, int print_every) {
    const int N = (int)w.size();
    std::vector<i64> S(N + 1, 0);
    for (int i = 0; i < N; ++i) S[i + 1] = S[i] + w[i];
    uint64_t qhist[5] = {};
    int qmin_after20 = 4, qmin_pos = -1;
    int forced_run = 0, max_forced = 0, n_forced_pos = 0;
    int n_q0 = 0, first_q0 = -1, first_q1 = -1;
    int n_active_sum = 0, n_cov_sum = 0;
    int max_active = 0;
    std::map<int, uint64_t> run_hist;
    // profile: 16 bins of d/n, vote letter 0-3 or none
    std::unordered_map<uint64_t, int> prof_first;
    int n_prof_repeat = 0, first_prof_rep = -1, first_prof_gap = -1;
    for (int n = 1; n <= N; ++n) {
        int mask = 0, act = 0;
        int dmax = (n + 1) / 3;
        for (int d = 1; d <= dmax; ++d) {
            int x = vote_x(S, n, d);
            if (x == -99) continue;
            ++act;
            if (x >= 0) mask |= (1 << x);
        }
        int q = 4 - pop4(mask);
        qhist[q]++;
        if (n >= 20 && q < qmin_after20) {
            qmin_after20 = q;
            qmin_pos = n;
        }
        if (q == 0) {
            ++n_q0;
            if (first_q0 < 0) first_q0 = n;
        }
        if (q == 1 && first_q1 < 0) first_q1 = n;
        if (q == 1) {
            ++forced_run;
            ++n_forced_pos;
            if (forced_run > max_forced) max_forced = forced_run;
        } else {
            if (forced_run) run_hist[forced_run]++;
            forced_run = 0;
        }
        n_active_sum += act;
        n_cov_sum += pop4(mask);
        if (act > max_active) max_active = act;

        // 16-bin profile signature
        uint64_t sig = (uint64_t)(mask & 15);
        int bins = 16;
        for (int b = 0; b < bins; ++b) {
            int lo = 1 + (b * dmax) / bins;
            int hi = (b + 1) * dmax / bins;
            int bm = 0;
            for (int d = lo; d <= hi && d >= 1; ++d) {
                int x = vote_x(S, n, d);
                if (x >= 0) bm |= (1 << x);
            }
            sig = sig * 17u + (uint64_t)bm;
        }
        auto it = prof_first.find(sig);
        if (it == prof_first.end()) prof_first[sig] = n;
        else {
            ++n_prof_repeat;
            if (first_prof_rep < 0) {
                first_prof_rep = n;
                first_prof_gap = n - it->second;
            }
        }
        if (print_every > 0 && n % print_every == 0) {
            std::cout << "at n=" << n << " q=" << q << " act=" << act << " mask=" << mask << "\n";
        }
    }
    if (forced_run) run_hist[forced_run]++;
    std::cout << "N " << N << "\n";
    std::cout << "qhist";
    for (int q = 0; q <= 4; ++q) std::cout << " " << q << ":" << qhist[q];
    std::cout << "\n";
    std::cout << "qmin_after20 " << qmin_after20 << " at " << qmin_pos << "\n";
    std::cout << "n_q0 " << n_q0 << " first_q0 " << first_q0 << "\n";
    std::cout << "first_q1 " << first_q1 << " n_forced_pos " << n_forced_pos << " max_forced_run " << max_forced << "\n";
    std::cout << "mean_active " << (double)n_active_sum / N << " mean_cov " << (double)n_cov_sum / N
              << " max_active " << max_active << "\n";
    std::cout << "n_profile_sigs " << prof_first.size() << " n_prof_repeat " << n_prof_repeat
              << " first_rep " << first_prof_rep << " gap " << first_prof_gap << "\n";
    std::cout << "forced_run_hist";
    for (auto& kv : run_hist) std::cout << " " << kv.first << ":" << kv.second;
    std::cout << "\n";
}

static void mode_trie(int nmax) {
    // Exact q histogram of all ACF words of each length <= nmax.
    std::vector<std::array<uint64_t, 5>> H(nmax + 1);
    std::vector<uint64_t> tot(nmax + 1, 0);
    uint64_t n_q1_words = 0;
    int min_n_q1 = -1;
    std::string ex_q1;
    Builder b;
    std::function<void()> rec = [&]() {
        int n = b.size();
        if (n > 0) {
            int q = 0;
            for (int a = 0; a < 4; ++a)
                if (b.try_push((u8)a)) {
                    ++q;
                    b.pop();
                }
            H[n][q]++;
            tot[n]++;
            if (q == 1) {
                ++n_q1_words;
                if (min_n_q1 < 0) {
                    min_n_q1 = n;
                    ex_q1 = to_string(b.w);
                }
            }
        }
        if (n == nmax) return;
        for (int a = 0; a < 4; ++a)
            if (b.try_push((u8)a)) {
                rec();
                b.pop();
            }
    };
    rec();
    std::cout << "# n tot q0 q1 q2 q3 q4\n";
    for (int n = 1; n <= nmax; ++n) {
        std::cout << n << " " << tot[n];
        for (int q = 0; q <= 4; ++q) std::cout << " " << H[n][q];
        std::cout << "\n";
    }
    std::cout << "min_n_q1 " << min_n_q1 << " n_q1_seen " << n_q1_words << " ex_q1 " << ex_q1 << "\n";
}

static int two_branch_R(Builder& b, int R) {
    // Return min r in 1..R such that >=2 ACF descendants of length |w|+r exist, else 0.
    // BFS by layers of extension strings.
    const int n0 = b.size();
    std::vector<std::vector<u8>> layer;
    layer.push_back({});
    for (int r = 1; r <= R; ++r) {
        std::vector<std::vector<u8>> nxt;
        for (auto& pref : layer) {
            for (u8 a : pref) {
                bool ok = b.try_push(a);
                (void)ok;
            }
            for (int a = 0; a < 4; ++a) {
                if (b.try_push((u8)a)) {
                    auto p = pref;
                    p.push_back((u8)a);
                    nxt.push_back(p);
                    b.pop();
                }
            }
            for (int i = 0; i < (int)pref.size(); ++i) b.pop();
        }
        if ((int)nxt.size() >= 2) return r;
        if (nxt.empty()) return -1;  // dead
        layer.swap(nxt);
        if ((int)layer.size() > 256) {
            layer.resize(256);  // cap
        }
    }
    (void)n0;
    return 0;
}

static void mode_branch(int n, int R, int samples) {
    // All ACF n-mers if small, else first `samples` in lex DFS.
    Builder b;
    int seen = 0, n_ok = 0, n_dead = 0, n_fail = 0;
    int rhist[64] = {};
    int worst_r = 0;
    std::string worst;
    std::function<void()> rec = [&]() {
        if (b.size() == n) {
            if (samples > 0 && seen >= samples) return;
            ++seen;
            int r = two_branch_R(b, R);
            if (r > 0) {
                ++n_ok;
                if (r < 64) rhist[r]++;
                if (r > worst_r) {
                    worst_r = r;
                    worst = to_string(b.w);
                }
            } else if (r < 0)
                ++n_dead;
            else
                ++n_fail;
            return;
        }
        for (int a = 0; a < 4; ++a) {
            if (samples > 0 && seen >= samples) return;
            if (b.try_push((u8)a)) {
                rec();
                b.pop();
            }
        }
    };
    rec();
    std::cout << "n " << n << " R " << R << " seen " << seen << " two_branch " << n_ok << " dead_before_R "
              << n_dead << " no_two_in_R " << n_fail << "\n";
    std::cout << "rhist";
    for (int r = 1; r <= R && r < 64; ++r)
        if (rhist[r]) std::cout << " " << r << ":" << rhist[r];
    std::cout << "\n";
    std::cout << "worst_r " << worst_r << " worst " << worst << "\n";
}

static void mode_random(int depth, int trials, uint64_t seed) {
    std::mt19937_64 rng(seed);
    uint64_t survive = 0;
    uint64_t qsum = 0, nq = 0;
    int maxd = 0;
    std::array<uint64_t, 5> qh{};
    for (int t = 0; t < trials; ++t) {
        Builder b;
        bool live = true;
        for (int n = 0; n < depth && live; ++n) {
            std::vector<u8> opts;
            for (int a = 0; a < 4; ++a)
                if (b.try_push((u8)a)) {
                    opts.push_back((u8)a);
                    b.pop();
                }
            qh[opts.size()]++;
            qsum += opts.size();
            ++nq;
            if (opts.empty()) {
                live = false;
                maxd = std::max(maxd, b.size());
                break;
            }
            std::uniform_int_distribution<int> dist(0, (int)opts.size() - 1);
            b.try_push(opts[dist(rng)]);
        }
        if (live) {
            ++survive;
            maxd = std::max(maxd, b.size());
        }
    }
    std::cout << "depth " << depth << " trials " << trials << " survive " << survive
              << " p=" << (double)survive / trials << " max_depth_seen " << maxd << "\n";
    std::cout << "mean_q " << (double)qsum / nq << " qhist";
    for (int q = 0; q <= 4; ++q) std::cout << " " << q << ":" << qh[q];
    std::cout << "\n";
}

static uint64_t packk(const std::vector<u8>& w, int i, int k) {
    uint64_t x = 0;
    for (int t = 0; t < k; ++t) x = (x << 2) | w[i + t];
    return x;
}

static void mode_suffix(const std::vector<u8>& w, int k) {
    const int N = (int)w.size();
    if (N <= k + 2) return;
    std::unordered_map<uint64_t, std::array<int, 4>> trans;
    std::unordered_map<uint64_t, int> first_pos;
    int n_conflict = 0;  // same k-mer, two different next letters in the word
    for (int i = 0; i + k < N; ++i) {
        uint64_t p = packk(w, i, k);
        int a = w[i + k];
        auto& row = trans[p];
        if (first_pos.find(p) == first_pos.end()) first_pos[p] = i;
        if (row[a] == 0) {
            // check if another letter already used
            int others = 0;
            for (int b = 0; b < 4; ++b)
                if (b != a && row[b]) ++others;
            if (others) ++n_conflict;
        }
        row[a]++;
    }
    int n_det = 0, n_branch = 0, n_states = (int)trans.size();
    int max_outdeg = 0;
    for (auto& kv : trans) {
        int od = 0;
        for (int a = 0; a < 4; ++a)
            if (kv.second[a]) ++od;
        if (od == 1) ++n_det;
        if (od >= 2) ++n_branch;
        if (od > max_outdeg) max_outdeg = od;
    }
    std::cout << "k " << k << " N " << N << " states " << n_states << " det " << n_det << " branch " << n_branch
              << " max_outdeg " << max_outdeg << " positions_with_prior_other_letter " << n_conflict << "\n";

    // Falsify: take a branching k-mer, follow an unused-in-original next letter if ACF locally,
    // see if a short continuation cubes. Sample up to 200 branching states.
    int n_alt_cube = 0, n_alt_ok = 0, tried = 0;
    for (auto& kv : trans) {
        if (tried >= 200) break;
        int od = 0;
        for (int a = 0; a < 4; ++a)
            if (kv.second[a]) ++od;
        if (od < 2) continue;
        int i0 = first_pos[kv.first];
        ++tried;
        // reconstruct prefix w[0..i0+k)
        Builder B;
        bool okp = true;
        for (int j = 0; j < i0 + k; ++j)
            if (!B.try_push(w[j])) {
                okp = false;
                break;
            }
        if (!okp) continue;
        for (int a = 0; a < 4; ++a) {
            if (kv.second[a]) continue;  // skip observed
            if (B.try_push((u8)a)) {
                ++n_alt_ok;
                B.pop();
            } else {
                ++n_alt_cube;
            }
        }
    }
    std::cout << "branch_states_tried " << tried << " unobserved_next_acf " << n_alt_ok
              << " unobserved_next_cube " << n_alt_cube << "\n";
}

static void mode_updown(const std::vector<u8>& w, int period) {
    const int N = (int)w.size();
    int nb = N / period;
    std::cout << "period " << period << " N " << N << " nblocks " << nb << "\n";
    if (nb < 3) return;
    std::map<std::array<int, 4>, int> parikh_types;
    int n_equal_adj = 0, n_rev_adj = 0;
    std::vector<std::array<int, 4>> P(nb);
    std::vector<i64> sum(nb, 0);
    for (int b = 0; b < nb; ++b) {
        std::array<int, 4> c{{0, 0, 0, 0}};
        i64 s = 0;
        for (int t = 0; t < period; ++t) {
            u8 a = w[b * period + t];
            c[a]++;
            s += a;
        }
        P[b] = c;
        sum[b] = s;
        parikh_types[c]++;
        if (b > 0) {
            if (P[b] == P[b - 1]) ++n_equal_adj;
            bool rev = true;
            for (int t = 0; t < period; ++t)
                if (w[b * period + t] != w[b * period - 1 - t]) {
                    rev = false;
                    break;
                }
            if (rev) ++n_rev_adj;
        }
    }
    std::cout << "distinct_parikh " << parikh_types.size() << " equal_adj " << n_equal_adj << " exact_rev_adj "
              << n_rev_adj << "\n";
    std::cout << "block_sums";
    for (int b = 0; b < std::min(nb, 12); ++b) std::cout << " " << sum[b];
    std::cout << "\n";
    // Is block b+1 a function of block b as a string? (deterministic substitution)
    std::unordered_map<std::string, std::unordered_set<std::string>> nxt;
    for (int b = 0; b + 1 < nb; ++b) {
        std::string u(period, '0'), v(period, '0');
        for (int t = 0; t < period; ++t) {
            u[t] = char('0' + w[b * period + t]);
            v[t] = char('0' + w[(b + 1) * period + t]);
        }
        nxt[u].insert(v);
    }
    int det = 0, nondet = 0, maximg = 0;
    for (auto& kv : nxt) {
        int s = (int)kv.second.size();
        if (s == 1) ++det;
        else ++nondet;
        if (s > maximg) maximg = s;
    }
    std::cout << "block_map_states " << nxt.size() << " det " << det << " nondet " << nondet << " max_images "
              << maximg << "\n";
}

static void mode_find_q1(int nmax, uint64_t budget) {
    // Chronological search for a word with q=1 at each n, and q=0 for n>14.
    Builder b;
    std::vector<int> found_q1(nmax + 1, 0);
    std::vector<int> found_q0(nmax + 1, 0);
    std::vector<std::string> ex1(nmax + 1), ex0(nmax + 1);
    uint64_t nodes = 0;
    std::function<void()> rec = [&]() {
        if (nodes++ > budget) return;
        int n = b.size();
        if (n > nmax) return;
        if (n >= 1) {
            int q = 0;
            for (int a = 0; a < 4; ++a)
                if (b.try_push((u8)a)) {
                    ++q;
                    b.pop();
                }
            if (q == 1 && !found_q1[n]) {
                found_q1[n] = 1;
                ex1[n] = to_string(b.w);
            }
            if (q == 0 && !found_q0[n]) {
                found_q0[n] = 1;
                ex0[n] = to_string(b.w);
            }
        }
        bool all = true;
        for (int n2 = 14; n2 <= nmax; ++n2)
            if (!found_q1[n2] && !found_q0[n2]) all = false;
        if (all && n >= 14) return;
        for (int a = 0; a < 4; ++a) {
            if (nodes > budget) return;
            if (b.try_push((u8)a)) {
                rec();
                b.pop();
            }
        }
    };
    rec();
    std::cout << "budget " << budget << " nodes " << nodes << "\n";
    std::cout << "q1_lengths";
    for (int n = 1; n <= nmax; ++n)
        if (found_q1[n]) std::cout << " " << n;
    std::cout << "\n";
    std::cout << "q0_lengths";
    for (int n = 1; n <= nmax; ++n)
        if (found_q0[n]) std::cout << " " << n;
    std::cout << "\n";
    for (int n = 1; n <= nmax; ++n) {
        if (found_q1[n]) std::cout << "ex1 n=" << n << " " << ex1[n] << "\n";
        if (found_q0[n]) std::cout << "ex0 n=" << n << " " << ex0[n] << "\n";
    }
}

static void mode_inject(int n) {
    // For every ACF n-mer, count ACF extensions wa with q(wa)>=2.
    Builder b;
    uint64_t words = 0, with2 = 0, with1 = 0, with0 = 0;
    int min_good = 99;
    std::string worst;
    std::function<void()> rec = [&]() {
        if (b.size() == n) {
            ++words;
            int good = 0;
            for (int a = 0; a < 4; ++a) {
                if (!b.try_push((u8)a)) continue;
                int q = 0;
                for (int c = 0; c < 4; ++c)
                    if (b.try_push((u8)c)) {
                        ++q;
                        b.pop();
                    }
                if (q >= 2) ++good;
                b.pop();
            }
            if (good >= 2) ++with2;
            else if (good == 1) ++with1;
            else ++with0;
            if (good < min_good) {
                min_good = good;
                worst = to_string(b.w);
            }
            return;
        }
        for (int a = 0; a < 4; ++a)
            if (b.try_push((u8)a)) {
                rec();
                b.pop();
            }
    };
    rec();
    std::cout << "inject n=" << n << " words " << words << " ge2_good_ext " << with2 << " eq1 " << with1
              << " eq0 " << with0 << " min_good " << min_good << " worst " << worst << "\n";
}

static std::vector<u8> cassaigne_native(int n) {
    std::array<std::vector<u8>, 5> h;
    h[0] = {0, 3};
    h[1] = {4, 3};
    h[3] = {1};
    h[4] = {0, 1};
    std::vector<u8> w = {0};
    while ((int)w.size() < n) {
        std::vector<u8> nxt;
        for (u8 a : w) {
            const auto& img = h[a];
            nxt.insert(nxt.end(), img.begin(), img.end());
        }
        if (nxt.size() <= w.size()) break;
        w.swap(nxt);
    }
    if ((int)w.size() > n) w.resize(n);
    return w;
}

// q on a general 4-letter integer alphabet (not necessarily {0,1,2,3}).
static int q_alphabet(const std::vector<i64>& S, int n, const std::array<int, 4>& alph) {
    int mask = 0;  // bit i = alph[i] forbidden
    int dmax = (n + 1) / 3;
    for (int d = 1; d <= dmax; ++d) {
        const int N = n + 1;
        const int i1 = N - 3 * d, i2 = N - 2 * d, i3 = N - d;
        if (i1 < 0) continue;
        i64 s1 = S[i2] - S[i1];
        i64 s2 = S[i3] - S[i2];
        if (s1 != s2) continue;
        i64 last = S[n] - S[i3];
        i64 x = s1 - last;
        for (int i = 0; i < 4; ++i)
            if (x == alph[i]) mask |= (1 << i);
    }
    return 4 - pop4(mask);
}

static void mode_cass(int N) {
    auto w = cassaigne_native(N);
    const int n = (int)w.size();
    std::vector<i64> S(n + 1, 0);
    for (int i = 0; i < n; ++i) S[i + 1] = S[i] + w[i];
    uint64_t qhist[5] = {};
    int first_q1 = -1, first_q0 = -1, max_forced = 0, run = 0, n_forced = 0;
    std::array<int, 4> alph{{0, 1, 3, 4}};
    for (int k = 1; k <= n; ++k) {
        int q = q_alphabet(S, k, alph);
        if (q > 4) q = 4;
        if (q < 0) q = 0;
        qhist[q]++;
        if (q == 0 && first_q0 < 0) first_q0 = k;
        if (q == 1 && first_q1 < 0) first_q1 = k;
        if (q == 1) {
            ++run;
            ++n_forced;
            if (run > max_forced) max_forced = run;
        } else
            run = 0;
    }
    std::cout << "cassaigne_native N " << n << " ACF " << (is_acf(w) ? 1 : 0) << "\n";
    std::cout << "qhist";
    for (int q = 0; q <= 4; ++q) std::cout << " " << q << ":" << qhist[q];
    std::cout << "\n";
    std::cout << "first_q0 " << first_q0 << " first_q1 " << first_q1 << " n_forced " << n_forced
              << " max_forced_run " << max_forced << "\n";
}

static void mode_walk(const std::vector<u8>& w) {
    const int N = (int)w.size();
    std::vector<i64> S(N + 1, 0);
    for (int i = 0; i < N; ++i) S[i + 1] = S[i] + w[i];
    i64 mn = 0, mx = 0;
    for (int i = 0; i <= N; ++i) {
        mn = std::min(mn, S[i]);
        mx = std::max(mx, S[i]);
    }
    // Equal-time 4-APs = additive cubes. Count them (should be 0 if ACF).
    uint64_t n_eqtime = 0;
    std::optional<Cube> c = find_cube(w);
    // 4-term APs in the SET of heights, ignoring time: S(a),S(b),S(c),S(d) in AP
    // with a<b<c<d not necessarily equal gaps. Sample via height multiplicity.
    std::map<i64, int> occ;
    for (int i = 0; i <= N; ++i) occ[S[i]]++;
    int n_heights = (int)occ.size();
    int max_mult = 0;
    for (auto& kv : occ)
        if (kv.second > max_mult) max_mult = kv.second;
    // Return-time histogram for a few popular heights.
    std::cout << "N " << N << " Srange [" << mn << "," << mx << "] n_heights " << n_heights
              << " max_height_mult " << max_mult << " mean " << (double)S[N] / N << "\n";
    std::cout << "equal_time_cube " << (c ? 1 : 0);
    if (c) std::cout << " i=" << c->i << " d=" << c->d << " sum=" << c->sum;
    std::cout << "\n";
    (void)n_eqtime;
    // Local slopes: S(i+k)-S(i) for k=8,32,128 at every 128th position.
    for (int k : {8, 32, 128, 512}) {
        i64 sl_min = 1e18, sl_max = -1e18;
        int cnt = 0;
        for (int i = 0; i + k <= N; i += std::max(1, k / 4)) {
            i64 sl = S[i + k] - S[i];
            sl_min = std::min(sl_min, sl);
            sl_max = std::max(sl_max, sl);
            ++cnt;
        }
        std::cout << "slope_window " << k << " min " << sl_min << " max " << sl_max << " samples " << cnt
                  << "\n";
    }
}

static std::vector<u8> reversed(const std::vector<u8>& w) {
    auto r = w;
    std::reverse(r.begin(), r.end());
    return r;
}

static void mode_recgen(int cap) {
    // Seeds: all ACF words of length 2,3,4.
    std::vector<std::vector<u8>> seeds;
    Builder b;
    std::function<void()> rec = [&]() {
        if (b.size() >= 2) seeds.push_back(b.w);
        if (b.size() == 4) return;
        for (int a = 0; a < 4; ++a)
            if (b.try_push((u8)a)) {
                rec();
                b.pop();
            }
    };
    rec();
    int n_ww = 0, n_wxw = 0, n_wxrev = 0, n_wrevxw = 0;
    int best_wxrev = 0;
    std::string best_name;
    for (auto& seed : seeds) {
        // W W  (no separator)
        {
            auto w = seed;
            bool ok = true;
            int rounds = 0;
            for (; rounds < 12 && (int)w.size() < cap; ++rounds) {
                auto nxt = w;
                nxt.insert(nxt.end(), w.begin(), w.end());
                if (!is_acf(nxt)) {
                    ok = false;
                    break;
                }
                w.swap(nxt);
            }
            if (ok) {
                ++n_ww;
                std::cout << "WW_survive seed " << to_string(seed) << " len " << w.size() << "\n";
            }
        }
        // W x W
        for (u8 x = 0; x < 4; ++x) {
            auto w = seed;
            bool ok = true;
            int rounds = 0;
            for (; rounds < 12 && (int)w.size() < cap; ++rounds) {
                auto nxt = w;
                nxt.push_back(x);
                nxt.insert(nxt.end(), w.begin(), w.end());
                if (!is_acf(nxt)) {
                    ok = false;
                    break;
                }
                w.swap(nxt);
            }
            if (ok) {
                ++n_wxw;
                std::cout << "WxW_survive seed " << to_string(seed) << " x " << (int)x << " len " << w.size()
                          << "\n";
            }
        }
        // W x W^R   nested palindrome / paperfold-like
        for (u8 x = 0; x < 4; ++x) {
            auto w = seed;
            bool ok = true;
            int rounds = 0;
            for (; rounds < 16 && (int)w.size() < cap; ++rounds) {
                auto nxt = w;
                nxt.push_back(x);
                auto r = reversed(w);
                nxt.insert(nxt.end(), r.begin(), r.end());
                if (!is_acf(nxt)) {
                    ok = false;
                    break;
                }
                w.swap(nxt);
            }
            if (ok) {
                ++n_wxrev;
                std::cout << "WxWR_survive seed " << to_string(seed) << " x " << (int)x << " len " << w.size()
                          << "\n";
            } else if ((int)w.size() > best_wxrev) {
                best_wxrev = (int)w.size();
                best_name = "WxWR " + to_string(seed) + " x" + std::to_string(x);
            }
        }
        // W W^R x W  (too free; skip x variation on W^R W)
        {
            auto w = seed;
            bool ok = true;
            for (int rounds = 0; rounds < 12 && (int)w.size() < cap; ++rounds) {
                auto nxt = w;
                auto r = reversed(w);
                nxt.insert(nxt.end(), r.begin(), r.end());
                if (!is_acf(nxt)) {
                    ok = false;
                    break;
                }
                w.swap(nxt);
            }
            if (ok) {
                ++n_wrevxw;
                std::cout << "WWR_survive seed " << to_string(seed) << " len " << w.size() << "\n";
            }
        }
    }
    std::cout << "recgen_nseeds " << seeds.size() << " WW " << n_ww << " WxW " << n_wxw << " WxWR " << n_wxrev
              << " WWR " << n_wrevxw << " best_fail_WxWR_len " << best_wxrev << " " << best_name << "\n";
}

static void mode_grow2(int depth, uint64_t budget) {
    // Unfold a 2-ary tree: at q>=2 take the two smallest legal letters as children;
    // at q==1 follow the forced letter (unary); at q==0 die.
    // Reports whether any leaf reaches `depth`, and the first death depth.
    struct Node {
        std::vector<u8> w;
    };
    std::vector<Node> cur;
    cur.push_back(Node{{}});
    uint64_t nodes = 0;
    int first_death = -1, n_death = 0, n_reach = 0;
    int max_unary = 0;
    for (int d = 0; d < depth; ++d) {
        std::vector<Node> nxt;
        for (auto& nd : cur) {
            if (nodes++ > budget) {
                std::cout << "grow2 budget_hit depth " << d << " frontier " << cur.size() << "\n";
                return;
            }
            Builder b;
            bool ok = true;
            for (u8 a : nd.w)
                if (!b.try_push(a)) {
                    ok = false;
                    break;
                }
            if (!ok) continue;
            std::vector<u8> opts;
            for (int a = 0; a < 4; ++a)
                if (b.try_push((u8)a)) {
                    opts.push_back((u8)a);
                    b.pop();
                }
            if (opts.empty()) {
                if (first_death < 0) first_death = (int)nd.w.size();
                ++n_death;
                continue;
            }
            if ((int)opts.size() == 1) {
                auto ch = nd.w;
                ch.push_back(opts[0]);
                nxt.push_back(Node{std::move(ch)});
                ++max_unary;  // count unary steps, not max run
            } else {
                // two smallest
                auto ch0 = nd.w;
                ch0.push_back(opts[0]);
                auto ch1 = nd.w;
                ch1.push_back(opts[1]);
                nxt.push_back(Node{std::move(ch0)});
                nxt.push_back(Node{std::move(ch1)});
            }
        }
        if (nxt.empty()) {
            std::cout << "grow2 extinct at depth " << d << " deaths " << n_death << " first_death "
                      << first_death << "\n";
            return;
        }
        // cap frontier
        if (nxt.size() > 4096) nxt.resize(4096);
        cur.swap(nxt);
    }
    n_reach = (int)cur.size();
    std::cout << "grow2 depth " << depth << " frontier " << n_reach << " deaths_seen " << n_death
              << " first_death " << first_death << " unary_steps " << max_unary << " nodes " << nodes << "\n";
}

static std::vector<u8> legal_letters(Builder& b) {
    std::vector<u8> opts;
    for (int a = 0; a < 4; ++a)
        if (b.try_push((u8)a)) {
            opts.push_back((u8)a);
            b.pop();
        }
    return opts;
}

static double word_mean(const Builder& b) {
    if (b.w.empty()) return 1.5;
    return (double)b.S.back() / (double)b.size();
}

static void dump_word(const char* path, const std::vector<u8>& w) {
    std::ofstream out(path);
    for (u8 a : w) out << char('0' + a);
    out << "\n";
}

static void mode_beam(int depth, int beam, int style) {
    // style 0: two smallest legal letters (unary if q=1)
    // style 1: two letters whose new mean is closest to 1.5
    // Cap keeps nodes closest to mean 1.5 (left-truncation biased toward 0 and death).
    struct Nd {
        Builder b;
    };
    std::vector<Nd> cur;
    cur.emplace_back();
    uint64_t n_death = 0, n_unary = 0;
    int first_death = -1;
    int qmin = 4;
    for (int d = 0; d < depth; ++d) {
        std::vector<Nd> nxt;
        nxt.reserve((size_t)std::min(beam * 2, 1 << 18));
        int layer_qmin = 4, n_q0 = 0, n_q1 = 0, n_q2 = 0;
        i64 sum_S = 0;
        for (auto& nd : cur) {
            auto opts = legal_letters(nd.b);
            int q = (int)opts.size();
            if (q < layer_qmin) layer_qmin = q;
            if (q == 0) {
                ++n_q0;
                ++n_death;
                if (first_death < 0) first_death = nd.b.size();
                continue;
            }
            if (q == 1) ++n_q1;
            if (q >= 2) ++n_q2;
            std::vector<u8> pick;
            if (q == 1)
                pick = {opts[0]};
            else if (style == 0)
                pick = {opts[0], opts[1]};
            else {
                double n = (double)nd.b.size();
                double s = (double)nd.b.S.back();
                std::vector<std::pair<double, u8>> sc;
                for (u8 a : opts) sc.push_back({std::abs((s + a) / (n + 1.0) - 1.5), a});
                std::sort(sc.begin(), sc.end());
                pick.push_back(sc[0].second);
                pick.push_back(sc[1].second);
            }
            if (pick.size() == 1) ++n_unary;
            for (size_t pi = 0; pi < pick.size(); ++pi) {
                if (pi == 0) {
                    nd.b.try_push(pick[0]);
                    sum_S += nd.b.S.back();
                    nxt.push_back(std::move(nd));
                } else {
                    Nd ch;
                    ch.b.w = nxt.back().b.w;
                    ch.b.S = nxt.back().b.S;
                    ch.b.w.pop_back();
                    ch.b.S.pop_back();
                    ch.b.try_push(pick[pi]);
                    sum_S += ch.b.S.back();
                    nxt.push_back(std::move(ch));
                }
            }
        }
        int nlen = d + 1;
        double mean = nxt.empty() ? 0 : (double)sum_S / (double)(nxt.size() * std::max(1, nlen));
        if (d < 20 || (d + 1) % 20 == 0 || nxt.empty())
            std::cout << "layer " << nlen << " frontier " << nxt.size() << " qmin " << layer_qmin << " q0 " << n_q0
                      << " q1 " << n_q1 << " q2 " << n_q2 << " mean " << mean << "\n";
        if (nxt.empty()) {
            std::cout << "beam EXTINCT at " << nlen << " deaths " << n_death << " first_death " << first_death
                      << "\n";
            return;
        }
        if ((int)nxt.size() > beam) {
            std::vector<size_t> idx(nxt.size());
            for (size_t i = 0; i < idx.size(); ++i) idx[i] = i;
            std::nth_element(idx.begin(), idx.begin() + beam, idx.end(), [&](size_t i, size_t j) {
                return std::abs(word_mean(nxt[i].b) - 1.5) < std::abs(word_mean(nxt[j].b) - 1.5);
            });
            std::vector<Nd> kept;
            kept.reserve((size_t)beam);
            for (int i = 0; i < beam; ++i) kept.push_back(std::move(nxt[idx[i]]));
            nxt.swap(kept);
        }
        cur.swap(nxt);
        if (layer_qmin < qmin && layer_qmin > 0) qmin = layer_qmin;
    }
    std::cout << "beam depth " << depth << " style " << style << " frontier " << cur.size() << " deaths "
              << n_death << " first_death " << first_death << " unary " << n_unary << "\n";
    if (!cur.empty()) {
        dump_word("data/beam_word.txt", cur[0].b.w);
        std::cout << "beam_dump data/beam_word.txt len " << cur[0].b.size() << " mean " << word_mean(cur[0].b)
                  << " prefix20 " << to_string(std::vector<u8>(cur[0].b.w.begin(), cur[0].b.w.begin() + std::min(20, (int)cur[0].b.w.size())))
                  << "\n";
    }
}

static void greedy_extend(Builder& b, int cap, int style) {
    while (b.size() < cap) {
        auto opts = legal_letters(b);
        if (opts.empty()) return;
        u8 a = opts[0];
        if (style == 1 && opts.size() >= 2) {
            double n = (double)b.size();
            double s = (double)b.S.back();
            double best = 1e99;
            for (u8 x : opts) {
                double m = std::abs((s + x) / (n + 1.0) - 1.5);
                if (m < best) {
                    best = m;
                    a = x;
                }
            }
        }
        b.try_push(a);
    }
}

static void mode_mutate(const char* path, int samples, int extra, int stride) {
    // Walk the archive once. At q>=2 positions, copy the prefix, take another
    // legal letter, then balanced-greedy extend by `extra`. Incremental: no
    // O(N^2) rebuild per sample.
    auto w = load_word(path);
    const int N = (int)w.size();
    const int walk_cap = std::min(N, 25000);  // O(n) per letter; 25k is ~3e8 checks
    Builder live;
    int n_try = 0, n_live = 0, n_die = 0;
    int best_alt = 0, best_pos = -1, worst_die = 0, worst_die_pos = -1;
    int next_n = 40;
    for (int n = 0; n < walk_cap && n_try < samples; ++n) {
        if (!live.try_push(w[n])) {
            std::cout << "mutate archive not ACF at " << n + 1 << "\n";
            return;
        }
        int pos = live.size();  // length of prefix, next letter is w[pos] if pos < N
        if (pos < next_n || pos + 1 >= N) continue;
        auto opts = legal_letters(live);
        if ((int)opts.size() < 2) continue;
        u8 orig = w[pos];
        bool orig_legal = false;
        for (u8 a : opts)
            if (a == orig) orig_legal = true;
        if (!orig_legal) continue;
        ++n_try;
        next_n = pos + std::max(1, stride);
        for (u8 a : opts) {
            if (a == orig) continue;
            Builder b;
            b.w = live.w;
            b.S = live.S;
            if (!b.try_push(a)) continue;
            greedy_extend(b, pos + extra, 1);
            int L = b.size();
            if (L >= pos + extra) {
                ++n_live;
                if (L > best_alt) {
                    best_alt = L;
                    best_pos = pos;
                }
            } else {
                ++n_die;
                if (L > worst_die) {
                    worst_die = L;
                    worst_die_pos = pos;
                }
            }
        }
    }
    std::cout << "mutate N " << N << " walked " << walk_cap << " tried_pos " << n_try << " alt_live " << n_live
              << " alt_die " << n_die << " best_alt " << best_alt << " best_pos " << best_pos << " extra "
              << extra << " worst_die " << worst_die << " worst_die_pos " << worst_die_pos << "\n";
}

static void mode_inject_iter(int n0, int rounds, int r) {
    // Start from all ACF n0-mers. Each round: keep words that have >=2 ACF
    // r-letter extensions whose result still has q>=2. Count survivors.
    // This is a finite injection test, not a theorem.
    std::vector<std::vector<u8>> cur;
    {
        Builder b;
        std::function<void()> rec = [&]() {
            if (b.size() == n0) {
                cur.push_back(b.w);
                return;
            }
            for (int a = 0; a < 4; ++a)
                if (b.try_push((u8)a)) {
                    rec();
                    b.pop();
                }
        };
        rec();
    }
    std::cout << "inject_iter n0=" << n0 << " r=" << r << " start " << cur.size() << "\n";
    for (int rd = 1; rd <= rounds; ++rd) {
        std::vector<std::vector<u8>> nxt;
        int n_ge2 = 0, n_1 = 0, n_0 = 0;
        for (auto& w : cur) {
            Builder b;
            for (u8 a : w) b.try_push(a);
            // DFS r-extensions
            std::vector<std::vector<u8>> good;
            std::function<void(int)> rec = [&](int left) {
                if (left == 0) {
                    int q = (int)legal_letters(b).size();
                    if (q >= 2) good.push_back(b.w);
                    return;
                }
                auto opts = legal_letters(b);
                for (u8 a : opts) {
                    b.try_push(a);
                    rec(left - 1);
                    b.pop();
                }
            };
            rec(r);
            if ((int)good.size() >= 2) {
                ++n_ge2;
                // keep two of them (first two)
                nxt.push_back(good[0]);
                nxt.push_back(good[1]);
            } else if ((int)good.size() == 1)
                ++n_1;
            else
                ++n_0;
        }
        std::cout << " round " << rd << " parents " << cur.size() << " ge2 " << n_ge2 << " eq1 " << n_1
                  << " eq0 " << n_0 << " children " << nxt.size() << "\n";
        if (nxt.empty()) {
            std::cout << "inject_iter EXTINCT round " << rd << "\n";
            return;
        }
        if (nxt.size() > 8000) {
            std::vector<size_t> idx(nxt.size());
            for (size_t i = 0; i < idx.size(); ++i) idx[i] = i;
            std::nth_element(idx.begin(), idx.begin() + 8000, idx.end(), [&](size_t i, size_t j) {
                i64 si = 0, sj = 0;
                for (u8 a : nxt[i]) si += a;
                for (u8 a : nxt[j]) sj += a;
                double mi = (double)si / (double)nxt[i].size();
                double mj = (double)sj / (double)nxt[j].size();
                return std::abs(mi - 1.5) < std::abs(mj - 1.5);
            });
            std::vector<std::vector<u8>> kept;
            kept.reserve(8000);
            for (int i = 0; i < 8000; ++i) kept.push_back(std::move(nxt[idx[i]]));
            nxt.swap(kept);
        }
        cur.swap(nxt);
    }
    std::cout << "inject_iter SURVIVE rounds " << rounds << " frontier " << cur.size() << " len "
              << cur[0].size() << "\n";
    dump_word("data/inject_word.txt", cur[0]);
}

static void mode_lcp() {
    const char* files[] = {"data/word_updown_p100_n20306.txt", "data/word_updown_p1000_n65986.txt",
                           "data/word_updown_p2000_n400000.txt", "data/word_updown_p5000_n400000.txt",
                           "data/word_fixed_p1000_n24396.txt", "data/word_drift_p1000_n13394.txt"};
    std::vector<std::vector<u8>> ws;
    std::vector<std::string> names;
    for (auto* f : files) {
        std::ifstream in(f);
        if (!in) continue;
        auto w = load_word(f);
        ws.push_back(w);
        names.push_back(f);
    }
    std::cout << "loaded " << ws.size() << " words\n";
    for (size_t i = 0; i < ws.size(); ++i) {
        for (size_t j = i + 1; j < ws.size(); ++j) {
            int L = (int)std::min(ws[i].size(), ws[j].size());
            int k = 0;
            while (k < L && ws[i][k] == ws[j][k]) ++k;
            std::cout << "lcp " << k << " " << names[i] << " " << names[j] << "\n";
        }
    }
}

static void mode_macro(const std::vector<u8>& w, int L) {
    const int N = (int)w.size();
    if (N < 4 * L) return;
    std::unordered_map<std::string, int> freq;
    for (int i = 0; i + L <= N; ++i) {
        std::string s(L, '0');
        for (int t = 0; t < L; ++t) s[t] = char('0' + w[i + t]);
        freq[s]++;
    }
    std::vector<std::pair<int, std::string>> top;
    for (auto& kv : freq) top.push_back({kv.second, kv.first});
    std::sort(top.begin(), top.end(), std::greater<>());
    int take = std::min(8, (int)top.size());
    std::cout << "L " << L << " distinct " << freq.size() << " top";
    for (int i = 0; i < take; ++i) std::cout << " " << top[i].second << ":" << top[i].first;
    std::cout << "\n";
    // For the two most frequent blocks, test BBB, BCB, CBC, and mixed length-6 concatenations.
    if (take < 2) return;
    auto to_w = [](const std::string& s) {
        std::vector<u8> v;
        for (char c : s) v.push_back((u8)(c - '0'));
        return v;
    };
    std::vector<std::vector<u8>> B;
    for (int i = 0; i < take; ++i) B.push_back(to_w(top[i].second));
    auto cat = [](const std::vector<std::vector<u8>>& parts) {
        std::vector<u8> o;
        for (auto& p : parts) o.insert(o.end(), p.begin(), p.end());
        return o;
    };
    int n_free_ok = 0, n_free_cube = 0;
    // All words of length 3 over the take-symbol alphabet.
    int nsym = std::min(take, 4);
    std::function<void(std::vector<int>&)> rec = [&](std::vector<int>& idx) {
        if ((int)idx.size() == 3) {
            std::vector<std::vector<u8>> parts;
            for (int t : idx) parts.push_back(B[t]);
            auto ww = cat(parts);
            if (is_acf(ww)) ++n_free_ok;
            else ++n_free_cube;
            return;
        }
        for (int a = 0; a < nsym; ++a) {
            idx.push_back(a);
            rec(idx);
            idx.pop_back();
        }
    };
    std::vector<int> idx;
    rec(idx);
    std::cout << "macro3 nsym " << nsym << " acf " << n_free_ok << " cube " << n_free_cube << "\n";
    // Periodic BBB always cubes (Theorem A). Record d.
    auto bbb = cat({B[0], B[0], B[0]});
    auto c0 = find_cube(bbb);
    std::cout << "BBB_cube " << (c0 ? 1 : 0) << " d=" << (c0 ? c0->d : -1) << " (expect d=L=" << L << ")\n";
}

static void mode_lookahead(int n, int R) {
    // |K_R(n)| / c_n and |K_R^{(2)}(n)| among all ACF n-mers.
    Builder b;
    uint64_t tot = 0, surv = 0, branch = 0, dead = 0;
    std::function<int(int)> n_desc = [&](int r) -> int {
        if (r == 0) return 1;
        int s = 0;
        for (int a = 0; a < 4; ++a)
            if (b.try_push((u8)a)) {
                s += n_desc(r - 1);
                b.pop();
            }
        return s;
    };
    std::function<void()> rec = [&]() {
        if (b.size() == n) {
            ++tot;
            int nd = n_desc(R);
            if (nd == 0) ++dead;
            else ++surv;
            if (nd >= 2) ++branch;
            return;
        }
        for (int a = 0; a < 4; ++a)
            if (b.try_push((u8)a)) {
                rec();
                b.pop();
            }
    };
    rec();
    std::cout << "lookahead n=" << n << " R=" << R << " tot " << tot << " surv " << surv << " branch2 " << branch
              << " dead " << dead << "\n";
}

static void mode_operators(int n, int r) {
    // Search for TWO fixed words u,v of length r such that for every ACF n-mer w,
    // both wu and wv are ACF. (Uniform two-extension operators.)
    // If none, report how many n-mers fail the best pair among a sample of pairs.
    Builder b;
    std::vector<std::vector<u8>> words;
    std::function<void()> rec = [&]() {
        if (b.size() == n) {
            words.push_back(b.w);
            return;
        }
        for (int a = 0; a < 4; ++a)
            if (b.try_push((u8)a)) {
                rec();
                b.pop();
            }
    };
    rec();
    // All ACF r-mers as candidate operators.
    Builder br;
    std::vector<std::vector<u8>> ops;
    std::function<void()> recr = [&]() {
        if (br.size() == r) {
            ops.push_back(br.w);
            return;
        }
        for (int a = 0; a < 4; ++a)
            if (br.try_push((u8)a)) {
                recr();
                br.pop();
            }
    };
    recr();
    std::cout << "operators n=" << n << " r=" << r << " n_words " << words.size() << " n_ops " << ops.size()
              << "\n";
    int best_pair = -1, best_i = -1, best_j = -1;
    int nops = (int)ops.size();
    if (nops > 80) nops = 80;  // cap pairs
    for (int i = 0; i < nops; ++i) {
        for (int j = i + 1; j < nops; ++j) {
            int ok = 0;
            for (auto& w : words) {
                Builder x, y;
                bool g1 = true, g2 = true;
                for (u8 a : w)
                    if (!x.try_push(a)) g1 = false;
                for (u8 a : w)
                    if (!y.try_push(a)) g2 = false;
                if (!g1 || !g2) continue;
                for (u8 a : ops[i])
                    if (!x.try_push(a)) {
                        g1 = false;
                        break;
                    }
                for (u8 a : ops[j])
                    if (!y.try_push(a)) {
                        g2 = false;
                        break;
                    }
                if (g1 && g2) ++ok;
            }
            if (ok > best_pair) {
                best_pair = ok;
                best_i = i;
                best_j = j;
            }
            if (ok == (int)words.size()) {
                std::cout << "UNIFORM_PAIR " << to_string(ops[i]) << " " << to_string(ops[j]) << "\n";
                return;
            }
        }
    }
    std::cout << "no_uniform_pair best_ok " << best_pair << " / " << words.size() << " ops "
              << (best_i >= 0 ? to_string(ops[best_i]) : "-") << " "
              << (best_j >= 0 ? to_string(ops[best_j]) : "-") << "\n";
}

static void mode_basin() {
    // Length-14 dead-ends: unique 13-prefixes? other children of those prefixes?
    std::ifstream in("data/deadends14.txt");
    if (!in) {
        std::cerr << "need data/deadends14.txt\n";
        std::exit(1);
    }
    std::unordered_map<std::string, int> pref;
    std::unordered_set<std::string> all;
    std::string line;
    while (std::getline(in, line)) {
        std::string w;
        for (char c : line)
            if (c >= '0' && c <= '3') w.push_back(c);
        if ((int)w.size() != 14) continue;
        all.insert(w);
        pref[w.substr(0, 13)]++;
    }
    int n_multi = 0;
    for (auto& kv : pref)
        if (kv.second > 1) ++n_multi;
    // For each 13-prefix, how many ACF 14-extensions, how many dead?
    int n_pref_all_dead = 0, n_pref_has_live = 0, n_live_sib = 0;
    for (auto& kv : pref) {
        Builder b;
        bool ok = true;
        for (char c : kv.first)
            if (!b.try_push((u8)(c - '0'))) {
                ok = false;
                break;
            }
        if (!ok) continue;
        int live = 0, dead = 0;
        for (int a = 0; a < 4; ++a) {
            if (!b.try_push((u8)a)) continue;
            int q = 0;
            for (int c = 0; c < 4; ++c)
                if (b.try_push((u8)c)) {
                    ++q;
                    b.pop();
                }
            if (q == 0) ++dead;
            else ++live;
            b.pop();
        }
        if (live == 0) ++n_pref_all_dead;
        else ++n_pref_has_live;
        n_live_sib += live;
    }
    std::cout << "dead14 " << all.size() << " distinct_pref13 " << pref.size() << " pref_with_ge2_dead_children "
              << n_multi << "\n";
    std::cout << "pref_all_children_dead " << n_pref_all_dead << " pref_has_live_sib " << n_pref_has_live
              << " total_live_siblings " << n_live_sib << "\n";
}

static void mode_cycle(int n) {
    // One cycle in the ACF n-mer overlap graph, then u^ω contains the ordinary cube uuu.
    std::vector<std::vector<u8>> nodes;
    Builder b;
    std::function<void()> rec = [&]() {
        if (b.size() == n) {
            nodes.push_back(b.w);
            return;
        }
        for (int a = 0; a < 4; ++a)
            if (b.try_push((u8)a)) {
                rec();
                b.pop();
            }
    };
    rec();
    std::unordered_map<std::string, int> idx;
    for (int i = 0; i < (int)nodes.size(); ++i) idx[to_string(nodes[i])] = i;
    std::vector<std::vector<int>> adj(nodes.size());
    for (int i = 0; i < (int)nodes.size(); ++i) {
        Builder B;
        for (u8 a : nodes[i]) B.try_push(a);
        for (int a = 0; a < 4; ++a) {
            if (!B.try_push((u8)a)) continue;
            std::string suf = to_string(B.w).substr(1);
            auto it = idx.find(suf);
            if (it != idx.end()) adj[i].push_back(it->second);
            B.pop();
        }
    }
    std::vector<int> color(nodes.size(), 0);
    std::vector<int> stack;
    int cyc_s = -1;
    std::function<bool(int)> dfs = [&](int u) -> bool {
        color[u] = 1;
        stack.push_back(u);
        for (int v : adj[u]) {
            if (color[v] == 0) {
                if (dfs(v)) return true;
            } else if (color[v] == 1) {
                cyc_s = v;
                return true;
            }
        }
        stack.pop_back();
        color[u] = 2;
        return false;
    };
    for (int i = 0; i < (int)nodes.size() && cyc_s < 0; ++i)
        if (color[i] == 0) dfs(i);
    if (cyc_s < 0) {
        std::cout << "cycle n=" << n << " NONE\n";
        return;
    }
    std::vector<int> cyc2;
    bool on = false;
    for (int u : stack) {
        if (u == cyc_s) on = true;
        if (on) cyc2.push_back(u);
    }
    cyc2.push_back(cyc_s);  // close
    // Period word = successive added letters along the cycle.
    std::string period;
    for (size_t i = 0; i + 1 < cyc2.size(); ++i) {
        auto& nxt = nodes[cyc2[i + 1]];
        period.push_back(char('0' + nxt.back()));
    }
    std::vector<u8> uuu;
    for (int t = 0; t < 3; ++t)
        for (char c : period) uuu.push_back((u8)(c - '0'));
    auto cu = find_cube(uuu);
    std::cout << "cycle n=" << n << " period_len " << period.size() << " period " << period << "\n";
    std::cout << "uuu_len " << uuu.size() << " cube " << (cu ? 1 : 0) << " d=" << (cu ? cu->d : -1) << "\n";
}

static void mode_detfsm() {
    // Exhaustive 1-letter memory: next letter = f(last letter). 4^4 = 256 maps.
    int n_maps = 0, n_cube = 0, n_alive100 = 0, max_acf = 0;
    for (int code = 0; code < 256; ++code) {
        int f[4];
        int t = code;
        for (int a = 0; a < 4; ++a) {
            f[a] = t & 3;
            t >>= 2;
        }
        ++n_maps;
        int best = 0;
        for (int seed = 0; seed < 4; ++seed) {
            Builder b;
            if (!b.try_push((u8)seed)) continue;
            int last = seed;
            bool live = true;
            for (int i = 0; i < 100 && live; ++i) {
                int nxt = f[last];
                if (!b.try_push((u8)nxt)) {
                    live = false;
                    break;
                }
                last = nxt;
            }
            best = std::max(best, b.size());
            if (live) ++n_alive100;
        }
        if (best < 100) ++n_cube;
        if (best > max_acf) max_acf = best;
    }
    std::cout << "detfsm k=1 maps " << n_maps << " all_seeds_cube_before_100 " << n_cube << " seed_alive100 "
              << n_alive100 << " max_acf_prefix " << max_acf << "\n";
}

static uint64_t pack_word(const std::vector<u8>& w, int i, int k) {
    uint64_t x = 0;
    for (int t = 0; t < k; ++t) x = (x << 2) | w[i + t];
    return x;
}

struct NmerGraph {
    int n = 0;
    std::vector<std::vector<u8>> nodes;
    std::vector<std::vector<std::pair<u8, int>>> adj;  // letter, next node
    std::vector<char> in2core;
};

static NmerGraph build_nmer_graph(int n) {
    NmerGraph G;
    G.n = n;
    Builder b;
    std::function<void()> rec = [&]() {
        if (b.size() == n) {
            G.nodes.push_back(b.w);
            return;
        }
        for (int a = 0; a < 4; ++a)
            if (b.try_push((u8)a)) {
                rec();
                b.pop();
            }
    };
    rec();
    std::unordered_map<uint64_t, int> idx;
    idx.reserve(G.nodes.size() * 2);
    for (int i = 0; i < (int)G.nodes.size(); ++i) idx[pack_word(G.nodes[i], 0, n)] = i;
    G.adj.resize(G.nodes.size());
    for (int i = 0; i < (int)G.nodes.size(); ++i) {
        Builder B;
        for (u8 a : G.nodes[i]) B.try_push(a);
        for (int a = 0; a < 4; ++a) {
            if (!B.try_push((u8)a)) continue;
            uint64_t q = pack_word(B.w, 1, n);
            auto it = idx.find(q);
            if (it != idx.end()) G.adj[i].push_back({(u8)a, it->second});
            B.pop();
        }
    }
    std::vector<char> alive(G.nodes.size(), 1);
    bool changed = true;
    while (changed) {
        changed = false;
        for (int i = 0; i < (int)G.nodes.size(); ++i) {
            if (!alive[i]) continue;
            int od = 0;
            for (auto& e : G.adj[i])
                if (alive[e.second]) ++od;
            if (od < 2) {
                alive[i] = 0;
                changed = true;
            }
        }
    }
    G.in2core = std::move(alive);
    return G;
}

static int twocore_count(const NmerGraph& G) {
    int c = 0;
    for (char x : G.in2core) c += x;
    return c;
}

static int driver_bit(const std::string& drv, uint64_t i) {
    if (drv == "tm") return __builtin_popcountll(i) & 1;
    if (drv == "pf") {
        uint64_t x = i + 1;
        return __builtin_ctzll(x) & 1;
    }
    if (drv == "sturm") {
        // mechanical word of golden ratio
        const double phi = 1.6180339887498948482;
        int a = (int)((i + 1) * (phi - 1.0));
        int b = (int)(i * (phi - 1.0));
        return a - b;
    }
    return (int)(i & 1);  // period-2, will cube; control
}

static void mode_twocore(int n) {
    auto G = build_nmer_graph(n);
    int n2 = twocore_count(G);
    int n1 = 0, n0 = 0;
    for (int i = 0; i < (int)G.nodes.size(); ++i) {
        int od = (int)G.adj[i].size();
        if (od == 0) ++n0;
        if (od == 1) ++n1;
    }
    std::cout << "n " << n << " nodes " << G.nodes.size() << " out0 " << n0 << " out1 " << n1 << " twocore "
              << n2 << "\n";
}

static void mode_drive(int n, const std::string& drv, int cap, const char* variant) {
    // variant "blind": follow 2-core edges, stop at first global cube
    // variant "legal": among globally legal letters, prefer 2-core, TM-select
    auto G = build_nmer_graph(n);
    int n2 = twocore_count(G);
    int start = -1;
    for (int i = 0; i < (int)G.nodes.size(); ++i)
        if (G.in2core[i]) {
            start = i;
            break;
        }
    std::cout << "drive n=" << n << " drv=" << drv << " var=" << variant << " twocore " << n2 << " start "
              << (start < 0 ? "-" : to_string(G.nodes[start])) << " cap " << cap << "\n";
    if (start < 0) {
        std::cout << "empty_2core\n";
        return;
    }
    std::unordered_map<uint64_t, int> idx;
    idx.reserve(G.nodes.size() * 2);
    for (int i = 0; i < (int)G.nodes.size(); ++i) idx[pack_word(G.nodes[i], 0, n)] = i;

    Builder b;
    for (u8 a : G.nodes[start]) b.try_push(a);
    int v = start;
    uint64_t step = 0;
    if (std::string(variant) == "blind") {
        while (b.size() < cap) {
            std::vector<std::pair<u8, int>> opts;
            for (auto& e : G.adj[v])
                if (G.in2core[e.second]) opts.push_back(e);
            if (opts.empty()) {
                std::cout << "stuck_graph at " << b.size() << "\n";
                return;
            }
            int bit = driver_bit(drv, step++);
            auto e = opts[bit % (int)opts.size()];
            if (!b.try_push(e.first)) {
                // try_push already popped; reconstruct the cube on the rejected letter
                b.S.push_back(b.S.back() + e.first);
                b.w.push_back(e.first);
                auto c = find_cube_ending_at(b.S, b.size());
                std::cout << "CUBE at " << b.size() << " i=" << (c ? c->i : -1) << " d=" << (c ? c->d : -1)
                          << " steps " << step << " word " << to_string(b.w) << "\n";
                b.w.pop_back();
                b.S.pop_back();
                return;
            }
            v = e.second;
        }
    } else {
        while (b.size() < cap) {
            std::vector<u8> legal, core;
            for (int a = 0; a < 4; ++a) {
                if (!b.try_push((u8)a)) continue;
                legal.push_back((u8)a);
                uint64_t q = pack_word(b.w, b.size() - n, n);
                auto it = idx.find(q);
                if (it != idx.end() && G.in2core[it->second]) core.push_back((u8)a);
                b.pop();
            }
            if (legal.empty()) {
                std::cout << "DEAD at " << b.size() << " steps " << step << "\n";
                return;
            }
            auto& pool = !core.empty() ? core : legal;
            int bit = driver_bit(drv, step++);
            u8 a = pool[bit % (int)pool.size()];
            b.try_push(a);
        }
    }
    std::cout << "SURVIVE " << b.size() << " steps " << step << " mean " << (double)b.S.back() / b.size()
              << "\n";
}

static void mode_drive_scan(int n, const std::string& drv, int cap, int nstarts) {
    auto G = build_nmer_graph(n);
    int best = 0, n_tried = 0;
    std::string bests;
    int best_d = -1, best_i = -1;
    for (int s = 0; s < (int)G.nodes.size() && n_tried < nstarts; ++s) {
        if (!G.in2core[s]) continue;
        ++n_tried;
        Builder b;
        bool ok = true;
        for (u8 a : G.nodes[s])
            if (!b.try_push(a)) {
                ok = false;
                break;
            }
        if (!ok) continue;
        int v = s;
        uint64_t step = 0;
        int reached = b.size();
        int cube_i = -1, cube_d = -1;
        while (b.size() < cap) {
            std::vector<std::pair<u8, int>> opts;
            for (auto& e : G.adj[v])
                if (G.in2core[e.second]) opts.push_back(e);
            if (opts.empty()) break;
            auto e = opts[driver_bit(drv, step++) % (int)opts.size()];
            if (!b.try_push(e.first)) {
                b.S.push_back(b.S.back() + e.first);
                b.w.push_back(e.first);
                auto c = find_cube_ending_at(b.S, b.size());
                cube_i = c ? c->i : -1;
                cube_d = c ? c->d : -1;
                b.w.pop_back();
                b.S.pop_back();
                break;
            }
            v = e.second;
            reached = b.size();
        }
        if (reached > best) {
            best = reached;
            bests = to_string(G.nodes[s]);
            best_d = cube_d;
            best_i = cube_i;
        }
        if (reached >= cap) {
            std::cout << "SCAN_SURVIVE n=" << n << " drv=" << drv << " start " << to_string(G.nodes[s])
                      << " cap " << cap << "\n";
            return;
        }
    }
    std::cout << "scan n=" << n << " drv=" << drv << " starts " << n_tried << " cap " << cap << " best_len "
              << best << " best_start " << bests << " cube_i " << best_i << " d " << best_d << "\n";
}

static std::vector<std::vector<u8>> all_acf_len(int n) {
    std::vector<std::vector<u8>> out;
    Builder b;
    std::function<void()> rec = [&]() {
        if (b.size() == n) {
            out.push_back(b.w);
            return;
        }
        for (int a = 0; a < 4; ++a)
            if (b.try_push((u8)a)) {
                rec();
                b.pop();
            }
    };
    rec();
    return out;
}

static int seq_bit(const std::string& drv, uint64_t n) {
    if (drv == "pf") return __builtin_ctzll(n + 1) & 1;
    if (drv == "rs") {
        int b = 0;
        uint64_t x = n;
        while (x) {
            if ((x & 3ull) == 3ull) b ^= 1;
            x >>= 1;
        }
        return b;
    }
    if (drv == "pd") {
        int flips = 0;
        while (n & 1ull) {
            flips ^= 1;
            n >>= 1;
        }
        return flips;
    }
    if (drv == "fib") {
        const double phi = 1.6180339887498948482;
        int a = (int)std::floor((n + 1) * phi);
        int b = (int)std::floor(n * phi);
        return a - b - 1;
    }
    return __builtin_popcountll(n) & 1;  // tm
}

static int tm_block_len(const std::vector<u8>& u, const std::vector<u8>& v, int cap, Cube* cube_out,
                        uint64_t start, const std::string& drv) {
    Builder b;
    uint64_t i = start;
    while (b.size() < cap) {
        int bit = seq_bit(drv, i);
        const auto& blk = bit ? v : u;
        ++i;
        for (u8 a : blk) {
            if (!b.try_push(a)) {
                b.S.push_back(b.S.back() + a);
                b.w.push_back(a);
                auto c = find_cube_ending_at(b.S, b.size());
                if (cube_out && c) *cube_out = *c;
                return b.size();
            }
        }
    }
    return cap;
}

static void mode_tmblocks(int lu, int lv, int cap, int pair_limit, uint64_t start, const std::string& drv) {
    auto U = all_acf_len(lu);
    auto V = (lu == lv) ? U : all_acf_len(lv);
    uint64_t ntry = 0, n_hit_cap = 0;
    int best = 0, worst = 1 << 30;
    std::string bestu, bestv, worstu, worstv;
    Cube bestc{}, worstc{};
    int ui_max = (int)U.size();
    int vi_max = (int)V.size();
    for (int i = 0; i < ui_max; ++i) {
        for (int j = 0; j < vi_max; ++j) {
            if (lu == lv && j == i) continue;
            if (pair_limit > 0 && (int)ntry >= pair_limit) goto done;
            ++ntry;
            Cube c{};
            int L = tm_block_len(U[i], V[j], cap, &c, start, drv);
            if (L >= cap) {
                ++n_hit_cap;
                if (L >= best) {
                    best = L;
                    bestu = to_string(U[i]);
                    bestv = to_string(V[j]);
                    bestc = c;
                }
            } else {
                if (L > best) {
                    best = L;
                    bestu = to_string(U[i]);
                    bestv = to_string(V[j]);
                    bestc = c;
                }
                if (L < worst) {
                    worst = L;
                    worstu = to_string(U[i]);
                    worstv = to_string(V[j]);
                    worstc = c;
                }
            }
        }
    }
done:
    std::cout << "tmblocks lu=" << lu << " lv=" << lv << " cap " << cap << " start " << start << " drv " << drv
              << " pairs " << ntry << " hit_cap " << n_hit_cap << " best " << best << " u " << bestu << " v "
              << bestv << " cube_i " << bestc.i << " d " << bestc.d << " worst "
              << (worst == (1 << 30) ? 0 : worst) << " wu " << worstu << " wv " << worstv << "\n";
}

static std::vector<u8> greedy_cubefree_ternary(int n) {
    std::vector<u8> w;
    for (int i = 0; i < n; ++i) {
        bool placed = false;
        for (u8 a = 0; a < 3; ++a) {
            w.push_back(a);
            bool cube = false;
            int N = (int)w.size();
            for (int d = 1; 3 * d <= N && !cube; ++d) {
                bool eq = true;
                for (int t = 0; t < d; ++t) {
                    if (w[N - d + t] != w[N - 2 * d + t] || w[N - d + t] != w[N - 3 * d + t]) {
                        eq = false;
                        break;
                    }
                }
                if (eq) cube = true;
            }
            if (!cube) {
                placed = true;
                break;
            }
            w.pop_back();
        }
        if (!placed) break;
    }
    return w;
}

static int concat_blocks(Builder& b, const std::vector<u8>& blk) {
    for (u8 a : blk) {
        if (!b.try_push(a)) return 0;
    }
    return 1;
}

static void mode_triblocks(int l0, int l1, int l2, int cap, int pair_limit) {
    auto A0 = all_acf_len(l0);
    auto A1 = all_acf_len(l1);
    auto A2 = all_acf_len(l2);
    auto T = greedy_cubefree_ternary(80);
    std::cout << "ternary_cubefree_len " << T.size() << " prefix ";
    for (int i = 0; i < std::min(24, (int)T.size()); ++i) std::cout << (int)T[i];
    std::cout << "\n";
    uint64_t ntry = 0, n_hit = 0;
    int best = 0;
    std::string b0, b1, b2;
    Cube bc{};
    int n0 = (int)A0.size(), n1 = (int)A1.size(), n2 = (int)A2.size();
    for (int i = 0; i < n0; ++i) {
        for (int j = 0; j < n1; ++j) {
            for (int k = 0; k < n2; ++k) {
                if (pair_limit > 0 && (int)ntry >= pair_limit) goto done;
                ++ntry;
                const std::vector<u8>* B[3] = {&A0[i], &A1[j], &A2[k]};
                Builder b;
                Cube c{};
                int died = 0;
                for (u8 t : T) {
                    for (u8 a : *B[t]) {
                        if (!b.try_push(a)) {
                            b.S.push_back(b.S.back() + a);
                            b.w.push_back(a);
                            auto cc = find_cube_ending_at(b.S, b.size());
                            if (cc) c = *cc;
                            died = b.size();
                            goto after;
                        }
                    }
                    if (b.size() >= cap) break;
                }
            after:
                int L = died ? died : b.size();
                if (!died && L >= cap) ++n_hit;
                if (L > best) {
                    best = L;
                    b0 = to_string(A0[i]);
                    b1 = to_string(A1[j]);
                    b2 = to_string(A2[k]);
                    bc = c;
                }
            }
        }
    }
done:
    std::cout << "triblocks " << l0 << "," << l1 << "," << l2 << " cap " << cap << " pairs " << ntry
              << " hit_cap " << n_hit << " best " << best << " b0 " << b0 << " b1 " << b1 << " b2 " << b2
              << " cube_i " << bc.i << " d " << bc.d << "\n";
}

static void mode_returns(const char* path, const char* marker, int cap) {
    auto w = load_word(path);
    std::string m = marker;
    int k = (int)m.size();
    std::vector<int> pos;
    for (int i = 0; i + k <= (int)w.size(); ++i) {
        bool ok = true;
        for (int t = 0; t < k; ++t)
            if (char('0' + w[i + t]) != m[t]) {
                ok = false;
                break;
            }
        if (ok) pos.push_back(i);
    }
    std::map<std::string, int> freq;
    std::map<std::string, std::map<std::string, int>> trans;
    std::vector<std::string> rets;
    for (size_t j = 0; j + 1 < pos.size(); ++j) {
        int a = pos[j], b = pos[j + 1];
        std::string r;
        r.reserve(b - a);
        for (int t = a; t < b; ++t) r.push_back(char('0' + w[t]));
        freq[r]++;
        rets.push_back(r);
        if (j + 2 < pos.size()) {
            std::string s;
            for (int t = pos[j + 1]; t < pos[j + 2]; ++t) s.push_back(char('0' + w[t]));
            trans[r][s]++;
        }
    }
    std::cout << "returns marker " << m << " N " << w.size() << " occ " << pos.size() << " distinct "
              << freq.size() << "\n";
    std::vector<std::pair<int, std::string>> top;
    for (auto& kv : freq) top.push_back({kv.second, kv.first});
    std::sort(top.begin(), top.end(), std::greater<>());
    int take = std::min(8, (int)top.size());
    std::cout << "top";
    for (int i = 0; i < take; ++i)
        std::cout << " " << top[i].second.size() << ":" << top[i].first << ":" << top[i].second.substr(0, 20);
    std::cout << "\n";
    if (take < 2) return;
    auto to_blk = [](const std::string& s) {
        std::vector<u8> v;
        for (char c : s) v.push_back((u8)(c - '0'));
        return v;
    };
    // Drive the two most frequent of different lengths if possible, else top two.
    int i0 = 0, i1 = 1;
    for (int i = 1; i < take; ++i)
        if ((int)top[i].second.size() != (int)top[0].second.size()) {
            i1 = i;
            break;
        }
    Cube c{};
    int Ltm = tm_block_len(to_blk(top[i0].second), to_blk(top[i1].second), cap, &c, 0, "fib");
    std::cout << "return_fib bestpair lens " << top[i0].second.size() << "," << top[i1].second.size()
              << " acf_until " << Ltm << " cube_i " << c.i << " d " << c.d << "\n";
    Ltm = tm_block_len(to_blk(top[i0].second), to_blk(top[i1].second), cap, &c, 0, "pd");
    std::cout << "return_pd acf_until " << Ltm << " cube_i " << c.i << " d " << c.d << "\n";
    Ltm = tm_block_len(to_blk(top[i0].second), to_blk(top[i1].second), cap, &c, 0, "rs");
    std::cout << "return_rs acf_until " << Ltm << " cube_i " << c.i << " d " << c.d << "\n";
}

static std::array<std::vector<u8>, 4> rand_morph(std::mt19937_64& rng, int k) {
    std::array<std::vector<u8>, 4> h;
    std::uniform_int_distribution<int> d(0, 3);
    for (int a = 0; a < 4; ++a) {
        h[a].resize(k);
        for (int t = 0; t < k; ++t) h[a][t] = (u8)d(rng);
        if (a == 0) h[0][0] = 0;  // prolongable-ish
    }
    return h;
}

static void mode_sadic(int k, int trials, int cap, uint64_t seed) {
    std::mt19937_64 rng(seed);
    int best = 0, n_hit = 0;
    std::string report;
    for (int t = 0; t < trials; ++t) {
        auto h0 = rand_morph(rng, k);
        auto h1 = rand_morph(rng, k);
        // W_{n+1} = h_{pd(n)}(W_n), start 0, until length cap or cube
        std::vector<u8> w = {0};
        int depth = 0;
        int acf_ok = (int)w.size();
        int died = 0;
        Cube c{};
        while ((int)w.size() < cap && depth < 16) {
            int bit = seq_bit("pd", (uint64_t)depth);
            const auto& h = bit ? h1 : h0;
            std::vector<u8> nxt;
            nxt.reserve(w.size() * k);
            for (u8 a : w) nxt.insert(nxt.end(), h[a].begin(), h[a].end());
            if (!is_acf(nxt)) {
                auto cc = find_cube(nxt);
                if (cc) c = *cc;
                died = (int)nxt.size();
                break;
            }
            w.swap(nxt);
            acf_ok = (int)w.size();
            ++depth;
        }
        int L = acf_ok;
        if (!died && acf_ok >= cap) ++n_hit;
        if (L > best) {
            best = L;
            std::ostringstream oss;
            oss << "depth " << depth << " died " << died << " i=" << c.i << " d=" << c.d << " h0 ";
            for (int a = 0; a < 4; ++a) oss << to_string(h0[a]) << (a < 3 ? "," : " h1 ");
            for (int a = 0; a < 4; ++a) oss << to_string(h1[a]) << (a < 3 ? "," : "");
            report = oss.str();
        }
    }
    std::cout << "sadic k=" << k << " trials " << trials << " cap " << cap << " hit_cap " << n_hit << " best "
              << best << " " << report << "\n";
}

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cerr
            << "core_scan trie N | word FILE [pe] | suffix FILE k | updown FILE P | random D T seed |\n"
            << "  branch n R samples | findq1 N budget | inject n | basin | cass N | walk FILE |\n"
            << "  recgen CAP | grow2 D budget | beam D BEAM STYLE | mutate FILE samples extra stride |\n"
            << "  inject_iter n0 rounds r | lcp | macro FILE L | look n R | ops n r |\n"
            << "  cycle n | detfsm | twocore n | drive n drv cap blind|legal | dscan | tmblocks lu lv cap lim\n";
        return 1;
    }
    std::string cmd = argv[1];
    if (cmd == "trie") {
        int n = (argc > 2) ? std::atoi(argv[2]) : 11;
        mode_trie(n);
    } else if (cmd == "word") {
        auto w = load_word(argv[2]);
        int pe = (argc > 3) ? std::atoi(argv[3]) : 0;
        mode_word(w, pe);
    } else if (cmd == "suffix") {
        auto w = load_word(argv[2]);
        int k = (argc > 3) ? std::atoi(argv[3]) : 8;
        mode_suffix(w, k);
    } else if (cmd == "updown") {
        auto w = load_word(argv[2]);
        int p = (argc > 3) ? std::atoi(argv[3]) : 1000;
        mode_updown(w, p);
    } else if (cmd == "random") {
        int D = std::atoi(argv[2]);
        int T = std::atoi(argv[3]);
        uint64_t seed = (argc > 4) ? std::strtoull(argv[4], nullptr, 10) : 1;
        mode_random(D, T, seed);
    } else if (cmd == "branch") {
        int n = std::atoi(argv[2]);
        int R = std::atoi(argv[3]);
        int samples = (argc > 4) ? std::atoi(argv[4]) : 0;
        mode_branch(n, R, samples);
    } else if (cmd == "findq1") {
        int n = std::atoi(argv[2]);
        uint64_t bud = (argc > 3) ? std::strtoull(argv[3], nullptr, 10) : 2000000ull;
        mode_find_q1(n, bud);
    } else if (cmd == "inject") {
        mode_inject(std::atoi(argv[2]));
    } else if (cmd == "basin") {
        mode_basin();
    } else if (cmd == "cass") {
        mode_cass((argc > 2) ? std::atoi(argv[2]) : 8000);
    } else if (cmd == "walk") {
        mode_walk(load_word(argv[2]));
    } else if (cmd == "recgen") {
        mode_recgen((argc > 2) ? std::atoi(argv[2]) : 256);
    } else if (cmd == "grow2") {
        int D = (argc > 2) ? std::atoi(argv[2]) : 40;
        uint64_t bud = (argc > 3) ? std::strtoull(argv[3], nullptr, 10) : 200000ull;
        mode_grow2(D, bud);
    } else if (cmd == "beam") {
        int D = (argc > 2) ? std::atoi(argv[2]) : 80;
        int B = (argc > 3) ? std::atoi(argv[3]) : 2048;
        int st = (argc > 4) ? std::atoi(argv[4]) : 0;
        mode_beam(D, B, st);
    } else if (cmd == "mutate") {
        const char* path = (argc > 2) ? argv[2] : "data/word_updown_p2000_n400000.txt";
        int samples = (argc > 3) ? std::atoi(argv[3]) : 80;
        int extra = (argc > 4) ? std::atoi(argv[4]) : 2000;
        int stride = (argc > 5) ? std::atoi(argv[5]) : 200;
        mode_mutate(path, samples, extra, stride);
    } else if (cmd == "inject_iter") {
        int n0 = (argc > 2) ? std::atoi(argv[2]) : 4;
        int rounds = (argc > 3) ? std::atoi(argv[3]) : 12;
        int r = (argc > 4) ? std::atoi(argv[4]) : 2;
        mode_inject_iter(n0, rounds, r);
    } else if (cmd == "lcp") {
        mode_lcp();
    } else if (cmd == "macro") {
        mode_macro(load_word(argv[2]), (argc > 3) ? std::atoi(argv[3]) : 8);
    } else if (cmd == "look") {
        mode_lookahead(std::atoi(argv[2]), std::atoi(argv[3]));
    } else if (cmd == "ops") {
        mode_operators(std::atoi(argv[2]), std::atoi(argv[3]));
    } else if (cmd == "cycle") {
        mode_cycle((argc > 2) ? std::atoi(argv[2]) : 6);
    } else if (cmd == "detfsm") {
        mode_detfsm();
    } else if (cmd == "twocore") {
        mode_twocore((argc > 2) ? std::atoi(argv[2]) : 8);
    } else if (cmd == "drive") {
        int n = (argc > 2) ? std::atoi(argv[2]) : 6;
        std::string drv = (argc > 3) ? argv[3] : "tm";
        int cap = (argc > 4) ? std::atoi(argv[4]) : 20000;
        const char* var = (argc > 5) ? argv[5] : "blind";
        mode_drive(n, drv, cap, var);
    } else if (cmd == "dscan") {
        int n = std::atoi(argv[2]);
        std::string drv = (argc > 3) ? argv[3] : "tm";
        int cap = (argc > 4) ? std::atoi(argv[4]) : 2000;
        int ns = (argc > 5) ? std::atoi(argv[5]) : 200;
        mode_drive_scan(n, drv, cap, ns);
    } else if (cmd == "tmblocks") {
        int lu = (argc > 2) ? std::atoi(argv[2]) : 3;
        int lv = (argc > 3) ? std::atoi(argv[3]) : lu;
        int cap = (argc > 4) ? std::atoi(argv[4]) : 200;
        int lim = (argc > 5) ? std::atoi(argv[5]) : 0;
        uint64_t start = (argc > 6) ? std::strtoull(argv[6], nullptr, 10) : 0;
        std::string drv = (argc > 7) ? argv[7] : "tm";
        mode_tmblocks(lu, lv, cap, lim, start, drv);
    } else if (cmd == "triblocks") {
        int a = std::atoi(argv[2]);
        int b = std::atoi(argv[3]);
        int c = std::atoi(argv[4]);
        int cap = (argc > 5) ? std::atoi(argv[5]) : 200;
        int lim = (argc > 6) ? std::atoi(argv[6]) : 8000;
        mode_triblocks(a, b, c, cap, lim);
    } else if (cmd == "returns") {
        const char* path = argv[2];
        const char* mk = (argc > 3) ? argv[3] : "01";
        int cap = (argc > 4) ? std::atoi(argv[4]) : 500;
        mode_returns(path, mk, cap);
    } else if (cmd == "sadic") {
        int k = (argc > 2) ? std::atoi(argv[2]) : 2;
        int tr = (argc > 3) ? std::atoi(argv[3]) : 3000;
        int cap = (argc > 4) ? std::atoi(argv[4]) : 512;
        uint64_t seed = (argc > 5) ? std::strtoull(argv[5], nullptr, 10) : 1;
        mode_sadic(k, tr, cap, seed);
    } else {
        std::cerr << "unknown cmd\n";
        return 1;
    }
    return 0;
}
