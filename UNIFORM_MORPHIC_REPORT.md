# Uniformly morphic ACF words over {0,1,2,3}

**Date:** 2026-09-04  
**Central question:** Can `W = σ^ω(a)` be additive-cube-free for a prolongable *m*-uniform morphism `σ` over `{0,1,2,3}`?  
**Original infinite problem:** still **OPEN**.  
**Rank:** **LEVEL 4** as an explanation of why the known Cassaigne construction distinguishes the two alphabets; **LEVEL 3** as impossibility for restricted uniform classes. No uniformly morphic infinite ACF word was found.

---

## 1. THE RESULT

Three facts, in order of structural weight.

**R1 (Cassaigne kernel, alphabet-dependent).**  
For the Cassaigne-shaped morphism the incidence matrix `M` and its contracting left eigenvector `τ` are independent of the numerical alphabet. Additive cubes in the fixed point would produce a nonzero vector `x` in the lattice

```
L(A) = { v ∈ Z^4 : 1·v = 0 and A·v = 0 }
```

with `|τ·x| < C ≈ 2.1758` (Liétard–Rosenfeld Lemma 3.3, from CCSS Theorem 8).  

On `B = {0,1,3,4}` the only sub-threshold lattice vectors are `±(1,-2,2,-1)`, which CCSS handle by an ancestor graph.  
On `A = {0,1,2,3}` there are **three** such pairs, including

```
±(1,-2,1,0)     |τ·x| ≈ 1.22
```

which encodes the AP relation `0+2 = 1+1`. This vector is **not** in `L(B)`. That is the precise point at which the sufficient `|τ·x| ≥ C` test fails for the AP alphabet, and it is exactly the extra sumset collision `|nA| = 3n+1`.

This is the invariant asked for in mission type (3). It is a clarification of Liétard–Rosenfeld (they exclude `c=2` and `d=2c-1`), not a new classification of 4-letter alphabets.

**R2 (uniform classes that are impossible on every integer alphabet).**  
- At most two distinct image sums: aligned additive cubes, by Dekking (already Theorem C).  
- Primitive uniform with all non-Perron eigenvalues `|λ| < 1`: bounded discrepancy, hence additive `k`-powers for all `k` by Ardal–Brown–Jungić–Sahasrabudhe 2012.  

Neither class distinguishes `A` from `B`. The known Cassaigne morphism evades both: it is **not uniform**, and it has **two** expanding eigenvalues `≈ 1.69` and `≈ -1.51`.

**R3 (2-uniform forensics; failure already known).**  
Among prolongable 2-uniform morphisms the longest ACF prefix found is 119, from

```
0 → 32,  1 → 31,  2 → 20,  3 → 01,  seed 2.
```

Image sums `T = (5,4,2,1)` are four distinct values, affine-equivalent as a set to `{0,1,3,4}`. Incidence eigenvalues `{2, √2, -√2, 0}`: not Pisot (`|λ2|=√2>1`). The first cube is

```
i=45, d=25, sum=35
```

odd offset, `d` not a power of 2: **unaligned**, sitting between substitution levels `2^4=16` and `2^5=32`. Independently verified by a Python triple-loop. So the 2-uniform survivors of the Dekking filter still die of large-`d` cubes that do **not** desubstitute to aligned image-sum cubes.

---

## 2. EXACT THEOREMS

### Theorem U1 (two image sums; known)

Let `σ` be `m`-uniform over a finite subset of `ℤ` with at most two distinct image sums. Any infinite concatenate of images `σ(a_i)` has an additive cube of block length `m`.

Proof: the block-sum sequence is a word over a 2-element subset of `ℤ`. Additive cubes coincide with abelian cubes on two letters; Dekking (1979) says abelian cubes are unavoidable over two letters. □

### Theorem U2 (Pisot-uniform; packaged from known ingredients)

