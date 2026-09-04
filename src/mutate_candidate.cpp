#include "acf.hpp"
#include "search.hpp"
#include <sstream>
#include <random>
using namespace acf;

// Mutate a promising size-3 Cassaigne-shaped morphism, hunting for a longer ACF prefix.
static int survive_to(const std::array<std::vector<u8>, 4>& h, int cap, Cube* out) {
    auto [w, c] = fixed_point_incremental(h, 0, cap);
    if (c) {
        if (out) *out = *c;
        return c->i + 3 * c->d;
    }
    return (int)w.size();
}

int main() {
    std::array<std::vector<u8>, 4> base;
    base[0] = {0, 0, 1};
    base[1] = {0, 3, 1};
    base[2] = {1};
    base[3] = {2, 3, 3};
    int best = 0;
    std::string bests;
    std::mt19937 rng(7);
    // All single-symbol mutants of images of total length 10 (3+3+1+3), plus the base.
    // 10 positions * 4 symbols = 40, plus nearby random.
    auto eval = [&](const std::array<std::vector<u8>, 4>& h) {
        if (h[0].empty() || h[0][0] != 0) return;
        Cube c{};
        int s = survive_to(h, 5000, &c);
        if (s > best) {
            best = s;
            std::ostringstream o;
            for (int a = 0; a < 4; ++a) o << a << "->" << to_string(h[a]) << " ";
            bests = o.str();
            std::cout << "best " << best << "  " << bests;
            if (s < 5000) std::cout << " cube i=" << c.i << " d=" << c.d;
            std::cout << "\n";
        }
    };
    eval(base);
    // mutate each symbol
    for (int a = 0; a < 4; ++a) {
        for (size_t i = 0; i < base[a].size(); ++i) {
            if (a == 0 && i == 0) continue;
            for (u8 x = 0; x < 4; ++x) {
                auto h = base;
                h[a][i] = x;
                eval(h);
            }
        }
    }
    // two-point mutants, sample
    for (int t = 0; t < 2000; ++t) {
        auto h = base;
        for (int k = 0; k < 2; ++k) {
            int a = rng() % 4;
            if (h[a].empty()) continue;
            int i = rng() % (int)h[a].size();
            if (a == 0 && i == 0) continue;
            h[a][i] = rng() % 4;
        }
        eval(h);
    }
    std::cout << "final_best " << best << " " << bests << "\n";
}
