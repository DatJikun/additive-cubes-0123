# Triple graph of additive cubes under uniform substitution

**Date:** 2026-09-04  
**Question:** Does the shared-middle constraint \(X\to Y\to Z\) with \(\omega(X)=\omega(Y)=\omega(Z)\) create a dynamical obstruction invisible to pairwise defect windows?  
**Infinite ACF problem over \(\{0,1,2,3\}\):** still **OPEN**.  
**Rank:** LEVEL 4 for the exact type/junction theorems; LEVEL 2 for pair-vs-triple counts. Not Level 5.

The pair window is not reopened. The letter-cube dream is not revived. \(Q=\mathbb{Z}^4/\ker(T)\) is not reused.

---

## 1. EXECUTIVE VERDICT

\(\omega(F):=T\cdot\psi_F+\delta_F\) equals the numerical sum of the corresponding interval in \(W=\sigma(U)\). Therefore

```
ω(X)=ω(Y)=ω(Z)
```

**is** the additive-cube condition, for every residue configuration. It is not a new equation.

What the pairwise quotient missed, and what is exact, is the **geometry of the triple**:

* there are exactly \(m^2\) residue triple types, indexed by \((r,s)\), not \(m^3\);
* fragment *slots* (which boundaries exist, and their lengths) are determined by \((m,r,s)\) alone;
* fragment *letters* require up to four ancestor letters, and the two junctions are shared;
* at an unaligned junction the two complementary fragments of one image sum to \(T(a_{\mathrm{junc}})\).

That finite data does **not** decide cubes. The unavoidable unbounded coordinate is the interior complete-block contribution \(T\cdot\psi\), equivalently the window-sum \(\omega\) itself (grows with \(d\)).

Shared-middle coupling is real as a **filter**: on the famous prefix of length 512 there are 1995 additive squares and only 59 cubes (1524 XY-pairs with a Z that fail to close). It is **not** a new obstruction: it is the original cube condition, and Cassaigne’s word over \(\{0,1,3,4\}\) has 2724 squares and 0 cubes on a prefix of length 800. Recurrence plus pair matches does not force a triple.

No \(d\)-independent finite weighted factor graph characterises cubes. For each fixed \(d\) the concatenation graph is a disjoint union of \(d\) paths on positions (finite after identifying \(d\)-factors, of size \(p(d)\)). That is the original search.

**ENDGAME C.**

---

## 2. EXACT TRIPLE FORMULATION

Write \(W=\sigma(U)\) with \(\sigma\) \(m\)-uniform, \(T(a)=\sum\sigma(a)\). For an interval \(F=W[p,p+d)\),

```
S(F) = T·ψ_F + δ_F =: ω(F)
```

with \(\psi_F\) the Parikh vector of complete ancestor blocks and \(\delta_F\) the sum of at most two proper image fragments (Theorem K). Set \(X,Y,Z\) consecutive of length \(d\), \(v_{XY}=\psi_X-\psi_Y\), \(v_{YZ}=\psi_Y-\psi_Z\). Then

```
T·v_XY = δ_Y − δ_X
T·v_YZ = δ_Z − δ_Y
```

if and only if \(\omega(X)=\omega(Y)=\omega(Z)\), if and only if \(XYZ\) is an additive cube. This holds for every \((r,s)\).

Linear relations (always, cube or not):

```
ψ_X − ψ_Z = v_XY + v_YZ
1·v_XY = n_X − n_Y = Δn(r,s)
1·v_YZ = n_Y − n_Z
ω(X) − ω(Y) = T·v_XY − (δ_Y − δ_X)
```

The last vanishes on cubes. The shared middle is \(\psi_Y\) in both \(v\)'s and \(\delta_Y\) in both defect differences. These are not independent scalar equations on two free vectors: they are two evaluations of the same window-sum sequence at spacing \(d\).

Equivalent prefix-sum form (known): \(\omega_i=S(i+d)-S(i)\), and a cube is

```
ω_i = ω_{i+d} = ω_{i+2d},
```

i.e. a 4-term arithmetic progression \((i,S(i))\), \((i+d,S(i+d))\), \((i+2d,S(i+2d))\), \((i+3d,S(i+3d))\) on the graph of \(S\). Additive squares are the 3-term analogue.

---

## 3. TRIPLE STATE SPACE

Let \(i\equiv r\pmod m\), \(d\equiv s\pmod m\). Then

```
r_Y = (r+s) mod m,   r_Z = (r+2s) mod m,   r_end = (r+3s) mod m.
```

**Theorem R.** There are exactly \(m^2\) residue triple types, indexed by \((r,s)\in\{0,\ldots,m-1\}^2\). The pair \((r_Y,r_Z)\) is not extra data.