Let `σ` be a primitive `m`-uniform morphism over a finite subset of `ℤ`, with incidence matrix `M`. Suppose every eigenvalue of `M` other than the Perron value `m` satisfies `|λ| < 1`. Then any infinite fixed point of `σ` has bounded prefix-sum discrepancy, hence bounded additive complexity, hence contains additive `k`-powers for every `k ≥ 1`. In particular it is not ACF.

Proof: Column-sums of `M` are all `m`, so `1^T M = m 1^T`. Primitivity gives a unique Perron eigenvalue `m` with positive eigenvectors. Parikh vectors of prefixes therefore satisfy `ψ(W[0,n)) = n f + e(n)` with `f` the normalised right Perron vector and `e(n)` the component in the complementary invariant space. At substitution depth `k`, that component is `O(|λ2|^k)`; since `n ≍ m^k` one has `|e(n)| = O(n^{log_m |λ2|})` with negative exponent, hence `e(n)` is bounded (prefixes off substitution boundaries add an `O(1)` piece of an image). Then `S(n) = A·ψ = n (A·f) + O(1)`. Factor sums of length `ℓ` therefore lie in an interval of `n`-independent width `O(1)` about the mean, so additive complexity is bounded. Ardal–Brown–Jungić–Sahasrabudhe, *Integers* 12 (2012), Theorem 5, give additive `k`-powers for all `k`. □

Hypotheses that were checked: primitivity, uniformity (so Perron equals `m`), `|λ2|<1`. The conclusion holds for `{0,1,2,3}` and `{0,1,3,4}` alike.

### Theorem U3 (injective image sums vs aligned cubes)

Let `σ` be `m`-uniform and `T(a) = sum(σ(a))`. Write `W` as a concatenation of length-`m` images. Aligned additive cubes of length `m` in `W` are exactly additive cubes of block length 1 in the coded word `T(W)`. If `T` is injective, these are ordinary cubes `xxx` in `W`.

Proof: three consecutive images have equal sums iff the three ancestor letters have equal `T`-values. Injectivity turns that into equal letters. □

Corollary: a cube-free (ordinary) infinite uniform-morphic word with injective `T` has **no aligned** additive cube of length `m`. It may still have unaligned cubes.

### Theorem U4 (Cassaigne lattice distinction; known mechanism, explicit vectors)

Let `M` and `τ` be those of `φ_{a,b,c,d}` (Cassaigne shape). Let `C ≈ 2.1758` be the CCSS constant. For the ordered alphabets `(0,1,3,4)` and `(0,1,2,3)`:

| vector | in `L(B)` | in `L(A)` | `|τ·x|` |
|---|---|---|---|
| `(1,-2,2,-1)` | yes | no | 1.54 `< C` |
| `(1,-2,1,0)` | no | yes | 1.22 `< C` |
| `(0,-1,2,-1)` | no | yes | 1.73 `< C` |
| `(1,-3,3,-1)` | no | yes | 1.99 `< C` |
| `(1,-1,-1,1)` | yes | yes | 2.24 `> C` |

The AP-only vector `(1,-2,1,0)` is the Parikh difference of `02` versus `11`: length 2, sum 2, the relation `0+2=1+1`. It is the smallest `|τ|` obstruction and does not exist in `L(B)`.

Consequently the sufficient test “every nonzero `x ∈ L` has `|τ·x| ≥ C`” holds for many non-AP 4-letter alphabets (Liétard–Rosenfeld Theorem 4.1) and fails for `{0,1,2,3}`.

---

## 3. PROOF notes

U1–U3 are given above. U4 is a finite computation of `|τ·x|` on `{v ∈ Z^4 : ‖v‖_∞ ≤ 8} ∩ L`, with `τ` the published CCSS contracting eigenvector. The two alphabets are compared with the **same** `τ` because `M` does not depend on letter values.

Why `{0,1,2,3}` produces extra lattice vectors: `L(A)` is the integer kernel of

```
( 1  1  1  1 )
( 0  1  2  3 )
```

