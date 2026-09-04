// Independent integer linear algebra for T, ker(T), Krylov rank, T2·v.
// Dual of python/quotient_dyn.py (minors for rank; famous T2·v; 2-uniform census).
#include "acf.hpp"
#include <iostream>
#include <array>
using namespace acf;

static int det3(int a,int b,int c,int d,int e,int f,int g,int h,int i) {
    return a*(e*i-f*h)-b*(d*i-f*g)+c*(d*h-e*g);
}
static int det4(const int A[4][4]) {
    int s = 0;
    for (int j = 0; j < 4; ++j) {
        int m[3][3];
        for (int r = 1; r < 4; ++r) {
            int cc = 0;
            for (int c = 0; c < 4; ++c) if (c != j) m[r-1][cc++] = A[r][c];
        }
        int d3 = det3(m[0][0],m[0][1],m[0][2], m[1][0],m[1][1],m[1][2], m[2][0],m[2][1],m[2][2]);
        s += ((j % 2 == 0) ? 1 : -1) * A[0][j] * d3;
    }
    return s;
}
static int rank_rows(const int rows[][4], int n) {
    if (n <= 0) return 0;
    int kmax = n < 4 ? n : 4;
    if (kmax == 4 && n >= 4) {
        int A[4][4];
        for (int a = 0; a < n; ++a)
        for (int b = a+1; b < n; ++b)
        for (int c = b+1; c < n; ++c)
        for (int d = c+1; d < n; ++d) {
            for (int j = 0; j < 4; ++j) {
                A[0][j] = rows[a][j]; A[1][j] = rows[b][j];
                A[2][j] = rows[c][j]; A[3][j] = rows[d][j];
            }
            if (det4(A) != 0) return 4;
        }
    }
    if (kmax >= 3) {
        for (int a = 0; a < n; ++a)
        for (int b = a+1; b < n; ++b)
        for (int c = b+1; c < n; ++c) {
            for (int x = 0; x < 4; ++x)
            for (int y = x+1; y < 4; ++y)
            for (int z = y+1; z < 4; ++z) {
                if (det3(rows[a][x],rows[a][y],rows[a][z],
                         rows[b][x],rows[b][y],rows[b][z],
                         rows[c][x],rows[c][y],rows[c][z]) != 0)
                    return 3;
            }
        }
    }
    for (int a = 0; a < n; ++a)
    for (int b = a+1; b < n; ++b)
    for (int x = 0; x < 4; ++x)
    for (int y = x+1; y < 4; ++y)
        if (rows[a][x]*rows[b][y] - rows[a][y]*rows[b][x] != 0) return 2;
    for (int a = 0; a < n; ++a)
        if (rows[a][0]|rows[a][1]|rows[a][2]|rows[a][3]) return 1;
    return 0;
}

// y = M^T x
static void mulMT(const int M[4][4], const int x[4], int y[4]) {
    for (int i = 0; i < 4; ++i) {
        y[i] = 0;
        for (int j = 0; j < 4; ++j) y[i] += M[j][i] * x[j];
    }
}

static int krylov_A_dim(const int M[4][4], const int A[4], const int T[4]) {
    int rows[6][4];
    for (int j = 0; j < 4; ++j) {
        rows[0][j] = 1;
        rows[1][j] = A[j];
        rows[2][j] = T[j];
    }
    int x[4]; for (int j=0;j<4;++j) x[j]=T[j];
    int n = 3;
    for (int k = 0; k < 3; ++k) {
        int y[4]; mulMT(M, x, y);
        for (int j=0;j<4;++j) { rows[n][j]=y[j]; x[j]=y[j]; }
        n++;
    }
    return rank_rows(rows, n);
}

static int krylov_T_dim(const int M[4][4], const int T[4]) {
    int rows[5][4];
    for (int j = 0; j < 4; ++j) {
        rows[0][j] = 1;
        rows[1][j] = T[j];
    }
    int x[4]; for (int j=0;j<4;++j) x[j]=T[j];
    int n = 2;
    for (int k = 0; k < 3; ++k) {
        int y[4]; mulMT(M, x, y);
        for (int j=0;j<4;++j) { rows[n][j]=y[j]; x[j]=y[j]; }
        n++;
    }
    return rank_rows(rows, n);
}

static int n_distinct(const int T[4]) {
    int n = 0;
    for (int i = 0; i < 4; ++i) {
        int seen = 0;
        for (int j = 0; j < i; ++j) if (T[j] == T[i]) { seen = 1; break; }
        if (!seen) n++;
    }
    return n;
}

