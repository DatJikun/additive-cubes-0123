// Dump the vote 4-tuples (x_d for d=1,2,3,4) on ACF words of length 11
// where all four d are active. Seek an invariant that forces a collision.
#include "acf.hpp"
#include "search.hpp"
#include <iostream>
#include <map>
using namespace acf;

static bool votes(const Builder& b, int xd[6], int* act, int* cov) {
    const int n = b.size();
    const int N = n + 1;
    const int dmax = N / 3;
    int forb[4] = {0, 0, 0, 0};
    int a = 0;
    for (int d = 1; d <= 5; ++d) xd[d] = -9;
    for (int d = 1; d <= dmax; ++d) {
        int i1 = n + 1 - 3 * d;
        int i2 = n + 1 - 2 * d;
        int i3 = n + 1 - d;
        if (i1 < 0) continue;
        i64 s1 = b.S[i2] - b.S[i1];
        i64 s2 = b.S[i3] - b.S[i2];
        if (s1 != s2) continue;
        ++a;
        i64 last = b.S[n] - b.S[i3];
        i64 x = s1 - last;
        if (x >= 0 && x <= 3) {
            xd[d] = (int)x;
            forb[x] = 1;
        } else {
            xd[d] = -1;  // active but x not in alphabet
        }
    }
    *act = a;
    *cov = forb[0] + forb[1] + forb[2] + forb[3];
    return true;
}

int main() {
    std::map<std::array<int, 4>, int> tuples11, tuples12, tuples13;
    std::map<int, int> miss11;
    uint64_t n4_11 = 0, n4_12 = 0, n4_13 = 0;
    Builder b;
    std::function<void()> rec = [&]() {
        int n = b.size();
        if (n >= 11 && n <= 13) {
            int xd[6], act = 0, cov = 0;
            votes(b, xd, &act, &cov);
            if (act >= 4) {
                std::array<int, 4> t{xd[1], xd[2], xd[3], xd[4]};
                if (n == 11) {
                    ++n4_11;
                    tuples11[t]++;
                    int miss = 0;
                    bool seen[4]{};
                    for (int d = 1; d <= 4; ++d)
                        if (xd[d] >= 0) seen[xd[d]] = true;
                    for (int a = 0; a < 4; ++a)
                        if (!seen[a]) miss += 1 << a;
                    miss11[miss]++;
                } else if (n == 12) {
                    ++n4_12;
                    tuples12[t]++;
                } else {
                    ++n4_13;
                    tuples13[t]++;
                }
            }
        }
        if (n == 13) return;
        for (int a = 0; a < 4; ++a)
            if (b.try_push((u8)a)) {
                rec();
                b.pop();
            }
    };
    rec();
    auto show = [](const char* name, const std::map<std::array<int, 4>, int>& m, uint64_t tot) {
        std::cout << name << " 4-active=" << tot << " distinct_tuples=" << m.size() << "\n";
        std::vector<std::pair<int, std::array<int, 4>>> v;
        for (auto& kv : m) v.push_back({kv.second, kv.first});
        std::sort(v.begin(), v.end(), [](auto& x, auto& y) { return x.first > y.first; });
        for (size_t i = 0; i < std::min<size_t>(30, v.size()); ++i) {
            auto t = v[i].second;
            // permutation? collision?
            int seen[4]{}, col = 0, oob = 0;
            for (int j = 0; j < 4; ++j) {
                if (t[j] < 0) {
                    ++oob;
                    continue;
                }
                if (seen[t[j]]++) ++col;
            }
            std::cout << "  " << v[i].first << "  x=(" << t[0] << "," << t[1] << "," << t[2] << ","
                      << t[3] << ") coll=" << col << " oob=" << oob << "\n";
        }
    };
    show("n=11", tuples11, n4_11);
    std::cout << "n=11 missed-letter bitmasks (bit a = letter a missing):\n";
    for (auto& kv : miss11) {
        std::cout << "  mask=" << kv.first << " bits";
        for (int a = 0; a < 4; ++a)
            if (kv.first & (1 << a)) std::cout << " miss" << a;
        std::cout << " count=" << kv.second << "\n";
    }
    show("n=12", tuples12, n4_12);
    show("n=13", tuples13, n4_13);

    // Parity / sum invariants of (x1,x2,x3,x4) at n=11
    std::map<int, int> sumx, xorx, nunique;
    for (auto& kv : tuples11) {
        auto t = kv.first;
        int s = 0, x = 0, u = 0;
        bool se[8]{};
        for (int j = 0; j < 4; ++j) {
            if (t[j] >= 0) {
                s += t[j];
                x ^= t[j];
                if (!se[t[j]]) {
                    se[t[j]] = true;
                    ++u;
                }
            }
        }
        sumx[s] += kv.second;
        xorx[x] += kv.second;
        nunique[u] += kv.second;
    }
    std::cout << "n=11 vote-sum hist:";
    for (auto& kv : sumx) std::cout << " " << kv.first << ":" << kv.second;
    std::cout << "\n n=11 vote-xor hist:";
    for (auto& kv : xorx) std::cout << " " << kv.first << ":" << kv.second;
    std::cout << "\n n=11 #distinct votes hist:";
    for (auto& kv : nunique) std::cout << " " << kv.first << ":" << kv.second;
    std::cout << "\n";
    return 0;
}
