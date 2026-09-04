// Dual integer certificates for m-kernel / k-regular block-sum identities.
// Independent of Python: famous DFAO, S(2n)=T2·ψ(n), D1(0,n)=L·ψ(n),
// kernel fingerprint counts, modular vs integer zeros, Thue–Morse,
// Cassaigne kernel growth, 3-letter 2-uniform exhaustive, random m=3.
#include "acf.hpp"
#include <algorithm>
#include <array>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <map>
#include <set>
#include <string>
#include <vector>
using namespace acf;

static const int H[4][2] = {{3, 2}, {3, 1}, {2, 0}, {0, 1}};
static const int T[4] = {5, 4, 2, 1};
static const int T2[4] = {3, 5, 7, 9};
static const int Lform[4] = {7, 3, -3, -7}; // 2T - T2 = D1(0,n)/ψ(n)

static std::vector<u8> famous_iterate(int cap) {
    std::vector<u8> w = {2};
    while ((int)w.size() < cap) {
        std::vector<u8> nxt;
        nxt.reserve(w.size() * 2);
        for (u8 a : w) {
            nxt.push_back((u8)H[a][0]);
            nxt.push_back((u8)H[a][1]);
        }
        if (nxt.size() <= w.size()) break;
        w.swap(nxt);
    }
    if ((int)w.size() > cap) w.resize(cap);
    return w;
}

static int letter_at(int n) {
    if (n == 0) return 2;
    int digits[32];
    int k = 0;
    int x = n;
    while (x) {
        digits[k++] = x & 1;
        x >>= 1;
    }
    int s = 2;
    for (int i = k - 1; i >= 0; --i) s = H[s][digits[i]];
    return s;
}

static int kernel_fingerprints(const std::vector<int>& seq, int m, int max_r, int prefix) {
    std::set<std::vector<int>> seen;
    int stride = 1;
    for (int r = 0; r <= max_r; ++r) {
        for (int q = 0; q < stride; ++q) {
            if (q >= (int)seq.size()) break;
            std::vector<int> key;
            key.reserve(prefix);
            for (int n = 0; n < prefix; ++n) {
                int j = stride * n + q;
                if (j >= (int)seq.size()) {
                    key.clear();
                    break;
                }
                key.push_back(seq[j]);
            }
            if ((int)key.size() == prefix) seen.insert(key);
        }
        if (stride > 100000000 / m) break;
        stride *= m;
    }
    return (int)seen.size();
}

static std::vector<u8> cassaigne(int cap) {
    std::array<std::vector<u8>, 5> h;
    h[0] = {0, 3};
    h[1] = {4, 3};
    h[3] = {1};
    h[4] = {0, 1};
    std::vector<u8> w = {0};
    while ((int)w.size() < cap) {
        std::vector<u8> nxt;
        for (u8 a : w) nxt.insert(nxt.end(), h[a].begin(), h[a].end());
        if (nxt.size() <= w.size()) break;
        w.swap(nxt);
    }
    if ((int)w.size() > cap) w.resize(cap);
    return w;
}

