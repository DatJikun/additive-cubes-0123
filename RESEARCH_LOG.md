# Research Log

## 2026-09-05 — affine discrepancy cubes / high-disc 2-tree

Mission: explain the mean-beam extinction; try the opposite 2-tree. Do **not** repeat TM 2-cores, equal TM blocks, S-adic samples, sofic all-paths.

Proved: affine \(\Delta\) at \(\{d,2d,3d\}\) is an additive cube (Thm AW, elementary). Mean-beam dead-end + `3` is \(\Delta=(1,2,3)\) at scale 160. Computationally: 400k archive \(\max\lvert\Delta\rvert=3199.5\), 0 affine hits; mean-beam \(\max\lvert\Delta\rvert=7\). High-disc beams survive 500 in the 0-heavy greedy basin (H42), not a new core. Dual Python. Infinite problem still open.

## 2026-09-05 — two-operator injection / globally pruned 2-tree

Mission: a composing pair \(E_0,E_1\) or a closed 2-injection, else a hard obstruction. Do **not** rediscover unique-vote, B(m), TM n-mer 2-cores, equal-length TM blocks, S-adic samples.

Computationally verified: Cesàro band \(\lvert\mathrm{mean}-3/2\rvert\le 1/4\) is not a 2-injection at \(n=8\) (Thm AV, witness `01303120`). Two mean-balancing letters extinct at 480 with cap 16384, 16384 distinct crucial words of length 479 (H40). Two-smallest with cap 4096 extincts at 416 (cap artifact); with cap 16384 alive at 500 after a bottleneck at 480. Archive unused legal letters: 133/133 die, mean extra 47 (H41). Iterated r=2 injection leaky at length 84. Dual C++/Python. No closed injection. Infinite problem still open. Report: `CORE_REPORT.md` §19.

## 2026-09-04 — self-sustaining core / finite-language geometry

Mission: extract a finite-memory / macro-block / two-branch core of the ACF language, or a rigorous obstruction. Do **not** repeat unique-vote, B(m), Cassaigne lattice, Krylov, triples, automaticity, bounded \(p_T\).

Proved: no recurrent all-paths letter- or block-labelled core (Thm AI, AK) — this is Theorem A, not a new obstruction for aperiodic generators. Computationally verified: the ACF n-mer 2-core driven by Thue–Morse cubes by length 37 for every start at \(n\le 8\) (Thm AR); \(q=1\) from length 8 (Thm AL); length-14 dead-ends are isolated leaves (Thm AM); no uniform two-extension operators (Thm AN); nested palindrome/doubling templates die (Thm AO); Up-and-Down LCP equals one period (Thm AP); the p=1000 archive is a genuine dead-end (Thm AQ). Dual C++/Python. Report: `CORE_REPORT.md`. Infinite problem still open. The aperiodic k-mer core was the real target; it died. Other finite states are not ruled out. Follow-up: equal-length TM blocks always cube (Thm AS); drivers with 000 cube (Thm AT); S-adic sample max ACF prefix 243 (Thm AU).

## 2026-09-04 — bounded \(p_T\) / exhaustive \(B(3)\)

Mission: does automatic ACF force bounded additive complexity (Conjecture A)? Kill it or prove it. Exhaustive m=3. Compactness at fixed m. Do **not** chase generic \((D_1,D_2)\) zeros.

Proved: compactness at fixed m is a finite maximum (Thm AD); unbounded \(p_T\) iff unbounded diameter (Thm AE); VdW on weight colourings does not force \(d=n\) (Thm AF); Conjecture A + Ardal is the open automatic problem (Cor AH). Computationally verified: \(B(3)=1647\) (Thm AG), maximizer \(0\mapsto 100,1\mapsto 102,2\mapsto 132,3\mapsto 332\), cube \((982,222,229)\), expanding eigenvalue \(1+\sqrt{2}\). Dual C++/Python. Report: `PT_REPORT.md`. Infinite problem still open. ENDGAME C (fixed base m=3 only).

