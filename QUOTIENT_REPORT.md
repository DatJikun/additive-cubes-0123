# Quotient dynamics of the defect equation

**Date:** 2026-09-04  
**Question:** Can `T·v + Δδ = 0` be reduced to a finite-state obstruction after quotienting by `ker(T)`?  
**Infinite ACF problem over `{0,1,2,3}`:** still **OPEN**.  
**Rank:** **LEVEL 4** for the algebraic reduction theorems; **LEVEL 2** for the Krylov census. Not Level 5.

The letter-cube dream is not revived. The object remains `T·v + Δδ = 0`.

---

## 1. EXECUTIVE VERDICT

The proposed quotient

```
Q = Z^4 / ker(T)
```

is **not** a finite-state dynamical space, and quotienting by `ker(T)` does **not** produce a new obstruction.

Four exact facts:

1. `Q ≅ gcd(T)ℤ`, hence **infinite**. The cube equation says only that `[v] ∈ Q` lies in a **finite subset** `C_σ` (the classes with `T·v ∈ −D`, `D` the finite defect-difference set). That subset is Theorem L restated, not a dynamical reduction.

2. The incidence matrix `M` **does not preserve** `ker(T)` or `L := ker(1)∩ker(T)` except on classified subclasses. The one-step law is

```
T · (M v) = T₂ · v,     T₂ := Mᵀ T,
1 · (M v) = m (1·v).
```

If `T₂ ∉ span{T}`, the class `[v] ∈ Q` does not determine `[Mv] ∈ Q`. The smallest `M`-closed linear package containing the defect form is the Krylov subspace `K_T = span{1, T, T₂, …}`. For the witness this has dimension **3**, so one lattice direction can be dropped and a rank-1 coordinate `T₂·v` **cannot**.

3. Reachable classes already fill the defect window. Consecutive equal-length factors of the ancestor of the witness realize **every** `T·v ∈ {−8,…,8}` at least 1895 times among 75520 pairs in a length-1024 ancestor. So `R_n mod ker(T)` intersecting `C_σ` is **not** an obstruction: it happens constantly and is not sufficient for a cube.

4. Closure of `(1·v, T·v)` is the arithmetic condition `T₂ ∈ span{1,T}` (`L`-invariance), **not** non-Pisot spectrum, and **not** affinity of `T` to the letters. Among 2-uniform morphisms over `{0,1,2,3}` there are **9548** `L`-invariant morphisms, of which only 854 are affine (`T ∈ span{1,A}`). The remaining include **332** four-sum non-affine examples and **2626** three-sum examples. The previously claimed collapse “4-sum and `L`-invariant ⇒ affine” is **false** (H13).

Non-Pisot spectrum is the wrong condition for this closure. The arithmetic condition is Krylov rank of `T` under `Mᵀ`.

**ENDGAME C.**

---

## 2. EXACT QUOTIENT / DEFECT THEOREM

### Residue types: `Δn` and `Δδ`

An interval of length `d = km + s` starting at offset `r` contains

```
n = k                 if r = 0,
n = k + (r+s)//m − 1  if r > 0
```

complete substitution blocks (for `k` large enough that the interval is not confined to one block). For a cube the three blocks have residues `r`, `(r+s) mod m`, `(r+2s) mod m`, and

```
1 · v_XY = n_X − n_Y =: Δn_XY(r,s),
T · v_XY = −(δ_X − δ_Y).
```

Exact tables for `k≥1`:

```
m=2: Δn = 0 for all four types (r,s) ∈ {0,1}²
m=3: Δn ∈ {−1, 0, 1}
```

For fixed `σ`, one-fragment defects lie in the finite set of proper prefix/suffix sums of images; two-fragment defects lie in the sumset. Witness: `D₂ = {0,…,6}`, so `T·v ∈ {−6,…,6}`.

The affine solution set of type `(r,s, Δδ)` is a coset of `L = ker(1)∩ker(T)`. For the witness,

```
L = { (x, y, −4x−3y, 3x+2y) : x,y ∈ Z },
```