The second row is an arithmetic progression, so `0+2=1+1` gives a length-preserving sum-preserving exchange that `{0,1,3,4}` does not admit (`0+3 ≠ 1+1`). This is the same phenomenon as `|nA|=3n+1 < |nB|=4n+1`.

CCSS is not uniform, so Theorem U2 does not apply to it: `M` has two expanding eigenvalues, discrepancy is unbounded, and `L(B)` cuts the expanding plane so that equal-length equal-sum differences remain bounded.

---

## 4. COMPUTATIONAL CERTIFICATE

| Claim | Space / algorithm | Stop | Independent check |
|---|---|---|---|
| U4 short vectors | all `v∈Z^4` with `‖v‖_∞≤8` in `L(A)` and `L(B)` | bound 8; next `|τ|` already `>C` | `python3 python/lattice_tau.py`; matches CCSS Table 1 on `B` |
| 2-uniform alignment scan | all `4^8=65536` morphisms, prolongable seeds only, cap 200 | first cube or cap | `bin/morph_forensics scan2 200`: 20970 aligned / 44566 unaligned first cubes; best prefix 119 |
| Best 2-uniform cube | iterate `0→32,1→31,2→20,3→01` from 2 | first cube | C++ `find_cube` and Python prefix-sum: `i=45,d=25,sum=35`; ACF prefix 119 |
| Dual ACF testers | as in previous archive | — | `make test` |

Reproduce:

```
python3 python/lattice_tau.py
g++ -std=c++17 -O3 -o bin/morph_forensics src/morph_forensics.cpp
./bin/morph_forensics scan2 200
./bin/morph_forensics one 2 32 31 20 01 2 250
```

No SAT solver was used. Uniform size `≤7` failure is **not** claimed as new (Liétard).

---

## 5. COMPARISON `{0,1,2,3}` vs `{0,1,3,4}`

```
property                         A={0,1,2,3}              B={0,1,3,4}
----------------------------------------------------------------------
sumset |nX|                      3n+1                     4n+1
Cassaigne-shape L sub-C vectors  3 pairs                  1 pair ±(1,-2,2,-1)
AP exchange (1,-2,1,0)           in L, |τ|≈1.22           not in L
(1,-1,-1,1)                      in L, |τ|≈2.24>C         in L, |τ|≈2.24>C
known infinite ACF morphic word  unknown                  yes, non-uniform
2 image sums (uniform)           impossible (U1)          impossible (U1)
Pisot-uniform                    impossible (U2)          impossible (U2)
letter-to-letter recode of CCSS  dies by length 41        identity is ACF
uniform m=2 longest prefix here  119, unaligned d=25      not the construction used
```

The mechanism that **distinguishes** the alphabets is **R1**: extra vectors in `L(A)` coming from 4-term AP sum collisions. It is a property of the alphabet’s lattice, visible already for the Cassaigne shape, and it is **absent** in `B`.

Theorems U1–U2 do **not** distinguish the alphabets. Any argument that only uses Dekking or Ardal cannot explain exceptionality.

---

## 6. COUNTEREXAMPLES / failed hypotheses

| Hypothesis | Result |
|---|---|
| Uniform morphisms fail because they always have ≤2 image sums | False. Best 2-uniform has four distinct sums `(5,4,2,1)`. |
| Uniform morphisms fail because they are Pisot / bounded-discrepancy | False for the best 2-uniform: `λ = 2, ±√2, 0`, discrepancy ~ `n^{1/2}`. |
| First cubes of 2-uniform maps are aligned (Dekking-type) | False for a majority of prolongable seeds (44566 unaligned vs 20970 aligned). The record example is unaligned, `d=25`. |
| Early dead-ends (length 14) explain why morphisms fail | False (previous report): `{0,1,3,4}` has the same first crucial length. |
| Copying Cassaigne’s image-length pattern at size 7–9 yields long words | Already failed; those maps are not uniform. |
| `T` injective ⇒ no additive cubes at all | False (U3 only kills *aligned* length-`m` cubes). |
| Lemma 3.3 (`|τ·x|≥C` for all `x∈L\{0}`) holds on `B` | False: `B` has `±(1,-2,2,-1)` below `C`; CCSS needs the ancestor graph. The AP is worse (three pairs below `C`), not uniquely “the case where some vector is short”. |

