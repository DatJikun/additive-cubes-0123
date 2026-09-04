# Desubstitution of unaligned additive cubes in uniformly morphic words

**Date:** 2026-09-04  
**Objective:** Prove or disprove a desubstitution theorem for *unaligned* additive cubes in uniformly morphic words.  
**Original infinite ACF problem over `{0,1,2,3}`:** still **OPEN**.  
**Rank:** **LEVEL 4** (restricted structural theorem + classified failure of the universal claim). Not Level 5 (no infinite uniformly morphic ACF word). Not Level 6.

The Cassaigne / Liétard–Rosenfeld lattice distinction `L(A)` vs `L(B)` is **not** claimed as new.

---

## 1. Strongest result

Three statements, in order of logical strength.

**S1 (proved).** For any `m`-uniform morphism `σ` and any interval in `W = σ(U)`,

```
sum(W[p, p+d)) = T · ψ + δ
```

where `T(a) = sum(σ(a))`, `ψ` is the Parikh vector of the *complete* ancestor letters, and the **boundary defect** `δ` is the sum of at most two proper prefix/suffix fragments of images. The set of possible defects depends only on `σ` (hence only on `m` and the four images), not on substitution depth.

An additive cube `XYZ` is therefore exactly the pair of integer equations

```
T · (ψ_X − ψ_Y) + (δ_X − δ_Y) = 0
T · (ψ_Y − ψ_Z) + (δ_Y − δ_Z) = 0.
```

This is the correct desubstitution identity. It is *not* tautological as a statement about cubes: it names the precise lattice in which the ancestor Parikh difference must live,

```
v ∈ { x ∈ Z^4 : 1·x = Δn,  T·x = −Δδ }
```

with `Δn ∈ {−1,0,1}` for `k ≥ 1` and `Δδ` from a finite set.

**S2 (proved; kills the dream theorem).** The universal claim

> every sufficiently large additive cube in a uniformly morphic word desubstitutes to an ancestor *additive cube* (equal letter-sums on `U`)

is **false**. So is the claim that the ancestor Parikh difference `v` lies in the AP lattice `L(A) = {v : 1·v = 0, A·v = 0}`.

The leftmost cube of a prolongable uniform fixed point *cannot* be the `σ`-image of an additive cube of the ancestor, because that ancestor is a strictly shorter prefix of the same word and is therefore itself ACF (Theorem J below). Independently, the standard 2-uniform witness has first cube

```
σ:  0→32, 1→31, 2→20, 3→01,  seed 2
i=45, d=25, sum=35,  (r,s,k)=(1,1,12)
v = ψ_X − ψ_Y = (1,−1,1,−1)  ∉ L(A)
T · v = 2 = −(δ_X − δ_Y),   δ = (1,3,1)
```

The ancestor factor is ACF. The interiors are *not* a `T`-cube either (`T·v ≠ 0`); the cube exists only because a nonzero defect cancels a `T`-imbalance.

**S3 (computational, both alphabets).** Exhaustive 2-uniform geometry and structured `m=2..10` families do **not** distinguish `{0,1,2,3}` from `{0,1,3,4}` at the level of residue types: both alphabets die by length 200 in every prolongable 2-uniform iterate, and both are dominated by *unaligned* first cubes. What *does* distinguish them, for a *fixed combinatorial morphism*, is whether `T` is affine to an AP. Recoding the witness onto `{0,1,3,4}` makes `T=(7,5,3,1)` an AP, and the first cube becomes aligned of length 50 with `v=(0,−1,2,−1) ∈ L(T)=L(AP)` and defect `0` — still **not** a letter-sum cube on `{0,1,3,4}`.

---

## 2. Exact theorem / conjecture

### Theorem J — First cubes are not letter-desubstitutions
**Status: PROVED.** Novelty: UNVERIFIED NOVELTY (elementary; packaged here because it is the obstruction the dream theorem ignored).

