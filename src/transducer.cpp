// Evolutionary search: 2-state letter-to-letter transducers recoding
// the Cassaigne word over {0,1,3,4} into {0,1,2,3}.
#include "acf.hpp"
#include "search.hpp"
#include <iostream>
#include <random>
using namespace acf;

static std::vector<u8> cassaigne(int n) {
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

// trans[s][inp] = (new_state << 2) | out, inp encoded 0,1,3,4 -> 0,1,2,3
static int recode_len(const std::vector<u8>& src, const std::array<std::array<u8, 4>, 2>& T, int cap) {
    Builder b;
    int s = 0;
    static const int idx[5] = {-1, 1, -1, 2, 3};  // 0->0 hardcoded
    for (int i = 0; i < cap && i < (int)src.size(); ++i) {
        int inp = src[i];
        int j = (inp == 0) ? 0 : idx[inp];
        u8 code = T[s][j];
        u8 out = code & 3;
        int ns = code >> 2;
        if (!b.try_push(out)) return b.size();
        s = ns;
    }
    return b.size();
}

int main(int argc, char** argv) {
    int trials = (argc > 1) ? std::atoi(argv[1]) : 50000;
    int cap = (argc > 2) ? std::atoi(argv[2]) : 1500;
    uint32_t seed = (argc > 3) ? (uint32_t)std::stoul(argv[3]) : 1u;
    auto src = cassaigne(std::max(cap + 10, 4000));
    std::mt19937 rng(seed);
    std::uniform_int_distribution<int> bit(0, 7);  // 2-state * 4-out
    int best = -1;
    std::array<std::array<u8, 4>, 2> bestT{};
    int n100 = 0, n500 = 0, n1000 = 0;
    for (int t = 0; t < trials; ++t) {
        std::array<std::array<u8, 4>, 2> T{};
        for (int s = 0; s < 2; ++s)
            for (int j = 0; j < 4; ++j) T[s][j] = (u8)bit(rng);
        int L = recode_len(src, T, cap);
        if (L >= 100) ++n100;
        if (L >= 500) ++n500;
        if (L >= 1000) ++n1000;
        if (L > best) {
            best = L;
            bestT = T;
            std::cout << "new best " << best << " trial " << t << " T=";
            for (int s = 0; s < 2; ++s)
                for (int j = 0; j < 4; ++j) std::cout << (int)T[s][j] << (s + j == 5 ? "" : ",");
            std::cout << "\n" << std::flush;
        }
    }
    std::cout << "trials=" << trials << " cap=" << cap << " best=" << best
              << " n>=100=" << n100 << " n>=500=" << n500 << " n>=1000=" << n1000 << "\n";
    std::cout << "bestT state0: ";
    for (int j = 0; j < 4; ++j) std::cout << (int)bestT[0][j] << " ";
    std::cout << " state1: ";
    for (int j = 0; j < 4; ++j) std::cout << (int)bestT[1][j] << " ";
    std::cout << "\n";
    return 0;
}
