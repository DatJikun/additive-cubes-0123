#pragma once
// Search algorithms for additive-cube-free words over {0,1,2,3}.

#include "acf.hpp"
#include <algorithm>
#include <chrono>
#include <cmath>
#include <functional>
#include <map>
#include <random>
#include <stdexcept>

namespace acf {

struct SearchStats {
    uint64_t nodes = 0;
    uint64_t extensions_tried = 0;
    uint64_t backtracks = 0;
    int max_depth = 0;
    int dead_end_depth_sum = 0;
    int dead_ends = 0;
    std::array<uint64_t, 4> symbol_counts{{0, 0, 0, 0}};
};

// Incremental builder.
struct Builder {
    std::vector<u8> w;
    std::vector<i64> S;  // S[0]=0, S[k]=sum of first k letters
    SearchStats stats;

    Builder() { S.push_back(0); }

    int size() const { return (int)w.size(); }

    bool try_push(u8 a) {
        ++stats.extensions_tried;
        S.push_back(S.back() + a);
        w.push_back(a);
        if (has_cube_ending_at(S, (int)w.size())) {
            w.pop_back();
            S.pop_back();
            return false;
        }
        ++stats.nodes;
        ++stats.symbol_counts[a];
        stats.max_depth = std::max(stats.max_depth, (int)w.size());
        return true;
    }