Let `σ` be `m`-uniform, `m≥2`, prolongable at `a`, and `W=σ^ω(a)`. Let `XYZ` be an additive cube of `W` of minimal endpoint `N=p+3d`. Let `n` be such that `m^n < N ≤ m^{n+1}`, and let `U=σ^n(a)`. Then `|U|=m^n < N` and `U` is a prefix of the ACF word `W[:N−1]`, hence `U` is ACF. The ancestor letters of `XYZ` lie in `U`. Therefore `XYZ` is not `σ` of an additive cube of `U`.

### Theorem K — Exact unaligned identity
**Status: PROVED.** Novelty: UNVERIFIED NOVELTY as a packaged additive-cube statement; the split itself is the standard complete/partial decomposition of a uniform substitution.

Let `σ` be `m`-uniform, `T(a)=sum(σ(a))`. Write `p=qm+r`, `d=km+s` with `0≤r,s<m`. For `k≥1` the interval `[p,p+d)` consists of

- a left fragment of length `0` if `r=0`, else `m−r` (a suffix of `σ(U[q])`);
- `n = k + (r+s)//m − [r>0]` complete images;
- a right fragment of length `(r+s) mod m` (a prefix of a later image; empty if that residue is 0).

Then `sum = T·ψ + δ` with `δ` the sum of the two fragments. The identity was checked on:

- 500 random intervals plus the witness cube, by an independent ceil/floor implementation that reads defects from `W` rather than from images (`python/desub_verify.py`);
- all 442 additive cubes in the witness iterate of length 2048, independently in Python and C++ (`eq_fail=0`, `recon_fail=0`);
- all 2700 cubes in the length-8192 iterate (C++);
- all 65536 prolongable 2-uniform first cubes over `{0,1,2,3}` and all 65536 over `{0,1,3,4}` (C++).

### Theorem L — Finite defects, affine ancestor lattice
**Status: PROVED.**

For fixed `σ`, each fragment sum is a proper prefix or suffix sum of one of four images, so the one-fragment set `D₁` is finite (`|D₁| ≤ 1+8(m−1)` before collisions) and the two-fragment set `D₂=D₁+D₁` is finite. Cube equations force `v=ψ_X−ψ_Y` to lie in one of finitely many affine lattices

```
Λ(Δn, Δδ) = { x : 1·x = Δn, T·x = −Δδ },   Δδ ∈ D₂−D₂.
```

When `Δn=Δδ=0` this is `ker(1)∩ker(T)`, i.e. the Cassaigne lattice of the *image-sum alphabet*, **not** of the letter alphabet.

### Failed universal claims (counterexamples)

| Claim | Smallest counterexample | Why it fails |
|---|---|---|
| Every cube desubstitutes to an ancestor letter-sum cube | Witness `i=45,d=25`; in fact **every** leftmost cube of a prolongable uniform word (Thm J) | Ancestor is ACF |
| `v ∈ L(A)` | 2-uniform first cubes with `d=3,i=0,v=(−1,0,0,1)` (exhaustive); structurally the witness `v=(1,−1,1,−1)` | Equation is `T·v=−Δδ`, not `A·v=0` |
| `‖v‖₁` is bounded independently of `d` | Witness word, unaligned cube `d=1767`, `v=(6,−12,13,−7)`, `‖v‖₁=38` at length 8192 | Coefficients on a rank-2 lattice grow |
| Unaligned ⇔ defect ≠ 0 always distinguishes A from B at the first cube | Exhaustive 2-uniform: A has 43876 unaligned / 21660 aligned first cubes; B has 44448 / 21088 | Same type distribution |
| Two image-sums ⇒ the *first* cube is aligned of length `m` | 4190 first cubes on A with `n_sums≤2` that are unaligned | Dekking/U1 guarantees *some* aligned cube of length `m`, not that it appears first |

### Surviving conjecture (not proved)

**Conjecture D.** In a primitive `m`-uniform infinite word, every additive cube with `k=⌊d/m⌋` large belongs to one of finitely many residue types `(r,s)` and its ancestor difference `v` lies in `Λ(Δn,Δδ)` for a defect class determined by `(r,s)`. The set of types is finite (`m²`). This does **not** by itself imply that a cube occurs.

---

## 3. Desubstitution classification

### Residue types

For `k≥1` there are exactly `m²` types `(r,s)`. Closed form, checked against interval decomposition for all `m=2..10`, `k=0..5`:

