# Counterexamples

Each item is a hypothesis that was formulated strongly and then killed.

---

## H1. `01230123` is additive-cube-free
**Status:** KILLED.

Blocks `12 | 30 | 12` have length 2 and sums 3,3,3.  
Used as a verifier regression test.

---

## H2. Fixed letter order produces arbitrarily long ACF words with modest search
**Status:** KILLED at length 24,396.

Priority `(0,1,2,3)`, budget 2e6: the search reaches 24,396 and then spends essentially the whole remaining budget failing to extend (Liétard’s stall, independently reproduced). Mean has drifted to 0.96.

---

## H3. A random uniform morphism of size 7–12 over {0,1,2,3} yields a long ACF prefix
**Status:** KILLED.

30,000 random uniform morphisms, cap 2000: best survival **84**.  
50,000 more (sizes 7–10): best **75**.  
Random search does not produce morphic candidates on this alphabet. This matches Liétard–Rosenfeld’s qualitative report, now with explicit numbers.

---

## H4. Transporting φ₀ to {0,1,2,3} by 4 ↦ 2 gives a Cupisca morphism
**Status:** KILLED (size 2).

Exhaustive scan of all 65,536 two-uniform morphisms, all four seeds, cap 250: **0 survivors**. Best prefix length **119**:
```
0 → 32,  1 → 31,  2 → 20,  3 → 01   seed 2
```
Cassaigne shape `2,2,1,2` (16,384 morphisms, cap 400): 0 survivors, best 112.
Shape `2,2,2,1`: 0 survivors, best 81.

---

## H5. The size-3 morphism `0→001, 1→031, 2→1, 3→233` is Cupisca
**Status:** KILLED at length 378.

This morphism survived a cap-300 shape search (shape 3,3,1,3) and was the unique survivor there. Pushing the cap:

- length 300: ACF
- first cube at end-position 378: `i=201`, `d=59`, `sum=66`

All one-symbol mutants and 2000 two-symbol mutants of this morphism failed to beat 378.

The cube is **not** aligned with the substitution (d=59 is not a multiple of 3). Aligned-sum filters would have missed it.

---

## H6. Long Up-and-Down words are secretly morphic / automatic / low-complexity
**Status:** KILLED.

The 400,000-letter period-2000 word has p(6)=3152 (every ACF 6-mer) and p(10)=143,258. Purely morphic words satisfy p(n)=O(n). These objects are high-complexity.

---

## H7. Every finite ACF word has mean close to 3/2
**Status:** KILLED.

The 24,396-letter fixed-order word is ACF with mean 0.959.  
**Repaired form (still open):** every *infinite* ACF word, if one exists, has Cesàro mean 3/2.

---

## H8. Eventually periodic ACF words exist
**Status:** KILLED by a one-line proof (ordinary cube uuu). Not new.

---

## H9. Alternating priority with a very short period is optimal
**Status:** KILLED as an optimality claim.

Period 100, budget 2e6: length 20,306 (worse than fixed order’s 24,396).  
Period 1000: 65,986.  
Period 2000 and 5000: both reached the 400,000 cap.  
Short periods thrash and lose.

---

## H10. There are no right-crucial ACF words of small length
**Status:** KILLED at length 14.

Every ACF word of length ≤ 13 extends; 8170 words of length 14 do not.  
See `data/deadends14.txt` and `RESULTS.md`.

---

## H11. Every large additive cube in a uniformly morphic word desubstitutes to an ancestor letter-sum cube
**Status:** KILLED.

Dream theorem of the unaligned-desubstitution attack. False for two independent reasons:

1. Theorem J: the leftmost cube of a prolongable uniform fixed point has an ACF ancestor.
2. Witness `0→32,1→31,2→20,3→01`, seed 2, cube `i=45,d=25`: ancestor Parikh difference `v=(1,-1,1,-1)` not in `L(A)`, cancelled by defect `(1,3)`. Dual C++/Python.

The claim `v` in `L(A)` already fails at exhaustive 2-uniform first cubes with `d=3`, `v=(-1,0,0,1)`.

