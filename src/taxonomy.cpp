#include "acf.hpp"
#include "search.hpp"

#include <algorithm>
#include <array>
#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <vector>

using namespace acf;

static std::string complement_word(const std::string& w) {
    std::string c = w;
    for (char& ch : c) ch = static_cast<char>('0' + (3 - (ch - '0')));
    return c;
}

static std::string reverse_word(const std::string& w) {
    return std::string(w.rbegin(), w.rend());
}

static std::string orbit_key(const std::string& w) {
    std::string c = complement_word(w);
    std::string r = reverse_word(w);
    std::string cr = reverse_word(c);
    return std::min(std::min(w, c), std::min(r, cr));
}

// For each appended letter: block length d of the shortest cube created.
struct Sig {
    int d[4];
    int imod[4];
    bool operator<(const Sig& o) const {
        for (int a = 0; a < 4; ++a) {
            if (d[a] != o.d[a]) return d[a] < o.d[a];
            if (imod[a] != o.imod[a]) return imod[a] < o.imod[a];
        }
        return false;
    }
};

static Sig classify(const std::string& w) {
    Builder b;
    for (char c : w) {
        if (!b.try_push(static_cast<u8>(c - '0'))) {
            std::cerr << "not ACF: " << w << "\n";
            std::exit(1);
        }
    }
    Sig s{};
    for (int a = 0; a < 4; ++a) {
        b.w.push_back(static_cast<u8>(a));
        b.S.push_back(b.S.back() + a);
        auto wtn = find_cube_ending_at(b.S, static_cast<int>(b.w.size()));
        if (!wtn) {
            std::cerr << "extendable? " << w << " +" << a << "\n";
            std::exit(1);
        }
        s.d[a] = wtn->d;
        s.imod[a] = wtn->i % wtn->d;
        b.w.pop_back();
        b.S.pop_back();
    }
    return s;
}