```
n_complete = k + (r+s)//m − [r>0]
left_len   = 0 if r=0 else m−r
right_len  = (r+s) mod m
```

**m=2 table** (the four types of a 2-uniform cube):

| `(r,s)` | left | right | `n` | start of next block |
|---|---|---|---|---|
| `(0,0)` | 0 | 0 | `k` | aligned cube of `k` images |
| `(0,1)` | 0 | 1 | `k` | complete `k` + prefix 1 |
| `(1,0)` | 1 | 1 | `k−1` | suffix 1 + complete `k−1` + prefix 1 |
| `(1,1)` | 1 | 0 | `k` | suffix 1 + complete `k` (end on a block boundary) |

A cube uses three consecutive intervals of equal length, so the three start residues are `r`, `(r+s) mod m`, `(r+2s) mod m`. For the witness, type `(1,1)` then `(0,1)` then `(1,1)`.

### The witness, fully expanded

Ancestor `U = σ^6(2)` of length 64, ACF. Cube uses `U[:60]`:

```
U = 203201203231203201200131203201203231203232310131203201203231
```

- `X=[45,70)`: suffix of `U[22]=3`, i.e. the letter `1` of `σ(3)=01`, plus complete `U[23:35]=120320120323`. `T·ψ_X=34`, `δ_X=1`, sum `35`.
- `Y=[70,95)`: complete `U[35:47]=120323231013` plus prefix of `U[47]=1`, i.e. the letter `3` of `σ(1)=31`. `T·ψ_Y=32`, `δ_Y=3`, sum `35`.
- `Z=[95,120)`: suffix of `U[47]=1` (the letter `1`) plus complete `U[48:60]=203201203231`. `T·ψ_Z=34`, `δ_Z=1`, sum `35`.

Parikh:

```
ψ_X = (3,2,4,3)
ψ_Y = (2,3,3,4)
ψ_Z = (3,2,4,3)
v_XY = (1,−1,1,−1)     A·v = −2 ≠ 0
T     = (5,4,2,1)      affine to {0,1,3,4}
T·v   = 2 = −Δδ
```

The interior exchange is one extra `{0,2}` versus `{1,3}` in complete blocks (`T(0)+T(2)=7`, `T(1)+T(3)=5`). The AP relation `0+2=1+1` is **not** used. The defect pair `(1,3)` is two single letters of `W`.

### Coarser substitutions `σ^k` (same infinite word, larger `m`)

The *same* cube against `σ^k`:

| `k` | `m` | `(r,s)` | `n_complete` | `v` | comment |
|---|---|---|---|---|---|
| 1 | 2 | 1,1 | 12 | `(1,−1,1,−1)` | the analysis above |
| 2 | 4 | 1,1 | 5 | `(0,−1,1,0)` | still unaligned |
| 3 | 8 | 5,1 | 2 | `(1,0,0,−1)` | |
| 4 | 16 | 13,9 | 1 | `(0,0,1,0)` | one complete block |
| 5 | 32 | 13,25 | 0 | `0` | **pure defect** (`d < m`) |
| 6 | 64 | 45,25 | 0 | `0` | intra-block at this scale |

Any fixed cube becomes a pure boundary phenomenon once `m > d`. “Sufficiently large” in a desubstitution theorem must mean `d ≫ m` for a *fixed* morphism, not large versus iterates `σ^k`.

### Cube-type automaton (Phase 10)

**States:** residue pairs `(r,s)`. Finite: `m²`. For `m=2` all four states occur as first cubes (exhaustive) and as cubes in the long witness iterate.

**Transition under `σ` (going up):** a cube at `(i,d)` maps to a cube at `(mi, md)`, which is always type `(0,0)`. Unaligned types are transient under iteration of `σ`. The recurrent class of the *image* graph is `{(0,0)}`.

**Desubstitution (going down):** an aligned vanishing-defect cube with `T·v=0` is a `T`-cube in the ancestor. It is a *letter-sum* cube of `U` only if `ker(T)∩ker(1)` forces `A·v=0` (e.g. `T` affine to `A`). Unaligned states have no A-cube predecessor (Theorem J for the first cube; for later cubes the interiors remain `T`-weighted). They are **sources**: spontaneous cubes created by defect cancellation.