## 2026-09-04 — m-kernel / k-regular block sums

Mission: structure of \(R_\sigma\) via m-kernel, linear representations, zero sets; force cubes or find an automatic ACF candidate. **No factor graph.**

Proved: synchronized summatory iff bounded discrepancy (Thm AA); aligned kernel of \(S\) is the Krylov orbit (Thm AB); famous prefix T-squares are 4-adic (Thm AC) and do not yield prefix cubes. Modular zeros strictly larger than integer zeros. Candidate D killed. No automatic ACF candidate (best 3-letter uniform ACF prefix 108). Dual C++/Python. Report: `KERNEL_REPORT.md`. Infinite problem still open. ENDGAME C.

## 2026-09-04 — non-affine uniform templates

Mission: whether rank \(K_T\ge 3\) / non-affine uniform morphisms admit an additive-complexity or template obstruction forcing cubes. **No finite-state cube graph.**

Proved: length rigidity (Thm V, V′); interior \(E_\sigma\) (Thm W); Cassaigne not automatic (Thm X); same-word uniformization impossible (Thm Y); Krylov escape false (Thm Z). Dual C++/Python. Report: `TEMPLATE_REPORT.md`. Infinite problem still open. ENDGAME C.

## 2026-09-04  — session start

### Phase 0
Literature audit completed. File: `NOVELTY_AUDIT.md`.

**Verdict:** the infinite additive-cube-free problem over `{0,1,2,3}` is **open** as of 2026. Green’s Problem 15 is the *additive-square / 3-AP Lipschitz graph* problem and is strictly harder; this project attacks the exceptional 4-AP case.

Known record: Liétard 2020, length 70,880,000, Up-and-Down priority reversal. Previous: Rao ~1.4e5. Morphisms of size ≤ 6 computationally excluded by Liétard.

### Phase 1
Implemented C++ prefix-sum verifier (`src/acf.hpp`) with:
- full O(n²) scan
- incremental O(n) cubes-ending-at-n
- independent brute-force Python (`python/brute_verify.py`)

Tests:
- all 5461 words of length ≤ 6: full = brute = incremental
- 400 random words each of lengths 20,50,80,120
- hand cubes and non-cubes
- Cassaigne–Currie–Schaeffer–Shallit morphism over `{0,1,3,4}` prefix of length 20,000: ACF (sanity check that the verifier does not false-positive on a known infinite construction)

**Caught a wrong hand example:** `01230123` *does* contain the additive cube `12|30|12` (sums 3). The verifier was correct.

### Phase 2 — search-space geometry

Exact counts of ACF words over `{0,1,2,3}` (independently computed; n≤8 was tabulated by Liétard, exact digits not recovered from the HTML thesis dump):

| n | # ACF words | branching | dead-end fraction |
|---|---|---|---|
| 0 | 1 | 4.000 | 0 |
| 1 | 4 | 4.000 | 0 |
| 2 | 16 | 3.750 | 0 |
| 3 | 60 | 3.800 | 0 |
| 4 | 228 | 3.789 | 0 |
| 5 | 864 | 3.648 | 0 |
| 6 | 3152 | 3.638 | 0 |
| 7 | 11468 | 3.669 | 0 |
| 8 | 42070 | 3.579 | 0 |
| 9 | 150560 | 3.575 | 0 |
| 10 | 538214 | 3.576 | 0 |
| 11 | 1924738 | 3.519 | 0 |
| 12 | 6772220 | — | 0 |

n=1..5 coincide with OEIS A051043 (ordinary cubefree quaternary), as they must: the only possible additive cubes for n<6 have d=1, i.e. ordinary cubes `aaa`. First divergence at n=6: ACF=3152 vs ordinary-cubefree=3264.

**Dead-ends:** none of length ≤ 14. Every ACF word of length ≤ 14 has at least one ACF one-letter extension.

Letter frequencies of the uniform measure on ACF words of length 12: ≈ (0.251, 0.249, 0.249, 0.251). Extremely close to uniform, with a slight excess of 0 and 3.