basis `e₁=(1,0,−4,3)`, `e₂=(0,1,−3,2)`. A complement to `ker(T)` is any vector with `T·v = gcd(T)`; equivalently `Q` is coordinatized by the single integer `T·v`.

### Theorem M — the quotient is infinite
**Status: PROVED.** Novelty: ELEMENTARY/LIKELY KNOWN.

`Z^4 / ker(T) ≅ gcd(T)ℤ ≅ ℤ`. What survives in `Q` is exactly `T·v`. It is **not** finite. It is an unbounded arithmetic coordinate. On cubes it is constrained to a finite window; on all ancestor factor-pairs it is not.

Witness: `gcd(T)=1`, first cube `T·v=2`.

### Theorem L′ — cubes occupy finitely many classes in infinite `Q`
**Status: PROVED** (repackaging of Theorem L).

`[v] ∈ C_σ ⊂ Q` with `|C_σ| < ∞`. Witness: 13 classes `T·v ∈ {−6,…,6}`.

### Theorem N — `M` preserves `L` iff `T₂ ∈ span{1,T}`
**Status: PROVED.**

See `proofs/THEOREMS.md`. Witness: rank`[1,T,T₂]=3`, `T₂·eᵢ=2≠0`. The difference `e₁−e₂=(1,−1,−1,1)` spans `ker(M)` and **is** killed; the complementary `L`-direction mixes into `T`.

### Theorem O — Krylov closure is the maximal invariant package
**Status: PROVED.**

`K_T = span{1, T, T₂, …}` is the smallest `Mᵀ`-invariant space containing the defect form. Quotientable lattice: `K_T^⊥`. Witness `dim K_T = 3`, `det[1,T,T₂,T₃]=0`.

### Theorem P — `dim K_A = 2` iff `T` affine
**Status: PROVED.** The name “affine morphism” is **KNOWN** (CMRS, Andrade–Mol).

`K_A = span{1,A,T,T₂,…}`. For four distinct letter values, `dim K_A=2` iff `T ∈ span{1,A}`. This is strictly smaller than `L`-invariance (`T₂ ∈ span{1,T}`).

---

## 3. TRANSITION LAW

Let `v` be an ancestor Parikh difference of complete blocks, and let `β` be the Parikh vector of the `O(1)` boundary letters in `U` after one more desubstitution (`‖β‖₁ < 2m`). Then

```
v = M v′ + β
T · v = T₂ · v′ + T · β
1 · v = m (1 · v′) + 1 · β.
```

**Exact linear state:**

```
(r, s, 1·v, T·v, T₂·v, …, T_{κ−1}·v, fragment letters)
```

where `κ = dim K_T`. For the witness `κ=3`, so the closed linear state is `(1·v, T·v, T₂·v)` plus residue type plus a finite fragment label.

This is **not** a function of `[v] ∈ Q` alone.

Specializations:

- `T₂ ∈ span{T}` (`ker(T)` invariant): `T·(Mv) = λ T·v`, a genuine map on `Q` (plus `β`). Among 2-uniform maps on `{0,1,2,3}`: **1182** morphisms, **373** with `n_sums≥3`.
- `T₂ ∈ span{1,T}` (`L` invariant): `(1·v, T·v)` closes. Downward, the two equations determine `(1·v′, T·v′)` from `(1·v, T·v)` and `β` when the `T`-coefficient of `T₂` is nonzero. This is a finite-state **classifier of cube types**, not an obstruction: the cube window on these two coordinates is already saturated by reachable pairs.
- `T ∈ span{1,A}` (affine): additionally `T·v` is a letter-sum, so the CMRS 2-coordinate additive templates apply. **KNOWN**.

Residue type `(r,s)` maps under `σ` (going up) to aligned type `(0,0)` with defect `0`.

---

## 4. REACHABLE QUOTIENT DYNAMICS

Let `R` be Parikh differences of consecutive equal-length factors of the ancestor `U`. Then