Fragment lengths:

```
X-left  = m−r   if r≠0 else 0     (suffix)
X-right = r_Y                      (prefix of the XY junction)
Y-left  = m−r_Y if r_Y≠0 else 0
Y-right = r_Z
Z-left  = m−r_Z if r_Z≠0 else 0
Z-right = r_end
```

These lengths determine \((r,s)\) uniquely (\(r\) from X-left, then \(s\) from \(r_Y\)). Hence there are also exactly \(m^2\) distinct slot signatures — not a coarser classification. Census \(m=2,\ldots,7\): `slot_sigs = m²` in C++ and Python.

**Ancestor letters.** Slots that need a letter: X-left (if \(r\neq 0\)), XY-junction (if \(r_Y\neq 0\)), YZ-junction (if \(r_Z\neq 0\)), Z-right (if \(r_{\mathrm{end}}\neq 0\)). At most **four** letters. They are **not** determined by \((m,r,s)\); they are the ancestor letters at four block indices. Minimal extra information: those (at most four) letters, plus the complete-block word of length \(\Theta(d/m)\).

Famous cube \((i,d)=(45,25)\), type \((1,1)\): \(r_Y=0\), \(r_Z=1\), two letters `U[22]=3` (X-left) and `U[47]=1` (YZ-junction).

---

## 4. T-WEIGHTED FACTOR GRAPH

For fixed \(d\), vertices = starting positions \(\{0,\ldots,n-d\}\) with weight \(\omega_i\), and edges \(i\to i+d\). This graph is a **disjoint union of \(d\) paths** (one per residue class modulo \(d\)). A cube is a path of length 2 with equal vertex weights.

Identifying positions that carry the same \(d\)-factor yields a finite graph of size at most \(p(d)\). Cubes become equal-weight directed 2-paths. This graph depends on \(d\) and is not a single finite object for the morphism.

**No:** additive cubes cannot be characterised as 2-paths in a \(d\)-independent finite weighted factor graph. The unavoidable unbounded information is \(\omega\) (or \(T\cdot\psi\) of \(\Theta(d/m)\) complete blocks).

**Yes, tautologically, per \(d\):** the Rauzy/concatenation graph of \(d\)-factors. That is the original \(O(n^2)\) search.

Sample: famous prefix 256, \(d=8\), offset 0: 4 distinct blocks, 1 equal-weight consecutive pair, **0** equal-weight 2-paths. Cubes of the word are unaligned with that parsing. One offset’s block graph misses the phenomenon.

---

## 5. MIDDLE-BLOCK COUPLING

For an *occurrence* \(Y=W[j,j+d)\) with both neighbours present, \(P(Y)\) and \(S(Y)\) each have size at most 1: the unique left block \(W[j-d,j)\) and unique right block \(W[j+d,j+2d)\). A cube is exactly \(P\) and \(S\) both matching \(Y\) in sum.

On famous \(n=512\):

| | count |
|---|---|
| middles | 43605 |
| \(P\) and \(S\) (cubes) | 59 |
| \(P\) only | 1524 |
| \(S\) only | 1739 |
| neither | 40283 |
| additive squares (any, Z need not exist) | 1995 |

Coupling is a severe filter (1524 one-sided XY matches vs 59 triples) and does **not** empty the triple set.

On Cassaigne \(n=800\): 2724 squares, **0** cubes, 2404 XY-matches with a Z that fail. One side populated, the other empty, for every middle that has a pair. Primitivity and uniform recurrence do **not** force overlap of \(P\) and \(S\). That word is the infinite uniformly recurrent counterexample to “pairs plus recurrence \(\Rightarrow\) triples”.

If \(Y\) is taken as an abstract \(d\)-mer, different occurrences have different neighbours; that only restates the concatenation graph of §4.

---

## 6. CORRECTED-WEIGHT / COCYCLE STRUCTURE

\(\omega(F)=T\cdot\psi_F+\delta_F\) is exactly \(S(F)\). The “corrected weight” is the original sum. Substitution splits it into a finite defect (the cocycle on fragment letters) plus an unbounded interior \(T\)-walk.

Shift cocycle, exact:

```
ω_{i+1} − ω_i = W[i+d] − W[i].
```

The two letters \(W[i],W[i+d]\) are determined by \((i\bmod m, U[\lfloor i/m\rfloor])\) and \(((i+d)\bmod m, U[\lfloor(i+d)/m\rfloor])\). Tracking them requires a sliding ancestor window of length \(\Theta(d/m)\), unbounded in \(d\).

