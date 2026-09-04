# Non-affine uniform templates and the T-weighted interior problem

**Date:** 2026-09-04  
**Question:** Can a primitive uniform morphism with \(\mathrm{rank}\,K_T\ge 3\) (especially non-affine \(T\)) keep T-weighted consecutive \(n\)-factors outside the finite defect pair set \(E_\sigma\)?  
**Infinite ACF problem over \(\{0,1,2,3\}\):** still **OPEN**.  
**Rank:** LEVEL 4 for length rigidity, automaticity, and the Krylov-escape countertheorem; LEVEL 2 for sampled \(m\ge 3\). Not Level 5.

No finite-state cube graph is constructed. The previous missions already showed that the missing information is an interior T-weighted window of length \(\Theta(d/m)\).

---

## 1. EXECUTIVE VERDICT

The CCSS avoidance mechanism is a **length-and-Parikh template**. For the Cassaigne morphism \(\varphi\) one has the exact identity

\[
|\varphi(w)|=2|w|-|w|_3.
\]

Equal image length is therefore a Parikh constraint, and ancestor lengths of equal-length image factors may differ by \(\Theta(|w|)\). A primitive **uniform** morphism has instead

\[
|\sigma(w)|=m|w|
\]

for every finite \(w\). Equal complete-image length forces equal ancestor length, and three consecutive length-\(d\) factors in \(\sigma(U)\) have complete-block counts differing by at most \(1\) (and by **exactly \(0\)** when \(m=2\)).

That is the first algebraic distinction between

1. arbitrary primitive morphisms, and
2. primitive uniform morphisms.

It is not a slogan. It is equivalent to: the incidence matrix has all column sums equal to the integer \(m\); the fixed point is \(m\)-automatic; letter frequencies are rational.

The Cassaigne word itself **cannot** be realized as a coding of any constant-length morphism: \(\varphi\) is primitive, its characteristic polynomial \(x^4-x^3-2x^2+2x-1\) is irreducible over \(\mathbb{Q}\), the Perron eigenvalue lies in \((1,2)\) hence is irrational, and letter frequencies are therefore irrational. Automatic sequences with existing letter frequencies have rational frequencies (Allouche–Shallit; Allouche–Shallit–Zamboni Prop. 5.3). Padding \(\varphi(3)=1\) to length 2 produces a 2-uniform morphism on \(\{0,1,3,4\}\), all of which have cubes (exhaustive; this run: best padded ACF prefix \(48\)).

The proposed **Krylov-escape** obstruction is **false**. On the famous 2-uniform witness, cubes satisfy \(T\cdot v\in[-4,3]\) (inside the finite defect window) while the extra coordinate \(T_2\cdot v\) ranges over \([-10,8]\) on the 442 cubes of the length-2048 iterate, with \(\|v\|_1\) up to \(18\). Higher Krylov coordinates grow **inside** the T-window; they do not eject from it.

Bounded additive complexity of the T-word forces cubes (Ardal et al. 2012, Thm. 5). Unbounded additive complexity does not prevent them (famous) and does not create them (Cassaigne). Sublinear / linear / eventually affine \(p_T\), balancedness, abelian complexity, and recurrence do not by themselves force \(S_X=S_Y=S_Z\) except through the bounded-\(p_T\) case or through 2-letter Dekking.

The strongest exact statement covering the non-affine uniform regime is an **interior reduction**, not a force-cubes theorem:

> A primitive \(m\)-uniform fixed point contains an additive cube if and only if its T-coded automatic word has three consecutive \(n\)-factors whose sum differences lie in a finite coupled set \(E_\sigma\), with \(n=\lfloor d/m\rfloor+O(1)\).

Whether every such automatic word hits \(E_\sigma\) remains open for \(m\ge 3\). For \(m=2\) on four letters it is computationally true (exhaustive, cap 200; true longest ACF prefix 119).

**ENDGAME C.**

---

## 2. WEIGHTED ADDITIVE-COMPLEXITY RESULT

Write \(a_i=T(U[i])\), a numerical word over the finite set \(\{T(0),T(1),T(2),T(3)\}\). Define

