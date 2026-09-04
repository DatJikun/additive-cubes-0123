# Search Algorithms

All implementations live in `src/acf.hpp`, `src/search.hpp`, `src/acf_tool.cpp`.  
Build: `make` (produces `bin/acf_tool`).

## Verifier

**Full scan.** Prefix sums \(S[0]=0\), \(S[k]=\sum_{i<k} a_i\). For every \(i,d\) with \(i+3d\le n\),
\[
S[i+d]-S[i] \;=\; S[i+2d]-S[i+d] \;=\; S[i+3d]-S[i+2d].
\]
Complexity \(O(n^2)\).

**Incremental.** If the prefix of length \(n-1\) is ACF, a cube in the prefix of length \(n\) must end at position \(n-1\). Loop \(d=1..n/3\) only. Complexity \(O(n)\) per appended letter, \(O(N^2)\) to build a word of length \(N\).

This is the same idea as Liétard’s Algorithm 4 (thesis §6.3). Independently reimplemented and cross-checked against a Python triple-loop with no prefix sums.

## Finite-language exploration

Depth-first enumeration of the trie of ACF words. At each node, try letters 0,1,2,3; keep those that pass the incremental test.

Commands:
- `./bin/acf_tool count N` — `#` of ACF words of each length `0..N`
- `./bin/acf_tool characterize N` — counts, branching factor, dead-end histogram, letter frequencies
- `./bin/acf_tool deadend NMAX` — shortest ACF words with no ACF extension

No SAT/CP solver is installed in this environment. Exact enumeration *is* the constraint search for small `N`. A SAT encoding would be: Boolean/integer variables \(a_i\in\{0,1,2,3\}\) and, for each \(i,d\), a clause forbidding equal block sums. That encoding is useful only for subclass restrictions (e.g. morphic, periodic, balanced), not for the unrestricted existence question, where a word of length 70.88M already exists.

## Greedy chronological backtracking (Liétard classical / Up-and-Down)

Maintain a stack of “next letter index in the current priority order”. Always try the first remaining letter that extends. Backtrack when all four fail.

Letter orders:
- `fixed`: always `(0,1,2,3)`
- `fixed-down`: always `(3,2,1,0)`
- `updown`: reverse the order every `PERIOD` letters
- `drift`: choose order from the sign of \(S(n)-\tfrac32 n\)

Command: `./bin/acf_tool grow MODE BUDGET CAP [PERIOD]`

## Morphisms

- `uniform K CAP [LIMIT]`: exhaustive \(K\)-uniform morphisms, all four seeds, iterate until a cube or `CAP`.
- `shape L0,L1,L2,L3 CAP`: exhaustive morphisms with prescribed image lengths (Cassaigne shape = `2,2,1,2`).
- `evo KMIN KMAX uniform|nonuniform CAP TRIALS SEED`: random morphisms, force \(h(0)\) to start with 0.

Filter used as a lemma, not as a silent skip in the final k=2 classification: if the four image-sums take at most two values, aligned cubes are forced in any infinite iterate (abelian cubes unavoidable over 2 letters).

## Periodicity

`periodic PMAX` builds \(u^3\) for periods \(1..PMAX\). This is a check of the ordinary-cube obstruction, not a search for a periodic ACF word (none exist).

## Analysis

`analyze FILE` reports length, ACF status, mean, letter frequencies, discrepancy \(\max_k |S(k)-\mu k|\), and factor complexity \(p(m)\) for \(m\le 10\).

`verify FILE` is the full O(n²) check.

## Reproducible commands

```bash
make
./bin/acf_tool test
python3 python/brute_verify.py
./bin/acf_tool count 8
./bin/acf_tool characterize 12
./bin/acf_tool deadend 14
./bin/acf_tool periodic 40
./bin/acf_tool grow fixed 2000000 30000
./bin/acf_tool grow updown 2000000 200000 1000
./bin/acf_tool uniform 2 250
./bin/acf_tool shape 2,2,1,2 400
./bin/acf_tool evo 7 12 uniform 2000 30000 1
./bin/acf_tool analyze data/word_updown_p1000_n65986.txt
```

## Benchmarks (this machine, 4 cores, g++ -O3)

| Task | Time |
|---|---|
| Full test suite (incl. Cassaigne 20k) | ~2 s |
| Characterize n=11 (~1.9e6 words of length 11) | ~1 s |
| Dead-end search n=14 | ~1 s |
| Grow fixed, budget 2e6, length 24396 | 4.7 s |
| Grow updown p=1000, budget 2e6, length 65986 | 12.9 s |
| Exhaustive uniform k=2, cap 800 (prolongable only) | few seconds |

## Finite-language core scan

`bin/core_scan` (Theorems AI–AQ, `CORE_REPORT.md`):

```bash
./bin/core_scan trie 12
./bin/core_scan inject 8
./bin/core_scan basin
./bin/core_scan detfsm
./bin/core_scan cycle 6
./bin/core_scan cass 8000
./bin/core_scan recgen 256
./bin/core_scan lcp
./bin/core_scan word data/word_updown_p1000_n65986.txt
python3 python/core_verify.py
```

