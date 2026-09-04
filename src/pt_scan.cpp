// Exhaustive m=3 primitive/prolongable ACF-prefix census on {0,1,2,3}.
// Independent of Python. Incremental cubes-ending-at. Exact integers.
#include "acf.hpp"
#include <algorithm>
#include <array>
#include <cstdint>
#include <iostream>
#include <vector>
using namespace acf;

static void apply3(const u8 img[4][3], const std::vector<u8>& w, std::vector<u8>& nxt) {
    nxt.clear();
    nxt.reserve(w.size() * 3);
    for (u8 a : w) {
        nxt.push_back(img[a][0]);
        nxt.push_back(img[a][1]);
        nxt.push_back(img[a][2]);
    }
}

static bool primitive4(const u8 img[4][3]) {
    int M[4][4] = {};
    for (int a = 0; a < 4; ++a)
        for (int r = 0; r < 3; ++r) M[img[a][r]][a]++;
    int acc[4][4];
    for (int i = 0; i < 4; ++i)
        for (int j = 0; j < 4; ++j) acc[i][j] = M[i][j];
    for (int p = 0; p < 12; ++p) {
        bool ok = true;
        for (int i = 0; i < 4; ++i)
            for (int j = 0; j < 4; ++j)
                if (acc[i][j] <= 0) ok = false;
        if (ok) return true;
        int nxt[4][4] = {};
        for (int i = 0; i < 4; ++i)
            for (int k = 0; k < 4; ++k)
                for (int j = 0; j < 4; ++j)
                    nxt[i][j] += acc[i][k] * M[k][j];
        for (int i = 0; i < 4; ++i)
            for (int j = 0; j < 4; ++j) acc[i][j] = nxt[i][j];
    }
    return false;
}

// Grow the prolongable iterate, return ACF prefix (cap if no cube).
static int acf_prefix3(const u8 img[4][3], u8 seed, int cap) {
    std::vector<u8> w = {seed};
    std::vector<i64> S = {0, (i64)seed};
    int n = 1;
    std::vector<u8> nxt;
    while (n < cap) {
        apply3(img, w, nxt);
        if ((int)nxt.size() <= n) break;
        for (int i = n; i < (int)nxt.size() && i < cap; ++i) {
            S.push_back(S.back() + nxt[i]);
            int nn = i + 1;
            if (has_cube_ending_at(S, nn)) return nn - 1;
        }
        n = std::min((int)nxt.size(), cap);
        w.swap(nxt);
        if ((int)w.size() > cap) w.resize(cap);
    }
    return n;
}