\[
p_T(n)=\bigl|\{\,T\cdot\mathrm{Parikh}(V): V\text{ an }n\text{-factor of }U\,\}\bigr|.
\]

| Regime | What it implies for consecutive triples |
|---|---|
| bounded \(p_T(n)\) | Additive \(k\)-powers for **all** \(k\) (Ardal–Brown–Jungić–Sahasrabudhe, Integers 12 (2012), Thm. 5). In particular aligned cubes in \(W=\sigma(U)\). Sufficient, not necessary. |
| Banero anchors | A morphism sends **every** word to bounded \(p_T\) iff all image slopes \(T(a)/|\sigma(a)|\) coincide. For uniform \(\sigma\) this is \(n_{\mathrm{sums}}=1\). Controls images of arbitrary words, not a single fixed point. |
| sublinear / linear / eventually affine \(p_T\) | No theorem found that forces three consecutive equal sums. Morphic words typically have linear factor complexity; that is orthogonal. |
| balancedness | \(C\)-balanced T-weights make \(p_T\) bounded, hence reduce to Ardal. Binary balanced words have cubes by Dekking. |
| unbounded \(p_T\) | Necessary for ACF, far from sufficient. Famous: \(p_T(1..24)\) reaches 30 on a 256-letter nested prefix, and the word has cubes. Cassaigne: \(p_{\mathrm{add}}(1..24)\) reaches 32 on an 800-letter prefix, and the word is ACF. |
| abelian complexity / recurrence | Uniform recurrence plus unbounded abelian complexity is the Cassaigne setting. Recurrence produces arbitrarily long factors, not three consecutive equal T-weights. |

Famous T-word of the nested prefix \(W[:256]\): first additive cube at \(i=90,\,d=50,\,\mathrm{sum}=145\); true ACF prefix of \(T\circ W[:256]\) is **164**, longer than the letter-word ACF prefix 119. Non-affine recoding neither preserves nor is implied by letter-cubes. The T-word of the **covering** ancestor of the first W-cube (\(|U|=60\), itself ACF) is ACF: the first W-cube is unaligned and uses defects, not an exact T-cube of that short ancestor.

**Do not overclaim:** unbounded \(p_T\) is not an obstruction and not a construction.

---

## 3. KRYLOV / SPECTRAL ANALYSIS

Let \(T_{k+1}=M^\top T_k\) with \(T_1=T\), and \(K_T=\mathrm{span}\{1,T,T_2,\ldots\}\).

On a cube, \(1\cdot v=\Delta n\in\{-1,0,1\}\) and \(T\cdot v=-\Delta\delta\) is bounded. If \(T_2=a\cdot 1+b\cdot T+R\) with \(R\neq 0\), then on \(L=\ker(1)\cap\ker(T)\) one has \(T_2\cdot v=R\cdot v\). Mass in the complementary direction of \(L\) becomes a growing \(T_2\)-coordinate **without leaving the T-window**.

Famous witness: \(T=(5,4,2,1)\), \(T_2=(3,5,7,9)\), \(\dim K_T=3\). On 442 cubes of the length-2048 iterate (CSV, dual C++/Python):

```
T·v  ∈ [-4, 3]     (165 of them T·v=0)
T_2·v ∈ [-10, 8]
‖v‖_1 ∈ [0, 18]
```

First cube: \(v=(1,-1,1,-1)\), \(T\cdot v=2\), \(T_2\cdot v=-4\). Cubes occur **with** a nonzero extra Krylov coordinate. The target statement “a nonzero expanding Krylov component forces T-weight out of every bounded defect window after enough substitutions” confuses growth of a coordinate with impossibility of cancellation. Cancellation in the T-direction is the definition of \(L\), and it is realized.

2-uniform census (matrix only, previous mission, C++ matches Python): \(\mathrm{krylov}_T\) dimensions \(452/9096/36012/19976\) for ranks \(1/2/3/4\). Generic is rank 4, famous is rank 3. Rank \(\ge 3\) is not rare and is not a cube-forcing spectral gap.