int main(int argc, char** argv) {
    const char* path = (argc > 1) ? argv[1] : "data/deadends14.txt";
    std::vector<std::string> words;
    {
        std::ifstream in(path);
        if (!in) {
            std::cerr << "cannot open " << path << "\n";
            return 1;
        }
        std::string line;
        while (std::getline(in, line)) {
            if (line.empty() || line[0] == '#') continue;
            // keep only digit lines
            if (line.find_first_not_of("0123") != std::string::npos) continue;
            words.push_back(line);
        }
    }
    std::cerr << "loaded " << words.size() << " dead-ends\n";

    std::map<Sig, int> sig_count;
    std::map<Sig, std::string> sig_example;
    std::map<int, int> d_hist[4];
    std::map<std::string, int> orbit;
    std::map<std::string, int> suffix_count[9];  // last 1..8
    std::map<int, int> end_run;                  // trailing run length of last letter
    int n_self_comp = 0, n_self_rev = 0;
    int n_end_double = 0;
    int type_d1_forbids = 0;  // how many letters forbidden by d=1
    std::map<int, int> n_d1_forbids_hist;
    int all_d_le5 = 0;

    for (const auto& w : words) {
        Sig s = classify(w);
        sig_count[s]++;
        if (!sig_example.count(s)) sig_example[s] = w;
        for (int a = 0; a < 4; ++a) d_hist[a][s.d[a]]++;
        orbit[orbit_key(w)]++;
        if (w == complement_word(w)) ++n_self_comp;
        if (w == reverse_word(w)) ++n_self_rev;
        if (w.size() >= 2 && w[w.size() - 1] == w[w.size() - 2]) ++n_end_double;
        int run = 1;
        for (int i = (int)w.size() - 2; i >= 0 && w[i] == w.back(); --i) ++run;
        end_run[run]++;
        int d1c = 0;
        bool small = true;
        for (int a = 0; a < 4; ++a) {
            if (s.d[a] == 1) ++d1c;
            if (s.d[a] > 5) small = false;
        }
        n_d1_forbids_hist[d1c]++;
        type_d1_forbids += d1c;
        if (small) ++all_d_le5;
        for (int k = 1; k <= 8; ++k) {
            if ((int)w.size() >= k) suffix_count[k][w.substr(w.size() - k)]++;
        }
    }

    std::cout << "n_deadends=" << words.size() << "\n";
    std::cout << "n_orbits=" << orbit.size() << "\n";
    std::cout << "n_signatures=" << sig_count.size() << "\n";
    std::cout << "self_complement=" << n_self_comp << " self_reverse=" << n_self_rev << "\n";
    std::cout << "end_with_double_letter=" << n_end_double << "\n";
    std::cout << "words_with_all_witness_d_le5=" << all_d_le5 << "\n";

    std::cout << "\n# trailing-run length of last letter\n";
    for (auto& kv : end_run) std::cout << "run=" << kv.first << " count=" << kv.second << "\n";

    std::cout << "\n# how many of the 4 extensions are ordinary cubes (d=1)\n";
    for (auto& kv : n_d1_forbids_hist)
        std::cout << "d1_forbids=" << kv.first << " words=" << kv.second << "\n";

    std::cout << "\n# d-histogram per appended letter\n";
    for (int a = 0; a < 4; ++a) {
        std::cout << "letter " << a << ":";
        for (auto& kv : d_hist[a]) std::cout << " d=" << kv.first << ":" << kv.second;
        std::cout << "\n";
    }

    std::cout << "\n# most common suffixes (k=1..6)\n";
    for (int k = 1; k <= 6; ++k) {
        std::vector<std::pair<int, std::string>> v;
        for (auto& kv : suffix_count[k]) v.push_back({kv.second, kv.first});
        std::sort(v.begin(), v.end(), [](auto& x, auto& y) { return x.first > y.first; });
        std::cout << "k=" << k << " distinct=" << v.size() << " top:";
        for (size_t i = 0; i < std::min<size_t>(8, v.size()); ++i)
            std::cout << " " << v[i].second << ":" << v[i].first;
        std::cout << "\n";
    }

    std::vector<std::pair<int, Sig>> ranked;
    for (auto& kv : sig_count) ranked.push_back({kv.second, kv.first});
    std::sort(ranked.begin(), ranked.end(), [](auto& x, auto& y) { return x.first > y.first; });

    std::cout << "\n# top signatures (count, d0,d1,d2,d3, imod, example)\n";
    int shown = 0;
    for (auto& pr : ranked) {
        if (shown++ >= 25) break;
        const Sig& s = pr.second;
        std::cout << pr.first << "  d=(" << s.d[0] << "," << s.d[1] << "," << s.d[2] << "," << s.d[3]
                  << ") imod=(" << s.imod[0] << "," << s.imod[1] << "," << s.imod[2] << "," << s.imod[3]
                  << ") ex=" << sig_example[s] << "\n";
    }

    std::map<std::array<int, 4>, int> dquad;
    std::map<std::array<int, 4>, std::string> dquad_ex;
    for (auto& kv : sig_count) {
        std::array<int, 4> q{kv.first.d[0], kv.first.d[1], kv.first.d[2], kv.first.d[3]};
        dquad[q] += kv.second;
        if (!dquad_ex.count(q)) dquad_ex[q] = sig_example[kv.first];
    }
    std::cout << "distinct d-quadruples: " << dquad.size() << "\n";
    std::vector<std::pair<int, std::array<int, 4>>> rq;
    for (auto& kv : dquad) rq.push_back({kv.second, kv.first});
    std::sort(rq.begin(), rq.end(), [](auto& x, auto& y) { return x.first > y.first; });
    std::cout << "\n# all d-quadruples (count >= 20) or top 40\n";
    for (size_t i = 0; i < rq.size(); ++i) {
        if (i >= 40 && rq[i].first < 20) continue;
        auto& q = rq[i].second;
        std::cout << rq[i].first << "  (" << q[0] << "," << q[1] << "," << q[2] << "," << q[3]
                  << ") ex=" << dquad_ex[q] << "\n";
    }

    // Sorted unique d-set used by the four witnesses
    std::map<std::string, int> dset;
    for (auto& kv : sig_count) {
        std::array<int, 4> arr{kv.first.d[0], kv.first.d[1], kv.first.d[2], kv.first.d[3]};
        std::sort(arr.begin(), arr.end());
        std::string key = std::to_string(arr[0]) + "," + std::to_string(arr[1]) + "," +
                          std::to_string(arr[2]) + "," + std::to_string(arr[3]);
        dset[key] += kv.second;
    }
    std::cout << "\n# sorted d-multiset of the four witnesses\n";
    std::vector<std::pair<int, std::string>> ds;
    for (auto& kv : dset) ds.push_back({kv.second, kv.first});
    std::sort(ds.begin(), ds.end(), [](auto& x, auto& y) { return x.first > y.first; });
    for (auto& pr : ds) std::cout << pr.first << "  {" << pr.second << "}\n";
    return 0;
}
