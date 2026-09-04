// Independent C++ dual of the m=3 census claims.
// Does NOT re-run the 16,777,216 ACF search. It:
//   1. recounts prolongable / primitive / 3-sum filters (no ACF);
//   2. rebuilds every cap-243 survivor from data/pt_scan3.txt and recomputes
//      ACF prefixes at cap 2187, trying every prolongable seed;
//   3. lex-first cube of the maximizer via find_cube (not only ending-at);
//   4. famous 2-uniform ACF prefix and a few p_T values.
#include "acf.hpp"
#include <array>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <set>
#include <sstream>
#include <string>
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

static void decode(uint32_t code, u8 img[4][3]) {
    uint32_t t = code;
    for (int a = 0; a < 4; ++a)
        for (int r = 0; r < 3; ++r) {
            img[a][r] = (u8)(t & 3u);
            t >>= 2;
        }
}

static int n_image_sums(const u8 img[4][3]) {
    bool seen[13] = {};
    int nsum = 0;
    for (int a = 0; a < 4; ++a) {
        int x = (int)img[a][0] + img[a][1] + img[a][2];
        if (x < 0 || x > 12) continue;
        if (!seen[x]) {
            seen[x] = true;
            ++nsum;
        }
    }
    return nsum;
}

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

static std::vector<u8> iterate3(const u8 img[4][3], u8 seed, int cap) {
    std::vector<u8> w = {seed};
    std::vector<u8> nxt;
    while ((int)w.size() < cap) {
        apply3(img, w, nxt);
        if (nxt.size() <= w.size()) break;
        w.swap(nxt);
    }
    if ((int)w.size() > cap) w.resize(cap);
    return w;
}

static int pT_at(const std::vector<u8>& w, int n) {
    if (n <= 0 || n > (int)w.size()) return 0;
    auto S = prefix_sums(w);
    std::set<i64> vals;
    for (int i = 0; i + n <= (int)w.size(); ++i) vals.insert(S[i + n] - S[i]);
    return (int)vals.size();
}

static int diam_at(const std::vector<u8>& w, int n) {
    if (n <= 0 || n > (int)w.size()) return 0;
    auto S = prefix_sums(w);
    i64 L = S[n] - S[0], H = L;
    for (int i = 0; i + n <= (int)w.size(); ++i) {
        i64 v = S[i + n] - S[i];
        if (v < L) L = v;
        if (v > H) H = v;
    }
    return (int)(H - L);
}