**Theorem S (junction covering).** If \(r_Y\neq 0\), the XY junction letter \(a\) satisfies \(\delta_X^{\mathrm{right}}+\delta_Y^{\mathrm{left}}=T(a)\), because the prefix of length \(r_Y\) and the complementary suffix partition \(\sigma(a)\). Likewise at YZ. Famous cube: YZ junction \(U[47]=1\), \(\sigma(1)=31\), prefix `3` + suffix `1` sum to \(4=T(1)\). Dual check: complementary offsets, no failures.

This is a finite cocycle on junctions. It does not constrain the interior \(T\cdot\psi\). A primitive substitution can avoid three consecutive equal \(\omega\)-values: Cassaigne does (non-uniform). No uniform example on \(\{0,1,2,3\}\) is known; none is produced here.

---

## 7. SUBSTITUTION DYNAMICS

Applying \(\sigma\) to a cube of \(U\) produces an **aligned** \((r,s)=(0,0)\) cube of block length \(md\) in \(W\), defects zero. Desubstituting an unaligned cube of \(W\) need not produce a cube of \(U\) (Theorem J: famous first cube, ancestor ACF). Cube states are not downward-closed.

The finite type \((r,s)\) evolves under a one-letter shift by \(r\mapsto r+1\bmod m\) (\(s\) fixed). That cycle of length \(m\) carries no cube information.

There is no finite transition relation on \((r,s,\text{fragment letters})\) whose recurrent SCCs decide cubes: those states omit \(\omega\). Adding \(\omega\) makes the state space infinite (Claim C).

Claim B (“every recurrent triple-state SCC contains a cube”): for a finite type-graph, cubes are not states. For the subshift itself, if the infinite word is uniformly recurrent and contains one cube then the cube factor recurs — so Claim B is the original existence question. It is **false** for primitive morphic words in general (Cassaigne). It is **open** for primitive *uniform* morphisms on \(\{0,1,2,3\}\). Two-uniform maps are already known to fail (Theorem F) on both alphabets.

---

## 8. A vs B COMPARISON

| | A famous uniform | B Cassaigne (non-uniform) |
|---|---|---|
| additive squares, \(n=512\) / \(800\) | 1995 | 2724 |
| additive cubes | 59 | 0 |
| \(P\) only / \(P\) and \(S\) | 1524 / 59 | 2404 / 0 |
| first cube | \((45,25,35)\) | none |
| smallest-\(d\) square | \((19,1,0)\) (`00`) | \((6,1,1)\) (`11`) |

The joint triple distinguishes A from B only in the same way the original problem does: Cassaigne avoids triples and not pairs. The finite type graph \((r,s)\) is a uniform-substitution object; it is not defined for Cassaigne’s variable-length images, and at \(m=2\) every prolongable uniform map on **both** alphabets produces a cube (previous exhaustive scan). Pairwise defect windows also failed to separate the alphabets at \(m=2\). Interior AP vectors remain (previous \(d=27\)); they are not a triple-graph topology distinction.

The structural A/B split remains Liétard–Rosenfeld’s lattice \(L(A)\) vs \(L(B)\) for the *Cassaigne-shaped* (non-uniform) family, not a new uniform triple-SCC invariant.

---

## 9. SMALLEST COUNTEREXAMPLES

| Claim | Status | Smallest counterexample |
|---|---|---|
| A. Every primitive uniformly recurrent word has an additive cube | **FALSE** | Cassaigne \(\varphi_0^\omega(0)\) over \(\{0,1,3,4\}\) (KNOWN) |
| B. Every recurrent triple-state SCC contains a cube | **FALSE** without uniformity (Cassaigne); **open** for primitive uniform on A; tautological if the word already has a cube and is uniformly recurrent | Cassaigne |
| C. Corrected-weight graph finite-state for every fixed uniform \(\sigma\) | **FALSE** | Famous: \(\omega\)-range at \(d=1\) is \(\{0,1,2,3\}\); at \(d=100\) is \([129,171]\) (42 values). Unbounded in \(d\) |
| D. Triple coupling eliminates pair-window saturation | **FALSE as an obstruction** | Famous still has 59 cubes. **TRUE as a filter**: 1524 pair-not-triple vs 59 cubes; Cassaigne 2404 vs 0 |
| E. Middle \(Y\) decides both defect equations | **FALSE** | Famous cube: X-left is \(U[22]=3\), not in Y’s ancestor span \([35,47]\). Need the outer fragment letter |
| Finite \(d\)-independent 2-path characterisation | **FALSE** | Concatenation graph depends on \(d\); aligned \(d=8\) graph of famous has 0 equal-weight 2-paths while cubes exist unaligned |

Strongest correct weakening of D: coupling is the square-to-cube step; it is strictly stronger than pair matching and is not implied by uniform recurrence.

---

## 10. COMPUTATIONAL CERTIFICATES