static bool mat4_primitive(const int M[4][4]) {
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

static std::vector<u8> iterate_morph(const std::vector<std::vector<u8>>& imgs, u8 seed, int cap) {
    std::vector<u8> w = {seed};
    while ((int)w.size() < cap) {
        std::vector<u8> nxt;
        nxt.reserve(w.size() * imgs[0].size());
        for (u8 a : w) nxt.insert(nxt.end(), imgs[a].begin(), imgs[a].end());
        if (nxt.size() <= w.size()) break;
        w.swap(nxt);
    }
    if ((int)w.size() > cap) w.resize(cap);
    return w;
}

int main() {
    const int cap = 4096;
    auto U = famous_iterate(cap);
    int dfao_fail = 0;
    for (int n = 0; n < 512; ++n)
        if (letter_at(n) != (int)U[n]) ++dfao_fail;

    std::vector<int> a(U.size());
    for (size_t i = 0; i < U.size(); ++i) a[i] = T[U[i]];
    std::vector<u8> a8(a.begin(), a.end());
    auto S = prefix_sums(a8); // S[n] = sum a[0:n)

    int S2n_fail = 0, L_fail = 0;
    int pr[4] = {0, 0, 0, 0};
    int L_absmax = 0;
    int nmaxL = std::min(2000, (int)U.size());
    for (int n = 0; n <= nmaxL; ++n) {
        if (n > 0) pr[U[n - 1]]++;
        int t2 = T2[0] * pr[0] + T2[1] * pr[1] + T2[2] * pr[2] + T2[3] * pr[3];
        int Lf = Lform[0] * pr[0] + Lform[1] * pr[1] + Lform[2] * pr[2] + Lform[3] * pr[3];
        if (n <= 400 && 2 * n < (int)S.size() && t2 != (int)S[2 * n]) ++S2n_fail;
        if (n >= 1 && n <= 400 && 2 * n < (int)S.size()) {
            i64 d1 = 2 * S[n] - S[0] - S[2 * n];
            if (d1 != Lf) ++L_fail;
        }
        if (std::abs(Lf) > L_absmax) L_absmax = std::abs(Lf);
    }

    int a_fp = kernel_fingerprints(a, 2, 8, 48);
    std::vector<int> Uints(U.begin(), U.end());
    int U_fp = kernel_fingerprints(Uints, 2, 8, 48);

    std::vector<int> chi_sq;
    for (int d = 1; 2 * d < (int)S.size(); ++d)
        chi_sq.push_back(S[2 * d] == 2 * S[d] ? 1 : 0);
    std::vector<int> chi_cu;
    for (int d = 1; 3 * d < (int)S.size(); ++d) {
        i64 s1 = S[d];
        i64 s2 = S[2 * d] - S[d];
        i64 s3 = S[3 * d] - S[2 * d];
        chi_cu.push_back(s1 == s2 && s2 == s3 ? 1 : 0);
    }
    int sq_fp = kernel_fingerprints(chi_sq, 2, 7, 32);
    int cu_fp = kernel_fingerprints(chi_cu, 2, 7, 32);
    int n_sq = 0, n_cu = 0;
    for (int x : chi_sq) n_sq += x;
    for (int x : chi_cu) n_cu += x;

    // diam R at n=64, missing d < 200, mod vs int
    int diam64 = 0, n00 = 0;
    {
        const int n = 64;
        for (int i = 0; i + 3 * n < (int)S.size(); ++i) {
            i64 d1 = (S[i + n] - S[i]) - (S[i + 2 * n] - S[i + n]);
            i64 d2 = (S[i + 2 * n] - S[i + n]) - (S[i + 3 * n] - S[i + 2 * n]);
            int mag = (int)std::max(std::llabs(d1), std::llabs(d2));
            if (mag > diam64) diam64 = mag;
            if (d1 == 0 && d2 == 0) {
                /* counted below per n */
            }
        }
    }
    for (int n = 1; n <= 64; ++n) {
        bool hit = false;
        for (int i = 0; i + 3 * n < (int)S.size(); ++i) {
            i64 d1 = (S[i + n] - S[i]) - (S[i + 2 * n] - S[i + n]);
            i64 d2 = (S[i + 2 * n] - S[i + n]) - (S[i + 3 * n] - S[i + 2 * n]);
            if (d1 == 0 && d2 == 0) {
                hit = true;
                break;
            }
        }
        n00 += hit;
    }
    int n_cube_len = 0, n_missing = 0;
    std::vector<int> missing_head;
    for (int n = 1; n < 200; ++n) {
        bool hit = false;
        for (int i = 0; i + 3 * n < (int)S.size(); ++i) {
            i64 d1 = (S[i + n] - S[i]) - (S[i + 2 * n] - S[i + n]);
            i64 d2 = (S[i + 2 * n] - S[i + n]) - (S[i + 3 * n] - S[i + 2 * n]);
            if (d1 == 0 && d2 == 0) {
                hit = true;
                break;
            }
        }
        if (hit) ++n_cube_len;
        else {
            ++n_missing;
            if ((int)missing_head.size() < 30) missing_head.push_back(n);
        }
    }

    auto cubeA = find_cube(a8);
    auto cubeU = find_cube(U);

    int pT1 = 0, pT16 = 0, pT64 = 0;
    {
        std::set<i64> v1, v16, v64;
        for (int i = 0; i + 1 < (int)S.size(); ++i) v1.insert(S[i + 1] - S[i]);
        for (int i = 0; i + 16 < (int)S.size(); ++i) v16.insert(S[i + 16] - S[i]);
        for (int i = 0; i + 64 < (int)S.size(); ++i) v64.insert(S[i + 64] - S[i]);
        pT1 = (int)v1.size();
        pT16 = (int)v16.size();
        pT64 = (int)v64.size();
    }

    std::cout << "dfao_fail " << dfao_fail << "\n";
    std::cout << "S2n_fail " << S2n_fail << "\n";
    std::cout << "L_fail " << L_fail << "\n";
    std::cout << "L_absmax " << L_absmax << "\n";
    std::cout << "a_kernel_fp " << a_fp << "\n";
    std::cout << "U_kernel_fp " << U_fp << "\n";
    std::cout << "sq_kernel_fp " << sq_fp << "\n";
    std::cout << "cu_kernel_fp " << cu_fp << "\n";
    std::cout << "n_prefix_squares " << n_sq << "\n";
    std::cout << "n_prefix_cubes " << n_cu << "\n";
    std::cout << "diam_64_val " << diam64 << "\n";
    std::cout << "n_with_00_le64 " << n00 << "\n";
    std::cout << "n_cube_len_lt200 " << n_cube_len << "\n";
    std::cout << "n_missing_lt200 " << n_missing << "\n";
    std::cout << "pT_1 " << pT1 << "\n";
    std::cout << "pT_16 " << pT16 << "\n";
    std::cout << "pT_64 " << pT64 << "\n";
    if (cubeA)
        std::cout << "first_cube_a " << cubeA->i << " " << cubeA->d << " " << cubeA->sum << "\n";
    else
        std::cout << "first_cube_a none\n";
    if (cubeU)
        std::cout << "first_cube_U " << cubeU->i << " " << cubeU->d << " " << cubeU->sum << "\n";
    else
        std::cout << "first_cube_U none\n";

    int mods[5] = {2, 3, 4, 5, 8};
    for (int mi = 0; mi < 5; ++mi) {
        int Mmod = mods[mi];
        int n_tot = 0, n_mod = 0, n_int = 0;
        for (int n = 1; n < 40; ++n) {
            int imax = std::min(200, (int)S.size() - 3 * n);
            for (int i = 0; i < imax; ++i) {
                i64 x = (S[i + n] - S[i]) - (S[i + 2 * n] - S[i + n]);
                i64 y = (S[i + 2 * n] - S[i + n]) - (S[i + 3 * n] - S[i + 2 * n]);
                ++n_tot;
                if (x % Mmod == 0 && y % Mmod == 0) {
                    ++n_mod;
                    if (x == 0 && y == 0) ++n_int;
                }
            }
        }
        std::cout << "mod" << Mmod << " " << n_tot << " " << n_mod << " " << n_int << "\n";
    }

    auto C = cassaigne(2048);
    std::vector<int> Cints(C.begin(), C.end());
    std::cout << "cass_2ker " << kernel_fingerprints(Cints, 2, 7, 32) << "\n";
    std::cout << "cass_3ker " << kernel_fingerprints(Cints, 3, 5, 24) << "\n";
    auto cc = find_cube(C);
    std::cout << "cass_cube " << (cc ? "yes" : "none") << "\n";

    std::vector<u8> tm(2048);
    for (int n = 0; n < 2048; ++n) {
        int b = 0, x = n;
        while (x) {
            b ^= (x & 1);
            x >>= 1;
        }
        tm[n] = (u8)b;
    }
    auto tmS = prefix_sums(tm);
    int dmax = 0;
    for (int n = 0; n < (int)tmS.size(); ++n) {
        int d = (int)std::llabs(2 * tmS[n] - n);
        if (d > dmax) dmax = d;
    }
    auto tmc = find_cube(tm);
    std::vector<int> tmints(tm.begin(), tm.end());
    std::vector<int> tm_chi;
    for (int d = 1; 2 * d < (int)tmS.size(); ++d)
        tm_chi.push_back(tmS[2 * d] == 2 * tmS[d] ? 1 : 0);
    std::cout << "tm_cube " << (tmc ? "yes" : "none") << "\n";
    std::cout << "tm_Delta_max " << dmax << "\n";
    std::cout << "tm_a_ker " << kernel_fingerprints(tmints, 2, 6, 40) << "\n";
    std::cout << "tm_sq_ker " << kernel_fingerprints(tm_chi, 2, 6, 24) << "\n";

    // 3-letter 2-uniform exhaustive, alphabet {0,1,2}, n_sums>=2, prolongable, cap 80
    int n_tried = 0, n_surv = 0, best_acf = -1;
    std::string best_code;
    const int alph3[3] = {0, 1, 2};
    for (int code = 0; code < 729; ++code) {
        int t = code;
        std::vector<std::vector<u8>> imgs(3, std::vector<u8>(2));
        int Tsum[3];
        int M[3][3] = {};
        for (int a = 0; a < 3; ++a) {
            for (int r = 0; r < 2; ++r) {
                imgs[a][r] = (u8)alph3[t % 3];
                t /= 3;
            }
            Tsum[a] = (int)imgs[a][0] + (int)imgs[a][1];
            for (int r = 0; r < 2; ++r) M[imgs[a][r]][a]++;
        }
        std::set<int> sums(Tsum, Tsum + 3);
        if ((int)sums.size() < 2) continue;
        bool prol = false;
        for (int a = 0; a < 3; ++a)
            if (imgs[a][0] == a) prol = true;
        if (!prol) continue;
        ++n_tried;
        int local = 0;
        bool surv = false;
        for (int seed = 0; seed < 3; ++seed) {
            if (imgs[seed][0] != seed) continue;
            auto W = iterate_morph(imgs, (u8)seed, 80);
            auto Sloc = prefix_sums(W);
            int acf = (int)W.size();
            for (int n = 3; n <= (int)W.size(); ++n) {
                if (has_cube_ending_at(Sloc, n)) {
                    acf = n - 1;
                    break;
                }
            }
            if (acf == (int)W.size() && !find_cube(W)) surv = true;
            if (acf > local) local = acf;
        }
        if (surv) ++n_surv;
        if (local > best_acf) {
            best_acf = local;
            best_code = std::to_string(code);
        }
    }
    std::cout << "scan3_2_tried " << n_tried << "\n";
    std::cout << "scan3_2_surv " << n_surv << "\n";
    std::cout << "scan3_2_best " << best_acf << "\n";

    // 3-letter 2-uniform n_sums>=3
    n_tried = 0;
    n_surv = 0;
    best_acf = -1;
    for (int code = 0; code < 729; ++code) {
        int t = code;
        std::vector<std::vector<u8>> imgs(3, std::vector<u8>(2));
        int Tsum[3];
        for (int a = 0; a < 3; ++a) {
            for (int r = 0; r < 2; ++r) {
                imgs[a][r] = (u8)alph3[t % 3];
                t /= 3;
            }
            Tsum[a] = (int)imgs[a][0] + (int)imgs[a][1];
        }
        std::set<int> sums(Tsum, Tsum + 3);
        if ((int)sums.size() < 3) continue;
        bool prol = false;
        for (int a = 0; a < 3; ++a)
            if (imgs[a][0] == a) prol = true;
        if (!prol) continue;
        ++n_tried;
        int local = 0;
        bool surv = false;
        for (int seed = 0; seed < 3; ++seed) {
            if (imgs[seed][0] != seed) continue;
            auto W = iterate_morph(imgs, (u8)seed, 80);
            auto Sloc = prefix_sums(W);
            int acf = (int)W.size();
            for (int n = 3; n <= (int)W.size(); ++n) {
                if (has_cube_ending_at(Sloc, n)) {
                    acf = n - 1;
                    break;
                }
            }
            if (acf == (int)W.size() && !find_cube(W)) surv = true;
            if (acf > local) local = acf;
        }
        if (surv) ++n_surv;
        if (local > best_acf) best_acf = local;
    }
    std::cout << "scan3_2_nsums3_tried " << n_tried << "\n";
    std::cout << "scan3_2_nsums3_surv " << n_surv << "\n";
    std::cout << "scan3_2_nsums3_best " << best_acf << "\n";

    // random primitive 3-uniform 4-letter, n_sums>=3, cap 160, seed 20260904
    uint64_t rng = 20260904ULL;
    auto rnd = [&]() -> int {
        rng = rng * 6364136223846793005ULL + 1;
        return (int)(rng >> 33);
    };
    int n_ok = 0, n_surv4 = 0, best4 = -1;
    std::string best4code;
    for (int trial = 0; trial < 8000; ++trial) {
        std::vector<std::vector<u8>> imgs(4, std::vector<u8>(3));
        for (int a = 0; a < 4; ++a)
            for (int r = 0; r < 3; ++r) imgs[a][r] = (u8)(rnd() % 4);
        int s0 = rnd() % 4;
        imgs[s0][0] = (u8)s0;
        int Tsum[4];
        int M[4][4] = {};
        std::set<int> sums;
        for (int a = 0; a < 4; ++a) {
            Tsum[a] = (int)imgs[a][0] + (int)imgs[a][1] + (int)imgs[a][2];
            sums.insert(Tsum[a]);
            for (int r = 0; r < 3; ++r) M[imgs[a][r]][a]++;
        }
        if ((int)sums.size() < 3) continue;
        if (!mat4_primitive(M)) continue;
        ++n_ok;
        int local = 0;
        bool surv = false;
        for (int seed = 0; seed < 4; ++seed) {
            if (imgs[seed][0] != seed) continue;
            auto W = iterate_morph(imgs, (u8)seed, 160);
            auto Sloc = prefix_sums(W);
            int acf = (int)W.size();
            for (int n = 3; n <= (int)W.size(); ++n) {
                if (has_cube_ending_at(Sloc, n)) {
                    acf = n - 1;
                    break;
                }
            }
            if (acf == (int)W.size() && !find_cube(W)) surv = true;
            if (acf > local) local = acf;
        }
        if (surv) ++n_surv4;
        if (local > best4) {
            best4 = local;
            best4code.clear();
            for (int a = 0; a < 4; ++a) {
                if (a) best4code += " ";
                for (int r = 0; r < 3; ++r) best4code += char('0' + imgs[a][r]);
            }
        }
    }
    std::cout << "rand_m3_n_ok " << n_ok << "\n";
    std::cout << "rand_m3_surv " << n_surv4 << "\n";
    std::cout << "rand_m3_best " << best4 << "\n";
    std::cout << "rand_m3_code " << best4code << "\n";
    return 0;
}
