#include "acf.hpp"
#include "search.hpp"
using namespace acf;
int main() {
    std::array<std::vector<u8>, 4> h;
    h[0] = {0, 0, 1};
    h[1] = {0, 3, 1};
    h[2] = {1};
    h[3] = {2, 3, 3};
    for (int cap : {300, 1000, 5000, 20000, 100000, 500000}) {
        auto t0 = std::chrono::steady_clock::now();
        auto [w, c] = fixed_point_incremental(h, 0, cap);
        auto t1 = std::chrono::steady_clock::now();
        std::cout << "cap=" << cap << " got=" << w.size()
                  << " acf=" << (c ? "NO" : "yes")
                  << " time=" << std::chrono::duration<double>(t1 - t0).count();
        if (c) std::cout << " cube i=" << c->i << " d=" << c->d << " sum=" << c->sum
                         << " end=" << (c->i + 3 * c->d);
        std::cout << "\n";
        if (c) break;
    }
    return 0;
}