Perron eigenvalue of a uniform morphism is the integer \(m\). Transverse eigenvalues may expand (\(|\lambda_2|=\sqrt{2}\) on famous). Expansion of Parikh discrepancy under \(M\) inflates \(\|v\|_1\) at the next scale; the T-form of that inflated vector is \(T_2\cdot v_{\mathrm{old}}\), i.e. the next Krylov coordinate, not a contradiction to a cube at the old scale. Theorem J already says the first cube does not desubstitute.

---

## 4. CCSS TEMPLATE COMPARISON

CCSS (JACM 2014): \(\varphi(0)=03\), \(\varphi(1)=43\), \(\varphi(3)=1\), \(\varphi(4)=01\), fixed point over \(\{0,1,3,4\}\) is ACF.

**What allows \(\mathrm{wt}(X)=\mathrm{wt}(Y)=\mathrm{wt}(Z)\) to be avoided.**  
The proof is a finite ancestor-graph / template search: along a hypothetical path to a cube the Parikh differences \(u=\psi(Y)-\psi(X)\), \(v=\psi(Z)-\psi(Y)\) remain bounded because two expanding eigencoordinates are controlled by the lattice \(L(B)\) and the contracting form \(\tau\) (Liétard–Rosenfeld Lemma 3.3). The morphism is **not affine** and **not uniform**. Image length depends on the letter \(3\).

Exact length algebra:

```
|φ(w)| = 2|w| − |w|_3
|φ(X)| = |φ(Y)|  ⟺  2(|X|−|Y|) = |X|_3 − |Y|_3
```

so \(|X|-|Y|\) may be \(\Theta(|X|)\). Concrete: \(|\varphi(00)|=|\varphi(330)|=4\) with ancestor lengths \(2\neq 3\).

\(\varphi^2\) has lengths \((3,3,2,4)\); \(\varphi^3\) has \((5,6,3,6)\). Powers never uniformize. Iterate lengths \(|\varphi^k(0)|=1,2,3,5,8,14,23,40,66,\ldots\) (not \(2^k\); the 6th term is \(14\), not Fibonacci \(13\)).

**A.** The length-compensation mechanism fundamentally requires non-uniform image lengths. For uniform \(\sigma\), \(|\sigma(X)|=|\sigma(Y)|\) iff \(|X|=|Y|\) on complete images. The only remaining slack is an \(O(1)\) boundary, i.e. \(E_\sigma\).

**B.** Uniformization by coding / higher-block / return words produces a **different** word, or fails to be uniform. Higher 2-blocks of \(\varphi\) have image lengths in \(\{2,3,4\}\). Return words of the Cassaigne prefix of length 800 to a single letter already use lengths \(\{1,2,3,4,5,6,7,9,14,17,26\}\).

**C.** Uniformity does not make weighted discrepancies “scale differently” in a way that by itself forces cubes. It removes one degree of freedom (ancestor length) from the CCSS template.

**D.** The invariant is not “uniformity” as a label. It is **integer Perron eigenvalue / automaticity / \(|\sigma(w)|=m|w|\)**, which are equivalent for primitive constant-length substitutions.

Affine uniform morphisms are the CMRS / Andrade–Mol class: length and sum of images are affine in the letter. Templates with two coordinates (length, sum) then close. Famous \(T=(5,4,2,1)\) is not affine to \(A=(0,1,2,3)\). That is the unresolved regime, and CCSS does not cover it because CCSS is not uniform.

---

## 5. RETURN-WORD ANALYSIS

For a primitive uniformly recurrent word, each factor \(Y\) has finitely many return words. On the famous length-1024 iterate, return words to a single letter have **multiple slopes**: letter \(2\) has 10 types and 10 distinct \((\mathrm{length},\mathrm{sum})\) pairs. Cassaigne letter \(0\) has 4 types, 4 slopes, lengths \(\{3,5,6\}\).