Correct replacement: `T·v + Δδ = 0` with `δ` from a finite fragment set. See `DESUBSTITUTION_REPORT.md`.

---

## H12. After quotienting by `ker(T)`, cube defects become a finite-state dynamical obstruction
**Status:** KILLED.

`Q = Z^4 / ker(T) ≅ gcd(T) Z` is infinite (Theorem M). Cubes occupy a finite subset of `Q` (Theorem L restated). The incidence matrix does not act on `Q` unless `T_2 ∈ span{T}`. On the witness, `T_2 · e_i = 2 ≠ 0` for a basis vector of `L`, so `[Mv]` is not a function of `[v]`. Reachable `T·v` of consecutive ancestor factors already fills `{−8,…,8}` (75520 pairs, minimum occupancy 1895). See `QUOTIENT_REPORT.md`.

---

## H13. Among 2-uniform 4-sum morphisms, `L` invariant implies `T` affine to the letters
**Status:** KILLED.

Conflated `dim K_A = 2` (which *does* equal `T` affine: 854 morphisms, of which 402 have four image sums) with `L`-invariance. Dual C++/Python census: **734** four-sum `L`-invariant morphisms, of which **332** are not affine. Smallest code 383:
`0→33`, `1→31`, `2→10`, `3→00`, `T=(6,4,1,0)`, `T_2=(0,4,10,12)=12·1−2T`.
There are also **2626** three-sum `L`-invariant morphisms (automatically not affine). The interesting closed `(1,T)` class exists already at `m=2`.

---

## H14. Every primitive uniformly recurrent word has an additive cube (Claim A)
**Status:** KILLED (KNOWN). Cassaigne \(\varphi_0^\omega(0)\) over `{0,1,3,4}`.

---

## H15. The corrected-weight graph is finite-state for every uniform morphism (Claim C)
**Status:** KILLED. Famous witness: window-sum range grows with `d` (`d=1` four values; `d=100` interval `[129,171]`, 42 values). The unbounded coordinate is \(\omega\) itself.

---

## H16. The middle factor Y determines both defect equations (Claim E)
**Status:** KILLED. Famous cube: X-left ancestor letter is `U[22]=3`, not in Y's ancestor span `[35,47]`. Outer fragment letter is extra.

---

## H17. Triple coupling eliminates pair-window saturation as an obstruction (Claim D)
**Status:** KILLED as an obstruction; TRUE as a filter.

Famous `n=512`: 1995 squares, 59 cubes, 1524 XY-pairs that do not extend. Cassaigne `n=800`: 2724 squares, 0 cubes. Coupling is the original cube condition. Dual C++/Python.

---

## H19. Expanding Krylov coordinates force T-weight out of the defect window (Claim, Phase 4–5)
**Status:** KILLED.

Famous first cube: \(T\cdot v=2\) (inside the window) and \(T_2\cdot v=-4\neq 0\). On 442 cubes, \(T\cdot v\) stays in \([-4,3]\) while \(T_2\cdot v\) ranges over \([-10,8]\). Expansion of a higher coordinate is cancellation in \(L\), not ejection from the T-window. See Theorem Z.

---

## H20. The Cassaigne word is a coding of a uniform morphism / can be padded to a 2-uniform ACF map (Claims E, F as universals)
**Status:** KILLED for the *word* and for padding.

Theorem X: Cassaigne frequencies are irrational, so the word is not automatic. Theorem Y: it is not a uniform morphic coding. All 8 pads of \(\varphi(3)\) to length 2 have cubes by length 48. The CCSS *length-compensation* mechanism requires non-uniform image lengths (Theorem V vs \(|\varphi(w)|=2|w|-|w|_3\)). Existence of some other uniform ACF morphism remains open.

---

## H21. CSV lex-first `d` / `acf_prefix` from a length-200 iterate measures survival
**Status:** KILLED as a statistic.

`find_cube` is smallest start index, then smallest `d`. Code 7754 seed 2: lex-first cube \(d=65\) in a length-200 word, true ACF prefix **8**. Famous remains the 2-uniform record (true ACF prefix 119). Do not correlate Krylov rank against CSV `acf_prefix`.

