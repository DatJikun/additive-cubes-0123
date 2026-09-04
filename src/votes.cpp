// Count, for every ACF word of length n, how many block-lengths d are "active"
// (s1==s2, so they forbid at most one letter) and whether the active votes
// cover {0,1,2,3}. Explains why minimal dead-ends cannot occur at n=11,12,13.
#include "acf.hpp"
#include "search.hpp"
#include <iostream>
#include <map>
using namespace acf;

struct Row {
    uint64_t words = 0;
    uint64_t n_active[8]{};  // how many words have exactly k active d's
    uint64_t n_cover[5]{};   // how many distinct letters forbidden
    uint64_t n_full_cover = 0;
    uint64_t n_four_active = 0;
    uint64_t n_five_active = 0;
};

static void votes_at(const Builder& b, int* n_active, int* n_cov, int* cover_mask) {
    const int n = b.size();
    const int N = n + 1;  // after a hypothetical append
    const int dmax = N / 3;
    bool used_d[16]{};
    int forb[4];
    for (int a = 0; a < 4; ++a) forb[a] = 0;
    int act = 0;
    for (int d = 1; d <= dmax; ++d) {
        // s1,s2 from current S, independent of x; positions as if length N
        // S has size n+1 (indices 0..n). After append, S[N]=S[n]+x.
        // s1 = S[N-2d]-S[N-3d] = S[n+1-2d]-S[n+1-3d]
        // s2 = S[N-d]-S[N-2d] = S[n+1-d]-S[n+1-2d]
        int i1 = n + 1 - 3 * d;
        int i2 = n + 1 - 2 * d;
        int i3 = n + 1 - d;
        if (i1 < 0) continue;
        i64 s1 = b.S[i2] - b.S[i1];
        i64 s2 = b.S[i3] - b.S[i2];
        if (s1 != s2) continue;
        ++act;
        used_d[d] = true;
        i64 last = b.S[n] - b.S[i3];  // sum of last (d-1) letters; i3 = n+1-d
        i64 x = s1 - last;
        if (x >= 0 && x <= 3) forb[x] = 1;
    }
    int cov = forb[0] + forb[1] + forb[2] + forb[3];
    int mask = forb[0] + 2 * forb[1] + 4 * forb[2] + 8 * forb[3];
    *n_active = act;
    *n_cov = cov;
    *cover_mask = mask;
    (void)used_d;
}

int main(int argc, char** argv) {
    int nmax = (argc > 1) ? std::atoi(argv[1]) : 14;
    std::vector<Row> R(nmax + 1);
    Builder b;
    std::function<void()> rec = [&]() {
        int n = b.size();
        if (n >= 3) {
            int act = 0, cov = 0, mask = 0;
            votes_at(b, &act, &cov, &mask);
            R[n].words++;
            if (act < 8) R[n].n_active[act]++;
            else R[n].n_active[7]++;
            if (cov <= 4) R[n].n_cover[cov]++;
            if (cov == 4) R[n].n_full_cover++;
            if (act >= 4) R[n].n_four_active++;
            if (act >= 5) R[n].n_five_active++;
        } else if (n <= nmax) {
            R[n].words++;
        }
        if (n == nmax) return;
        for (int a = 0; a < 4; ++a)
            if (b.try_push((u8)a)) {
                rec();
                b.pop();
            }
    };
    rec();
    std::cout << "# n words  act0..5  cover0..4  four_active full_cover(=dead_candidates)\n";
    for (int n = 0; n <= nmax; ++n) {
        std::cout << n << " " << R[n].words << "  act";
        for (int k = 0; k <= 5; ++k) std::cout << " " << R[n].n_active[k];
        std::cout << "  cov";
        for (int k = 0; k <= 4; ++k) std::cout << " " << R[n].n_cover[k];
        std::cout << "  4act=" << R[n].n_four_active << " full=" << R[n].n_full_cover << "\n";
    }
    return 0;
}