Three possibilities from the mission:

1. The type graph of A-cubes has no infinite path of unaligned types under desubstitution — true, because unaligned types do not desubstitute to A-cubes.
2. Cycles exist — only the aligned class `(0,0)` cycles under `σ`.
3. Transient + recurrent — unaligned types are the transients; `(0,0)` is recurrent as images. Spontaneous production of transients at every scale still occurs in non-ACF words (the witness has all four types among 442 cubes of a length-2048 prefix).

This automaton does **not** prove uniform-morphic impossibility: it classifies how cubes sit relative to `σ`, it does not force their existence.

---

## 4. Smallest counterexample to the false universal claim

Lexicographic among disproved claims, as requested (`m`, then depth, then `d`):

1. **`v ∈ L(A)` for every unaligned cube**, smallest `m=2`:
   first cube `d=3`, `i=0`, `v=(−1,0,0,1)` in the exhaustive 2-uniform scan. `1·v=0` but `A·v=3≠0`.
2. **Letter-desubstitution of a structurally large unaligned cube**, smallest interesting witness:
   `m=2`, substitution depth 7 (`|W|=128`), `d=25`, `i=45`, `v=(1,−1,1,−1)`, boundary complexity two fragments of length 1. Independently verified by C++ and two Python implementations.
3. **Bounded `‖v‖₁` for large unaligned cubes:**
   same morphism, `d=1767`, `v=(6,−12,13,−7)=6(1,−2,2,−1)+(0,0,1,−1)`, `‖v‖₁=38`.

The vector `(1,−2,2,−1)` that dominates later cubes of the witness (102 of 442 cubes at length 2048) is the CCSS short vector of `L({0,1,3,4})`, i.e. of `ker(1)∩ker(T)` for this `T`, and is **absent** from `L(A)`.

---

## 5. AP vs non-AP comparison

The same machinery on `A={0,1,2,3}` and `B={0,1,3,4}`.

### Lattices (primitive, `‖v‖_∞≤4`)

`L(A)` is spanned by `(1,−2,1,0)` and `(2,−3,0,1)`. Short primitives include the AP exchanges

```
(1,−2,1,0)    0+2 = 1+1
(0,1,−2,1)    1+3 = 2+2
(1,−3,3,−1)
(1,−1,−1,1)    0+3 = 1+2     (also in L(B))
```

`L(B)` is spanned by `(2,−3,1,0)` and `(3,−4,0,1)`. It contains `(1,−2,2,−1)` and **not** `(1,−2,1,0)`.

This distinction is Liétard–Rosenfeld / CCSS. Recorded, not claimed new.

### Realisability (do not confuse with occurrence of a cube)

On the witness over `A`, ancestor differences of actual cubes occupy both lattices:

- `(1,−1,1,−1)` (first cube) — neither `L(A)` nor `ker(T)`
- `(0,−1,2,−1)` (second listed cube `i=47,d=27`) — **in `L(A)`**, AP-only
- `(1,−2,2,−1)` (most frequent at length 2048) — **in `ker(T)=L(B)`**, not in `L(A)`
- `(1,−3,3,−1)` — in `L(A)`

So AP vectors *can* occur as ancestor Parikh differences of unaligned cubes over `A`. They are not the typical large-`d` mechanism of this morphism; the `T`-lattice is.

On exhaustive 2-uniform first cubes, primitive `(1,−2,1,0)` occurs **148** times over `A` and **333** times over `B` (where it is *not* a letter-sum relation). Presence of that Parikh shape is not an AP-only phenomenon.

### Same combinatorics, opposite `T`

| | witness on `A` | recoded on `B` |
|---|---|---|
| images | `32,31,20,01` | `43,41,30,01` |
| `T` | `(5,4,2,1)` ~ `{0,1,3,4}` | `(7,5,3,1)` ~ `{0,1,2,3}` |
| ACF prefix | 119 | 164 |
| first cube | unaligned `d=25` | aligned `d=50` |
| `v` | `(1,−1,1,−1) ∉ L(A)` | `(0,−1,2,−1) ∈ L(AP)=L(T)` |
| defect | `(1,3,1)` | `(0,0,0)` |
| letter-cube on `U`? | no | no (`B·v=1≠0`) |

