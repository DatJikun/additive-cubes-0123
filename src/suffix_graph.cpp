// Overlap graph of ACF n-mers: nodes = ACF words of length n,
// edge u -a-> v if v is the length-n suffix of ua and ua is ACF.
// A cycle yields an infinite word with no additive cube of block length
// d <= floor((n+1)/3). (Periodic walking of the cycle still has large-d cubes.)
#include "acf.hpp"
#include "search.hpp"
#include <cstdlib>
#include <iostream>
#include <unordered_map>
#include <unordered_set>
using namespace acf;

static uint64_t pack(const std::vector<u8>& w) {
    uint64_t x = 0;
    for (u8 a : w) x = (x << 2) | a;
    return x;
}

int main(int argc, char** argv) {
    int n = (argc > 1) ? std::atoi(argv[1]) : 8;
    if (n < 2 || n > 12) {
        std::cerr << "n in 2..12\n";
        return 1;
    }
    std::vector<uint64_t> nodes;
    std::unordered_map<uint64_t, int> idx;
    Builder b;
    std::function<void()> rec = [&]() {
        if (b.size() == n) {
            uint64_t p = pack(b.w);
            idx[p] = (int)nodes.size();
            nodes.push_back(p);
            return;
        }
        for (int a = 0; a < 4; ++a)
            if (b.try_push((u8)a)) {
                rec();
                b.pop();
            }
    };
    rec();
    std::cout << "n=" << n << " nodes=" << nodes.size() << "\n";

    int edges = 0;
    int n_out0 = 0, n_out4 = 0;
    std::array<int, 5> outhist{};
    std::vector<int> outdeg(nodes.size(), 0);
    std::vector<std::vector<int>> adj(nodes.size());
    for (size_t i = 0; i < nodes.size(); ++i) {
        uint64_t p = nodes[i];
        // unpack
        std::vector<u8> w(n);
        uint64_t t = p;
        for (int k = n - 1; k >= 0; --k) {
            w[k] = t & 3;
            t >>= 2;
        }
        Builder B;
        for (u8 a : w) B.try_push(a);
        int od = 0;
        for (int a = 0; a < 4; ++a) {
            if (B.try_push((u8)a)) {
                ++od;
                uint64_t q = pack(B.w) & ((n == 32) ? ~0ull : ((1ull << (2 * n)) - 1ull));
                // suffix n of the n+1 word: drop first 2 bits of the n+1 packing
                // rebuild from B.w[1..]
                std::vector<u8> suf(B.w.begin() + 1, B.w.end());
                q = pack(suf);
                auto it = idx.find(q);
                if (it != idx.end()) {
                    adj[i].push_back(it->second);
                    ++edges;
                }
                B.pop();
            }
        }
        outdeg[i] = od;
        outhist[od]++;
        if (od == 0) ++n_out0;
        if (od == 4) ++n_out4;
    }
    std::cout << "edges=" << edges << " out0=" << n_out0 << " out4=" << n_out4 << "\n";
    std::cout << "outdeg hist:";
    for (int d = 0; d <= 4; ++d) std::cout << " " << d << ":" << outhist[d];
    std::cout << "\n";

    // Kosaraju SCC (iterative to avoid stack overflow)
    int N = (int)nodes.size();
    std::vector<int> order;
    order.reserve(N);
    std::vector<char> seen(N, 0);
    std::vector<int> stack, itidx;
    for (int start = 0; start < N; ++start) {
        if (seen[start]) continue;
        stack.push_back(start);
        itidx.push_back(0);
        seen[start] = 1;
        while (!stack.empty()) {
            int u = stack.back();
            if (itidx.back() < (int)adj[u].size()) {
                int v = adj[u][itidx.back()++];
                if (!seen[v]) {
                    seen[v] = 1;
                    stack.push_back(v);
                    itidx.push_back(0);
                }
            } else {
                order.push_back(u);
                stack.pop_back();
                itidx.pop_back();
            }
        }
    }
    std::vector<std::vector<int>> radj(N);
    for (int u = 0; u < N; ++u)
        for (int v : adj[u]) radj[v].push_back(u);
    std::fill(seen.begin(), seen.end(), 0);
    int nscc = 0, maxscc = 0, nscc_cyc = 0;
    long long nodes_in_cyc = 0;
    auto dfs2_iter = [&](int start) -> int {
        int sz = 0;
        std::vector<int> st;
        st.push_back(start);
        seen[start] = 1;
        while (!st.empty()) {
            int u = st.back();
            st.pop_back();
            ++sz;
            for (int v : radj[u])
                if (!seen[v]) {
                    seen[v] = 1;
                    st.push_back(v);
                }
        }
        return sz;
    };
    // self-loops count as cyclic SCC of size 1
    std::vector<char> has_self(N, 0);
    for (int u = 0; u < N; ++u)
        for (int v : adj[u])
            if (v == u) has_self[u] = 1;
    for (int i = N - 1; i >= 0; --i) {
        int u = order[i];
        if (seen[u]) continue;
        int sz = dfs2_iter(u);
        ++nscc;
        if (sz > maxscc) maxscc = sz;
        if (sz > 1) {
            ++nscc_cyc;
            nodes_in_cyc += sz;
        }
    }
    int selfloops = 0;
    for (int u = 0; u < N; ++u)
        if (has_self[u]) ++selfloops;
    std::cout << "SCC=" << nscc << " maxSCC=" << maxscc << " SCC_size>1=" << nscc_cyc
              << " nodes_in_size>1=" << nodes_in_cyc << " selfloops=" << selfloops << "\n";
    std::cout << "d_max_avoided_by_any_cycle=" << (n + 1) / 3 << "  (cubes with d<=this are "
                 "avoided by infinite walks)\n";
    return 0;
}
