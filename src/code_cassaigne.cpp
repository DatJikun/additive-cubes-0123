// Recode the Cassaigne–Currie–Schaeffer–Shallit infinite ACF word over {0,1,3,4}
// by every letter-to-letter map into {0,1,2,3}, and measure the first additive cube.
// Also recode by affine-like maps and 2-block (pair) codes of small range.
#include "acf.hpp"
#include "search.hpp"
#include <algorithm>
#include <functional>
#include <iostream>
#include <map>
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

static int first_cube_len(const std::vector<u8>& src, const std::array<u8, 5>& code, int cap) {
    Builder b;
    for (int i = 0; i < cap && i < (int)src.size(); ++i) {
        u8 a = src[i];
        u8 x = code[a];
        if (!b.try_push(x)) return b.size();  // length of ACF prefix (failed to add)
    }
    return b.size();
}

int main() {
    const int SRC = 20000;
    const int CAP = 8000;
    auto src = cassaigne(SRC);
    std::cout << "cassaigne prefix " << src.size() << " ACF on {0,1,3,4}? "
              << (is_acf(src) ? "yes" : "NO") << "\n";

    // sanity: identity on {0,1,3,4} should survive CAP if we allowed 4
    // Here target is {0,1,2,3}. All 4^4 maps from {0,1,3,4} → {0,1,2,3}.
    const u8 letters[4] = {0, 1, 3, 4};
    int best = -1;
    std::array<u8, 5> best_code{};
    int n_survive_100 = 0, n_survive_1000 = 0, n_full = 0;
    std::map<int, int> len_hist_bucket;

    for (int m = 0; m < 256; ++m) {
        std::array<u8, 5> code{};
        int t = m;
        for (int j = 0; j < 4; ++j) {
            code[letters[j]] = t % 4;
            t /= 4;
        }
        int L = first_cube_len(src, code, CAP);
        if (L > best) {
            best = L;
            best_code = code;
        }
        if (L >= 100) ++n_survive_100;
        if (L >= 1000) ++n_survive_1000;
        if (L >= CAP) ++n_full;
        int bucket = (L / 50) * 50;
        len_hist_bucket[bucket]++;
    }
    std::cout << "letter-to-letter maps: 256\n";
    std::cout << "best ACF prefix length (cap " << CAP << "): " << best << "\n";
    std::cout << "best code: 0->" << (int)best_code[0] << " 1->" << (int)best_code[1]
              << " 3->" << (int)best_code[3] << " 4->" << (int)best_code[4] << "\n";
    std::cout << "survive>=100: " << n_survive_100 << " survive>=1000: " << n_survive_1000
              << " hit_cap: " << n_full << "\n";
    std::cout << "length buckets:\n";
    for (auto& kv : len_hist_bucket) std::cout << "  [" << kv.first << "," << kv.first + 49
                                               << "]: " << kv.second << "\n";

    // Pair coding: map each adjacent pair in the Cassaigne word (over 4 letters)
    // to a letter in {0,1,2,3} via (4*x+y) % 4, (x+y)%4, (x*y)%4, etc. — a few
    // structured pair codes, plus a scan of linear pair codes ax+by+c mod 4.
    auto pair_run = [&](const std::function<u8(u8, u8)>& f, const char* name) {
        Builder b;
        for (int i = 0; i + 1 < CAP && i + 1 < (int)src.size(); ++i) {
            if (!b.try_push(f(src[i], src[i + 1]))) {
                std::cout << "pair " << name << " ACF prefix " << b.size() << "\n";
                return;
            }
        }
        std::cout << "pair " << name << " ACF prefix " << b.size() << " (cap)\n";
    };
    pair_run([](u8 x, u8 y) { return (u8)((x + y) % 4); }, "x+y mod 4");
    pair_run([](u8 x, u8 y) { return (u8)((x + 2 * y) % 4); }, "x+2y mod 4");
    pair_run([](u8 x, u8 y) { return (u8)((2 * x + y) % 4); }, "2x+y mod 4");
    pair_run([](u8 x, u8 y) { return (u8)((x * y) % 4); }, "xy mod 4");
    pair_run([](u8 x, u8 y) { return (u8)((x + 3 * y + 1) % 4); }, "x+3y+1 mod 4");
    pair_run([](u8 x, u8 y) { return (u8)(std::min(x, y) % 4); }, "min");
    pair_run([](u8 x, u8 y) { return (u8)((x ^ y) % 4); }, "xor");
    pair_run([](u8 x, u8 y) { return (u8)((4 + (int)y - (int)x) % 4); }, "y-x mod 4");

    int best_lin = -1;
    int ba = 0, bb = 0, bc = 0;
    for (int a = 0; a < 4; ++a)
        for (int b = 0; b < 4; ++b)
            for (int c = 0; c < 4; ++c) {
                Builder B;
                int L = 0;
                for (int i = 0; i + 1 < CAP && i + 1 < (int)src.size(); ++i) {
                    u8 x = (u8)((a * src[i] + b * src[i + 1] + c) % 4);
                    if (!B.try_push(x)) {
                        L = B.size();
                        break;
                    }
                    L = B.size();
                }
                if (L > best_lin) {
                    best_lin = L;
                    ba = a;
                    bb = b;
                    bc = c;
                }
            }
    std::cout << "best linear pair code ax+by+c mod 4: a=" << ba << " b=" << bb << " c=" << bc
              << " ACF prefix " << best_lin << "\n";

    // All 24 bijections {0,1,3,4} -> {0,1,2,3}
    std::cout << "\n# bijections (perm of 0123 assigned to 0,1,3,4)\n";
    int bi_best = -1;
    int perm[4] = {0, 1, 2, 3};
    do {
        std::array<u8, 5> code{};
        code[0] = (u8)perm[0];
        code[1] = (u8)perm[1];
        code[3] = (u8)perm[2];
        code[4] = (u8)perm[3];
        int L = first_cube_len(src, code, CAP);
        std::cout << "  0->" << perm[0] << " 1->" << perm[1] << " 3->" << perm[2]
                  << " 4->" << perm[3] << " L=" << L << "\n";
        if (L > bi_best) bi_best = L;
    } while (std::next_permutation(perm, perm + 4));
    std::cout << "best bijection L=" << bi_best << "\n";
    return 0;
}