---

## H18. Cubes are 2-paths in a single finite T-weighted factor graph independent of `d`
**Status:** KILLED. For each `d` the step-`d` graph is `d` paths on positions. After identifying `d`-factors the size is \(p(d)\). Famous aligned `d=8` graph has 0 equal-weight 2-paths while unaligned cubes exist.

---

## H22. Prefix-square zeros of the famous T-word are not 2-automatic
**Status:** KILLED as a kernel-growth claim.

The characteristic sequence of \(\{n:S(2n)=2S(n)\}\) has 2-kernel fingerprints saturating at 9. The set is closed under \(n\mapsto 4n\) (Theorem AC). It is an infinite 2-recognizable family of additive **squares**, not cubes.

## H23. Every \(d>C(\sigma)\) is an \(E_\sigma\)-cube (Candidate D)
**Status:** KILLED. Famous T-word: 140 missing cube lengths below 200; smallest missing \(d=1\).

## H24. \(D_1\equiv D_2\equiv 0\pmod M\) plus primitivity forces an integer cube
**Status:** KILLED as a finite-modulus argument. Famous 7800-pair window: 2554 zeros mod 2 versus 6 integer zeros. Modular equality is automatic and strictly larger.

## H25. Growing \(\mathrm{diam} R_\sigma(n)\) is an automaticity-specific semilinearity obstruction
**Status:** KILLED. Sublinear diam is compatible with semilinear slices. Cassaigne (not automatic, ACF) has \(\mathrm{diam} R(64)=86\), larger than famous 32.

## H26. A 2-uniform 3-letter map on \(\{0,1,4\}\) is an automatic ACF candidate
**Status:** KILLED at ACF prefix 108. Map \(0\to 01,\ 1\to 14,\ 4\to 40\), seed 0, cube \((i,d,\mathrm{sum})=(2,63,105)\). Dual C++/Python. Alphabet not affine-equivalent to \(\{0,1,2\}\). All 210 prolongable 3-sum 2-uniform maps on \(\{0,1,2\}\) cube by length 26.

---

## H27. ACF \(\Rightarrow\) bounded additive complexity (Conjecture A without automaticity)
**Status:** KILLED (KNOWN). Cassaigne is ACF with \(p_{\max}=66\) already on a length-2048 prefix, diameter 95 at \(n=64\), holes present, 0 equal-weight triples. Dual Python. Ardal’s contrappositive says this must happen for any ACF word.

---

## H28. Unbounded diameter plus uniform recurrence forces additive cubes
**Status:** KILLED. Same witness as H27. Recurrence without automaticity is not enough. Do not repeat the “syndetic returns force cubes” argument.

---

## H29. Van der Waerden on the finite colouring \(i\mapsto\omega(w[i:i+n))\) forces an additive cube
**Status:** KILLED (Theorem AF). VdW produces monochromatic 3-APs whose difference need not be \(n\). Famous \(n=8\): 372 mono 3-AP starts, 2 with \(d=n\). Cassaigne \(n=8\): 524 starts, **0** with \(d=n\).

---

## H30. ACF forces a uniform discrepancy bound, so a large modulus converts modular zeros to integer zeros
**Status:** KILLED. Ardal contrappositive: ACF \(\Rightarrow\) unbounded \(p_T\) \(\Leftrightarrow\) unbounded diameter (Theorem AE). The modular-to-integer bridge requires an a priori bound that ACF forbids.

---

## H31. An expanding non-Perron eigenvalue prevents a long ACF prefix
**Status:** KILLED as a “quick cube” claim. The m=3 maximizer has eigenvalues \(3,1\pm\sqrt{2},1\), so \(\lvert\lambda_2\rvert=1+\sqrt{2}>1\), and still reaches ACF prefix **1647** before cubing. Expansion is compatible with a long finite ACF prefix. The map is not an infinite ACF word.

---

## H32. A recurrent finite-state / macro-block core can generate only ACF infinite paths
**Status:** KILLED for the all-paths reading (Theorems AI, AK). Not a kill of aperiodic walks on the same graph.

