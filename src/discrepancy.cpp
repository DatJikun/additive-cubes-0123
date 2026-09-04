// Prefix-sum discrepancy of known ACF constructions.
#include "acf.hpp"
#include "search.hpp"
#include <cmath>
#include <fstream>
#include <iostream>
#include <map>
using namespace acf;

static std::vector<u8> cassaigne(int n) {
    std::array<std::vector<u8>, 5> h;
    h[0] = {0, 3};
    h[1] = {4, 3};
    h[3] = {1};
    h[4] = {0, 1};
    std::vector<u8> w = {0};
    while ((int)w.size() < n) {
        std::vector<u8> nxt;
        for (u8 a : w) {
            const auto& img = h[a];
            nxt.insert(nxt.end(), img.begin(), img.end());
        }
        if (nxt.size() <= w.size()) break;
        w.swap(nxt);
    }
    if ((int)w.size() > n) w.resize(n);
    return w;
}

static void report(const std::string& name, const std::vector<u8>& w) {
    const int n = (int)w.size();
    auto S = prefix_sums(w);
    double mu = (double)S[n] / n;
    double maxabs = 0;
    int arg = 0;
    // Also track distinct rounded 2E = 2S - round(2μ)n if μ~p/q
    std::map<long, int> twoE;  // 2S[k]*n - 2*S[n]*k  scaled remainder
    for (int k = 1; k <= n; ++k) {
        double e = (double)S[k] - mu * k;
        if (std::fabs(e) > maxabs) {
            maxabs = std::fabs(e);
            arg = k;
        }
    }
    // Integer remainder against mean 3/2 (for {0,1,2,3}) and against empirical μ.
    long max_dev_32 = 0;
    for (int k = 1; k <= n; ++k) {
        // 2S - 3k
        long v = 2 * S[k] - 3L * k;
        if (std::labs(v) > max_dev_32) max_dev_32 = std::labs(v);
    }
    std::cout << name << " n=" << n << " mean=" << mu << " max|S-μk|=" << maxabs
              << " at k=" << arg << " max|2S-3k|=" << max_dev_32 << "\n";
    (void)twoE;
}

int main() {
    auto c = cassaigne(50000);
    std::cout << "cassaigne ACF on native alphabet? " << (is_acf(c) ? "yes" : "NO") << "\n";
    // mean of {0,1,3,4} word
    {
        auto S = prefix_sums(c);
        double mu = (double)S.back() / c.size();
        double maxabs = 0;
        int arg = 0;
        for (int k = 1; k <= (int)c.size(); ++k) {
            double e = (double)S[k] - mu * k;
            if (std::fabs(e) > maxabs) {
                maxabs = std::fabs(e);
                arg = k;
            }
        }
        std::cout << "cassaigne n=" << c.size() << " mean=" << mu << " max|S-μk|=" << maxabs
                  << " at k=" << arg << "\n";
        // distinct values of S[k] - floor(mu*k)
        std::map<int, int> bins;
        for (int k = 0; k <= (int)c.size(); ++k) {
            int r = (int)std::llround((double)S[k] - mu * k);
            bins[r]++;
        }
        std::cout << "cassaigne rounded-E distinct=" << bins.size() << " range=["
                  << bins.begin()->first << "," << bins.rbegin()->first << "]\n";
    }

    for (const char* path : {"data/word_fixed_p1000_n24396.txt", "data/word_updown_p2000_n400000.txt",
                             "data/word_updown_p1000_n65986.txt"}) {
        std::ifstream in(path);
        if (!in) {
            std::cout << "missing " << path << "\n";
            continue;
        }
        std::string s;
        in >> s;
        auto w = parse_digits(s);
        report(path, w);
    }
    return 0;
}
