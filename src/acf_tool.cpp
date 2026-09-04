#include "acf.hpp"
#include "search.hpp"

#include <cstdio>
#include <cstring>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <cmath>

using namespace acf;

static void die(const std::string& m) {
    std::cerr << m << "\n";
    std::exit(1);
}

static std::vector<u8> cassaigne_prefix(int n) {
    // φ: 0→03, 1→43, 3→1, 4→01 on alphabet {0,1,3,4}
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

// ---------------- tests ----------------
static int run_tests() {
    int fails = 0;
    auto check = [&](const std::string& name, bool cond) {
        if (!cond) {
            std::cerr << "FAIL " << name << "\n";
            ++fails;
        } else {
            std::cout << "OK   " << name << "\n";
        }
    };

    // Hand examples
    check("empty", is_acf({}));
    check("0", is_acf({0}));
    check("01", is_acf({0, 1}));
    check("000 is cube", !is_acf({0, 0, 0}));
    check("111 is cube", !is_acf({1, 1, 1}));
    check("012 acf", is_acf({0, 1, 2}));
    check("010 acf", is_acf({0, 1, 0}));
    check("012012012 cube", !is_acf(parse_digits("012012012")));
    check("0123 acf", is_acf({0, 1, 2, 3}));
    // 01230123 contains 12|30|12 with equal sums 3 (i=1,d=2).
    check("01230123 has cube 12|30|12", !is_acf(parse_digits("01230123")));
    check("001001001 cube", !is_acf(parse_digits("001001001")));
    check("012301230123 cube", !is_acf(parse_digits("012301230123")));
    check("012012021 cube (sums 3)", !is_acf(parse_digits("012012021")));
    check("0001 cube via prefix", !is_acf({0, 0, 0, 1}));
    check("1000 cube", !is_acf({1, 0, 0, 0}));

    // Incremental vs full vs brute on ALL words of length <= 6
    uint64_t compared = 0;
    bool mismatch = false;
    for (int n = 0; n <= 6; ++n) {
        int total = 1;
        for (int i = 0; i < n; ++i) total *= 4;
        for (int mask = 0; mask < total; ++mask) {
            std::vector<u8> w(n);
            int m = mask;
            for (int i = 0; i < n; ++i) {
                w[i] = m % 4;
                m /= 4;
            }
            bool full = is_acf(w);
            bool brute = brute_is_acf(w);
            Builder b;
            bool incr = true;
            for (u8 a : w) {
                if (!b.try_push(a)) {
                    incr = false;
                    break;
                }
            }
            if (full != brute || incr != brute) {
                std::cerr << "FAIL mismatch word=" << to_string(w)
                          << " full=" << full << " brute=" << brute
                          << " incr=" << incr << "\n";
                ++fails;
                mismatch = true;
            }
            ++compared;
        }
    }
    check("full=brute=incr on all words n<=6", !mismatch);
    std::cout << "      compared " << compared << " words\n";

    // Random words n=20,50,80
    std::mt19937 rng(12345);
    std::uniform_int_distribution<int> dist(0, 3);
    int rfail = 0;
    for (int n : {20, 50, 80, 120}) {
        for (int t = 0; t < 400; ++t) {
            std::vector<u8> w(n);
            for (int i = 0; i < n; ++i) w[i] = (u8)dist(rng);
            bool full = is_acf(w);
            bool brute = brute_is_acf(w);
            Builder b;
            bool incr = true;
            for (u8 a : w) {
                if (!b.try_push(a)) {
                    incr = false;
                    break;
                }
            }
            // Incremental reports ACF iff EVERY prefix is ACF, which is equivalent
            // to the whole word being ACF.
            if (full != brute || incr != brute) ++rfail;
        }
    }
    check("random words n=20..120", rfail == 0);

    // Cassaigne et al. morphism over {0,1,3,4} should be ACF on a long prefix.
    auto cass = cassaigne_prefix(20000);
    auto cube = find_cube(cass);
    check("Cassaigne prefix 20k ACF", !cube.has_value());
    if (cube) {
        std::cerr << "  cube at i=" << cube->i << " d=" << cube->d
                  << " sum=" << cube->sum << "\n";
    }
    // Incremental build of the same prefix
    {
        Builder b;
        bool ok = true;
        for (u8 a : cass) {
            if (!b.try_push(a)) {
                ok = false;
                break;
            }
        }
        check("Cassaigne 20k incremental", ok && b.size() == (int)cass.size());
    }

    // Known cubes: three equal-sum blocks
    check("203203203 cube", !is_acf(parse_digits("203203203")));

    // Period-p word u^3 is always a cube (ordinary, hence additive)
    {
        std::vector<u8> u = {0, 1, 2, 3, 1};
        std::vector<u8> w;
        w.insert(w.end(), u.begin(), u.end());
        w.insert(w.end(), u.begin(), u.end());
        w.insert(w.end(), u.begin(), u.end());
        auto c = find_cube(w);
        check("u^3 always a cube", c.has_value() && c->d == (int)u.size() && c->i == 0);
    }

    if (fails == 0) std::cout << "ALL TESTS PASSED\n";
    else std::cout << fails << " FAILURES\n";
    return fails ? 1 : 0;
}

// ---------------- characterize ----------------
struct DepthInfo {
    uint64_t words = 0;
    uint64_t extendable = 0;
    uint64_t dead_ends = 0;
    uint64_t extension_successes = 0;  // total successful children
    std::array<uint64_t, 4> letter{{0, 0, 0, 0}};
    std::array<uint64_t, 5> branch_hist{{0, 0, 0, 0, 0}};  // how many have 0..4 children
};

static void characterize(int nmax) {
    std::vector<DepthInfo> info(nmax + 1);
    Builder b;
    std::function<void()> rec = [&]() {
        int n = b.size();
        info[n].words++;
        if (n > 0) info[n].letter[b.w.back()]++;
        if (n == nmax) return;
        int children = 0;
        for (int a = 0; a < 4; ++a) {
            if (b.try_push((u8)a)) {
                ++children;
                rec();
                b.pop();
            }
        }
        info[n].branch_hist[children]++;
        info[n].extension_successes += children;
        if (children == 0) info[n].dead_ends++;
        else info[n].extendable++;
    };
    rec();

    std::cout << "# n  count  log4_count  branch  dead_frac  p0 p1 p2 p3  hist0..4\n";
    for (int n = 0; n <= nmax; ++n) {
        auto& I = info[n];
        double branch = I.words ? (double)I.extension_successes / (double)I.words : 0;
        // for n=nmax we didn't try extensions
        if (n == nmax) branch = -1;
        double dead = (n == nmax || I.words == 0) ? 0.0 : (double)I.dead_ends / (double)I.words;
        double totL = 0;
        for (int a = 0; a < 4; ++a) totL += I.letter[a];
        std::cout << n << " " << I.words << " ";
        if (I.words)
            std::cout << std::fixed << std::setprecision(6) << std::log(I.words) / std::log(4.0);
        else
            std::cout << 0;
        std::cout << " " << std::setprecision(4) << branch << " " << dead << " ";
        for (int a = 0; a < 4; ++a) {
            double p = totL ? I.letter[a] / totL : 0;
            std::cout << std::setprecision(3) << p << (a < 3 ? "," : " ");
        }
        if (n < nmax) {
            for (int k = 0; k <= 4; ++k)
                std::cout << I.branch_hist[k] << (k < 4 ? "," : "");
        }
        std::cout << "\n";
    }
}

// ---------------- grow (Up-and-Down / greedy) ----------------
static void grow(const std::string& mode, uint64_t budget, int cap, int period) {
    std::function<std::array<u8, 4>(int, const Builder&)> ord;
    if (mode == "fixed") {
        ord = [](int, const Builder&) { return order_fixed_up(); };
    } else if (mode == "fixed-down") {
        ord = [](int, const Builder&) { return order_fixed_down(); };
    } else if (mode == "updown") {
        ord = up_down_order(period);
    } else if (mode == "drift") {
        ord = drift_order(0.0);
    } else {
        die("unknown mode: " + mode);
    }
    auto t0 = std::chrono::steady_clock::now();
    auto w = greedy_backtrack(budget, ord, cap);
    auto t1 = std::chrono::steady_clock::now();
    double sec = std::chrono::duration<double>(t1 - t0).count();
    auto cube = find_cube(w);
    std::array<uint64_t, 4> freq{{0, 0, 0, 0}};
    i64 sum = 0;
    for (u8 a : w) {
        freq[a]++;
        sum += a;
    }
    std::cout << "mode=" << mode << " period=" << period << " budget=" << budget
              << " cap=" << cap << "\n";
    std::cout << "length=" << w.size() << " time_s=" << sec << "\n";
    std::cout << "verified_acf=" << (!cube.has_value() ? "yes" : "NO") << "\n";
    if (cube)
        std::cout << "cube i=" << cube->i << " d=" << cube->d << " sum=" << cube->sum << "\n";
    if (!w.empty()) {
        std::cout << "mean=" << (double)sum / (double)w.size() << "\n";
        std::cout << "freq";
        for (int a = 0; a < 4; ++a)
            std::cout << " " << a << "=" << (double)freq[a] / (double)w.size();
        std::cout << "\n";
    }
    // save
    std::ostringstream fn;
    fn << "/agent/data/word_" << mode << "_p" << period << "_n" << w.size() << ".txt";
    std::ofstream out(fn.str());
    out << to_string(w) << "\n";
    std::cout << "wrote " << fn.str() << "\n";
    if (w.size() <= 200) std::cout << "word=" << to_string(w) << "\n";
}

// ---------------- periodic ----------------
static void periodic_scan(int pmax, int copies = 3) {
    // For each period p, every word u of length p yields (u)^copies which contains
    // an ordinary cube when copies>=3. We still enumerate to record the cube.
    std::cout << "# Every purely periodic infinite word contains u^3, an ordinary cube.\n";
    std::cout << "# p  words_checked  all_have_cube\n";
    for (int p = 1; p <= pmax; ++p) {
        uint64_t total = 1;
        bool overflow = false;
        for (int i = 0; i < p; ++i) {
            if (total > (1ull << 20)) {
                overflow = true;
                break;
            }
            total *= 4;
        }
        int checked = 0, with_cube = 0;
        bool sampled = overflow || total > 4096;
        int limit = sampled ? 4096 : (int)total;
        std::mt19937 rng(p * 9973u + 11u);
        for (int mask = 0; mask < limit; ++mask) {
            std::vector<u8> u(p);
            if (!sampled) {
                int m = mask;
                for (int i = 0; i < p; ++i) {
                    u[i] = m % 4;
                    m /= 4;
                }
            } else {
                for (int i = 0; i < p; ++i) u[i] = rng() & 3u;
            }
            std::vector<u8> w;
            w.reserve((size_t)copies * (size_t)p);
            for (int r = 0; r < copies; ++r) w.insert(w.end(), u.begin(), u.end());
            // Proof: uuu is an ordinary cube of block length p, hence an additive cube.
            auto cube = find_cube(w);
            if (cube) ++with_cube;
            else die("periodic counterexample");
            ++checked;
        }
        std::cout << p << " " << checked << " yes"
                  << (sampled ? " sampled" : " exhaustive") << "\n";
    }
}

// ---------------- morphism search ----------------
static std::array<std::vector<u8>, 4> decode_uniform(int k, uint32_t code) {
    // 4 images of length k, each k symbols in base 4. Total 4k symbols.
    std::array<std::vector<u8>, 4> h;
    uint32_t c = code;
    // For k>=4 this doesn't fit in 32 bits. Caller must use a different encoding.
    for (int a = 0; a < 4; ++a) {
        h[a].resize(k);
        for (int i = 0; i < k; ++i) {
            h[a][i] = c & 3u;
            c >>= 2;
        }
    }
    return h;
}

static std::string morph_str(const std::array<std::vector<u8>, 4>& h) {
    std::ostringstream o;
    for (int a = 0; a < 4; ++a) {
        if (a) o << " | ";
        o << a << "->" << to_string(h[a]);
    }
    return o.str();
}

static int distinct_image_sums(const std::array<std::vector<u8>, 4>& h) {
    std::array<i64, 4> s;
    for (int a = 0; a < 4; ++a) s[a] = image_sum(h[a]);
    std::sort(s.begin(), s.end());
    return (int)(std::unique(s.begin(), s.end()) - s.begin());
}

// Iterate h from a single letter. Works whether or not h is prolongable.
static int grow_and_check(const std::array<std::vector<u8>, 4>& h, u8 seed, int cap,
                          Cube* out) {
    std::vector<u8> w = {seed};
    while ((int)w.size() < cap) {
        auto nxt = apply_morphism(h, w);
        if (nxt.size() <= w.size()) {
            auto c = find_cube(w);
            if (c && out) *out = *c;
            return c ? (c->i + 3 * c->d) : (int)w.size();
        }
        Builder b;
        bool ok = true;
        for (u8 x : nxt) {
            if (!b.try_push(x)) {
                ok = false;
                break;
            }
            if (b.size() >= cap) break;
        }
        if (!ok) {
            auto c = find_cube_ending_at(b.S, b.size());
            if (c && out) *out = *c;
            return b.size();
        }
        w.swap(b.w);
        if ((int)w.size() >= cap) {
            w.resize(cap);
            return (int)w.size();
        }
    }
    return (int)w.size();
}

static void search_uniform(int k, int cap, uint64_t limit) {
    if (k < 1 || k > 3) die("uniform exhaustive k in 1..3");
    uint64_t nmorph = 1;
    for (int i = 0; i < 4 * k; ++i) nmorph *= 4;
    std::cout << "# exhaustive uniform k=" << k << " morphisms=" << nmorph
              << " cap=" << cap << " (all 4 seeds)\n";
    int best = 0;
    std::string best_s;
    int n_survived = 0, n_sum2 = 0;
    uint64_t scanned = 0;
    for (uint64_t code = 0; code < nmorph; ++code) {
        auto h = decode_uniform(k, (uint32_t)code);
        ++scanned;
        if (limit && scanned > limit) break;
        bool few = distinct_image_sums(h) <= 2;
        if (few) ++n_sum2;
        for (int seed = 0; seed < 4; ++seed) {
            Cube c{};
            int surv = grow_and_check(h, (u8)seed, cap, &c);
            if (surv > best) {
                best = surv;
                best_s = morph_str(h) + " seed=" + std::to_string(seed);
                std::cout << "best " << best << "  " << best_s;
                if (surv < cap) std::cout << " cube i=" << c.i << " d=" << c.d;
                std::cout << "\n";
            }
            if (surv >= cap) {
                ++n_survived;
                std::cout << "SURVIVOR cap=" << cap << " " << morph_str(h)
                          << " seed=" << seed << "\n";
            }
        }
        if ((scanned & ((1ull << 16) - 1)) == 0)
            std::cerr << "# scanned " << scanned << "/" << nmorph << " best=" << best << "\n";
    }
    std::cout << "scanned=" << scanned << " few_sums=" << n_sum2
              << " survivors=" << n_survived << " best=" << best << " " << best_s << "\n";
}

static void search_shape(const std::array<int, 4>& lens, int cap) {
    int total_sym = 0;
    for (int a = 0; a < 4; ++a) total_sym += lens[a];
    uint64_t nmorph = 1;
    for (int i = 0; i < total_sym; ++i) {
        if (nmorph > (1ull << 24)) die("shape too large");
        nmorph *= 4;
    }
    std::cout << "# shape " << lens[0] << "," << lens[1] << "," << lens[2] << "," << lens[3]
              << " morphisms=" << nmorph << " cap=" << cap << "\n";
    int best = 0;
    std::string best_s;
    int n_survived = 0;
    for (uint64_t code = 0; code < nmorph; ++code) {
        std::array<std::vector<u8>, 4> h;
        uint64_t c = code;
        for (int a = 0; a < 4; ++a) {
            h[a].resize(lens[a]);
            for (int i = 0; i < lens[a]; ++i) {
                h[a][i] = (u8)(c & 3u);
                c >>= 2;
            }
        }
        for (int seed = 0; seed < 4; ++seed) {
            Cube cu{};
            int surv = grow_and_check(h, (u8)seed, cap, &cu);
            if (surv > best) {
                best = surv;
                best_s = morph_str(h) + " seed=" + std::to_string(seed);
                std::cout << "best " << best << "  " << best_s;
                if (surv < cap) std::cout << " cube i=" << cu.i << " d=" << cu.d;
                std::cout << "\n";
            }
            if (surv >= cap) {
                ++n_survived;
                std::cout << "SURVIVOR " << morph_str(h) << " seed=" << seed << "\n";
            }
        }
    }
    std::cout << "survivors=" << n_survived << " best=" << best << " " << best_s << "\n";
}

// Random / evolutionary search for larger k.
static std::array<std::vector<u8>, 4> random_morph(std::mt19937& rng, int kmin, int kmax,
                                                   bool uniform) {
    std::uniform_int_distribution<int> len(kmin, kmax);
    std::uniform_int_distribution<int> sym(0, 3);
    std::array<std::vector<u8>, 4> h;
    int ku = len(rng);
    for (int a = 0; a < 4; ++a) {
        int k = uniform ? ku : len(rng);
        h[a].resize(k);
        for (int i = 0; i < k; ++i) h[a][i] = (u8)sym(rng);
    }
    // make prolongable on 0
    if (h[0].empty()) h[0] = {0};
    h[0][0] = 0;
    return h;
}

static void evo_morph(int kmin, int kmax, bool uniform, int cap, uint64_t trials, uint32_t seed) {
    std::mt19937 rng(seed);
    int best = 0;
    std::array<std::vector<u8>, 4> best_h;
    uint64_t n_sum_skip = 0;
    for (uint64_t t = 0; t < trials; ++t) {
        auto h = random_morph(rng, kmin, kmax, uniform);
        if (distinct_image_sums(h) <= 2) {
            ++n_sum_skip;
            continue;
        }
        Cube c{};
        int surv = grow_and_check(h, 0, cap, &c);
        if (surv > best) {
            best = surv;
            best_h = h;
            std::cout << "best " << best << "  " << morph_str(h);
            if (surv < cap) std::cout << " cube i=" << c.i << " d=" << c.d << " sum=" << c.sum;
            std::cout << "\n";
        }
        if (surv >= cap) {
            std::cout << "SURVIVOR " << morph_str(h) << "\n";
        }
        if ((t & 1023ull) == 0 && t) {
            std::cerr << "# trial " << t << " best=" << best << "\n";
        }
    }
    std::cout << "trials=" << trials << " sum_skipped=" << n_sum_skip << " best=" << best
              << " " << morph_str(best_h) << "\n";
}

static void usage() {
    std::cerr
        << "usage:\n"
        << "  acf_tool test\n"
        << "  acf_tool count N\n"
        << "  acf_tool characterize N\n"
        << "  acf_tool grow MODE BUDGET CAP [PERIOD]\n"
        << "      MODE = fixed | fixed-down | updown | drift\n"
        << "  acf_tool periodic PMAX\n"
        << "  acf_tool uniform K CAP [LIMIT]\n"
        << "  acf_tool shape L0,L1,L2,L3 CAP\n"
        << "  acf_tool evo KMIN KMAX uniform|nonuniform CAP TRIALS SEED\n"
        << "  acf_tool deadend NMAX\n"
        << "  acf_tool analyze FILE\n"
        << "  acf_tool verify FILE\n";
}

int main(int argc, char** argv) {
    if (argc < 2) {
        usage();
        return 2;
    }
    std::string cmd = argv[1];
    try {
        if (cmd == "test") return run_tests();
        if (cmd == "count") {
            if (argc < 3) die("count N");
            int n = std::stoi(argv[2]);
            auto counts = count_acf_by_length(n, 4);
            std::cout << "# n count\n";
            for (int i = 0; i <= n; ++i) std::cout << i << " " << counts[i] << "\n";
            return 0;
        }
        if (cmd == "characterize") {
            if (argc < 3) die("characterize N");
            characterize(std::stoi(argv[2]));
            return 0;
        }
        if (cmd == "grow") {
            if (argc < 5) die("grow MODE BUDGET CAP [PERIOD]");
            std::string mode = argv[2];
            uint64_t budget = std::stoull(argv[3]);
            int cap = std::stoi(argv[4]);
            int period = (argc > 5) ? std::stoi(argv[5]) : 1000;
            grow(mode, budget, cap, period);
            return 0;
        }
        if (cmd == "periodic") {
            if (argc < 3) die("periodic PMAX");
            periodic_scan(std::stoi(argv[2]));
            return 0;
        }
        if (cmd == "uniform") {
            if (argc < 4) die("uniform K CAP");
            int k = std::stoi(argv[2]);
            int cap = std::stoi(argv[3]);
            uint64_t limit = (argc > 4) ? std::stoull(argv[4]) : 0;
            search_uniform(k, cap, limit);
            return 0;
        }
        if (cmd == "shape") {
            if (argc < 4) die("shape L0,L1,L2,L3 CAP");
            std::array<int, 4> lens{};
            {
                std::string s = argv[2];
                int idx = 0;
                std::string cur;
                for (size_t i = 0; i <= s.size(); ++i) {
                    if (i == s.size() || s[i] == ',') {
                        if (idx >= 4) die("need 4 lengths");
                        lens[idx++] = std::stoi(cur);
                        cur.clear();
                    } else
                        cur.push_back(s[i]);
                }
                if (idx != 4) die("need 4 lengths");
            }
            search_shape(lens, std::stoi(argv[3]));
            return 0;
        }
        if (cmd == "evo") {
            if (argc < 8) die("evo KMIN KMAX uniform|nonuniform CAP TRIALS SEED");
            int kmin = std::stoi(argv[2]);
            int kmax = std::stoi(argv[3]);
            bool uni = std::string(argv[4]) == "uniform";
            int cap = std::stoi(argv[5]);
            uint64_t trials = std::stoull(argv[6]);
            uint32_t seed = (uint32_t)std::stoul(argv[7]);
            evo_morph(kmin, kmax, uni, cap, trials, seed);
            return 0;
        }
        if (cmd == "deadend") {
            if (argc < 3) die("deadend NMAX");
            int nmax = std::stoi(argv[2]);
            std::vector<std::string> examples;
            int first = -1;
            std::vector<uint64_t> n_dead(nmax + 1, 0);
            Builder b;
            std::function<void()> rec = [&]() {
                int n = b.size();
                if (n == nmax) return;
                int children = 0;
                for (int a = 0; a < 4; ++a) {
                    if (b.try_push((u8)a)) {
                        ++children;
                        rec();
                        b.pop();
                    }
                }
                if (children == 0) {
                    n_dead[n]++;
                    if (first < 0) first = n;
                    if ((int)examples.size() < 8) examples.push_back(to_string(b.w));
                }
            };
            rec();
            std::cout << "# n n_deadends\n";
            for (int n = 0; n <= nmax; ++n) std::cout << n << " " << n_dead[n] << "\n";
            std::cout << "first_deadend_length=" << first << "\n";
            for (auto& e : examples) std::cout << "example " << e.size() << " " << e << "\n";
            return 0;
        }
        if (cmd == "analyze") {
            if (argc < 3) die("analyze FILE");
            std::ifstream in(argv[2]);
            std::stringstream buf;
            buf << in.rdbuf();
            auto w = parse_digits(buf.str());
            auto S = prefix_sums(w);
            auto cube = find_cube(w);
            std::array<uint64_t, 4> freq{{0, 0, 0, 0}};
            for (u8 a : w) freq[a]++;
            int n = (int)w.size();
            double mean = n ? (double)S.back() / n : 0;
            double maxdev = 0, sumdev2 = 0;
            for (int k = 1; k <= n; ++k) {
                double e = S[k] - mean * k;
                maxdev = std::max(maxdev, std::abs(e));
                sumdev2 += e * e;
            }
            std::cout << "n=" << n << " acf=" << (!cube ? "yes" : "NO") << " mean=" << mean << "\n";
            std::cout << "freq";
            for (int a = 0; a < 4; ++a)
                std::cout << " " << a << "=" << (n ? (double)freq[a] / n : 0);
            std::cout << "\n";
            std::cout << "max_|S(k)-mean*k|=" << maxdev
                      << " rms_dev=" << std::sqrt(sumdev2 / std::max(1, n)) << "\n";
            int mmax = std::min(n, 10);
            std::cout << "# m complexity p(m)\n";
            for (int m = 1; m <= mmax; ++m) {
                std::vector<uint32_t> seen;
                seen.reserve(n - m + 1);
                for (int i = 0; i + m <= n; ++i) {
                    uint32_t code = 0;
                    for (int t = 0; t < m; ++t) code = code * 4u + w[i + t];
                    seen.push_back(code);
                }
                std::sort(seen.begin(), seen.end());
                seen.erase(std::unique(seen.begin(), seen.end()), seen.end());
                std::cout << m << " " << seen.size() << " / " << (1 << (2 * m)) << "\n";
            }
            return 0;
        }
        if (cmd == "verify") {
            if (argc < 3) die("verify FILE");
            std::ifstream in(argv[2]);
            std::stringstream buf;
            buf << in.rdbuf();
            auto w = parse_digits(buf.str());
            auto t0 = std::chrono::steady_clock::now();
            auto c = find_cube(w);
            auto t1 = std::chrono::steady_clock::now();
            std::cout << "n=" << w.size() << " acf=" << (!c ? "yes" : "NO")
                      << " time_s=" << std::chrono::duration<double>(t1 - t0).count() << "\n";
            if (c) std::cout << "cube i=" << c->i << " d=" << c->d << " sum=" << c->sum << "\n";
            return c ? 1 : 0;
        }
        usage();
        return 2;
    } catch (const std::exception& e) {
        die(e.what());
    }
}
