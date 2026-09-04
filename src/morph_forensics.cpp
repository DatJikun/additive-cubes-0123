// Forensics: for a uniform morphism, iterate a prolongable seed and classify
// the first additive cube (aligned / offset / substitution level).
#include "acf.hpp"
#include "search.hpp"
#include <iostream>
#include <map>
using namespace acf;

struct Morph {
    int m;
    std::array<std::vector<u8>, 4> h;
    std::array<int, 4> T{};
    int n_sums = 0;
};

static Morph parse_morph(int m, const std::array<std::string, 4>& imgs) {
    Morph s;
    s.m = m;
    std::array<int, 4> seen{};
    for (int a = 0; a < 4; ++a) {
        for (char c : imgs[a]) {
            u8 x = (u8)(c - '0');
            s.h[a].push_back(x);
            s.T[a] += x;
        }
        if ((int)s.h[a].size() != m) {
            std::cerr << "bad length\n";
            std::exit(1);
        }
    }
    for (int a = 0; a < 4; ++a) {
        bool uniq = true;
        for (int b = 0; b < a; ++b)
            if (s.T[a] == s.T[b]) uniq = false;
        if (uniq) s.n_sums++;
    }
    // recount distinct
    s.n_sums = 0;
    for (int a = 0; a < 4; ++a) {
        bool seenb = false;
        for (int b = 0; b < a; ++b)
            if (s.T[a] == s.T[b]) seenb = true;
        if (!seenb) s.n_sums++;
    }
    return s;
}

static std::vector<u8> iterate(const Morph& s, int seed, int cap) {
    std::vector<u8> w = {(u8)seed};
    while ((int)w.size() < cap) {
        std::vector<u8> nxt;
        for (u8 a : w) nxt.insert(nxt.end(), s.h[a].begin(), s.h[a].end());
        if (nxt.size() <= w.size()) break;
        w.swap(nxt);
    }
    if ((int)w.size() > cap) w.resize(cap);
    return w;
}

static int first_acf(const std::vector<u8>& src) {
    Builder b;
    for (size_t i = 0; i < src.size(); ++i) {
        if (!b.try_push(src[i])) return (int)i;  // ACF prefix length
    }
    return (int)src.size();
}

static void classify_cube(const std::vector<u8>& w, int m) {
    auto c = find_cube(w);
    if (!c) {
        std::cout << "no cube in prefix " << w.size() << "\n";
        return;
    }
    int i = c->i, d = c->d;
    int level = 0, pk = 1;
    while (pk * m <= d) {
        pk *= m;
        ++level;
    }
    std::cout << "cube i=" << i << " d=" << d << " sum=" << c->sum
              << " level k with m^k <= d < m^{k+1}: k=" << level << " m^k=" << pk
              << " i mod m=" << (i % m) << " d mod m=" << (d % m)
              << " i mod m^k=" << (pk ? i % pk : 0) << " d mod m^k=" << (pk ? d % pk : 0)
              << " aligned_m=" << (i % m == 0 && d % m == 0)
              << " aligned_mk=" << (pk && i % pk == 0 && d % pk == 0) << "\n";
}

// Exhaustive 2-uniform prolongable morphisms: classify first-cube alignment.
// Search space: 4^8 = 65536 morphisms, 4 seeds, cap 250. Already known to all die;
// here we only record the ALIGNMENT signature, not the non-existence.
static void scan_uniform2(int cap) {
    uint64_t n_aligned = 0, n_unaligned = 0, n_d_div = 0;
    int best = -1;
    std::map<int, int> dmod;
    std::array<std::string, 4> best_img;
    int best_seed = -1;
    for (int code = 0; code < 65536; ++code) {
        Morph s;
        s.m = 2;
        int t = code;
        std::array<int, 4> usedT{};
        for (int a = 0; a < 4; ++a) {
            s.h[a] = {(u8)(t % 4), (u8)((t / 4) % 4)};
            t /= 16;
            s.T[a] = s.h[a][0] + s.h[a][1];
        }
        s.n_sums = 0;
        for (int a = 0; a < 4; ++a) {
            bool u = true;
            for (int b = 0; b < a; ++b)
                if (s.T[a] == s.T[b]) u = false;
            if (u) s.n_sums++;
        }
        for (int seed = 0; seed < 4; ++seed) {
            if (s.h[seed][0] != seed) continue;  // prolongable
            auto w = iterate(s, seed, cap);
            Builder b;
            int L = 0;
            bool ok = true;
            for (u8 x : w) {
                if (!b.try_push(x)) {
                    ok = false;
                    L = b.size();
                    break;
                }
                L = b.size();
            }
            if (ok) {
                // survived cap — record
                std::cout << "SURVIVOR cap " << cap << " seed " << seed << "\n";
                continue;
            }
            // cube ending at L+1 was popped; reconstruct from iterate prefix of L+3
            std::vector<u8> pref(w.begin(), w.begin() + std::min((int)w.size(), L + 60));
            auto cube = find_cube(pref);
            if (!cube) continue;
            if (cube->i % 2 == 0 && cube->d % 2 == 0) ++n_aligned;
            else ++n_unaligned;
            if (cube->d % 2 == 0) ++n_d_div;
            dmod[cube->d % 2]++;
            if (L > best) {
                best = L;
                best_seed = seed;
                for (int a = 0; a < 4; ++a) best_img[a] = to_string(s.h[a]);
            }
        }
    }
    std::cout << "2-uniform prolongable scan cap=" << cap
              << " aligned=" << n_aligned << " unaligned=" << n_unaligned
              << " d_even=" << n_d_div << " best_prefix=" << best << " seed=" << best_seed
              << " imgs";
    for (int a = 0; a < 4; ++a) std::cout << " " << a << "->" << best_img[a];
    std::cout << "\n";
}

int main(int argc, char** argv) {
    std::string cmd = (argc > 1) ? argv[1] : "scan2";
    if (cmd == "scan2") {
        scan_uniform2(argc > 2 ? std::atoi(argv[2]) : 250);
        return 0;
    }
    if (cmd == "one") {
        // usage: one m img0 img1 img2 img3 seed cap
        int m = std::atoi(argv[2]);
        Morph s = parse_morph(m, {argv[3], argv[4], argv[5], argv[6]});
        int seed = std::atoi(argv[7]);
        int cap = std::atoi(argv[8]);
        std::cout << "T=(" << s.T[0] << "," << s.T[1] << "," << s.T[2] << "," << s.T[3]
                  << ") n_sums=" << s.n_sums << "\n";
        auto w = iterate(s, seed, cap);
        int L = first_acf(w);
        std::cout << "ACF prefix " << L << " / generated " << w.size() << "\n";
        std::vector<u8> pref(w.begin(), w.begin() + std::min((int)w.size(), L + 80));
        classify_cube(pref, m);
        // independent full check
        auto c2 = find_cube(pref);
        bool brute = brute_is_acf(pref);
        std::cout << "brute_is_acf(pref)=" << brute << " find_cube="
                  << (c2 ? "yes" : "no") << "\n";
        return 0;
    }
    std::cerr << "usage: morph_forensics scan2 [cap] | one m i0 i1 i2 i3 seed cap\n";
    return 1;
}