Empirical growth rate λ ≈ 3.52.

### Phase 3 — reconstruct known records

Fixed-order greedy backtracking (priority 0>1>2>3), budget 2e6:

- **length 24396**, independently verified ACF.
- Matches Liétard’s stall point **exactly** (thesis Ch. 6: 24396, then two hours to add one letter).
- Mean 0.959, frequencies (0.439, 0.275, 0.172, 0.113) — the expected 0-drift.

Up-and-Down period 1000, budget 2e6: length **65986**, ACF, mean 1.494, frequencies (0.285, 0.217, 0.217, 0.281) — same qualitative symmetry as Liétard’s 70.88M word.

Larger-budget reconstruction in progress.

The 70,880,000 file itself was not retrieved from members.loria.fr.

### Phase 4 — morphisms (adversarial)

**Lemma (image sums, elementary).** A k-uniform morphism whose images take at most two distinct sums produces aligned additive cubes in any infinite iterate, because the block-sum sequence is an infinite word over a 2-letter integer alphabet, hence contains abelian cubes (Dekking: abelian cubes require 3 letters).

**Uniform k=2, exhaustive 4^8 = 65536 morphisms, cap 800, prolongable seeds:** 0 survivors. Best ACF prefix length 120 (`0→32, 1→31, 2→20, 3→01`, seed 2). Full all-seed rescan launched.

**Uniform k=1:** no infinite word (maps produce eventually periodic / non-expanding words).

**Random evo size 7–16, 30k trials, cap 2000:** best survival 84–87. Random morphisms fail fast on this alphabet, matching Liétard–Rosenfeld’s qualitative remark.

Cassaigne-shape exhaustive search (lengths 2,2,1,2 and permutations) launched.

### Phase 5 — structure of long finite words

Factor complexity of the 65,986 Up-and-Down word:

| m | p(m) | 4^m | # ACF words of length m |
|---|---|---|---|
| 1 | 4 | 4 | 4 |
| 2 | 16 | 16 | 16 |
| 3 | 60 | 64 | 60 |
| 4 | 228 | 256 | 228 |
| 5 | 864 | 1024 | 864 |
| 6 | 3134 | 4096 | 3152 |

It contains **every** ACF factor of length ≤ 5 and 3134/3152 of length 6. p(10)=37591 ≫ 10. These objects are **not** secretly morphic of linear complexity. Discrepancy |S(k)−μk| reaches 938 on length 66k (oscillatory, as designed).

### Phase 6 — periodicity and crucial words

**Theorem (elementary, not claimed as new).** Every eventually periodic infinite word contains an ordinary cube, hence an additive cube.

**NEW computational theorem.** The shortest right-crucial ACF words have length exactly 14 (8170 such words, 4083 reverse+complement orbits). Independently verified examples. No crucial word of length ≤ 13.

The size-3 morphism `0→001, 1→031, 2→1, 3→233` survives 300 letters and dies at 378 with an unaligned cube (d=59).

Up-and-Down period 2000 and 5000 both produced verified ACF words of length 400,000 (cap), not a record.


**Theorem (elementary, not claimed as new).** Every eventually periodic infinite word contains an ordinary cube, hence an additive cube. Computational check: every purely periodic word u^3 for |u|≤40 (exhaustive |u|≤6, sampled thereafter) contains an additive cube, as the proof predicts. No counterexample is possible.

### Failed hypotheses so far

- “A random uniform morphism of size 7–12 will survive 10^3 letters.” Killed; best ~80–90.
- “Transporting φ_0 to {0,1,2,3} by 4↦2 is a candidate.” Size 2, killed by exhaustive scan.
- “Fixed letter order yields long words.” Stalls at 24396, matching Liétard.
- “The 70M-type word is a hidden low-complexity substitution.” Factor complexity kills this.

### Next
- Finish large Up-and-Down reconstruction.
- Close morphism gaps (all seeds; Cassaigne shapes; size 3).
- Write proofs of elementary lemmas.
- Second novelty check on enumeration / dead-end / k=2 classification.
