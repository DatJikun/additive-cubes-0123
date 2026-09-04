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
