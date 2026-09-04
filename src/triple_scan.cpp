// Dual of python/triple_graph.py: pair/triple counts, first square, type census.
#include "acf.hpp"
#include <iostream>
#include <array>
#include <set>
#include <tuple>
using namespace acf;

static std::vector<u8> famous(int n) {
    std::array<std::vector<u8>, 4> h;
    h[0] = {3, 2}; h[1] = {3, 1}; h[2] = {2, 0}; h[3] = {0, 1};
    std::vector<u8> w = {2};
    while ((int)w.size() < n) {
        std::vector<u8> nxt;
        for (u8 a : w) nxt.insert(nxt.end(), h[a].begin(), h[a].end());
        if (nxt.size() <= w.size()) break;
        w.swap(nxt);
    }
    if ((int)w.size() > n) w.resize(n);
    return w;
}

static std::vector<u8> cassaigne(int n) {
    std::array<std::vector<u8>, 5> h;
    h[0] = {0, 3}; h[1] = {4, 3}; h[3] = {1}; h[4] = {0, 1};
    std::vector<u8> w = {0};
    while ((int)w.size() < n) {
        std::vector<u8> nxt;
        for (u8 a : w) nxt.insert(nxt.end(), h[a].begin(), h[a].end());
        if (nxt.size() <= w.size()) break;
        w.swap(nxt);
    }
    if ((int)w.size() > n) w.resize(n);
    return w;
}

struct Stats {
    long long n_sq = 0, n_cube = 0, n_pair_not_triple = 0, n_yz_only = 0;
    long long middles = 0, both = 0, left_only = 0, right_only = 0, neither = 0;
    int sq_i = -1, sq_d = -1, sq_s = 0;
    int cu_i = -1, cu_d = -1, cu_s = 0;
};

static Stats pair_triple(const std::vector<u8>& w) {
    const int n = (int)w.size();
    auto S = prefix_sums(w);
    Stats st;
    for (int d = 1; d <= n / 2; ++d) {
        for (int i = 0; i + 2 * d <= n; ++i) {
            i64 s1 = S[i + d] - S[i];
            i64 s2 = S[i + 2 * d] - S[i + d];
            if (s1 == s2) {
                st.n_sq++;
                if (st.sq_i < 0) { st.sq_i = i; st.sq_d = d; st.sq_s = (int)s1; }
            }
            if (i + 3 * d <= n) {
                i64 s3 = S[i + 3 * d] - S[i + 2 * d];
                if (s1 == s2 && s2 == s3) {
                    st.n_cube++;
                    if (st.cu_i < 0) { st.cu_i = i; st.cu_d = d; st.cu_s = (int)s1; }
                } else if (s1 == s2) st.n_pair_not_triple++;
                else if (s2 == s3) st.n_yz_only++;
            }
        }
    }
    for (int d = 1; d <= n / 3; ++d) {
        for (int j = d; j + 2 * d <= n; ++j) {
            st.middles++;
            i64 sX = S[j] - S[j - d];
            i64 sY = S[j + d] - S[j];
            i64 sZ = S[j + 2 * d] - S[j + d];
            bool L = sX == sY, R = sY == sZ;
            if (L && R) st.both++;
            else if (L) st.left_only++;
            else if (R) st.right_only++;
            else st.neither++;
        }
    }
    return st;
}

static void print_stats(const char* name, const Stats& st, int n) {
    std::cout << name << " n=" << n
              << " squares=" << st.n_sq
              << " cubes=" << st.n_cube
              << " pair_not_triple=" << st.n_pair_not_triple
              << " yz_only=" << st.n_yz_only
              << " first_sq=(" << st.sq_i << "," << st.sq_d << "," << st.sq_s << ")"
              << " first_cube=(" << st.cu_i << "," << st.cu_d << "," << st.cu_s << ")"
              << " middles=" << st.middles
              << " P_and_S=" << st.both
              << " P_only=" << st.left_only
              << " S_only=" << st.right_only
              << " neither=" << st.neither << "\n";
}

static void type_census(int m) {
    std::set<std::tuple<int,int,int,int,int,int,int,int,int>> sigs;
    int hist[5] = {0,0,0,0,0};
    for (int r = 0; r < m; ++r) for (int s = 0; s < m; ++s) {
        int rY = (r + s) % m;
        int rZ = (r + 2 * s) % m;
        int rEnd = (r + 3 * s) % m;
        int xL = r ? m - r : 0;
        int xR = rY, yL = rY ? m - rY : 0, yR = rZ;
        int zL = rZ ? m - rZ : 0, zR = rEnd;
        int xy = rY != 0, yz = rZ != 0;
        int nlet = (xL ? 1 : 0) + (xy ? 1 : 0) + (yz ? 1 : 0) + (zR ? 1 : 0);
        hist[nlet]++;
        sigs.insert(std::make_tuple(xL, xR, yL, yR, zL, zR, xy, yz, nlet));
    }
    std::cout << "m=" << m << " residue=" << (m * m)
              << " slot_sigs=" << sigs.size() << " n_letters";
    for (int k = 0; k <= 4; ++k) std::cout << " " << hist[k];
    std::cout << "\n";
}

int main(int argc, char** argv) {
    std::string cmd = argc > 1 ? argv[1] : "all";
    if (cmd == "types" || cmd == "all") {
        for (int m = 2; m <= 7; ++m) type_census(m);
        if (cmd == "types") return 0;
    }
    if (cmd == "famous" || cmd == "all") {
        auto w = famous(512);
        print_stats("famous", pair_triple(w), (int)w.size());
    }
    if (cmd == "cassaigne" || cmd == "all") {
        auto w = cassaigne(800);
        print_stats("cassaigne", pair_triple(w), (int)w.size());
        auto c2 = find_cube(w);
        std::cout << "cassaigne find_cube " << (c2 ? "FOUND" : "none") << "\n";
    }
    return 0;
}