int main() {
    const int cap = 243; // 3^5
    const uint32_t N = 1u << 24;
    int hist[256] = {};
    int n_prol = 0, n_prim = 0, n_3sum = 0, n_tried = 0;
    int n_surv = 0, best = -1, best_code = -1, best_seed = -1;
    int n_cube = 0;
    int n_ge81 = 0, n_ge243 = 0;
    std::vector<int> surv_codes;
    std::vector<u8> surv_seeds;

    for (uint32_t code = 0; code < N; ++code) {
        u8 img[4][3];
        uint32_t t = code;
        for (int a = 0; a < 4; ++a)
            for (int r = 0; r < 3; ++r) {
                img[a][r] = (u8)(t & 3u);
                t >>= 2;
            }
        bool prol_any = false;
        u8 seeds[4];
        int ns = 0;
        for (int a = 0; a < 4; ++a)
            if (img[a][0] == a) {
                seeds[ns++] = (u8)a;
                prol_any = true;
            }
        if (!prol_any) continue;
        ++n_prol;

        int T[4];
        for (int a = 0; a < 4; ++a) T[a] = (int)img[a][0] + img[a][1] + img[a][2];
        int nsum = 0;
        bool seen[13] = {};
        for (int a = 0; a < 4; ++a) {
            int x = T[a];
            if (x < 0 || x > 12) continue;
            if (!seen[x]) {
                seen[x] = true;
                ++nsum;
            }
        }
        if (nsum >= 3) ++n_3sum;
        bool prim = primitive4(img);
        if (prim) ++n_prim;
        if (nsum < 3 || !prim) continue; // B(m) for primitive, ≥3 image sums
        ++n_tried;

        int local = 0;
        int lseed = -1;
        bool surv = false;
        for (int s = 0; s < ns; ++s) {
            int L = acf_prefix3(img, seeds[s], cap);
            if (L > local) {
                local = L;
                lseed = seeds[s];
            }
            if (L >= cap) surv = true;
        }
        if (surv) {
            ++n_surv;
            surv_codes.push_back((int)code);
            surv_seeds.push_back((u8)lseed);
        } else {
            ++n_cube;
            if (local < 256) hist[local]++;
            if (local >= 81) ++n_ge81;
            if (local >= 243) ++n_ge243;
        }
        if (local > best) {
            best = local;
            best_code = (int)code;
            best_seed = lseed;
        }
    }

    std::cout << "N " << N << "\n";
    std::cout << "n_prol " << n_prol << "\n";
    std::cout << "n_prim " << n_prim << "\n";
    std::cout << "n_3sum_prol " << n_3sum << "\n";
    std::cout << "n_tried_prim_3sum " << n_tried << "\n";
    std::cout << "n_cube " << n_cube << "\n";
    std::cout << "n_surv_cap " << n_surv << "\n";
    std::cout << "cap " << cap << "\n";
    std::cout << "best_acf " << best << "\n";
    std::cout << "best_code " << best_code << "\n";
    std::cout << "best_seed " << best_seed << "\n";
    std::cout << "n_ge81 " << n_ge81 << "\n";
    std::cout << "n_ge243 " << n_ge243 << "\n";

    // decode best
    if (best_code >= 0) {
        uint32_t t = (uint32_t)best_code;
        std::cout << "best_imgs";
        for (int a = 0; a < 4; ++a) {
            std::cout << " ";
            for (int r = 0; r < 3; ++r) {
                std::cout << (t & 3u);
                t >>= 2;
            }
        }
        std::cout << "\n";
    }

    int p50 = 0, p90 = 0, p99 = 0, acc = 0;
    for (int L = 0; L < 256; ++L) acc += hist[L];
    int tot = acc;
    acc = 0;
    for (int L = 0; L < 256; ++L) {
        acc += hist[L];
        if (p50 == 0 && tot && acc * 2 >= tot) p50 = L;
        if (p90 == 0 && tot && acc * 10 >= tot * 9) p90 = L;
        if (p99 == 0 && tot && acc * 100 >= tot * 99) p99 = L;
    }
    std::cout << "hist_p50 " << p50 << "\n";
    std::cout << "hist_p90 " << p90 << "\n";
    std::cout << "hist_p99 " << p99 << "\n";
    std::cout << "hist_nonzero";
    for (int L = 0; L < 256; ++L)
        if (hist[L]) std::cout << " " << L << ":" << hist[L];
    std::cout << "\n";

    // Push cap-243 survivors to 3^7=2187, then 3^8=6561 if needed.
    const int cap2 = 2187;
    int n_surv2 = 0, best2 = -1, best2_code = -1, best2_seed = -1;
    std::cout << "SURV243 " << (int)surv_codes.size() << "\n";
    for (size_t k = 0; k < surv_codes.size(); ++k) {
        uint32_t code = (uint32_t)surv_codes[k];
        u8 img[4][3];
        uint32_t t = code;
        for (int a = 0; a < 4; ++a)
            for (int r = 0; r < 3; ++r) {
                img[a][r] = (u8)(t & 3u);
                t >>= 2;
            }
        int L = acf_prefix3(img, surv_seeds[k], cap2);
        std::cout << "push " << surv_codes[k] << " seed " << (int)surv_seeds[k] << " acf " << L << "\n";
        if (L >= cap2) {
            ++n_surv2;
            if (L > best2) {
                best2 = L;
                best2_code = surv_codes[k];
                best2_seed = surv_seeds[k];
            }
        } else if (L > best2) {
            best2 = L;
            best2_code = surv_codes[k];
            best2_seed = surv_seeds[k];
        }
    }
    std::cout << "n_surv_2187 " << n_surv2 << "\n";
    std::cout << "best2_acf " << best2 << "\n";
    std::cout << "best2_code " << best2_code << "\n";
    std::cout << "best2_seed " << best2_seed << "\n";
    return 0;
}