---

## 7. NOVELTY AUDIT

| Statement | Class |
|---|---|
| U1 two image sums | **KNOWN** (Dekking + aligned blocks; recorded as Theorem C) |
| Uniform size `≤7` die | **KNOWN** (Liétard); 2-uniform scan here is a reproduction with alignment labels |
| U2 Pisot-uniform ⇒ cubes | **UNVERIFIED NOVELTY** as a packaged theorem; ingredients (Perron of uniform `M` is `m`; `|λ2|<1` ⇒ bounded discrepancy; Ardal 2012) are all known. Do not inflate. Holds on both alphabets. |
| U3 injective `T` vs aligned cubes | **NEW LEMMA** (elementary). |
| U4 lattice vectors, `(1,-2,1,0)` | **REPRODUCED / clarified**. Liétard–Rosenfeld Theorem 4.1 already excludes `c=2`. The explicit AP vector and the 3-vs-1 sub-`C` count are the readable form of that exclusion. Matches CCSS Table 1 on `B`. |
| Best 2-uniform cube `i=45,d=25` | **NEW OBSERVATION** (computational forensics of a known-to-fail class). |
| Infinite uniformly morphic ACF word over `{0,1,2,3}` | **OPEN** |
| Infinite ACF word (any construction) over `{0,1,2,3}` | **OPEN** |

Literature: CCSS arXiv:1106.5204 (lattice `L`, `τ`, Table 1); Liétard–Rosenfeld DLT 2020 / Lemma 3.3 and Thm 4.1; Ardal et al. Integers 12 (2012) #A21; Dekking 1979; Liétard thesis (size `≥7`); Rao 2015 (3-letter substitutions, not a uniform solution on `{0,1,2,3}`).

---

## 8. NEXT ATTACK (exactly one)

**Desubstitute unaligned cubes.**

The 2-uniform maps that escape U1 and U2 still die of cubes with `d` not a multiple of `m^k` and start not on a substitution boundary (record: `d=25` between `16` and `32`). Aligned cubes desubstitute to additive cubes of the image-sum word `T(W)`; when `T(W)` lives on an alphabet affine to `{0,1,3,4}` those can be avoided. What is missing is a lemma of the form:

```
an additive cube of length d in σ(W), with d not a multiple of m,
produces one of finitely many additive relations among proper prefixes
of images (a finite template / ancestor pattern).
```

That is the CCSS parent-graph, rewritten for uniform `M` and lattice `L(A)`. If the template graph over `L(A)` has a path that CCSS does not have over `L(B)` — namely a walk using `(1,-2,1,0)` — that would be an impossibility theorem for a genuine uniform class, and it would use the same invariant that already distinguishes the alphabets.

Do not grow longer random uniform images until that desubstitution lemma exists.

---

## Nested classes (Phase 4 scorecard)

| Class | Verdict over `A` | Distinguishes `B`? |
|---|---|---|
| A/B primitive prolongable uniform | open | no theorem yet |
| C bounded discrepancy | impossible (Ardal) | no |
| D unique dominant + `|λ2|<1` | impossible (U2) | no |
| E rational frequencies | automatic for primitive morphic; not an obstruction | no |
| F ≤2 image sums | impossible (U1) | no |
| G affine image sums, injective `T` | no aligned `m`-cubes unless ordinary cubes (U3); unaligned open | no |
| H rank(`M`)=1 | image sums collapse; U1 | no |
| I synchronised (uniform) | the setting; not an obstruction | — |
| J linearly recurrent | typical primitive morphic; open | no |