static void census2(const int alph[4], const char* name) {
    int dimA[5] = {0,0,0,0,0};
    int dimT[5] = {0,0,0,0,0};
    int Lin = 0, kerTin = 0, Taff = 0;
    int n4_Linv_aff = 0, n4_Linv_notaff = 0, n4_Linv = 0;
    int n3_Linv = 0, n3_kerT = 0, nge3_Linv_notaff = 0, nge3_kerT = 0;
    int n4_kT2 = 0, n3_kT2 = 0;
    for (int code = 0; code < 65536; ++code) {
        int t = code;
        int T[4] = {0,0,0,0};
        int M[4][4] = {{0}};
        for (int a = 0; a < 4; ++a) {
            int x = t % 4; int y = (t/4)%4; t /= 16;
            T[a] = alph[x] + alph[y];
            M[x][a]++; M[y][a]++;
        }
        int kA = krylov_A_dim(M, alph, T);
        int kT = krylov_T_dim(M, T);
        if (kA >= 0 && kA <= 4) dimA[kA]++;
        if (kT >= 0 && kT <= 4) dimT[kT]++;
        int T2[4]; mulMT(M, T, T2);
        int onesT[2][4]; int onesTT2[3][4]; int onesA[2][4]; int onesAT[3][4];
        for (int j=0;j<4;++j) {
            onesT[0][j]=1; onesT[1][j]=T[j];
            onesTT2[0][j]=1; onesTT2[1][j]=T[j]; onesTT2[2][j]=T2[j];
            onesA[0][j]=1; onesA[1][j]=alph[j];
            onesAT[0][j]=1; onesAT[1][j]=alph[j]; onesAT[2][j]=T[j];
        }
        int Linv = rank_rows(onesTT2, 3) == rank_rows(onesT, 2);
        int TT2[2][4];
        int Trow[1][4];
        for (int j=0;j<4;++j) { TT2[0][j]=T[j]; TT2[1][j]=T2[j]; Trow[0][j]=T[j]; }
        int kerTinv = rank_rows(TT2, 2) == rank_rows(Trow, 1);
        int aff = rank_rows(onesAT, 3) == rank_rows(onesA, 2);
        if (Linv) Lin++;
        if (kerTinv) kerTin++;
        if (aff) Taff++;
        int ns = n_distinct(T);
        if (ns == 4 && Linv) {
            n4_Linv++;
            if (aff) n4_Linv_aff++; else n4_Linv_notaff++;
        }
        if (ns == 3 && Linv) n3_Linv++;
        if (ns == 3 && kerTinv) n3_kerT++;
        if (ns >= 3 && Linv && !aff) nge3_Linv_notaff++;
        if (ns >= 3 && kerTinv) nge3_kerT++;
        if (ns == 4 && kT == 2) n4_kT2++;
        if (ns == 3 && kT == 2) n3_kT2++;
    }
    std::cout << name << " krylov_A 0..4:";
    for (int k=0;k<=4;++k) std::cout << " " << dimA[k];
    std::cout << "\n" << name << " krylov_T 0..4:";
    for (int k=0;k<=4;++k) std::cout << " " << dimT[k];
    std::cout << "\n" << name << " L_invariant " << Lin
              << " kerT_invariant " << kerTin
              << " T_affine " << Taff << "\n";
    std::cout << name << " n4_Linv " << n4_Linv
              << " affine " << n4_Linv_aff
              << " not_affine " << n4_Linv_notaff << "\n";
    std::cout << name << " n3_Linv " << n3_Linv
              << " n3_kerT " << n3_kerT
              << " nge3_Linv_notaff " << nge3_Linv_notaff
              << " nge3_kerT " << nge3_kerT << "\n";
    std::cout << name << " n4_krylovT2 " << n4_kT2
              << " n3_krylovT2 " << n3_kT2 << "\n";
}

int main(int argc, char** argv) {
    std::string cmd = argc>1 ? argv[1] : "famous";
    int Aletter[4] = {0,1,2,3};
    int Bletter[4] = {0,1,3,4};
    if (cmd == "famous") {
        int M[4][4] = {{0,0,1,1},{0,1,0,1},{1,0,1,0},{1,1,0,0}};
        int T[4] = {5,4,2,1};
        int T2[4]; mulMT(M, T, T2);
        int v[4] = {1,-1,1,-1};
        int Tdot=0, T2dot=0;
        for (int i=0;i<4;++i) { Tdot += T[i]*v[i]; T2dot += T2[i]*v[i]; }
        int kA = krylov_A_dim(M, Aletter, T);
        int kT = krylov_T_dim(M, T);
        std::cout << "T2=(" << T2[0]<<","<<T2[1]<<","<<T2[2]<<","<<T2[3]
                  << ") T·v=" << Tdot << " T2·v=" << T2dot
                  << " krylov_A=" << kA << " krylov_T=" << kT << "\n";
        return 0;
    }
    if (cmd == "census") {
        census2(Aletter, "A");
        census2(Bletter, "B");
        return 0;
    }
    std::cerr << "usage: quotient_scan famous | census\n";
    return 1;
}