| Claim | Dual check |
|---|---|
| Type census \(m=2..7\): \(m^2\) types, max 4 letters | `python/triple_graph.py` and `./bin/triple_scan types` match histograms |
| Famous cube \(\omega=(35,35,35)\), \(v_{XY}+v_{YZ}=\psi_X-\psi_Z\), type \((1,1)\), 2 letters, YZ covering | `triple_verify.py`; junction fails `[]` |
| Famous \(n=512\): 1995 squares, 59 cubes, 1524 pair-not-triple, P/S split | Python `pair_triple_counts` = C++ `triple_scan famous` **exact** |
| Cassaigne \(n=800\): 2724 squares, 0 cubes, first \(d=1\) square \((6,1,1)\); lex-\(i\) square \((3,4,8)\) = `4301`/`1034` sums 8 | Python = C++; `find_cube` none |
| \(\omega\) grows with \(d\) | `triple_verify` max window \(d=100>d=1\) |

```
make test
python3 python/triple_verify.py
python3 python/triple_graph.py
./bin/triple_scan all
```

Exact integers. No SAT. No long ACF search.

---

## 11. LITERATURE / NOVELTY AUDIT

CCSS (arXiv:1106.5204 / J. ACM) already searches **triples of consecutive equal-length blocks** with Parikh vectors and bounded differences \(u=\psi_Y-\psi_X\), \(v=\psi_Z-\psi_Y\). Schaeffer’s slides describe a tree of such triples. The \(\omega\)-formulation is the definition of an additive cube. 4-term APs on the graph of the prefix sum are the standard translation (Pirillo–Varricchio / Halbeisen–Hungerbühler language for additive powers). Additive squares vs cubes: squares are open to avoid over \(\mathbb{Z}\); cubes are avoided over \(\{0,1,3,4\}\) (CCSS) and open over \(\{0,1,2,3\}\). Ardal et al.: bounded additive complexity \(\Rightarrow\) additive \(k\)-powers. Dekking: 2-letter aligned \(T\)-words force cubes (Theorem C). CMRS / Andrade–Mol: affine templates. Rao 2015: preservation, opposite direction.

**KNOWN:** Cassaigne ACF; Claim A false; \(\omega=\mathrm{sum}\); CCSS triple search tree; A vs B lattice (Liétard–Rosenfeld); 2-uniform failure on both alphabets.

**REPRODUCED:** Cassaigne has additive squares (consecutive `11` at \(i=6\); block square `4301`/`1034` at \(i=3\), \(d=4\)) and no cubes.

**ELEMENTARY/LIKELY KNOWN:** Theorem R (\(r_Y,r_Z\) determined by \((r,s)\)); Theorem S (partition of an image); occurrence-middle \(P,S\) of size \(\le 1\); concatenation graph = \(d\) paths.

**UNVERIFIED NOVELTY:** the packaged uniform junction+slot count (exactly \(m^2\), \(\le 4\) letters); the exact famous/Cassaigne pair-vs-triple numbers; Claim E’s explicit outer letter \(U[22]\). Do not inflate. Do **not** claim a “triple graph obstruction” as new: CCSS already works with triples.

---

## 12. WHAT THIS DOES AND DOES NOT PROVE

**Proves:** the \(\omega\)-triple equation is the original cube condition; the extra exact structure is \((r,s)\) plus \(\le 4\) ancestor letters plus junction covering; that finite package does not yield a finite-state obstruction; \(\omega\) is unbounded; middle \(Y\) does not determine the outer fragment; recurrence+pairs do not force triples (Cassaigne); coupling filters pairs down to cubes but does not empty cubes on the famous uniform witness.

**Does not prove:** that every primitive uniform morphism on \(\{0,1,2,3\}\) produces a cube; a uniformly morphic ACF construction; that Cassaigne’s square/cube split can be copied by a uniform morphism on A; unboundedness of anything beyond window sums of one witness.

---

## 13. SINGLE NEXT BOTTLENECK

The interior complete-block \(T\)-word of length \(\Theta(d/m)\), with an \(O(1)\) defect from at most four letters. That is the CCSS/template problem specialised to uniform \(\sigma\): whether a primitive uniform morphism with \(\ge 3\) image sums can keep the block-sum sequence over the image-sum alphabet free of three consecutive equal terms **after** bounded defect perturbations.

Templates for *affine* uniform morphisms already exist (CMRS). The missing case is non-affine uniform \(\sigma\) (Krylov dimension \(\ge 3\)), where length-and-sum templates do not close. That is a template-method question, not a new factor-graph question.

Until it is solved or a uniform morphism on A is proved ACF, the infinite problem remains open.

---

ENDGAME C
