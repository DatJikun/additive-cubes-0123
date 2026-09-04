// Dual integer certificates for the non-affine uniform template analysis.
// Independent of Python: prefix-sum cubes, Cassaigne incidence recurrence,
// padding, T-word of the famous iterate, additive complexity counts.
#include "acf.hpp"
#include <algorithm>
#include <array>
#include <cstdlib>
#include <iostream>
#include <map>
#include <set>
#include <vector>
using namespace acf;

static std::vector<u8> famous_iterate(int cap) {
    std::array<std::vector<u8>, 4> h = {{{3, 2}, {3, 1}, {2, 0}, {0, 1}}};
    std::vector<u8> w = {2};
    while ((int)w.size() < cap) {
        std::vector<u8> nxt;
        for (u8 a : w) nxt.insert(nxt.end(), h[a].begin(), h[a].end());
        if (nxt.size() <= w.size()) break;
        w.swap(nxt);
    }
    if ((int)w.size() > cap) w.resize(cap);
    return w;
}

static std::vector<u8> t_word(const std::vector<u8>& U) {
    const int T[4] = {5, 4, 2, 1};
    std::vector<u8> out;
    out.reserve(U.size());
    for (u8 a : U) out.push_back((u8)T[a]);
    return out;
}

static std::vector<int> additive_complexity(const std::vector<u8>& w, int nmax) {
    auto S = prefix_sums(w);
    std::vector<int> out;
    const int N = (int)w.size();
    for (int n = 1; n <= nmax; ++n) {
        std::set<i64> vals;
        for (int i = 0; i + n <= N; ++i) vals.insert(S[i + n] - S[i]);
        out.push_back((int)vals.size());
    }
    return out;
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

static int n_complete(int p, int d, int m) {
    int n = 0;
    int x = p;
    int end = p + d;
    while (x < end) {
        int off = x % m;
        int take = std::min(m - off, end - x);
        if (off == 0 && take == m) ++n;
        x += take;
    }
    return n;
}

static int first_acf(const std::vector<u8>& w) {
    auto S = prefix_sums(w);
    for (int n = 3; n <= (int)w.size(); ++n) {
        if (has_cube_ending_at(S, n)) return n - 1;
    }
    return (int)w.size();
}

int main(int argc, char** argv) {
    std::string cmd = argc > 1 ? argv[1] : "all";

    // Length rigidity
    std::cout << "DELTA_N";
    for (int m = 2; m <= 7; ++m) {
        int worst = 0;
        for (int r = 0; r < m; ++r)
            for (int s = 0; s < m; ++s) {
                int k = 6;
                int d = k * m + s;
                int nX = n_complete(r, d, m);
                int nY = n_complete(r + d, d, m);
                int nZ = n_complete(r + 2 * d, d, m);
                worst = std::max(worst, std::max(std::abs(nX - nY), std::abs(nY - nZ)));
            }
        std::cout << " m=" << m << ":" << worst;
    }
    std::cout << "\n";

    // Cassaigne column sums and iterate lengths via M^T
    int M[4][4] = {{1, 0, 0, 1}, {0, 0, 1, 1}, {1, 1, 0, 0}, {0, 1, 0, 0}};
    std::cout << "COL_SUMS";
    for (int j = 0; j < 4; ++j) {
        int s = 0;
        for (int i = 0; i < 4; ++i) s += M[i][j];
        std::cout << " " << s;
    }
    std::cout << "\n";
    int v[4] = {1, 1, 1, 1};
    std::cout << "PHI_LEN_0";
    std::cout << " 1";
    for (int k = 1; k <= 8; ++k) {
        int nv[4] = {0, 0, 0, 0};
        for (int a = 0; a < 4; ++a)
            for (int b = 0; b < 4; ++b) nv[a] += M[b][a] * v[b];
        for (int a = 0; a < 4; ++a) v[a] = nv[a];
        std::cout << " " << v[0];
    }
    std::cout << "\n";

    auto C = cassaigne(800);
    auto cC = find_cube(C);
    std::cout << "CASSAIGNE_CUBE " << (cC ? "YES" : "NO") << " n=" << C.size() << "\n";
    auto pC = additive_complexity(C, 16);
    std::cout << "CASSAIGNE_PADD";
    for (int x : pC) std::cout << " " << x;
    std::cout << "\n";

    // Famous T-word
    auto W = famous_iterate(512);
    std::vector<u8> U;
    U.reserve(W.size() / 2);
    for (size_t i = 0; i + 1 < W.size(); i += 2) {
        // decode 2-blocks of famous: 32,31,20,01
        u8 a = W[i], b = W[i + 1];
        int letter = -1;
        if (a == 3 && b == 2) letter = 0;
        else if (a == 3 && b == 1) letter = 1;
        else if (a == 2 && b == 0) letter = 2;
        else if (a == 0 && b == 1) letter = 3;
        if (letter < 0) {
            std::cerr << "decode fail at " << i << "\n";
            return 1;
        }
        U.push_back((u8)letter);
    }
    auto cW = find_cube(W);
    auto cU = find_cube(U);
    auto Tw = t_word(U);
    auto cT = find_cube(Tw);
    std::cout << "FAMOUS_W";
    if (cW) std::cout << " i=" << cW->i << " d=" << cW->d << " s=" << cW->sum;
    else std::cout << " NONE";
    std::cout << "\nFAMOUS_U_FULL " << (cU ? "CUBE" : "ACF") << " n=" << U.size();
    if (cU) std::cout << " i=" << cU->i << " d=" << cU->d;
    std::cout << "\n";
    int cover = cW ? (cW->i + 3 * cW->d + 1) / 2 : 0;
    std::vector<u8> Ucover(U.begin(), U.begin() + std::min(cover, (int)U.size()));
    auto cUc = find_cube(Ucover);
    std::cout << "FAMOUS_U_COVER " << (cUc ? "CUBE" : "ACF") << " n=" << Ucover.size() << "\n";
    std::cout << "FAMOUS_T";
    if (cT) std::cout << " i=" << cT->i << " d=" << cT->d << " s=" << cT->sum;
    else std::cout << " NONE n=" << Tw.size();
    std::cout << "\n";
    auto pT = additive_complexity(Tw, 16);
    auto pW = additive_complexity(W, 16);
    std::cout << "PT";
    for (int x : pT) std::cout << " " << x;
    std::cout << "\nPW";
    for (int x : pW) std::cout << " " << x;
    std::cout << "\n";

    // Pad Cassaigne
    const int letters[4] = {0, 1, 3, 4};
    int pad_best = 0;
    int pad_n = 0;
    for (int pad : letters) {
        for (int left = 0; left < 2; ++left) {
            std::array<std::vector<u8>, 5> h;
            h[0] = {0, 3};
            h[1] = {4, 3};
            h[3] = left ? std::vector<u8>{(u8)pad, 1} : std::vector<u8>{1, (u8)pad};
            h[4] = {0, 1};
            for (int seed : letters) {
                std::vector<u8> w = {(u8)seed};
                while ((int)w.size() < 200) {
                    std::vector<u8> nxt;
                    for (u8 a : w) nxt.insert(nxt.end(), h[a].begin(), h[a].end());
                    if (nxt.size() <= w.size()) break;
                    w.swap(nxt);
                }
                if ((int)w.size() > 200) w.resize(200);
                int L = first_acf(w);
                if (L > pad_best) pad_best = L;
                ++pad_n;
            }
        }
    }
    std::cout << "PAD_BEST " << pad_best << " n=" << pad_n << "\n";

    // Famous return-to-2 slopes
    std::vector<int> pos;
    for (int i = 0; i < (int)W.size(); ++i)
        if (W[i] == 2) pos.push_back(i);
    std::set<std::pair<int, i64>> slopes;
    for (size_t t = 0; t + 1 < pos.size(); ++t) {
        int a = pos[t], b = pos[t + 1];
        i64 sm = 0;
        for (int i = a; i < b; ++i) sm += W[i];
        slopes.insert({b - a, sm});
    }
    std::cout << "RET2_SLOPES " << slopes.size() << " npos=" << pos.size() << "\n";

    if (cmd == "quiet") return 0;
    return 0;
}