So \(\mathrm{wt}(R)\) is not a function of \(|R|\). Concatenations of return words of total length \(n\) have T-weight in an \(n\)-dependent subset of the semigroup generated by finitely many pairs \((|R_i|,\mathrm{wt}(R_i))\). That is a linear Diophantine problem **whose number of summands grows with \(n\)**. It is not a finite system independent of \(n\).

Potential theorem that survives as exact:

> For each fixed factor \(Y\), the set of return-word T-weights is finite. Equal-length predecessor/successor windows are concatenations of those return words (plus a bounded prefix/suffix of a return word). The defect equations become a bounded-length subset-sum in those weights, with a length constraint.

This is true and not a decision procedure at unbounded \(n\). It does not force the defect window to be hit (Cassaigne: finite return words, no cubes). It does not prevent it (famous). Raw factor recurrence is not weaker in any way that this run could make exact.

Semilinearity of \(R_n=\{\mathrm{Parikh}(V)-\mathrm{Parikh}(W): |V|=|W|=n\}\) as a family in \(n\) is **not** claimed. Finite computation of \(p_T(n)\) is not semilinearity.

---

## 6. UNIFORMIZATION TEST

| Operation | Uniform? | Same word as Cassaigne? | ACF? |
|---|---|---|---|
| \(\varphi\) itself | no | yes | yes (CCSS) |
| \(\varphi^k\) | no (\(\varphi^2\) lengths \(3,3,2,4\)) | yes | yes |
| letter-to-letter coding of a \(k\)-uniform morphism | yes | **no** (Thm. X) | then \(k\)-automatic, open in general |
| pad \(\varphi(3)=1\) to length 2 (8 pads \(\times\) 4 seeds) | yes | no | **no**; best ACF prefix 48 (dual C++/Python); contained in the exhaustive 2-uniform scan on \(B\) |
| 2-block coding of \(\varphi\) | no | recoding of factors | — |
| return-word coding | typically no | conjugate | Cassaigne-derived, still variable length |
| random 2-uniform on 5, 6, 8 letters coded onto \(B\) | yes | no | all 40+40+40 samples cubed by length 27, cap 160 |

**Claim E, corrected.** Uniformization of the Cassaigne *word* is impossible. Uniformization of the Cassaigne *mechanism* (length compensation) is impossible for constant-length substitutions. Existence of some other uniform ACF morphism, on some alphabet, is the original problem restricted to automatic sequences.

---

## 7. STRONGEST THEOREM OBTAINED

**Theorem V (length rigidity).**  
Let \(\sigma\) be \(m\)-uniform. Then \(|\sigma(w)|=m|w|\) for every finite word \(w\). For three consecutive intervals of length \(d=km+s\) starting at offset \(r\), the complete-block counts \(n_X,n_Y,n_Z\) are functions of \((m,r,s,k)\) only, and \(\lvert n_X-n_Y\rvert\le 1\), \(\lvert n_Y-n_Z\rvert\le 1\). Dual C++/Python for \(m=2,\ldots,7\).

**Theorem V′ (\(m=2\) interiors equal).**  
If \(m=2\), then \(n_X=n_Y=n_Z\) for every residue type and every \(k\ge 1\). The interior problem is exactly three consecutive \(n\)-factors of the ancestor.

**Theorem W (interior \(E_\sigma\)).**  
A cube in \(W=\sigma(U)\) is equivalent to

\[
\bigl(T\cdot\psi_X-T\cdot\psi_Y,\; T\cdot\psi_Y-T\cdot\psi_Z\bigr)
=(\delta_Y-\delta_X,\;\delta_Z-\delta_Y)\in E_\sigma,
\]

where \(E_\sigma\) is finite and obtained by enumerating at most four shared fragment letters. For the famous morphism, \(|E_{\mathrm{coupled}}|=59\), while the independent-letter product has 121 pairs. The aligned type \((r,s)=(0,0)\) has \(E=\{(0,0)\}\): aligned cubes iff the T-word has an additive cube. (Cites Theorems K, L, U3.)