    void pop() {
        if (w.empty()) return;
        w.pop_back();
        S.pop_back();
        ++stats.backtracks;
    }
};

// Count ACF words of exact length n over alphabet {0,1,...,A-1}.
inline uint64_t count_acf(int n, int A = 4, int start_letter_min = 0) {
    if (n == 0) return 1;
    Builder b;
    uint64_t count = 0;
    std::function<void()> rec = [&]() {
        if (b.size() == n) {
            ++count;
            return;
        }
        for (int a = 0; a < A; ++a) {
            if (b.size() == 0 && a < start_letter_min) continue;
            if (b.try_push((u8)a)) {
                rec();
                b.pop();
            }
        }
    };
    rec();
    return count;
}

// Count ACF words of every length 0..n inclusive. Returns counts[k] = # of length k.
inline std::vector<uint64_t> count_acf_by_length(int n, int A = 4) {
    std::vector<uint64_t> counts(n + 1, 0);
    Builder b;
    std::function<void()> rec = [&]() {
        counts[b.size()]++;
        if (b.size() == n) return;
        for (int a = 0; a < A; ++a) {
            if (b.try_push((u8)a)) {
                rec();
                b.pop();
            }
        }
    };
    rec();
    return counts;
}

// DFS with a letter-order function. order(depth) returns a permutation of {0,1,2,3}.
// Returns the longest word found within node_budget extension attempts.
inline std::vector<u8> dfs_longest(
    uint64_t node_budget,
    const std::function<std::array<u8, 4>(int depth, const Builder&)>& order_fn,
    int length_cap = 1 << 30
) {
    Builder b;
    std::vector<u8> best;
    std::function<bool()> rec = [&]() -> bool {
        if ((int)b.w.size() > (int)best.size()) best = b.w;
        if ((int)b.w.size() >= length_cap) return true;
        if (b.stats.extensions_tried >= node_budget) return true;
        auto ord = order_fn(b.size(), b);
        bool any = false;
        for (u8 a : ord) {
            if (b.stats.extensions_tried >= node_budget) return true;
            if (b.try_push(a)) {
                any = true;
                if (rec()) {
                    // pop not needed if we keep the word; but we backtrack to search more
                    // unless we hit the cap. For longest-word search we still backtrack
                    // only after exploring. We always pop after rec to continue.
                }
                b.pop();
                if (b.stats.extensions_tried >= node_budget) return true;
                if ((int)best.size() >= length_cap) return true;
            }
        }
        if (!any && b.size() > 0) {
            b.stats.dead_ends++;
            b.stats.dead_end_depth_sum += b.size();
        }
        return false;
    };
    rec();
    return best;
}

// Greedy: always take the first letter in the current order that extends.
// Backtrack only when stuck. This is Liétard's "classical" / Up-and-Down engine.
inline std::vector<u8> greedy_backtrack(
    uint64_t node_budget,
    const std::function<std::array<u8, 4>(int depth, const Builder&)>& order_fn,
    int length_cap = 1 << 30
) {
    // Iterative DFS that prefers the first successful letter, i.e. a stack of
    // next-to-try indices. This is standard chronological backtracking.
    std::vector<u8> w;
    std::vector<i64> S = {0};
    std::vector<int> next_choice;  // at each depth, index into current order already tried
    std::vector<std::array<u8, 4>> orders;
    SearchStats stats;
    auto push_level = [&]() {
        Builder dummy;
        dummy.w = w;
        dummy.S = S;
        auto ord = order_fn((int)w.size(), dummy);
        orders.push_back(ord);
        next_choice.push_back(0);
    };
    push_level();
    std::vector<u8> best;
    while (!next_choice.empty() && stats.extensions_tried < node_budget) {
        int depth = (int)w.size();
        if (depth >= length_cap) break;
        int& k = next_choice.back();
        auto& ord = orders.back();
        bool extended = false;
        while (k < 4 && stats.extensions_tried < node_budget) {
            u8 a = ord[k++];
            ++stats.extensions_tried;
            S.push_back(S.back() + a);
            w.push_back(a);
            if (!has_cube_ending_at(S, (int)w.size())) {
                extended = true;
                if ((int)w.size() > (int)best.size()) best = w;
                push_level();
                break;
            }
            w.pop_back();
            S.pop_back();
        }
        if (!extended) {
            // backtrack
            next_choice.pop_back();
            orders.pop_back();
            if (w.empty()) break;
            w.pop_back();
            S.pop_back();
            ++stats.backtracks;
        }
    }
    return best;
}

inline std::array<u8, 4> order_fixed_up() { return {0, 1, 2, 3}; }
inline std::array<u8, 4> order_fixed_down() { return {3, 2, 1, 0}; }

// Liétard Up-and-Down: reverse priority every `period` letters.
inline std::function<std::array<u8, 4>(int, const Builder&)> up_down_order(int period) {
    return [period](int depth, const Builder&) {
        int phase = (period <= 0) ? 0 : (depth / period) % 2;
        return phase == 0 ? order_fixed_down() : order_fixed_up();
        // phase 0 prefers large letters (up), matching Liétard's "croissant" first.
        // Using down-first (3,2,1,0) then (0,1,2,3).
    };
}

// Drift-triggered: if partial mean > 1.5, prefer small letters, else large.
inline std::function<std::array<u8, 4>(int, const Builder&)> drift_order(double threshold = 0.0) {
    return [threshold](int /*depth*/, const Builder& b) {
        if (b.w.empty()) return order_fixed_down();
        double mean = (double)b.S.back() / (double)b.w.size();
        if (mean > 1.5 + threshold) return order_fixed_up();    // prefer 0,1,2,3
        if (mean < 1.5 - threshold) return order_fixed_down();  // prefer 3,2,1,0
        return order_fixed_down();
    };
}

// Envelope: prefer the letter whose new |S-(3/2)n| is closest to c√n.
// This is the discrepancy regime of the 400k archive (c≈5), not mean-threshold drift.
inline std::function<std::array<u8, 4>(int, const Builder&)> envelope_bang_order(double c) {
    return [c](int /*depth*/, const Builder& b) {
        if (b.w.empty()) return order_fixed_down();
        double D = (double)b.S.back() - 1.5 * (double)b.size();
        double tgt = c * std::sqrt((double)b.size());
        bool grow = std::abs(D) <= tgt;
        if (grow) return D >= 0 ? order_fixed_down() : order_fixed_up();
        return D > 0 ? order_fixed_up() : order_fixed_down();
    };
}

inline std::function<std::array<u8, 4>(int, const Builder&)> envelope_order(double c) {
    return [c](int /*depth*/, const Builder& b) {
        int n = b.size();
        i64 S = b.S.back();
        std::array<std::pair<double, u8>, 4> sc;
        for (int a = 0; a < 4; ++a) {
            double Dn = std::abs((double)(S + a) - 1.5 * (n + 1));
            double tgt = c * std::sqrt((double)(n + 1));
            sc[a] = {std::abs(Dn - tgt), (u8)a};
        }
        std::sort(sc.begin(), sc.end());
        std::array<u8, 4> ord{};
        for (int i = 0; i < 4; ++i) ord[i] = sc[i].second;
        return ord;
    };
}

// Apply morphism images[a] to a word.
inline std::vector<u8> apply_morphism(const std::array<std::vector<u8>, 4>& h,
                                     const std::vector<u8>& w) {
    std::vector<u8> out;
    for (u8 a : w) {
        const auto& img = h[a];
        out.insert(out.end(), img.begin(), img.end());
    }
    return out;
}

// Iterate a prolongable morphism from seed letter s until length >= cap or cube found.
// Returns (word, first cube or nullopt).
inline std::pair<std::vector<u8>, std::optional<Cube>>
iterate_morphism(const std::array<std::vector<u8>, 4>& h, u8 seed, int cap) {
    std::vector<u8> w = {seed};
    // grow by applying h to the current word
    while ((int)w.size() < cap) {
        auto nxt = apply_morphism(h, w);
        if (nxt.size() <= w.size()) break;  // not expanding
        w.swap(nxt);
        if ((int)w.size() > cap) w.resize(cap);
    }
    auto c = find_cube(w);
    // If the truncated word is ACF we still need to make sure we didn't cut a cube;
    // find_cube on the prefix is the right check for the prefix.
    return {w, c};
}

// Faster: generate the fixed point incrementally while checking cubes.
// Only valid if h(seed) starts with seed.
inline std::pair<std::vector<u8>, std::optional<Cube>>
fixed_point_incremental(const std::array<std::vector<u8>, 4>& h, u8 seed, int cap) {
    if (h[seed].empty() || h[seed][0] != seed) {
        // not prolongable on seed; fall back to iterate-and-truncate
        return iterate_morphism(h, seed, cap);
    }
    Builder b;
    b.try_push(seed);
    // We generate by expanding the seed iteratively, but check incrementally
    // as symbols are produced. Produce h^∞ by a queue of production.
    // Simple approach: keep applying h to the remaining unexpanded prefix.
    std::vector<u8> generated;
    generated.push_back(seed);
    // Expand level by level until cap, checking each new symbol.
    std::vector<u8> current = {seed};
    while ((int)current.size() < cap) {
        std::vector<u8> nxt;
        nxt.reserve(std::min(cap, (int)current.size() * 4));
        Builder chk;
        for (u8 a : current) {
            for (u8 x : h[a]) {
                nxt.push_back(x);
                if (!chk.try_push(x)) {
                    auto S = prefix_sums(nxt);
                    auto c = find_cube_ending_at(S, (int)nxt.size());
                    if ((int)nxt.size() > cap) nxt.resize(cap);
                    return {nxt, c};
                }
                if ((int)nxt.size() >= cap) {
                    return {nxt, std::nullopt};
                }
            }
        }
        if (nxt.size() <= current.size()) break;
        current.swap(nxt);
    }
    if ((int)current.size() > cap) current.resize(cap);
    return {current, find_cube(current)};
}

// Sum of an image.
inline i64 image_sum(const std::vector<u8>& v) {
    i64 s = 0;
    for (u8 x : v) s += x;
    return s;
}

}  // namespace acf