**Mechanism that is possible on `A` and, for this shape, rearranges on `B`:** interiors see `T`, boundaries see letter values. If letters are an AP and `T` is not, unaligned defect cancellation is the cheap way to make `T·v+Δδ=0`. If letters are not an AP and `T` is, the first cube of this shape becomes an aligned `T`-cube (an AP-cube in the recoding). Neither is an infinite-ACF construction. Both 2-uniform classes are empty of survivors to cap 200 (`survivors=0` on each alphabet).

### What does *not* explain exceptionality of `A`

- Unique-vote / length-14 dead-ends (previous report): both alphabets.
- Two image-sums (U1) and Pisot-uniform (U2): both alphabets.
- Unaligned first-cube *frequency* at `m=2`: essentially identical (`~67%` vs `~68%`).
- Theorem J: alphabet-independent.

The remaining uniform-morphic distinction is not “unaligned cubes exist only on `A`”. It is whether letter-defects in `L(A)` can cancel interior `T`-imbalances *indefinitely* for some primitive `σ` of size `≥7`. That is not settled.

---

## 6. Proof and computational certificate

### Proofs

Theorem J is given in §2. Theorem K is the complete/partial split of an interval in a concatenation of equal-length blocks; the closed form for `n_complete` is the difference `⌊(p+d)/m⌋ − ⌈p/m⌉`, rewritten in `(r,s,k)`. Theorem L is finiteness of prefix/suffix sums of four words of length `m`.

### Search spaces

| Claim | Space | Algorithm | Stop | Independent check |
|---|---|---|---|---|
| Identity on witness cube | one morphism, cube `(45,25,35)` | ceil/floor vs image fragments vs raw `sum(W[p:p+d])` | exact | C++ `desub_scan famous`; Python `analyze_cube`; Python `desub_verify` (defects from `W`) |
| Identity on all cubes | witness prefixes 2048 and 8192 | enumerate all `(i,d)` with prefix sums | 442 / 2700 cubes, `eq_fail=0` | Python CSV vs C++ CSV: `i,d` mismatch `0` on 2048 |
| Residue formula | `m=2..10`, `k=0..5`, all `(r,s)` | closed form vs `interval_pieces` | 0 failures | `python` snippet in this archive |
| 2-uniform first-cube types | `4^8=65536` morphisms × 4 seeds, exactly 65536 prolongable pairs per alphabet | iterate to cap 200, `find_cube`, ceil/floor anatomy | 0 survivors; 0 identity failures | C++ only for the scan; Python independently on the witness and on structured families |
| Structured `m=2..10` | two-sum, three-sum, four-sum spread, cyclic, imprimitive, repeated images, Pisot-like, Cassaigne-padded, witness powers; both alphabets | first cube of prolongable iterates, cap `min(1024, max(256,16m))` | 134 first cubes, `eq_ok` all 1 | Python |
| Lattices | `‖v‖_∞≤6` in `ker(1)∩ker(A)` | brute | `data/lattice_vectors.txt` | `python/lattice_tau.py` for the CCSS `τ`-picture (known) |

Reproduce:

```
make test
python3 python/desub_geometry.py
python3 python/desub_collect.py
python3 python/desub_analyze.py
./bin/desub_scan famous
./bin/desub_scan all 2048 data/famous_all_cubes_cpp.csv
./bin/desub_scan scan2 200 A data/scan2_A.csv
./bin/desub_scan scan2 200 B data/scan2_B.csv
```

No SAT. No long ACF-word search. No random morphism hunt beyond the named structured families and the exhaustive `m=2` type scan.

### Dual implementations

Interval reconstruction:

1. Python `view_interval`: defects from `σ(a)[off:off']`.
2. Python `desub_verify.recon_independent`: complete blocks via `T[U[j]]`, defects via `sum(W[...])`.
3. C++ `view_interval` in `src/desub_scan.cpp`: same ceil/floor as (2).

