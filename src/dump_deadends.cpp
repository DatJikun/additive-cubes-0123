#include "acf.hpp"
#include "search.hpp"
#include <fstream>
using namespace acf;
int main() {
    std::ofstream out("/agent/data/deadends14.txt");
    uint64_t n = 0;
    Builder b;
    std::function<void()> rec = [&]() {
        if (b.size() == 15) return;
        int ch = 0;
        for (int a = 0; a < 4; ++a) {
            if (b.try_push((u8)a)) {
                ++ch;
                rec();
                b.pop();
            }
        }
        if (ch == 0 && b.size() == 14) {
            out << to_string(b.w) << "\n";
            ++n;
        }
    };
    rec();
    std::cout << "wrote " << n << " dead-ends of length 14\n";
}