```
R mod ker(T)  =  { T-sum(u) − T-sum(v) : |u|=|v|, uv a factor }.
```

On the witness ancestor of length 1024, for lengths `n=1..80`, this set hits **every** integer in `{−8,…,8}` at least **1895** times (**75520** pairs). The defect window `{−6,…,6}` is therefore saturated. Dual check: `python/quotient_verify.py`.

**Cubes** of the witness (the subset of `R` that actually occurs with matching defects and a third block):

| prefix of `W` | cubes | `T₂·v` range | `#` distinct `T₂·v` |
|---|---|---|---|
| 2048 | 442 | `[−10, 8]` | 10 |
| 8192 | 2700 | `[−14, 16]` | 16 |

The extra coordinate grows (slowly) on the set of cubes. Not a proof of unboundedness, but incompatible with a small fixed finite-state set unless the growth later saturates — no mechanism for saturation is visible, and `T₂·eᵢ=2` on the mixing `L`-direction makes linear growth of that coefficient produce linear growth of `T₂·v`.

Integer solutions of the affine equation are a finite union of rank-2 lattices (the `L`-fibers). Realizable `v` occupy sparse subsets of those fibers. The fibers are **not** finite. Intersection with `C_σ` is cheap and not a cube criterion.

---

## 5. FINITE-STATE OR NON-FINITE-STATE RESULT

**Non-finite-state**, in the sense asked of `Q = Z^4/ker(T)`.

| Candidate state | Finite? | Closed under `M`? | Cube-relevant? |
|---|---|---|---|
| `[v] ∈ Q = Z^4/ker(T)` | no (`≅ ℤ`) | iff `T₂ ∥ T` | the cube condition is a finite *subset* of `Q` |
| `(1·v, T·v)` | unbounded on `R`; finite on cubes | iff `dim K_T ≤ 2` | yes, that is the defect equation |
| `(1·v, T·v, T₂·v)` | unbounded | yes for the witness (`dim K_T=3`) | cubes constrain the first two; `T₂·v` remains |
| `(r,s, fragment letters)` | yes | maps to `(0,0, empty)` under `σ` | incomplete without `v` |
| `v mod K_T^⊥` | infinite (`≅ Z^{κ}`) | yes | complete linear data |

A finite automaton of **cube types** `(r,s, T·v)` exists as a *classifier* of cubes once found (Theorem L′). It is not a closed dynamical obstruction: reachable `[v]` already occupy all small classes, and `M` leaves `Q` except on a thin subclass.

The 2-uniform census (all 65536 morphisms, matrix only; C++ `quotient_scan census` matches an independent Python minors loop exactly):

```
                    A={0,1,2,3}          B={0,1,3,4}
krylov_A dim 2/3/4      854 / 12540 / 52142     462 / 11186 / 53888
krylov_T dim 1/2/3/4    452 / 9096 / 36012 / 19976   324 / 8034 / 36742 / 20436
L-invariant             9548                    8358
ker(T)-invariant        1182                    1016
T affine                854                     462
4-sum L-inv (aff/not)   734 (402 / 332)         426 (138 / 288)
3-sum L-inv             2626                    2212
n_sums≥3 and kerT-inv   373                     347
```

Generic 2-uniform morphisms have `dim K_A=4` (`K_A^⊥={0}`): **nothing** can be quotiented. The witness has `dim K_T=3`.

`dim K_A=2` is exactly the affine class (Theorem P). `dim K_T=2` is exactly `L`-invariance, a strictly larger class.

---

## 6. AP vs NON-AP COMPARISON

**Letter defects versus `T`-interiors** is true by construction: `δ` is a sum of letters of `W` (values in `A` or `B`); `T·ψ` is an image-sum of ancestor letters. That identity does not use exceptionality of `A`.

**“AP relations only through boundaries” is false.** Smallest witness cube with **nonzero** `v ∈ L(A)\L(B)`:

```
i=47, d=27, v=(0,−1,2,−1), Δδ=1
```