**Theorem X (Cassaigne is not automatic).**  
\(\varphi\) is primitive (\(M^6>0\)). Column sums are \(2,2,1,2\), so the Perron eigenvalue \(\lambda\) satisfies \(1\le\lambda\le 2\). The characteristic polynomial is \(x^4-x^3-2x^2+2x-1\), with no integer roots in \(\{1,2\}\) and no rational roots, and it does not factor into integer quadratics; \(\lambda\notin\mathbb{Z}\) is an irrational algebraic integer. Primitive morphic letter frequencies are the normalised PF eigenvector, hence not all rational. Therefore the Cassaigne word is not \(k\)-automatic for any \(k\ge 2\).

**Theorem Y (same-word uniformization).**  
A coding of a \(k\)-uniform morphism is \(k\)-automatic. By Theorem X the Cassaigne word is not such a coding.

**Theorem Z (Krylov escape is false).**  
There exist cubes whose ancestor Parikh difference has a nonzero component in a direction with \(T_2\cdot v\neq 0\), while \(T\cdot v\) remains in the finite defect window. Famous first cube and the 442-cube CSV are witnesses. The extra Krylov coordinate is not an obstruction.

**Covering reduction.**  
If \(\sigma\) is primitive \(m\)-uniform and prolongable, \(W=\sigma^\omega(a)\) is \(m\)-automatic, and \(W\) is ACF iff the automatic numerical word \(\tau_T(W)\) avoids \(E_\sigma\)-cubes at every scale. For \(n_{\mathrm{sums}}\le 2\) this is impossible (Theorem C). For Pisot-uniform it is impossible (Theorem H). For bounded \(p_T\) it is impossible (Ardal). The remainder is exactly non-Pisot, \(n_{\mathrm{sums}}\ge 3\), unbounded-\(p_T\) automatic words — the famous class.

No theorem of the form “\(\mathrm{rank}\,K_T\ge 3\) plus non-affine \(T\) forces cubes” is obtained. The hypotheses that would make a force-cubes theorem true at this writing are the already-known ones (two sums, Pisot, bounded \(p_T\)).

---

## 8. SMALLEST COUNTEREXAMPLES

| Claim | Status | Smallest witness / correction |
|---|---|---|
| A. Every primitive uniform numerical word has additive cubes | **OPEN** as a theorem. True for 2-letter alphabets (Dekking). Computationally true for all 2-uniform maps on 4 letters (cap 200; true best ACF prefix 119). | Not killed: Cassaigne is not uniform. |
| B. \(\mathrm{rank}\,K_T\ge 3\) forces cubes | **OPEN.** Famous has rank 3 **and** cubes, so it is not a counterexample. The claim is not proved. | Naive “higher rank dies sooner” is false as a 2-uniform heuristic: rank-1 maps die at \(d\le 2\); famous (rank 3) is the longest 2-uniform ACF prefix. |
| C. Non-affine \(T\) forces cubes | **OPEN.** Affine 2-uniform maps also all fail. | Affine \(\neq\) safe. Non-affine \(\neq\) doomed by a theorem here. |
| D. Cube below a bound depending only on \(m\) | Computationally true for \(m=2\) with bound 120. **Not proved.** \(m\ge 3\) unexhausted (\(4^{12}\) for \(m=3\)). | Sample of 40 primitive maps for each \(m=3,\ldots,8\), cap 180: all cubed; max ACF prefix 47. Not a bound theorem. |
| E. Uniformization of a primitive ACF morphism is impossible | **FALSE** as stated. **TRUE** for the Cassaigne *word* (Thm. Y). | Smallest uniformization that produces a different word and cubes: pad \(\varphi(3)\) on the right by \(3\), seed \(3\), first cube \(i=0,d=24,\mathrm{sum}=55\), ACF prefix 48. |
| F. Bounded-defect avoidance requires non-uniform image lengths | **FALSE** as a universal statement (that would solve the problem). **TRUE** for the CCSS length-compensation mechanism (Thm. V + Cassaigne length formula). | Equal-image unequal-ancestor: \(00\) vs \(330\), both image length 4. Impossible for any uniform \(\sigma\). |
| Krylov escape forces the T-window to miss | **FALSE.** | Famous \(v=(1,-1,1,-1)\), \(T\cdot v=2\), \(T_2\cdot v=-4\). |
| CSV `acf_prefix` / lex-first `d` measures survival | **FALSE.** | Code 7754, seed 2: lex-first cube \(d=65\) in a length-200 word, true ACF prefix **8**. Famous remains the 2-uniform record. |

