// Sample Cassaigne-shaped morphisms of size k over {0,1,2,3}:
//   0 -> (k letters starting with 0), 1 -> (k letters), 2 -> (1 letter), 3 -> (k letters)
// and iterate from seed 0. Liétard already excluded k<=6 uniformly; this is the
// non-uniform "φ-shape" at k>=7, attacked adversarially.
#include "acf.hpp"
#include "search.hpp"
#include <iostream>
#include <random>
using namespace acf;

static int iterate_until_cube(const std::array<std::vector<u8>, 4>& h, int cap) {
    std::vector<u8> w = {0};
    if (h[0].empty() || h[0][0] != 0) return 0;  // not prolongable on 0
    Builder b;
    // generate by iterating the morphism until length >= cap, checking incrementally
    // Easier: expand h^n(0) into a string, push letters.
    while ((int)w.size() < cap) {
        std::vector<u8> nxt;
        nxt.reserve(w.size() * 3);
        for (u8 a : w) {
            const auto& img = h[a];
            nxt.insert(nxt.end(), img.begin(), img.end());
        }
        if (nxt.size() <= w.size()) return b.size();
        w.swap(nxt);
    }
    b = Builder();
    for (int i = 0; i < cap && i < (int)w.size(); ++i) {
        if (!b.try_push(w[i])) return b.size();
    }
    return b.size();
}

int main(int argc, char** argv) {
    int k = (argc > 1) ? std::atoi(argv[1]) : 7;
    int trials = (argc > 2) ? std::atoi(argv[2]) : 20000;
    int cap = (argc > 3) ? std::atoi(argv[3]) : 800;
    uint32_t seed = (argc > 4) ? (uint32_t)std::stoul(argv[4]) : 7u;
    std::mt19937 rng(seed);
    std::uniform_int_distribution<int> let(0, 3);
    int best = -1;
    int n200 = 0, n400 = 0;
    std::array<std::vector<u8>, 4> best_h;
    for (int t = 0; t < trials; ++t) {
        std::array<std::vector<u8>, 4> h;
        h[0].push_back(0);
        for (int i = 1; i < k; ++i) h[0].push_back((u8)let(rng));
        for (int i = 0; i < k; ++i) h[1].push_back((u8)let(rng));
        h[2].push_back((u8)let(rng));
        for (int i = 0; i < k; ++i) h[3].push_back((u8)let(rng));
        int L = iterate_until_cube(h, cap);
        if (L >= 200) ++n200;
        if (L >= 400) ++n400;
        if (L > best) {
            best = L;
            best_h = h;
            std::cout << "best " << best << " trial " << t << "  0->" << to_string(h[0])
                      << " 1->" << to_string(h[1]) << " 2->" << to_string(h[2])
                      << " 3->" << to_string(h[3]) << "\n"
                      << std::flush;
        }
    }
    std::cout << "k=" << k << " trials=" << trials << " cap=" << cap << " best=" << best
              << " n>=200=" << n200 << " n>=400=" << n400 << "\n";
    return 0;
}
