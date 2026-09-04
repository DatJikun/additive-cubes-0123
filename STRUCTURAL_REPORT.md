# Structural report: additive cubes over {0,1,2,3}

**Date:** 2026-09-04  
**Problem status:** The existence of an infinite additive-cube-free (ACF) word over `{0,1,2,3}` remains **OPEN**.  
**Claimed success level:** **LEVEL 3** (obstruction theorems for natural restricted classes + a structural account of dead-ends). Not LEVEL 5/6.

This document is the deliverable for the structural-research goal. It retracts several overclaims from the previous computational dump (`RESULTS.md`).

---

## 1. Executive result

Three things were established.

### A. Unique-vote lemma (proved)

When a letter `x` is appended to an ACF word of length `n`, each block length `d` with `3d ≤ n+1` forbids **at most one** value of `x`. Consequently a right-crucial word (ACF, but no ACF one-letter extension) over a 4-letter alphabet cannot exist unless

\[
\lfloor (n+1)/3 \rfloor \ge 4 \qquad\text{i.e.}\qquad n \ge 11.
\]

This bound needs no search. It holds for every 4-letter integer alphabet, including `{0,1,3,4}`.

### B. First dead-ends occur at length 14 — on BOTH `{0,1,2,3}` and `{0,1,3,4}`

Computationally, there is no right-crucial ACF word of length `11, 12, 13` over `{0,1,2,3}`. At those lengths, some words have four active votes, but the four votes never cover `{0,1,2,3}`: they occupy one of **12** explicit 4-tuples, closed under complement `a ↦ 3-a`, each of which either repeats a letter or casts a vote outside the alphabet.

Length 14 is the first covering: **8170** right-crucial words, in **52** witness-`d` quadruples, **4083** reverse+complement orbits.

The same first-crucial length **14** occurs over the *solved* alphabet `{0,1,3,4}` (2656 dead-ends; 96.7M ACF 14-mers vs 83.8M). So **early dead-ends do not explain exceptionality**. Both alphabets develop traps at 14; `{0,1,3,4}` simply has an infinite path that avoids later traps (Cassaigne–Currie–Schaeffer–Shallit), while `{0,1,2,3}` is not known to.

### C. The Cassaigne word cannot be recoded onto `{0,1,2,3}` with finite letter memory

Every letter-to-letter map `{0,1,3,4} → {0,1,2,3}` (256 maps, exhaustive) produces an additive cube by length **41**. Every bijection dies by length **35**. The naive transport `0,1,3,4 ↦ 0,1,2,3` dies at length **9**. Linear pair-codings `ax+by+c (mod 4)` die by length **13**. Random 2-state transducers on 80k trials never reached length 500 (best 290).

Transporting the only well-understood infinite 4-letter construction is not how `{0,1,2,3}` will be solved, unless the recoding has growing memory.

---

## 2. Novelty status

| Claim | Status | Notes |
|---|---|---|
| Unique-vote lemma; no dead-end for `n≤10` on any 4-letter alphabet | **UNVERIFIED NOVELTY** (elementary, proved) | Not found in Liétard, Rao, CCSS, Ardal et al., Glen–Simpson–Smyth. Easy enough that it may be folklore-unwritten. Do not inflate. |
| No covering 4-vote at lengths 11–13 over `{0,1,2,3}`; exactly 12 vote tuples | **UNVERIFIED NOVELTY** (computational) | Independent C++/Python agreement on the unique-vote mechanism; enumeration uses one incremental engine, counts match a second alphabet-comparison engine. |
| Shortest right-crucial length 14, 8170 words, 4-type `d`-taxonomy | **UNVERIFIED NOVELTY** (computational) | User-known public counts stop at `c_12`. Glen–Simpson–Smyth abelian-cube crucial length is 20 over 4 letters (different problem). |
| `{0,1,3,4}` also has first dead-ends at 14 (2656 of them) | **UNVERIFIED NOVELTY** (computational) | Directly kills “exceptionality = earlier local death”. |
| `c_13 = 23818052`, `c_14 = 83821562` | **UNVERIFIED NOVELTY** (counts) | Easy to have been computed privately. Not the headline. |
| `c_8…c_12` | **KNOWN** (do not claim) | Given in the problem statement. Independently reproduced. |
| Uniform morphisms of image length `≤7` fail | **KNOWN** (Liétard) | Reproduced only the `k=2` slice previously; not re-advertised as new. |
| Bounded additive complexity ⇒ additive `k`-powers for all `k` | **KNOWN** | Ardal–Brown–Jungić–Sahasrabudhe, *Integers* 12 (2012), Thm. 5 / arXiv:1107.4654 Thm. 2.2. Specialises to C-balanced / finite-valued discrepancy walks. |
| Eventually periodic ⇒ additive cubes | **KNOWN** (elementary) | Ordinary cube `uuu`. |
| All 256 letter-to-letter recodings of Cassaigne die by length 41 | **UNVERIFIED NOVELTY** (computational certificate) | Independent Python check of the naive map (L=9) and the best bijection (L=35). |
| Infinite ACF word over `{0,1,2,3}` | **OPEN** | Not solved. |