This is an interior Parikh difference `1+3` vs `2+2`, defect 1. Independently present at later positions with the same `(d,v)`. In the 2048-prefix there are **34** cubes with `v ∈ L(A)\L(B)`. The AP collision `0+2=1+1` corresponding to `(1,−2,1,0)` is *not* this vector; `(0,−1,2,−1)` is the other AP-only short vector from the previous U4 table.

The common lattice vector `(1,−1,−1,1) ∈ L(A)∩L(B)` occurs earlier (`d=8`, `Δδ=0`) and is exactly `ker(M)` for this morphism — the direction Theorem O says **may** be quotiented.

On `{0,1,3,4}` the same 2-uniform combinatorics has the same Krylov picture up to replacing `A` by `B` in the letter vector; counts in §5 are similar, not a distinction. Exceptionality of `A` is **not** “`Q` is finite on `A` and not on `B`”.

When `T` **is** affine, `T·v = β A·v` on `ker(1)`, so `L = L(A)` and the AP letter lattice **is** the interior kernel. That happens for 854 two-uniform maps, including 402 with four image sums, and is the CMRS setting — the AP structure is then in the interior `T`, not a boundary-only effect. The 332 four-sum `L`-invariant **non-affine** maps show that image-sum dynamics can close without collapsing to `L(A)`.

---

## 7. SMALLEST COUNTEREXAMPLES

| Claim | Smallest counterexample |
|---|---|
| `Q` is finite | any `T≠0`; witness `Q≅ℤ` |
| `[Mv]` is a function of `[v]∈Q` | witness `T₂·e₁=2≠0` |
| AP only at boundaries | `d=27`, `i=47`, `v=(0,−1,2,−1)∈L(A)\L(B)` |
| `v∈L(A)` for cubes | already `d=3` in the 2-uniform first-cube scan (previous report); here first cube `v=(1,−1,1,−1)` |
| non-Pisot ⇒ defect window is hit as a closed finite-state law | witness is non-Pisot (`λ=√2`) and `dim K_T=3`, not 2 |
| `R_n` avoids `C_σ` | ancestor length 1024: every class in `{−8,…,8}` realized, min occupancy 1895 |
| 4-sum + `L` invariant ⇒ `T` affine | **332** counterexamples; smallest `0→33, 1→31, 2→10, 3→00`, `T=(6,4,1,0)`, `T₂=12·1−2T` |
| the interesting `n_sums≥3`, `dim K_T=2`, `T` not affine class first appears at `m≥3` | already **2958** two-uniform maps (2626 with three sums, 332 with four) |

---

## 8. COMPUTATIONAL CERTIFICATES

| Claim | Space | Independent check |
|---|---|---|
| `T₂`, `T·v`, `T₂·v`, `krylov_A=krylov_T=3` on witness | 4×4 minors | `python/quotient_verify.py`; `./bin/quotient_scan famous` — both give `T₂=(3,5,7,9)`, `T·v=2`, `T₂·v=−4`, dim 3 |
| 2-uniform Krylov histogram and L/affine split | 65536 morphisms, no words | C++ `quotient_scan census` and independent Python minors loop: **exact match** on every count in §5 |
| `T₂·v` on cubes | 442 (Python CSV) and 2700 (C++ 8k CSV) | ranges above; `quotient_verify` |
| Reachable `T·v` | 75520 consecutive pairs in `U[:1024]` | Python prefix sums of `T(U)`; window filled, min=1895 |
| AP interior `L(A)\L(B)` | 442 cubes | 34 hits; smallest `(d,i)=(27,47)` |
| `T₂·eᵢ=2` | `e₁,e₂` as in §2 | `quotient_verify` |

Reproduce:

```
make test
python3 python/quotient_verify.py
./bin/quotient_scan famous
./bin/quotient_scan census
```

Exact integer arithmetic only. No SAT. No long ACF search. The census is matrix-only; it does not claim anything about which of these morphisms produce cubes (that was already exhaustive: every prolongable 2-uniform map fails by length 120).

---

## 9. LITERATURE / NOVELTY AUDIT

