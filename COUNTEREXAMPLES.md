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

## H18. Cubes are 2-paths in a single finite T-weighted factor graph independent of `d`
**Status:** KILLED. For each `d` the step-`d` graph is `d` paths on positions. After identifying `d`-factors the size is \(p(d)\). Famous aligned `d=8` graph has 0 equal-weight 2-paths while unaligned cubes exist.