**Headline novelty label:** UNVERIFIED NOVELTY for the unique-vote lemma and the 11–13 covering obstruction, not a major result.

---

## 3. Exact mathematical statements

### Theorem 1 (unique vote)

Let `w = a_0…a_{n-1}` be a word over `ℤ` with prefix sums `S(0)=0`, `S(k)=a_0+⋯+a_{k-1}`. For `x∈ℤ` write `w' = wx` (length `N=n+1`). For each `d≥1` with `3d≤N`, the three consecutive length-`d` blocks of `w'` ending at position `N` have sums

\[
s_1 = S(N-2d)-S(N-3d),\quad
s_2 = S(N-d)-S(N-2d),\quad
s_3 = S(N)-S(N-d) = \bigl(S(n)-S(N-d)\bigr)+x.
\]

Here `s_1` and `s_2` do not depend on `x`. Hence:

- if `s_1 ≠ s_2`, no `x` makes this `d` an additive cube;
- if `s_1 = s_2`, there is **at most one** `x`, namely `x = s_1 - (S(n)-S(N-d))`.

Call `d` *active* when `s_1=s_2`, and call that unique `x` the *vote* of `d` (which may or may not lie in the alphabet).

### Corollary 2 (length lower bound)

Over an alphabet of `q` letters, a right-crucial ACF word must have at least `q` active votes whose votes are distinct alphabet letters. Therefore

\[
\lfloor (n+1)/3 \rfloor \ge q.
\]

For `q=4`, `n≥11`. In particular there is no right-crucial ACF word of length `≤10` over `{0,1,2,3}` or over `{0,1,3,4}`.

### Theorem 3 (computational: first covering over `{0,1,2,3}`)

Every ACF word of length `n≤13` over `{0,1,2,3}` has an ACF one-letter extension. There are exactly `8170` right-crucial ACF words of length `14`.

Moreover, for `n∈{11,12,13}`, writing `x_d` for the vote of `d∈{1,2,3,4}` (`-1` if the vote is not in `{0,1,2,3}`), every ACF word with four active `d∈{1,2,3,4}` has `(x_1,x_2,x_3,x_4)` equal to one of the 12 tuples

```
(0,3,-1,-1)  (3,0,-1,-1)
(0,2,-1,-1)  (3,1,-1,-1)
(1,2,3, 3)   (2,1, 0, 0)
(1,2,3, 1)   (2,1, 0, 2)
(0,1,2, 2)   (3,2, 1, 1)
(1,2,3,-1)   (2,1, 0,-1)
```

None is a permutation of `{0,1,2,3}`. These 12 tuples are closed under the complement `(x_1,x_2,x_3,x_4) ↦ (3-x_1,3-x_2,3-x_3,3-x_4)` with `-1` fixed. Every such length-11 word ends with a length-3 suffix of the form `baa` with `a=x_1` and `b=x_2`.

### Theorem 4 (computational: Cassaigne recoding)