Searched: CCSS; Rao 2015 (additive-cube-free substitutions — sufficient conditions by *preservation*, aligned abelian-style desubstitution, Theorem 2); Liétard–Rosenfeld (lattice `L(A)`, not `Z^4/ker(T)` for uniform image sums); Ardal–Brown–Jungić–Sahasrabudhe (additive complexity); Banero / Whiteland-style additive complexity of morphic words; Andrade–Mol 2024 and CMRS 2021 (affine morphisms: `|σ(x)|` and `∑σ(x)` linear in the letter); Rao–Rosenfeld templates / Jordan form; FSTTCS 2024 additive complexity + Walnut; Parikh automata (unrelated acceptance condition).

**KNOWN:** `L(A)` vs `L(B)` (Liétard–Rosenfeld). Affine morphisms and 2-coordinate (length, sum) templates (CMRS, Andrade–Mol). Theorems J, K, L from the previous run (ELEMENTARY/UNVERIFIED NOVELTY, not upgraded). Dekking / two image-sums. Bounded additive complexity ⇒ additive powers (Ardal). Rao’s sufficient conditions for a substitution to *preserve* ACF (opposite direction).

**ELEMENTARY/LIKELY KNOWN:** Theorem M. `1` is a left Perron eigenvector of any uniform incidence matrix. `T = Mᵀ A`. Theorem P as linear algebra (the *name* “affine” is known).

**UNVERIFIED NOVELTY:** Theorems N and O as a packaged obstruction to finite-state dynamics on `Q`; the split `K_A` vs `K_T`; the 2-uniform census including the 332 four-sum `L`-invariant non-affine morphisms; the explicit extra coordinate `T₂·v` on the witness cubes; AP-only interior `v=(0,−1,2,−1)` at `d=27`. Do not inflate to GENUINELY NEW WITH EVIDENCE — these are short linear-algebra observations that could sit in a thesis appendix.

**Not found:** a theorem that `Z^4/ker(T)` yields a finite automaton of additive-cube defects under uniform substitution.

---

## 10. WHAT THIS DOES AND DOES NOT PROVE

**Proves:** the proposed finite-state quotient by `ker(T)` is algebraically the wrong object. Cubes live in a finite subset of an infinite group; `M` does not act on that group except when `T₂ ∥ T`; reachable classes already occupy the subset; the invariant package is Krylov, typically dimension 3 or 4; AP collisions occur in interiors; non-Pisot is not the closure condition; `L`-invariance is strictly larger than affinity of `T`.

**Does not prove:** that every primitive uniform morphism produces a cube; that any uniform morphism is ACF; a finite-state obstruction of any other kind; unboundedness of `T₂·v` on cubes (only growth on prefixes 2048 and 8192); that the 2958 two-uniform `L`-invariant non-affine maps are interesting as ACF candidates (they are not: Theorem F already kills all prolongable 2-uniform maps).

Hitting `T·v ∈ −D` for one pair of consecutive ancestor factors is cheap. A cube still needs the *same* `d`, two matching defect equations, and fragment letters that actually realize those defects. Necessity is not sufficiency (unchanged).

For the `L`-invariant subclass, `(r,s, fragments, 1·v, T·v)` is a closed finite description of *cube types*. That is a classifier, not an obstruction, and in the affine subcase it is the CMRS template already in the literature.

---

## 11. SINGLE NEXT BOTTLENECK

The pair-window `T·v ∈ −D` is saturated. The missing object is the **joint triple** `(v_XY, v_YZ, fragments)` — equivalently, three consecutive equal-length ancestor factors whose `T`-sums differ only by defects, with compatible boundary letters.

A pair-quotient, even the closed `(1·v, T·v)` dynamics on the `L`-invariant subclass, cannot see the third block. The next attack is a factor-graph / return-word graph of the `T`-weighted ancestor, not another coordinate of `Q`.

Until a theorem forces a 3-block match or an explicit morphism keeps all triples out of the defect window, uniformly morphic ACF over `{0,1,2,3}` remains open.

---

ENDGAME C