---

## 9. COMPUTATIONAL CERTIFICATES

All arithmetic is integer. Dual C++ (`bin/template_scan`) and Python (`python/template_verify.py`, `python/template_core.py`).

| Claim | Exhaustive / sampled | Independent check |
|---|---|---|
| \(\max\|\Delta n\|\le 1\) for \(m=2..7\) | exhaustive residue types, \(k=6\) | C++ `DELTA_N` matches Python |
| \(m=2\Rightarrow\Delta n=0\) | exhaustive \((r,s)\), \(k=1..5\) | both |
| Cassaigne \(\lvert\varphi(w)\rvert=2\lvert w\rvert-\lvert w\rvert_3\) | 200 random words + 6 hand samples | Python; C++ uses incidence recurrence for \(\lvert\varphi^k(0)\rvert\) |
| \(\lvert\varphi^k(0)\rvert=1,2,3,5,8,14,23,40,66\) | exact \(M^\top\) recurrence | C++ `PHI_LEN_0` = Python |
| charpoly \(x^4-x^3-2x^2+2x-1\) | \(\det(xI-M)\) at \(x=-1..4\) | Python `det4` in `template_verify` (does not import interpolation) |
| primitive at \(k=6\) | \(M^6>0\) | Python |
| famous cube \((45,25,35)\); covering ancestor ACF \(\lvert U\rvert=60\); nested \(U=W[:256]\) has the same cube | exact | C++/Python |
| \(T\circ W[:256]\) cube \((90,50,145)\); \(p_T(1..16)\) | exact prefix sums | C++ `PT` = Python |
| Cassaigne prefix 800 ACF; \(p_{\mathrm{add}}(1..16)\) | exact | C++ `CASSAIGNE_CUBE NO`, `CASSAIGNE_PADD` |
| 8 pads of Cassaigne, 32 (pad,seed) pairs, cap 200 | exhaustive on that finite set | C++ `PAD_BEST 48` = Python 48 |
| famous return-to-2: 10 slopes | prefix 512 | C++ `RET2_SLOPES 10` |
| \(E_\sigma\) famous: coupled 59, pairwise 121, aligned \(\{(0,0)\}\) | exhaustive 4-letter fragment tuples | Python `template_core` |
| T2 on 442 cubes | CSV `data/famous_all_cubes.csv` | previous dual + this run |
| 2-uniform first cubes | exhaustive 65536, previous mission | do **not** read CSV `acf_prefix` as true ACF length |
| primitive uniform \(m=3..8\) | **sampled** 40 per \(m\), cap 180 | Python only; all cubed |
| larger automatic kernels onto \(B\) | **sampled** 40 per size 5,6,8, cap 160 | Python only; all cubed |

Reproduce:

```
make test
python3 python/template_core.py
./bin/template_scan
python3 python/template_verify.py
```

No SAT. No long-word search.

---

## 10. LITERATURE / NOVELTY AUDIT

