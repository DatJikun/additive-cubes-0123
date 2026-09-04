// Dual C++ anatomy of additive cubes in uniformly morphic words.
// Independent of python/desub_geometry.py except that both implement the same
// ceil/floor complete-block split: sum = T·psi + sum(left fragment) + sum(right fragment).
#include "acf.hpp"
#include <iostream>
#include <fstream>
#include <map>
#include <sstream>
using namespace acf;

struct Morph {
    int m = 2;
    std::array<std::vector<u8>, 4> h;
    std::array<int, 4> T{};
    std::array<int, 4> alph{};  // letter values; index 0..3
    int idx_of[16];             // value -> 0..3, -1 if unused
};

static Morph make_A(int m, const std::array<std::string, 4>& imgs) {
    Morph s;
    s.m = m;
    s.alph = {0, 1, 2, 3};
    for (int i = 0; i < 16; ++i) s.idx_of[i] = -1;
    for (int i = 0; i < 4; ++i) s.idx_of[s.alph[i]] = i;
    for (int a = 0; a < 4; ++a) {
        s.T[a] = 0;
        for (char c : imgs[a]) {
            u8 x = (u8)(c - '0');
            s.h[a].push_back(x);
            s.T[a] += x;
        }
    }
    return s;
}

static Morph make_B_from_digits(int m, const std::array<std::string, 4>& digit_imgs) {
    // digit_imgs use 0,1,2,3 as indices into {0,1,3,4}
    Morph s;
    s.m = m;
    s.alph = {0, 1, 3, 4};
    for (int i = 0; i < 16; ++i) s.idx_of[i] = -1;
    for (int i = 0; i < 4; ++i) s.idx_of[s.alph[i]] = i;
    for (int a = 0; a < 4; ++a) {
        s.T[a] = 0;
        for (char c : digit_imgs[a]) {
            int di = c - '0';
            u8 x = (u8)s.alph[di];
            s.h[a].push_back(x);
            s.T[a] += x;
        }
    }
    return s;
}

static std::vector<u8> apply_morph(const Morph& s, const std::vector<u8>& u) {
    std::vector<u8> w;
    w.reserve(u.size() * s.m);
    for (u8 x : u) {
        int i = s.idx_of[x];
        w.insert(w.end(), s.h[i].begin(), s.h[i].end());
    }
    return w;
}

struct View {
    int n_complete = 0;
    int psi[4] = {0, 0, 0, 0};
    int defect = 0;
    int recon = 0;
    int true_sum = 0;
    int r = 0, s = 0, k = 0;
};

static View view_interval(const Morph& sigma, const std::vector<u8>& W,
                          const std::vector<u8>& U, int p, int d) {
    View v;
    const int m = sigma.m;
    v.r = p % m;
    v.s = d % m;
    v.k = d / m;
    const int start_full = (p + m - 1) / m;  // ceil(p/m)
    const int end_full = (p + d) / m;        // floor((p+d)/m)
    for (int j = start_full; j < end_full; ++j) {
        int i = sigma.idx_of[U[j]];
        v.psi[i]++;
        v.n_complete++;
        v.recon += sigma.T[i];
    }
    int left_end = start_full * m;
    if (left_end > p) {
        for (int t = p; t < left_end && t < p + d; ++t) v.defect += W[t];
    }
    int right_start = end_full * m;
    if (p + d > right_start) {
        for (int t = std::max(right_start, p); t < p + d; ++t) v.defect += W[t];
    }
    v.recon += v.defect;
    for (int t = p; t < p + d; ++t) v.true_sum += W[t];
    return v;
}

static void print_view(const char* name, const View& v) {
    std::cout << name << " n=" << v.n_complete << " psi=(" << v.psi[0] << ","
              << v.psi[1] << "," << v.psi[2] << "," << v.psi[3] << ") def=" << v.defect
              << " recon=" << v.recon << " true=" << v.true_sum << " r,s,k=" << v.r
              << "," << v.s << "," << v.k << "\n";
}