All three agree on the witness. (1) and (3) agree on all 442 cubes of length 2048.

---

## 7. Literature novelty audit

Second search for *this* object (desubstitution of unaligned additive cubes in uniform morphic words), 2026-09-04.

**Already known, do not claim:**

- Infinite ACF over every 4-letter number alphabet except affine images of `{0,1,2,3}` (Liétard–Rosenfeld).
- Cassaigne kernel `L={v: 1·v=0, A·v=0}`, contracting `τ`, constant `C` (CCSS; Liétard–Rosenfeld Lemma 3.3).
- Extra short vectors of `L({0,1,2,3})`, including `(1,−2,1,0)` (Liétard–Rosenfeld; previous report U4).
- Uniform size `≤6` (Liétard: `≥7` if a morphism exists); size 2 fails.
- Dekking: 2-letter abelian cubes unavoidable ⇒ two image-sums give aligned cubes (U1).
- Ardal–Brown–Jungić–Sahasrabudhe: bounded additive complexity ⇒ additive `k`-powers.
- Rao: sufficient conditions for a substitution to preserve additive-cube-freeness (abelian-style desubstitution for *aligned* images, not the unaligned identity).
- Andrade–Mol 2024: additive powers in *rich* words; problem over `{0,1,2,3}` restated as open.
- WORDS list (arXiv:1705.04055): Rao’s question on `{0,1,2,3}` still listed.

**Not found in CCSS, Rao, Liétard, Liétard–Rosenfeld, Andrade–Mol, theses, arXiv through 2026:**

- The exact `(r,s)` classification of unaligned additive cubes in uniform images.
- Theorem J (first cube of a prolongable uniform word cannot be a letter-desubstitution).
- The packaged identity `T·v + Δδ = 0` with finite defect set as the *correct* descendant of the dream theorem.
- Exhaustive residue-type census of 2-uniform first cubes on both alphabets.

These are labelled **UNVERIFIED NOVELTY**. They are elementary once stated; a prior occurrence in a thesis appendix would not be surprising. They are **not** a solution of the infinite problem.

Rao’s additive-cube-free morphisms paper gives sufficient conditions for `h(u)` to be ACF when `u` is, by controlling abelian-type desubstitution of *images*. That is the opposite direction (preservation), and it does not analyse unaligned cubes of a uniform fixed point via `T` and defects.

---

## 8. Exactly one remaining bottleneck

The surviving identity says that a cube is a hit of the finite-defect window by a `T`-weighted ancestor imbalance. It does **not** say that such a hit must occur.

**Bottleneck:** prove or disprove that, for every primitive `m`-uniform morphism over `{0,1,2,3}` with at least three distinct image sums and a non-Pisot spectrum, the pair `(v_n, δ_n)` along prefixes of `σ^ω(a)` *must* eventually satisfy `T·v+Δδ=0` for some cube type `(r,s)` — using that letter-valued defects live in an AP while interiors live in `T`, **or** produce an explicit primitive uniform `σ` of size `≥7` whose defect window is missed at every scale.

Until that lemma exists, uniform morphic ACF over `{0,1,2,3}` remains open, and the correctly desubstituted object is `T·v + defect`, not an ancestor letter-cube and not `L(A)` alone.

---

## Endgame

**ENDGAME D.** The universal letter-desubstitution theorem is false. The failure is classified: unaligned and first cubes are spontaneous `T`+defect events; aligned vanishing-defect cubes are `T`-cubes, hence letter-cubes only when `T` is affine to the letter alphabet. The strongest correct theorem is Theorem K+L together with Theorem J. What remains is the bottleneck above, not another finite ACF word and not another random morphism scan.

**LEVEL 4** for K+L+J. **LEVEL 2** for the witness census (`(1,−2,2,−1)` dominates large cubes; AP vectors occur but are not the generic large-`d` engine of this morphism). Do not inflate Level 2 into Level 4: the census is one morphism.

No Endgame C (no infinite uniformly morphic ACF word). No Endgame B (not every primitive uniform morphism is proved to produce a cube beyond the classes already killed by U1, U2, and Liétard’s size `≤6`).