Let `φ` be the Cassaigne–Currie–Schaeffer–Shallit morphism `0↦03, 1↦43, 3↦1, 4↦01` and let `W=φ^ω(0)` over `{0,1,3,4}`. For every function `τ:{0,1,3,4}→{0,1,2,3}`, the recoded prefix `τ(W[1..41])` contains an additive cube. Among the 24 bijections the longest ACF prefix has length 35 (`0↦1, 1↦0, 3↦3, 4↦2` and its complement). The order-preserving bijection `0,1,3,4 ↦ 0,1,2,3` has ACF prefix length 9.

### Theorem 5 (known; recorded for the class-J attack)

(Ardal–Brown–Jungić–Sahasrabudhe 2012.) If an infinite word over a finite subset of `ℤ` has bounded additive complexity, then it contains an additive `k`-power for every `k≥1`. In particular it is not ACF. C-balanced words, and words whose prefix-sum discrepancy `S(n)-μn` takes finitely many values, have bounded additive complexity, hence are not ACF. This holds on every integer alphabet and does **not** distinguish `{0,1,2,3}`.

### Lemma 6 (sumset mechanism of exceptionality; known classification, spelled out)

Among 4-element subsets of `ℤ`, `|nA|` is minimised precisely when `A` is a 4-term arithmetic progression, in which case `|nA|=3n+1`. For `{0,1,3,4}` one has `|nA|=4n+1` for all large `n`. Liétard–Rosenfeld already proved that the exceptional 4-letter class is exactly the affine images of `{0,1,2,3}`. The sumset gap is the additive-combinatorial reason those alphabets maximise length-`d` sum collisions. This is an explanation of a known theorem, not a new classification.

---

## 4. Proofs and computational certificates

### Proof of Theorem 1

The displayed formulae for `s_1,s_2,s_3` are the definitions of the three block sums in terms of prefix sums of `wx`. Indices `N-d`, `N-2d`, `N-3d` are `≤ n`, so `s_1` and `s_2` ignore `x`. The equation `s_3=s_1` is linear of slope 1 in `x`. □

### Proof of Corollary 2

A new additive cube in `wx` must end at the new letter (the prefix was ACF). Each such cube has some `d≤⌊N/3⌋` and, by Theorem 1, that `d` excludes at most one `x`. To exclude `q` letters one needs `≥q` distinct active `d`. □

### Certificate for Theorem 3

- Dual ACF testers: `src/acf.hpp` (`find_cube` prefix-sum, `brute_is_acf` triple loop) and `python/brute_verify.py`. Cross-checked on all words of length `≤6` and on random longer words (`./bin/acf_tool test`).
- Unique-vote vs brute forbidden-set: `python/independent_checks.py` on all ACF words of length `≤6` (4325 words, 0 mismatches).
- Enumeration: `./bin/votes 14` reports `full_cover=8170` at `n=14` and `full=0` at `n≤13`. `./bin/compare_alph` independently counts 8170 dead-ends at length 14 by trying four extensions. `./bin/dump_deadends` writes `data/deadends14.txt` (8170 lines).
- Taxonomy: `./bin/taxonomy` and `python/taxonomy_check.py` agree on 52 `d`-quadruples and on the four sorted multisets `{1,2,4,5}` (4228), `{1,2,3,5}` (2502), `{1,3,4,5}` (970), `{2,3,4,5}` (470). All four witness `d`'s are distinct, as Theorem 1 requires.
- Vote tuples: `./bin/vote_tuples`, `./bin/dump_active11`.
- Counts: `c_n` for `n≤12` match the user-supplied public values. `c_13=23818052`, `c_14=83821562` from the same DFS (two binaries: `votes`, `compare_alph`).

### Certificate for Theorem 4

- Generator of `W`: same morphism as CCSS, `src/code_cassaigne.cpp`. Native prefix of length 20000 is ACF (`is_acf`).
- Exhaustive 256 maps, cap 8000; all die in `[0,49]`; best 41.
- 24 bijections printed by the same binary; best 35.
- Independent Python: `python/independent_checks.py` regenerates `W`, checks native ACF, naive L=9, best-bijection L=35.

### Comparison certificate

`./bin/compare_alph` (does not use `Builder::try_push`, whose `symbol_counts[a]` overflows on letter 4):

```
alphabet     c_14        dead_14   first_dead
{0,1,2,3}    83821562    8170      14
{0,1,3,4}    96684282    2656      14
```