int main() {
    int fails = 0;
    const uint32_t N = 1u << 24;
    int n_prol = 0, n_prim = 0, n_3sum_prol = 0, n_tried = 0;
    for (uint32_t code = 0; code < N; ++code) {
        u8 img[4][3];
        decode(code, img);
        bool prol = false;
        for (int a = 0; a < 4; ++a)
            if (img[a][0] == a) prol = true;
        if (!prol) continue;
        ++n_prol;
        int nsum = n_image_sums(img);
        if (nsum >= 3) ++n_3sum_prol;
        bool prim = primitive4(img);
        if (prim) ++n_prim;
        if (nsum >= 3 && prim) ++n_tried;
    }
    std::cout << "filter_n_prol " << n_prol << "\n";
    std::cout << "filter_n_prim " << n_prim << "\n";
    std::cout << "filter_n_3sum_prol " << n_3sum_prol << "\n";
    std::cout << "filter_n_tried " << n_tried << "\n";
    if (n_prol != 11468800) {
        std::cout << "FAIL filter n_prol\n";
        ++fails;
    }
    if (n_prim != 6993126) {
        std::cout << "FAIL filter n_prim\n";
        ++fails;
    }
    if (n_3sum_prol != 9992241) {
        std::cout << "FAIL filter n_3sum_prol\n";
        ++fails;
    }
    if (n_tried != 6116013) {
        std::cout << "FAIL filter n_tried\n";
        ++fails;
    }

    std::ifstream in("data/pt_scan3.txt");
    if (!in) {
        std::cout << "FAIL missing data/pt_scan3.txt\n";
        return 1;
    }
    std::vector<std::pair<int, int>> pushes; // code, recorded seed
    int file_best2 = -1, file_best2_code = -1, file_best2_seed = -1;
    int file_nsurv2187 = -1, file_surv243 = -1;
    std::string line;
    while (std::getline(in, line)) {
        std::istringstream iss(line);
        std::string key;
        iss >> key;
        if (key == "push") {
            int code, seed, acf;
            std::string sseed, sacf;
            iss >> code >> sseed >> seed >> sacf >> acf;
            pushes.push_back({code, seed});
        } else if (key == "n_surv_2187") {
            iss >> file_nsurv2187;
        } else if (key == "best2_acf") {
            iss >> file_best2;
        } else if (key == "best2_code") {
            iss >> file_best2_code;
        } else if (key == "best2_seed") {
            iss >> file_best2_seed;
        } else if (key == "SURV243") {
            iss >> file_surv243;
        }
    }
    std::cout << "file_push_lines " << (int)pushes.size() << "\n";
    if ((int)pushes.size() != 86 || file_surv243 != 86) {
        std::cout << "FAIL survivor count " << pushes.size() << " SURV243 " << file_surv243 << "\n";
        ++fails;
    }

    int best = -1, best_code = -1, best_seed = -1, n_surv2 = 0;
    const int cap2 = 2187;
    for (auto [code, rec_seed] : pushes) {
        u8 img[4][3];
        decode((uint32_t)code, img);
        if (!primitive4(img) || n_image_sums(img) < 3) {
            std::cout << "FAIL survivor not prim/3sum " << code << "\n";
            ++fails;
            continue;
        }
        int local = 0, lseed = -1;
        for (int a = 0; a < 4; ++a) {
            if (img[a][0] != a) continue;
            int L = acf_prefix3(img, (u8)a, cap2);
            if (L > local) {
                local = L;
                lseed = a;
            }
        }
        if (local >= cap2) ++n_surv2;
        if (local > best) {
            best = local;
            best_code = code;
            best_seed = lseed;
        }
        (void)rec_seed;
    }
    std::cout << "recompute_n_surv_2187 " << n_surv2 << "\n";
    std::cout << "recompute_best2_acf " << best << "\n";
    std::cout << "recompute_best2_code " << best_code << "\n";
    std::cout << "recompute_best2_seed " << best_seed << "\n";
    if (n_surv2 != 0) {
        std::cout << "FAIL survivors at 2187\n";
        ++fails;
    }
    if (best != 1647 || best_code != 12507201 || best_seed != 1) {
        std::cout << "FAIL maximizer identity\n";
        ++fails;
    }
    if (file_best2 != 1647 || file_best2_code != 12507201 || file_nsurv2187 != 0) {
        std::cout << "FAIL file maximizer fields\n";
        ++fails;
    }

    // Maximizer lex-first cube on a long enough iterate.
    {
        u8 img[4][3];
        decode(12507201u, img);
        auto w = iterate3(img, 1, 2187);
        auto c = find_cube(w);
        int acf = acf_prefix3(img, 1, 2187);
        std::cout << "max_acf " << acf << "\n";
        if (!c) {
            std::cout << "FAIL maximizer no cube\n";
            ++fails;
        } else {
            std::cout << "max_cube " << c->i << " " << c->d << " " << c->sum << "\n";
            if (c->i != 982 || c->d != 222 || c->sum != 229) {
                std::cout << "FAIL maximizer cube\n";
                ++fails;
            }
            if (c->i + 3 * c->d - 1 != 1647) {
                std::cout << "FAIL cube vs ACF arithmetic\n";
                ++fails;
            }
        }
        if (acf != 1647) {
            std::cout << "FAIL maximizer acf\n";
            ++fails;
        }
        // T and primitivity
        int T[4];
        for (int a = 0; a < 4; ++a) T[a] = img[a][0] + img[a][1] + img[a][2];
        std::cout << "max_T " << T[0] << " " << T[1] << " " << T[2] << " " << T[3] << "\n";
        if (T[0] != 1 || T[1] != 3 || T[2] != 6 || T[3] != 8) {
            std::cout << "FAIL maximizer T\n";
            ++fails;
        }
        if (!primitive4(img) || img[1][0] != 1) {
            std::cout << "FAIL maximizer prim/prol\n";
            ++fails;
        }
        std::cout << "max_imgs";
        for (int a = 0; a < 4; ++a) {
            std::cout << " ";
            for (int r = 0; r < 3; ++r) std::cout << (int)img[a][r];
        }
        std::cout << "\n";
        // p_T on the ACF prefix
        w.resize(1647);
        std::cout << "max_pT_1 " << pT_at(w, 1) << "\n";
        std::cout << "max_pT_8 " << pT_at(w, 8) << "\n";
        std::cout << "max_pT_16 " << pT_at(w, 16) << "\n";
        std::cout << "max_pT_64 " << pT_at(w, 64) << "\n";
        std::cout << "max_diam_64 " << diam_at(w, 64) << "\n";
    }

    // Famous 2-uniform control.
    {
        u8 img[4][2] = {{3, 2}, {3, 1}, {2, 0}, {0, 1}};
        std::vector<u8> w = {2};
        while ((int)w.size() < 256) {
            std::vector<u8> nxt;
            nxt.reserve(w.size() * 2);
            for (u8 a : w) {
                nxt.push_back(img[a][0]);
                nxt.push_back(img[a][1]);
            }
            w.swap(nxt);
        }
        w.resize(256);
        auto c = find_cube(w);
        auto S = prefix_sums(w);
        int acf = 256;
        for (int n = 3; n <= 256; ++n) {
            if (has_cube_ending_at(S, n)) {
                acf = n - 1;
                break;
            }
        }
        std::cout << "famous_acf " << acf << "\n";
        if (!c) {
            std::cout << "FAIL famous no cube\n";
            ++fails;
        } else {
            std::cout << "famous_cube " << c->i << " " << c->d << " " << c->sum << "\n";
            if (c->i != 45 || c->d != 25 || c->sum != 35 || acf != 119) {
                std::cout << "FAIL famous cube/acf\n";
                ++fails;
            }
        }
        w.resize(119);
        std::cout << "famous_acfpref_pT_8 " << pT_at(w, 8) << "\n";
        std::cout << "famous_acfpref_pT_16 " << pT_at(w, 16) << "\n";
        std::cout << "famous_acfpref_diam_16 " << diam_at(w, 16) << "\n";
    }

    std::cout << "pt_verify_fails " << fails << "\n";
    return fails ? 1 : 0;
}