static void analyze(const Morph& sigma, const std::vector<u8>& W,
                    const std::vector<u8>& U, Cube c) {
    View X = view_interval(sigma, W, U, c.i, c.d);
    View Y = view_interval(sigma, W, U, c.i + c.d, c.d);
    View Z = view_interval(sigma, W, U, c.i + 2 * c.d, c.d);
    int vxy[4], vyz[4];
    int Tdot = 0, Tdot2 = 0;
    for (int i = 0; i < 4; ++i) {
        vxy[i] = X.psi[i] - Y.psi[i];
        vyz[i] = Y.psi[i] - Z.psi[i];
        Tdot += sigma.T[i] * vxy[i];
        Tdot2 += sigma.T[i] * vyz[i];
    }
    int dbxy = X.defect - Y.defect;
    int dbyz = Y.defect - Z.defect;
    std::cout << "cube i=" << c.i << " d=" << c.d << " sum=" << c.sum
              << " aligned=" << (c.i % sigma.m == 0 && c.d % sigma.m == 0) << "\n";
    print_view("X", X);
    print_view("Y", Y);
    print_view("Z", Z);
    std::cout << "v_xy=(" << vxy[0] << "," << vxy[1] << "," << vxy[2] << "," << vxy[3]
              << ") T·v=" << Tdot << " db=" << dbxy << " eq=" << (Tdot + dbxy == 0)
              << " recon_ok=" << (X.recon == X.true_sum && Y.recon == Y.true_sum && Z.recon == Z.true_sum)
              << "\n";
    std::cout << "v_yz=(" << vyz[0] << "," << vyz[1] << "," << vyz[2] << "," << vyz[3]
              << ") T·v=" << Tdot2 << " db=" << dbyz << " eq=" << (Tdot2 + dbyz == 0) << "\n";
}

static std::pair<std::vector<u8>, std::vector<u8>> iterate_cover(const Morph& s, int seed,
                                                                int need) {
    std::vector<u8> u = {(u8)seed};
    while ((int)u.size() * s.m < need) {
        auto nxt = apply_morph(s, u);
        if (nxt.size() <= u.size()) break;
        u.swap(nxt);
    }
    auto w = apply_morph(s, u);
    return {w, u};
}

static void cmd_famous() {
    Morph s = make_A(2, {"32", "31", "20", "01"});
    auto [W, U] = iterate_cover(s, 2, 256);
    auto c = find_cube(W);
    if (!c) {
        std::cout << "no cube\n";
        return;
    }
    analyze(s, W, U, *c);
}

static void cmd_all(int n, const char* path) {
    Morph s = make_A(2, {"32", "31", "20", "01"});
    auto [W, U] = iterate_cover(s, 2, n);
    if ((int)W.size() > n) W.resize(n);
    std::ofstream out(path);
    out << "i,d,sum,r,s,k,aligned,nX,nY,nZ,v0,v1,v2,v3,l1,db,Tdot,eq,recon\n";
    auto S = prefix_sums(W);
    const int N = (int)W.size();
    int count = 0, eq_fail = 0, recon_fail = 0;
    for (int i = 0; i + 3 <= N; ++i) {
        int dmax = (N - i) / 3;
        for (int d = 1; d <= dmax; ++d) {
            i64 s1 = S[i + d] - S[i];
            i64 s2 = S[i + 2 * d] - S[i + d];
            i64 s3 = S[i + 3 * d] - S[i + 2 * d];
            if (s1 != s2 || s2 != s3) continue;
            View X = view_interval(s, W, U, i, d);
            View Y = view_interval(s, W, U, i + d, d);
            View Z = view_interval(s, W, U, i + 2 * d, d);
            int v[4], Tdot = 0, l1 = 0;
            for (int t = 0; t < 4; ++t) {
                v[t] = X.psi[t] - Y.psi[t];
                Tdot += s.T[t] * v[t];
                l1 += std::abs(v[t]);
            }
            int db = X.defect - Y.defect;
            bool eq = (Tdot + db == 0);
            bool recon = (X.recon == X.true_sum && Y.recon == Y.true_sum && Z.recon == Z.true_sum);
            if (!eq) ++eq_fail;
            if (!recon) ++recon_fail;
            out << i << "," << d << "," << s1 << "," << X.r << "," << X.s << "," << X.k << ","
                << (i % 2 == 0 && d % 2 == 0) << "," << X.n_complete << "," << Y.n_complete << ","
                << Z.n_complete << "," << v[0] << "," << v[1] << "," << v[2] << "," << v[3] << ","
                << l1 << "," << db << "," << Tdot << "," << eq << "," << recon << "\n";
            ++count;
        }
    }
    std::cout << "wrote " << count << " cubes eq_fail=" << eq_fail
              << " recon_fail=" << recon_fail << " to " << path << "\n";
}

static Morph morph2_from_code(int code, const std::array<int, 4>& alph) {
    Morph s;
    s.m = 2;
    s.alph = alph;
    for (int i = 0; i < 16; ++i) s.idx_of[i] = -1;
    for (int i = 0; i < 4; ++i) s.idx_of[alph[i]] = i;
    int t = code;
    for (int a = 0; a < 4; ++a) {
        int x = t % 4, y = (t / 4) % 4;
        t /= 16;
        s.h[a] = {(u8)alph[x], (u8)alph[y]};
        s.T[a] = s.h[a][0] + s.h[a][1];
    }
    return s;
}

