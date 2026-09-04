#pragma once
// Additive-cube-free (ACF) primitives over integer alphabets.
// All checks use prefix sums. Incremental checks only test cubes that
// end at the newest letter (valid iff the prefix was already ACF).

#include <cstdint>
#include <vector>
#include <string>
#include <utility>
#include <algorithm>
#include <optional>
#include <array>
#include <iostream>
#include <cassert>

namespace acf {

using u8 = uint8_t;
using i64 = int64_t;

struct Cube {
    int i;   // start index
    int d;   // block length
    i64 sum;
};

inline std::vector<i64> prefix_sums(const std::vector<u8>& w) {
    std::vector<i64> S(w.size() + 1, 0);
    for (size_t k = 0; k < w.size(); ++k) S[k + 1] = S[k] + w[k];
    return S;
}

// Full O(n^2) scan. Returns the lexicographically first cube (smallest i, then d).
inline std::optional<Cube> find_cube(const std::vector<u8>& w) {
    const int n = (int)w.size();
    if (n < 3) return std::nullopt;
    auto S = prefix_sums(w);
    for (int i = 0; i + 3 <= n; ++i) {
        const int dmax = (n - i) / 3;
        for (int d = 1; d <= dmax; ++d) {
            i64 s1 = S[i + d] - S[i];
            i64 s2 = S[i + 2 * d] - S[i + d];
            i64 s3 = S[i + 3 * d] - S[i + 2 * d];
            if (s1 == s2 && s2 == s3) return Cube{i, d, s1};
        }
    }
    return std::nullopt;
}

inline bool is_acf(const std::vector<u8>& w) {
    return !find_cube(w).has_value();
}

// O(n) test: does appending nothing new, i.e. does the prefix of length n
// contain a cube ENDING at position n-1? Requires S.size() == n+1.
inline bool has_cube_ending_at(const std::vector<i64>& S, int n) {
    if (n < 3) return false;
    const int dmax = n / 3;
    for (int d = 1; d <= dmax; ++d) {
        // blocks [n-3d, n-2d), [n-2d, n-d), [n-d, n)
        i64 s3 = S[n] - S[n - d];
        i64 s2 = S[n - d] - S[n - 2 * d];
        if (s3 != s2) continue;
        i64 s1 = S[n - 2 * d] - S[n - 3 * d];
        if (s1 == s2) return true;
    }
    return false;
}

inline std::optional<Cube> find_cube_ending_at(const std::vector<i64>& S, int n) {
    if (n < 3) return std::nullopt;
    const int dmax = n / 3;
    for (int d = 1; d <= dmax; ++d) {
        i64 s3 = S[n] - S[n - d];
        i64 s2 = S[n - d] - S[n - 2 * d];
        if (s3 != s2) continue;
        i64 s1 = S[n - 2 * d] - S[n - 3 * d];
        if (s1 == s2) return Cube{n - 3 * d, d, s1};
    }
    return std::nullopt;
}

inline std::vector<u8> parse_digits(const std::string& s) {
    std::vector<u8> w;
    w.reserve(s.size());
    for (char c : s) {
        if (c >= '0' && c <= '9') w.push_back((u8)(c - '0'));
        else if (c == ' ' || c == '\n' || c == '\t' || c == ',') continue;
        else throw std::runtime_error("bad symbol in word");
    }
    return w;
}

inline std::string to_string(const std::vector<u8>& w) {
    std::string s;
    s.resize(w.size());
    for (size_t i = 0; i < w.size(); ++i) s[i] = char('0' + w[i]);
    return s;
}

// Brute-force reference (no prefix sums): used only in tests.
inline bool brute_is_acf(const std::vector<u8>& w) {
    const int n = (int)w.size();
    for (int d = 1; 3 * d <= n; ++d) {
        for (int i = 0; i + 3 * d <= n; ++i) {
            i64 s1 = 0, s2 = 0, s3 = 0;
            for (int t = 0; t < d; ++t) {
                s1 += w[i + t];
                s2 += w[i + d + t];
                s3 += w[i + 2 * d + t];
            }
            if (s1 == s2 && s2 == s3) return false;
        }
    }
    return true;
}

}  // namespace acf
