// Examples and common suffixes of 4-active length-11 ACF words, by vote tuple.
#include "acf.hpp"
#include "search.hpp"
#include <iostream>
#include <map>
#include <set>
using namespace acf;

static std::array<int, 4> vote4(const Builder& b) {
    const int n = b.size();
    std::array<int, 4> xd{-9, -9, -9, -9};
    for (int d = 1; d <= 4; ++d) {
        int i1 = n + 1 - 3 * d;
        int i2 = n + 1 - 2 * d;
        int i3 = n + 1 - d;
        i64 s1 = b.S[i2] - b.S[i1];
        i64 s2 = b.S[i3] - b.S[i2];
        if (s1 != s2) continue;
        i64 last = b.S[n] - b.S[i3];
        i64 x = s1 - last;
        xd[d - 1] = (x >= 0 && x <= 3) ? (int)x : -1;
    }
    return xd;
}

int main() {
    std::map<std::array<int, 4>, std::vector<std::string>> groups;
    Builder b;
    std::function<void()> rec = [&]() {
        if (b.size() == 11) {
            auto t = vote4(b);
            bool four = true;
            for (int d = 1; d <= 4; ++d) {
                int i1 = 11 + 1 - 3 * d;
                int i2 = 11 + 1 - 2 * d;
                int i3 = 11 + 1 - d;
                i64 s1 = b.S[i2] - b.S[i1];
                i64 s2 = b.S[i3] - b.S[i2];
                if (s1 != s2) four = false;
            }
            if (four) groups[t].push_back(to_string(b.w));
            return;
        }
        for (int a = 0; a < 4; ++a)
            if (b.try_push((u8)a)) {
                rec();
                b.pop();
            }
    };
    rec();
    for (auto& kv : groups) {
        auto t = kv.first;
        auto& ws = kv.second;
        std::cout << "tuple (" << t[0] << "," << t[1] << "," << t[2] << "," << t[3]
                  << ") n=" << ws.size() << "\n";
        // common suffix length
        for (int k = 11; k >= 1; --k) {
            std::set<std::string> suf;
            for (auto& w : ws) suf.insert(w.substr(11 - k));
            if (suf.size() == 1) {
                std::cout << "  common suffix[" << k << "]=" << *suf.begin() << "\n";
                break;
            }
            if (k <= 6) {
                std::cout << "  distinct suffixes of len " << k << ": " << suf.size() << " e.g.";
                int c = 0;
                for (auto& s : suf) {
                    std::cout << " " << s;
                    if (++c >= 6) break;
                }
                std::cout << "\n";
            }
        }
        std::cout << "  examples:";
        for (size_t i = 0; i < std::min<size_t>(5, ws.size()); ++i) std::cout << " " << ws[i];
        std::cout << "\n";
    }
    return 0;
}