Through length 5 the two alphabets have identical counts (only `d=1` is possible). They split at `n=6`: 3152 vs 3180.

---

## 5. Independent verification

| Fact | Engine A | Engine B |
|---|---|---|
| ACF predicate | C++ prefix-sum `find_cube` | C++ `brute_is_acf` and Python triple loop |
| Unique-vote forbidden set | C++ `votes` / taxonomy | Python `forbidden_letters` vs `brute_forbidden`, `n≤6` exhaustive |
| 8170 dead-ends of length 14 | `dump_deadends` | `votes` (`full_cover`) and `compare_alph` |
| Taxonomy `d`-quads | C++ `taxonomy` | Python `taxonomy_check.py` |
| Cassaigne recode L=9 and L=35 | C++ `code_cassaigne` | Python `independent_checks.py` |
| `c_n` for `n≤6` | C++ DFS | Python exhaustive |
| `c_8…c_12` | C++ DFS | matches user-known public table |

Reproduce:

```
make test
python3 python/independent_checks.py
python3 python/taxonomy_check.py data/deadends14.txt
./bin/votes 14
./bin/taxonomy data/deadends14.txt
./bin/code_cassaigne
./bin/compare_alph
```

---

## 6. Literature audit

Searched: web/Google Scholar snippets, arXiv (CCSS 1106.5204; Rao; Liétard–Rosenfeld; Andrade–Mol 2408.15390; Ardal et al. 1107.4654; Banero 1209.4691; Popoli–Shallit–Stipulanti 2410.02409), OEIS A051043 (ordinary cubefree, not additive), Integers 12 (2012) #A21, Liétard thesis HAL tel-03203854, DLT 2020, Green 2024 Problem 15, GitHub `lgmol/Additive-Powers-Decision-Algorithm` and `word-structures/combinatorics-on-words-research`. Context.dev MCP was unavailable (auth). MathSciNet/zbMATH/TheoremDB were not directly accessible; the web/arXiv/HAL sweep is the audit.

**Does not conflate:** Green 15 / Fox / Pirillo–Varricchio / Halbeisen–Hungerbühler are additive *squares* (3-APs in the Lipschitz graph, or equal-sum adjacent blocks of *unequal* length in Halbeisen–Hungerbühler). Additive cubes are 4-APs in the partial-sum graph at equal time spacing.

**Already known, used as context:** CCSS infinite ACF word over `{0,1,3,4}`; Liétard–Rosenfeld all 4-letter alphabets except AP class; Rao 3-letter constructions; Liétard 70,880,000 and morphism size `≥7`; Ardal et al. bounded additive complexity; Dekking abelian cubes; Glen–Simpson–Smyth abelian crucial words (length 20 on 4 letters).

**Not found:** unique-vote lemma; crucial length 14; the 12 vote tuples; Cassaigne letter-to-letter recoding table; dead-end comparison `{0,1,2,3}` vs `{0,1,3,4}`. Absence of evidence is not a proof of novelty — hence UNVERIFIED NOVELTY.

---

## 7. Failed hypotheses and counterexamples

| Hypothesis | Fate |
|---|---|
| Exceptionality of `{0,1,2,3}` is that it dies locally earlier than `{0,1,3,4}` | **False.** Both have first dead-ends at length 14. `{0,1,3,4}` has fewer traps (2656 vs 8170) and more 14-mers, but the onset length is identical. |
| Bounded discrepancy / C-balanced walks could be a new obstruction | **Already Ardal et al. 2012.** Cassaigne prefix of length 50k already has `max\|S-μk\|≈1870` (unbounded, as required). Up-and-Down 400k word: discrepancy ≈2022. |
| Letter-to-letter (or 2-state) recoding of Cassaigne yields a long ACF word over `{0,1,2,3}` | **False.** Max 41 (1-state), max 290 in 80k random 2-state trials, none ≥500. |
| Pair-linear coding of adjacent Cassaigne letters | **False.** Best `x+2y (mod 4)`, length 13. |
| Cassaigne-shaped morphisms of size 7,8,9 over `{0,1,2,3}` (15k+12k+8k random prolongable samples) | No prefix ≥200. Consistent with Liétard’s size `≥7` being necessary but far from sufficient. |
| ACF `n`-mer overlap graph has no cycle, blocking infinite words even for short `d` | **False.** For `n=8` and `n=10` the graph is a **single** strongly connected component (42070 and 538214 nodes). Infinite walks avoiding `d≤3` exist in abundance. Periodic walks of those cycles still have large-`d` cubes (Theorem A / ordinary cubes). |
| Finite sofic generator of an infinite ACF word | Impossible on **any** alphabet: a nonempty sofic shift has a periodic point, and periodic points contain ordinary cubes. (Elementary; not specific to `{0,1,2,3}`.) |
| `{1,2,3,4}` as witness-`d` set at the first dead-ends | **Impossible** by suffix reduction: those cubes live in a length-11 suffix, which would itself be crucial. The 8170 length-14 dead-ends all include a `d=5` witness and omit exactly one of `{1,2,3,4}`. |
| Enumeration `c_8…c_12` as new | **Retracted.** User-known. |
| Uniform size `≤7` failure as new | **Retracted.** Liétard. |