## H32b. The ACF n-mer 2-core driven by Thue–Morse (paperfold, Sturmian) is an infinite ACF generator
**Status:** KILLED for \(n\le 8\) (Theorem AR). Every start cubes by length 37.

## H33. Every sufficiently long ACF word over {0,1,2,3} has at least two legal extensions
**Status:** KILLED at length 8 (`20010100`, \(q=1\)). Also false for Cassaigne over {0,1,3,4} (first \(q=1\) at 337), so the statement is not even a special obstruction of the exceptional alphabet.

## H34. Every ACF word two-branches within a fixed lookahead R
**Status:** KILLED by right-crucial words (length 14, and later length 38). True for all 8-mers with \(R=2\), which is not a theorem for all \(n\).

## H35. Up-and-Down is a nested recursive block substitution \(W_{n+1}=F(W_n)\)
**Status:** KILLED. Period-\(P\) and period-\(Q>P\) archives share a prefix of length \(P\), then diverge. Period-blocks have distinct Parikh vectors and are not reversals.

## H36. The period-1000 Up-and-Down archive of length 65986 is only budget-limited
**Status:** KILLED. Tail \(q=0\): the word is right-crucial. (The 400k period-2000/5000 archives are cap-limited: tail \(q=1\) and \(q=3\).)

## H37. Nested palindrome / doubling templates \(WxW^R\), \(WW\) from short seeds yield unbounded ACF words
**Status:** KILLED for all ACF seeds of length 2–4, cap 256. Best palindrome-fold prefix 9.

## H38. Positive finite-language growth implies a finite-state ACF core
**Status:** KILLED. Growth of \(c_n\) (open as a theorem, empirical through \(n=12\)) does not produce an all-paths finite graph of ACF infinite words (Theorem AI). König’s lemma needs unbounded lengths, not a sofic generator.

## H39. Words with \(\lvert\mathrm{mean}-3/2\rvert\le 1/4\) and \(q\ge 2\) form a 2-injective core
**Status:** KILLED at \(n=8\) (Theorem AV). Witness `01303120`.

## H40. Always taking the two legal letters closest to mean \(3/2\) generates unbounded ACF words
**Status:** KILLED computationally for this local rule with frontier caps 4096 and 16384. Extinct at lengths 399 and 480 respectively; at cap 16384 the 16384 dying parents of length 479 are pairwise distinct, ACF, and \(q=0\). Dual Python on the dump. Not a kill of every globally pruned 2-tree (two-smallest with cap 16384 is still alive at 500).

## H41. Unused legal letters of a long Up-and-Down word are equally good continuations
**Status:** KILLED. 133 alternative letters at 80 \(q\ge 2\) positions in the first 25000 letters of the period-2000 archive: all die under balanced greedy, mean extra length 46.6, max 156.

## H42. A 2-tree that maximises \(\lvert\mathrm{mean}-3/2\rvert\) is a new infinite ACF generator
**Status:** KILLED as a *new* generator. Styles 2–3 survive 500 with mean \(\approx 0.67\) and \(\max\lvert\Delta\rvert\approx 412\), but the prefix is the 0-heavy greedy family (LCP 14 with `word_fixed_p1000`), already known to die at 24396. The useful fact is the contrast with H40: far-from-\(3/2\) avoids the affine-\(\Delta\) wall; it does not produce a core.

## H43. Greedy \(\lvert\Delta\rvert\approx c\sqrt{n}\) (the 400k envelope) is an infinite ACF word
**Status:** KILLED. No-backtrack \(c=5\) dies at length 67, \(q=0\), still on the envelope (\(\lvert\Delta\rvert/\sqrt{n}=4.95\)). Dual Python. Bang-bang variant dies at 107.

## H44. Envelope letter-order backtracking is as good as Up-and-Down
**Status:** KILLED. \(10^7\) nodes \(\to\) 7066 (dead). Fixed-order with \(2\cdot 10^6\) already reached 24396. Up-and-Down \(p=2000\) reached 400k.
