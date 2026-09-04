// Compare right-crucial lengths over alphabets {0,1,2,3} vs {0,1,3,4}.
// Unique-vote implies no dead-end for n<11 on any 4-letter integer alphabet.
// Do not use Builder::try_push: its symbol_counts[4] overflows for letter 4.
#include "acf.hpp"
#include <functional>
#include <iostream>
using namespace acf;

static void count_dead(const std::array<int, 4>& A, int nmax, std::vector<uint64_t>& n_dead,
                       std::vector<uint64_t>& n_words) {
    n_dead.assign(nmax + 1, 0);
    n_words.assign(nmax + 1, 0);
    std::vector<u8> w;
    std::vector<i64> S{0};
    std::function<void()> rec = [&]() {
        int n = (int)w.size();
        n_words[n]++;
        int ch = 0;
        for (int a : A) {
            S.push_back(S.back() + a);
            w.push_back((u8)a);
            if (!has_cube_ending_at(S, (int)w.size())) {
                ++ch;
                if (n < nmax) rec();
            }
            w.pop_back();
            S.pop_back();
        }
        if (ch == 0 && n <= nmax) n_dead[n]++;
    };
    rec();
}

int main() {
    struct Spec {
        const char* name;
        std::array<int, 4> A;
    };
    for (Spec spec : {Spec{"{0,1,2,3}", {0, 1, 2, 3}}, Spec{"{0,1,3,4}", {0, 1, 3, 4}}}) {
        std::vector<uint64_t> dead, words;
        count_dead(spec.A, 14, dead, words);
        std::cout << spec.name << "\n";
        int first = -1;
        for (int n = 0; n <= 14; ++n) {
            std::cout << "  n=" << n << " words=" << words[n] << " dead=" << dead[n] << "\n";
            if (first < 0 && dead[n]) first = n;
        }
        std::cout << "  first_deadend=" << first << "\n";
    }
    return 0;
}