static void cmd_scan2(int cap, const char* alph_name, const char* path) {
    std::array<int, 4> alph = (std::string(alph_name) == "B")
                                  ? std::array<int, 4>{0, 1, 3, 4}
                                  : std::array<int, 4>{0, 1, 2, 3};
    std::ofstream out(path);
    out << "code,seed,T0,T1,T2,T3,n_sums,i,d,sum,r,s,k,aligned,nX,nY,nZ,v0,v1,v2,v3,l1,db,"
           "Tdot,eq,recon,acf_prefix\n";
    uint64_t n = 0, n_unaligned = 0, n_aligned = 0, n_eqfail = 0, n_surv = 0;
    std::map<std::pair<int, int>, int> types;
    std::map<int, int> l1hist;
    for (int code = 0; code < 65536; ++code) {
        Morph s = morph2_from_code(code, alph);
        int n_sums = 0;
        for (int a = 0; a < 4; ++a) {
            bool u = true;
            for (int b = 0; b < a; ++b)
                if (s.T[a] == s.T[b]) u = false;
            if (u) ++n_sums;
        }
        for (int si = 0; si < 4; ++si) {
            u8 seed = (u8)alph[si];
            if (s.h[si][0] != seed) continue;
            auto [W, U] = iterate_cover(s, seed, cap);
            if ((int)W.size() > cap) W.resize(cap);
            auto c = find_cube(W);
            if (!c) {
                ++n_surv;
                continue;
            }
            View X = view_interval(s, W, U, c->i, c->d);
            View Y = view_interval(s, W, U, c->i + c->d, c->d);
            View Z = view_interval(s, W, U, c->i + 2 * c->d, c->d);
            int v[4], Tdot = 0, l1 = 0;
            for (int t = 0; t < 4; ++t) {
                v[t] = X.psi[t] - Y.psi[t];
                Tdot += s.T[t] * v[t];
                l1 += std::abs(v[t]);
            }
            int db = X.defect - Y.defect;
            bool eq = (Tdot + db == 0);
            bool recon = (X.recon == X.true_sum && Y.recon == Y.true_sum && Z.recon == Z.true_sum);
            if (!eq) ++n_eqfail;
            bool al = (c->i % 2 == 0 && c->d % 2 == 0);
            if (al) ++n_aligned;
            else ++n_unaligned;
            types[{X.r, X.s}]++;
            l1hist[l1]++;
            out << code << "," << (int)seed << "," << s.T[0] << "," << s.T[1] << "," << s.T[2]
                << "," << s.T[3] << "," << n_sums << "," << c->i << "," << c->d << "," << c->sum
                << "," << X.r << "," << X.s << "," << X.k << "," << al << "," << X.n_complete
                << "," << Y.n_complete << "," << Z.n_complete << "," << v[0] << "," << v[1] << ","
                << v[2] << "," << v[3] << "," << l1 << "," << db << "," << Tdot << "," << eq << ","
                << recon << "," << c->i + 3 * c->d - 1 << "\n";
            ++n;
        }
        if (code % 8192 == 0 && code)
            std::cerr << "scan2 " << alph_name << " code=" << code << " n=" << n << "\n";
    }
    std::cout << "scan2 alph=" << alph_name << " cap=" << cap << " first_cubes=" << n
              << " aligned=" << n_aligned << " unaligned=" << n_unaligned
              << " survivors=" << n_surv << " eq_fail=" << n_eqfail << "\n";
    std::cout << "types (r,s):";
    for (auto& kv : types)
        std::cout << " (" << kv.first.first << "," << kv.first.second << ")=" << kv.second;
    std::cout << "\nL1 hist:";
    for (auto& kv : l1hist) std::cout << " " << kv.first << ":" << kv.second;
    std::cout << "\n";
}

int main(int argc, char** argv) {
    std::string cmd = (argc > 1) ? argv[1] : "famous";
    if (cmd == "famous") {
        cmd_famous();
        return 0;
    }
    if (cmd == "all") {
        int n = (argc > 2) ? std::atoi(argv[2]) : 2048;
        const char* path = (argc > 3) ? argv[3] : "data/famous_all_cubes_cpp.csv";
        cmd_all(n, path);
        return 0;
    }
    if (cmd == "scan2") {
        int cap = (argc > 2) ? std::atoi(argv[2]) : 160;
        const char* alph = (argc > 3) ? argv[3] : "A";
        std::string path = (argc > 4) ? argv[4]
                                      : (std::string("data/scan2_") + alph + ".csv");
        cmd_scan2(cap, alph, path.c_str());
        return 0;
    }
    std::cerr << "usage: desub_scan famous | all N path | scan2 cap A|B path\n";
    return 1;
}