| Statement | Classification |
|---|---|
| CCSS ACF over \(\{0,1,3,4\}\) | KNOWN (JACM 2014) |
| Liétard–Rosenfeld \(L(A)\) vs \(L(B)\); AP vector \((1,-2,1,0)\) | KNOWN (not re-proved as new) |
| Ardal et al. bounded additive complexity \(\Rightarrow\) additive \(k\)-powers | KNOWN (Integers 2012, Thm. 5) |
| Banero anchors / equal slopes | KNOWN (JIS 2013) |
| CMRS / Andrade–Mol affine length-and-sum templates | KNOWN |
| Dekking: abelian cubes unavoidable over 2 letters | KNOWN |
| Pisot-uniform \(\Rightarrow\) bounded discrepancy \(\Rightarrow\) cubes | KNOWN packaged as Theorem H / U2 |
| Automatic sequences: existing letter frequencies are rational | KNOWN (Allouche–Shallit; Allouche–Shallit–Zamboni “Hidden automatic sequences” Prop. 5.3) |
| Primitive + irrational PF eigenvalue \(\Rightarrow\) not automatic | KNOWN (same Prop. 5.3); **applied** to CCSS here |
| \(\lvert\sigma(w)\rvert=m\lvert w\rvert\) for uniform \(\sigma\) | ELEMENTARY |
| Theorems V, V′, W (interior \(E_\sigma\)) | ELEMENTARY / REPRODUCED from K, L, U3 |
| Theorem X (Cassaigne not automatic) | ELEMENTARY/LIKELY KNOWN as a remark; not located as a numbered theorem about CCSS in the papers searched |
| Theorem Y (same-word uniformization impossible) | ELEMENTARY from X |
| Theorem Z (Krylov escape false) | UNVERIFIED NOVELTY as a packaged countertheorem; the numbers are REPRODUCED from the previous mission’s CSV |
| Covering reduction to automatic \(E_\sigma\)-cubes | UNVERIFIED NOVELTY as packaging; not a solution |
| “Every 2-uniform 4-letter map has a cube by length 120” | REPRODUCTION of Liétard / this project’s Theorem F |
| Sampled \(m=3..8\) all fail | LEVEL 2, not a theorem |

Searched: CCSS; Rao 2015; Liétard–Rosenfeld 2020; Ardal 2012; Banero 2013; CMRS / Andrade–Mol; Allouche–Shallit automatic sequences; Allouche–Shallit–Zamboni hidden automatic sequences; Cobham; abelian / \(k\)-abelian complexity; substitution cocycles / Parikh discrepancy. No source was found that constructs a uniform ACF word over a 4-letter integer alphabet, or that proves none exist.

---

## 11. WHAT THIS DOES AND DOES NOT PROVE

**Proves**

- Uniformity kills the CCSS length-compensation degree of freedom, by exact algebra, not by labelling.
- The Cassaigne word is not automatic, hence not a uniform morphic coding.
- Padding / \(\varphi^k\) / 2-block induced maps do not uniformize that word.
- The interior of a uniform cube is an \(E_\sigma\)-cube of an automatic T-word, with \(\lvert\Delta n\rvert\le 1\).
- Krylov expansion does not yield a T-window obstruction.
- Bounded \(p_T\) is the only additive-complexity regime that forces cubes; it is already known and is not the famous/Cassaigne regime.

**Does not prove**

- That every primitive uniform morphism over \(\{0,1,2,3\}\) has a cube.
- That \(\mathrm{rank}\,K_T\ge 3\) or non-affine \(T\) forces cubes.
- That an automatic ACF word over \(\{0,1,3,4\}\) cannot exist (only that it cannot be the Cassaigne word, and that 2-uniform 4-letter maps fail).
- A bound on first-cube length depending only on \(m\) for \(m\ge 3\).
- Semilinearity of \(R_n\), or a finite return-word Diophantine obstruction.

The original infinite problem is open. The uniform non-affine subclass is open except for the already-excluded two-sum, Pisot, and bounded-\(p_T\) strata, and except for exhaustive \(m=2\) on four letters.

---

## 12. SINGLE NEXT BOTTLENECK

Decide whether **\(m\)-automatic** numerical words over a 4-letter integer alphabet can avoid \(E_\sigma\)-cubes.

Equivalently: is there a \(k\)-automatic additive-cube-free word over \(\{0,1,2,3\}\) or over \(\{0,1,3,4\}\)?

This is strictly smaller than the morphic problem (Cassaigne is morphic and not automatic). It is exactly the uniform problem. Affine automatic maps are in the CMRS template class. The remaining objects are non-affine, non-Pisot, \(n_{\mathrm{sums}}\ge 3\) automatic words — famous-shaped, with an unbounded interior T-window.

Do not build a factor graph. Do not search longer finite ACF words. The decision is automatic vs morphic additive-cube avoidance.

---

ENDGAME C