Hand trap (previous session): `01230123` is *not* ACF (`12|30|12` sums 3). Verifiers were right.

---

## 8. Most promising next direction

Prove **by hand** that the 12 vote tuples at length 11 are the only solutions of the simultaneous system “`d=1,2,3,4` all active” over `{0,1,2,3}`. The system lives on 11 letters (really on the last 12 positions of `wx`) and the suffixes are already rigid (`baa` with `(a,b)` in a 6-element set). A human proof that none of those 12 can be completed to a bijection of votes would replace the 11–13 computer search with a genuine local algebraic obstruction.

That still would not decide the infinite problem: `{0,1,3,4}` survives the same `n=14` onset.

---

## 9. Single most promising unresolved bottleneck

**Long-`d` cubes along morphic / S-adic orbits, not local dead-ends.**

Unique-vote makes the *next* letter a finite check. It does not give a finite sound state for *future* `d ~ n/3`. The length-8 and length-10 ACF de Bruijn graphs are strongly connected, so short cubes do not trap the walk. Exceptionality is invisible at the 14-letter scale: the solved alphabet has the same first-trap length.

The remaining gap is exactly the one Cassaigne et al. close on `{0,1,3,4}` with a kernel/lattice argument that uses extra sumset room (`|nA|=4n+1` vs `3n+1`). Either that argument can be made to work for some (necessarily size `≥7`, non-recoded-Cassaigne) morphism on `{0,1,2,3}`, or an impossibility for uniformly morphic / linearly recurrent words can be extracted from the missing sumset room. Random Cassaigne-shaped morphisms of size 7–9 dying before length 200 suggest the former will not be found by shape-copying.

Do not spend the next unit of work growing finite words. Spend it on a morphism with at least three well-spread image sums and a kernel invariant, or on a proof that every uniform morphism over an AP alphabet produces an aligned or near-aligned additive cube.

---

## Taxonomy snapshot (length-14 dead-ends over `{0,1,2,3}`)

- 8170 words, 4083 reverse+complement orbits, 0 palindromes, 0 self-complementary.
- 7700 end with a double letter (one `d=1` ordinary-cube vote); 470 do not (`d`-multiset `{2,3,4,5}`).
- Four `d`-multisets only, always containing 5, omitting exactly one of `{1,2,3,4}`: `{1,2,4,5}` (4228), `{1,2,3,5}` (2502), `{1,3,4,5}` (970), `{2,3,4,5}` (470).
- Perfect complement symmetry in the per-letter `d`-histograms (`0↔3`, `1↔2`).

## Extension-graph snapshot

`n=8`: 42070 nodes, 150560 edges, 1 SCC, outdegree 0:0.  
`n=10`: 538214 nodes, 1 SCC, outdegree 0:0.  
A cycle here is an infinite word without additive cubes of block length `d≤3`; it does not give a full ACF word.

## What this does *not* do

- Does not produce an infinite ACF word over `{0,1,2,3}`.
- Does not prove none exists.
- Does not beat Liétard’s 70,880,000.
- Does not decide Green’s Problem 15.
- Does not claim `c_8…c_12` or uniform-size `≤7` failure as new.
