# Proofs

Status tags: **PROVED** / **COMPUTATIONALLY VERIFIED** / **STRONGLY CONJECTURED**.  
Nothing in this file is claimed as a solution of the infinite problem.

---

## Theorem A — Eventually periodic words are not ACF
**Status: PROVED** (elementary; already known; recorded for completeness)

Let \(w\) be an infinite eventually periodic word over any alphabet. Then \(w = vu^\omega\) for finite \(v,u\) with \(u\neq\varepsilon\). The factor \(uuu\) of \(w\) is an ordinary cube, hence an additive cube (equal lengths, equal sums).

**Corollary.** There is no purely periodic or eventually periodic infinite additive-cube-free word over \(\{0,1,2,3\}\). Every infinite candidate must be aperiodic.

Computational check (`acf_tool periodic 40`): every tested \(u^3\) contains an additive cube, as the proof requires.

---

## Theorem B — Coincidence with ordinary cubes for \(n<6\)
**Status: PROVED**

If \(n<6\), the only possible block length is \(d=1\) (since \(3d\le n\)). An additive cube of block length 1 is three equal consecutive letters \(aaa\). Thus a word of length \(<6\) is additive-cube-free iff it is ordinary-cube-free.

**Corollary.** The counting sequence agrees with OEIS A051043 for \(0\le n\le 5\):
\[
1,4,16,60,228,864.
\]
They first differ at \(n=6\): ACF \(=3152\), ordinary cubefree \(=3264\) (A051043). The 112 extra ordinary-cubefree words of length 6 contain an additive cube of block length \(d=2\).

---

## Theorem C — Aligned cubes from two image-sums
**Status: PROVED** (elementary combination of Dekking’s theorem)

Let \(h\) be a \(k\)-uniform morphism on a finite integer alphabet, and suppose the set
\[
\Sigma = \bigl\{\textstyle\sum h(a) : a\in A\bigr\}
\]
has cardinality at most 2. Let \(w\) be any infinite iterate of \(h\) (e.g. \(h^\omega(a)\) if defined, or the limit of \(h^n(s)\) for a finite seed \(s\) that expands). Write \(w = b_0 b_1 b_2\cdots\) as a concatenation of length-\(k\) blocks, each \(b_i = h(a_i)\). The sequence of block sums \(\sigma_i = \sum b_i\) is an infinite word over the 2-element set \(\Sigma\subset\mathbb{Z}\).

Over a 2-letter integer alphabet, additive powers coincide with abelian powers. Infinite abelian-cube-free words require 3 letters (Dekking 1979). Hence \((\sigma_i)\) contains an abelian/additive cube \(\sigma_j=\sigma_{j+1}=\sigma_{j+2}\). The corresponding three consecutive blocks of \(w\) form an additive cube of block length \(k\).

**Consequence for this alphabet.** Any uniform morphism over \(\{0,1,2,3\}\) with at most two distinct image sums is eliminated as an infinite ACF construction.

---

## Theorem D — Unique vote, and no right-crucial word of length `≤10`
**Status: PROVED** (elementary). Novelty: UNVERIFIED NOVELTY (not located in the literature; may be unwritten folklore).

When appending a letter `x` to an ACF word of length `n`, each block length `d` with `3d≤n+1` forbids at most one value of `x` (the unique solution of `s_3=s_1` when `s_1=s_2`). Over a 4-letter alphabet a right-crucial word therefore requires `⌊(n+1)/3⌋≥4`, i.e. `n≥11`.

This holds for every 4-letter integer alphabet, including `{0,1,3,4}`. It does **not** explain exceptionality of `{0,1,2,3}`.

Independent check: `python/independent_checks.py` compares unique-vote forbidden sets to brute-force extensions on all ACF words of length `≤6`.

## Theorem D′ — First covering is at length 14 over `{0,1,2,3}`
**Status: COMPUTATIONALLY VERIFIED.** Novelty: UNVERIFIED NOVELTY.

There are no right-crucial ACF words of length `11,12,13`. At those lengths, every 4-active word has vote tuple in a fixed list of 12, none of which covers `{0,1,2,3}`. Length 14 has **8170** right-crucial words.

The same first-crucial length 14 occurs over `{0,1,3,4}` (2656 dead-ends). See `STRUCTURAL_REPORT.md`.

**Supersedes** the earlier false statement “no dead-end of length `≤14`” (off-by-one in a DFS that did not test extensions at the cap).

## Theorem E — Exact enumeration through length 12
**Status: COMPUTATIONALLY VERIFIED, REPRODUCTION of known values `c_8…c_12`**

Let \(c_n\) be the number of ACF words of length \(n\) over \(\{0,1,2,3\}\):

```
n  c_n
0  1
1  4
2  16
3  60
4  228
5  864
6  3152
7  11468
8  42070
9  150560
10 538214
11 1924738
12 6772220
```

The values \(c_8=42070,\ldots,c_{12}=6772220\) are already public and are reproduced here.
Also computed: \(c_{13}=23818052\), \(c_{14}=83821562\) (UNVERIFIED NOVELTY as counts).

Empirical branching \(c_{n+1}/c_n \approx 3.52\) at \(n=11\). Letter frequencies under the uniform measure on length-12 ACF words: \((0.251,0.249,0.249,0.251)\).

---

## Theorem F — Classification of 2-uniform morphisms (partial)
**Status: COMPUTATIONALLY VERIFIED**, with a residual caveat stated below

Every 2-uniform morphism \(h:\{0,1,2,3\}\to\{0,1,2,3\}^2\) that is prolongable on at least one letter \(a\) (i.e. \(h(a)\) begins with \(a\)) produces an additive cube in the prefix of \(h^\omega(a)\) of length \(\le 120\). Exhaustive scan of all \(4^8=65536\) morphisms; 0 survivors to length 800 among prolongable seeds. Best prefix: \(0\mapsto 32,\;1\mapsto 31,\;2\mapsto 20,\;3\mapsto 01\), seed 2, first cube at length 120 with \((i,d,\mathrm{sum})=(45,25,35)\), **unaligned**. Image sums \((5,4,2,1)\) are four distinct values; eigenvalues \(\{2,\sqrt{2},-\sqrt{2},0\}\).

This is a reproduction of Liétard’s size-2 failure with alignment forensics, not a new non-existence theorem.

Combined with Theorem C, morphisms with \(\le 2\) image sums are independently excluded for infinite iterates.

## Theorem H — Pisot-uniform morphisms are not ACF
**Status: PROVED** (combination of Perron-Frobenius + Ardal et al. 2012). Novelty: UNVERIFIED NOVELTY as a packaged statement.

Let \(\sigma\) be a primitive \(m\)-uniform morphism over a finite subset of \(\mathbb{Z}\) whose incidence matrix has every eigenvalue other than \(m\) of modulus \(<1\). Then any infinite fixed point has bounded prefix-sum discrepancy, hence bounded additive complexity, hence additive \(k\)-powers for all \(k\) (Ardal–Brown–Jungić–Sahasrabudhe, Integers 12 (2012), Thm. 5). This holds over \(\{0,1,2,3\}\) and over \(\{0,1,3,4\}\).

The Cassaigne morphism evades the theorem: it is not uniform and has two expanding eigenvalues.

## Theorem I — Cassaigne lattice distinction
**Status: COMPUTATIONALLY VERIFIED** against the published \(\tau\) and \(C\); mechanism **KNOWN** (Liétard–Rosenfeld Lemma 3.3 / Thm 4.1).

See `UNIFORM_MORPHIC_REPORT.md` Theorem U4. The AP-only lattice vector \((1,-2,1,0)\) (the relation \(0+2=1+1\)) has \(|\tau\cdot x|\approx 1.22<C\) and does not lie in \(L(\{0,1,3,4\})\).

---

## Theorem J — First cubes of prolongable uniform words are not letter-desubstitutions
**Status: PROVED.** Novelty: UNVERIFIED NOVELTY (elementary).

Let \(\sigma\) be \(m\)-uniform, \(m\ge 2\), prolongable at \(a\), and \(W=\sigma^\omega(a)\). Let \(XYZ\) be an additive cube of \(W\) of minimal endpoint \(N\). The ancestor prefix \(U=\sigma^n(a)\) with \(m^n<N\) is a prefix of the ACF word \(W[:N-1]\), hence ACF. So \(XYZ\) is not \(\sigma\) of an additive cube of \(U\).

See `DESUBSTITUTION_REPORT.md`.

## Theorem K — Unaligned identity
**Status: PROVED.** Novelty: UNVERIFIED NOVELTY as a packaged additive-cube statement.

For \(W=\sigma(U)\) with \(\sigma\) \(m\)-uniform and \(T(a)=\sum\sigma(a)\),

\[
\sum W[p,p+d) = T\cdot\psi + \delta
\]

where \(\psi\) is the Parikh vector of complete ancestor letters and \(\delta\) is the sum of at most two proper image fragments. An additive cube is equivalent to \(T\cdot(\psi_X-\psi_Y)+(\delta_X-\delta_Y)=0\) and the companion \(Y,Z\) equation. Dual C++/Python verification on the witness cube, on all 442 cubes of the length-2048 iterate, and on 65536+65536 two-uniform first cubes over \(\{0,1,2,3\}\) and \(\{0,1,3,4\}\).

## Theorem L — Finite defects, affine ancestor lattice
**Status: PROVED.**

For fixed \(\sigma\) the possible \(\delta\) form a finite set. Hence \(v=\psi_X-\psi_Y\) lies in one of finitely many affine lattices \(\{x:1\cdot x=\Delta n,\; T\cdot x=-\Delta\delta\}\). Vanishing defect and \(\Delta n=0\) give \(\ker(1)\cap\ker(T)\), the Cassaigne lattice of the *image-sum* alphabet, not of the letter alphabet.

The universal claims “\(v\in L(A)\)” and “every large cube desubstitutes to an ancestor letter-sum cube” are **false**. Counterexamples: exhaustive 2-uniform `d=3`, `v=(-1,0,0,1)`; witness `i=45`, `d=25`, `v=(1,-1,1,-1)`.

## Theorem M — \(\mathbb{Z}^4/\ker(T)\) is infinite
**Status: PROVED.** Novelty: ELEMENTARY/LIKELY KNOWN (first isomorphism theorem).

Let \(T\in\mathbb{Z}^4\) be nonzero and \(d=\gcd(T)\). Then \(\varphi_T(x)=T\cdot x\) has kernel \(\ker(T)\) and image \(d\mathbb{Z}\), so
\[
\mathbb{Z}^4/\ker(T)\;\cong\; d\mathbb{Z}\;\cong\;\mathbb{Z}.
\]
The only information surviving in this quotient is the integer \(T\cdot v\). Cubes occupy finitely many classes therein (Theorem L), but the quotient group is not a finite-state space. For the witness \(T=(5,4,2,1)\), \(d=1\).

## Theorem N — \(M\) preserves \(L\) iff \(T_2\in\mathrm{span}\{1,T\}\)
**Status: PROVED.** Novelty: UNVERIFIED NOVELTY as packaged for this defect equation; the linear algebra is elementary.

Write \(T_2:=M^\top T\). Always \(1^\top M=m\,1^\top\), so \(M\) preserves \(\ker(1)\). For \(k\in L:=\ker(1)\cap\ker(T)\),
\[
T\cdot(Mk)=T_2\cdot k.
\]
This vanishes for all \(k\in L\) iff \(T_2\in\mathrm{span}\{1,T\}\). Equivalently, \(\dim\mathrm{span}\{1,T,T_2,\ldots\}\le 2\). If moreover \(T_2\in\mathrm{span}\{T\}\), then \(M\) preserves \(\ker(T)\) itself and acts on \(\mathbb{Z}^4/\ker(T)\).

On the witness, \(T_2=(3,5,7,9)\). The \(3\times 3\) minors of the matrix with rows \(1,T,T_2\) are \((2,2,-2,-2)\), so the rank is 3 and \(L\) is not invariant. Parametrically \(L=\{(x,y,-4x-3y,3x+2y)\}\), with \(T_2\cdot e_i=2\neq 0\) on the complementary direction to \(\ker(M)\cap L=\langle(1,-1,-1,1)\rangle\).

## Theorem O — Krylov closure is the maximal invariant linear package
**Status: PROVED.** Novelty: UNVERIFIED NOVELTY as applied to additive-cube defects.

The linear forms appearing as interior sums under iterated desubstitution are \(T_k=(M^\top)^k A\) with \(T_1=T=M^\top A\). The smallest \(M^\top\)-invariant space containing \(T\) and \(1\) is
\[
K_T=\mathrm{span}\{1,T,T_2,T_3,\ldots\}.
\]
The maximal lattice that can be quotiented at every scale without losing defect data is \(K_T^\perp\cap\mathbb{Z}^4\). For the witness, \(\dim K_T=3\) and \(K_T^\perp\) has rank 1.

Write \(K_A=\mathrm{span}\{1,A,T,T_2,\ldots\}\). Always \(K_T\subseteq K_A\). On a 4-letter alphabet with \(\mathrm{rank}\{1,A\}=2\), one has \(\dim K_A=2\) if and only if \(T\in\mathrm{span}\{1,A\}\) (Theorem P). That is the *affine* case of Currie–Mol–Rampersad–Shallit / Andrade–Mol, already known.

## Theorem P — affine image-sums iff \(\dim K_A=2\)
**Status: PROVED.** Novelty: ELEMENTARY; the name “affine morphism” is KNOWN (CMRS 2021, Andrade–Mol 2024).

Let \(A\) have four distinct values, so \(\mathrm{rank}\{1,A\}=2\). Then \(T\in\mathrm{span}\{1,A\}\) iff \(\mathrm{rank}\{1,A,T\}=2\). If \(T=\alpha 1+\beta A\), then \(T_2=M^\top T=\alpha m\,1+\beta T\in\mathrm{span}\{1,T\}\subseteq\mathrm{span}\{1,A\}\), hence \(\dim K_A=2\). Conversely \(\dim K_A=2\) forces \(T\in\mathrm{span}\{1,A\}\).

This class is strictly smaller than “\(L\) is \(M\)-invariant”. Exhaustive 2-uniform census (C++ matches Python): on \(\{0,1,2,3\}\), \(T\) affine for 854 morphisms, while \(L\)-invariant for 9548, including **332** four-sum non-affine examples (smallest: \(0\mapsto 33,\;1\mapsto 31,\;2\mapsto 10,\;3\mapsto 00\), \(T=(6,4,1,0)\), \(T_2=12\cdot 1-2T\)) and **2626** three-sum \(L\)-invariant morphisms. So \((1\cdot v,\,T\cdot v)\) can close without \(T\) being a letter-sum.

The proposed quotient \(\mathbb{Z}^4/\ker(T)\) still does not carry an \(M\)-action unless \(T_2\in\mathrm{span}\{T\}\) (1182 morphisms; 373 of them have \(\ge 3\) image sums).

## Theorem Q — \(\omega\)-identity for triples
**Status: PROVED.** Novelty: ELEMENTARY (repackaging of Theorem K). The name is not a new cube criterion.

For \(W=\sigma(U)\) \(m\)-uniform and \(F=W[p,p+d)\), \(\omega(F):=T\cdot\psi_F+\delta_F\) equals \(\sum F\). Consecutive equal-length \(X,Y,Z\) form an additive cube iff \(\omega(X)=\omega(Y)=\omega(Z)\), for every residue type. Equivalent: \(\omega_i=\omega_{i+d}=\omega_{i+2d}\) with \(\omega_i=S(i+d)-S(i)\).

## Theorem R — exactly \(m^2\) triple types
**Status: PROVED.** Novelty: ELEMENTARY/LIKELY KNOWN.

If \(i\equiv r\pmod m\) and \(d\equiv s\pmod m\), then \(r_Y=(r+s)\bmod m\) and \(r_Z=(r+2s)\bmod m\). Fragment slot lengths are functions of \((m,r,s)\) and determine \((r,s)\) uniquely, so there are exactly \(m^2\) residue types and exactly \(m^2\) slot signatures. At most four ancestor letters appear as fragments (X-left, XY-junction, YZ-junction, Z-right). Dual census \(m=2,\ldots,7\).

## Theorem S — junction covering
**Status: PROVED.** Novelty: ELEMENTARY.

At an unaligned XY junction the right prefix of \(X\) and left suffix of \(Y\) are complementary pieces of one image \(\sigma(a)\), hence sum to \(T(a)\). Likewise YZ. Famous cube: YZ junction \(U[47]=1\), pieces `3`+`1` \(=T(1)=4\).

See `TRIPLE_REPORT.md`.

---

## Theorem V — Length rigidity of uniform morphisms
**Status: PROVED.** Novelty: ELEMENTARY.

Let \(\sigma\) be \(m\)-uniform, \(m\ge 2\). Then \(|\sigma(w)|=m|w|\) for every finite word \(w\). Consequently, concatenations of complete images have equal length if and only if they have equal ancestor length.

For three consecutive intervals of length \(d=km+s\) in \(\sigma(U)\) starting at offset \(r\), the complete-block counts \(n_X,n_Y,n_Z\) depend only on \((m,r,s,k)\), and
\[
|n_X-n_Y|\le 1,\qquad |n_Y-n_Z|\le 1.
\]
Dual C++/Python for \(m=2,\ldots,7\) (`bin/template_scan`, `python/template_verify.py`).

By contrast the Cassaigne morphism satisfies \(|\varphi(w)|=2|w|-|w|_3\), so equal image length is a Parikh constraint and ancestor lengths may differ by \(\Theta(|w|)\). Example: \(|\varphi(00)|=|\varphi(330)|=4\).

## Theorem V′ — 2-uniform interiors have equal complete-block length
**Status: PROVED.** Novelty: ELEMENTARY/LIKELY KNOWN.

If \(m=2\) and \(k\ge 1\), then \(n_X=n_Y=n_Z\) for every residue type \((r,s)\). The interior problem is exactly three consecutive \(n\)-factors of the ancestor. For \(m\ge 3\) the possible pairs \((n_X-n_Y,n_Y-n_Z)\) are a subset of \(\{-1,0,1\}^2\).

## Theorem W — Interior defect pairs \(E_\sigma\)
**Status: PROVED.** Novelty: ELEMENTARY (repackaging of Theorems K, L, U3).

A cube in \(W=\sigma(U)\) is equivalent to
\[
(T\cdot\psi_X-T\cdot\psi_Y,\; T\cdot\psi_Y-T\cdot\psi_Z)=(\delta_Y-\delta_X,\;\delta_Z-\delta_Y)\in E_\sigma,
\]
where \(E_\sigma\) is finite, depending only on \(\sigma\) and the residue type, and is computed by enumerating at most four shared fragment letters. For the famous morphism, the coupled set has 59 pairs; the independent-letter product has 121. The aligned type \((0,0)\) has \(E=\{(0,0)\}\): aligned cubes if and only if the T-coded ancestor has an additive cube.

If \(\sigma\) is primitive and prolongable, \(W\) is \(m\)-automatic and the T-coded word is a coding of \(W\). Cubes in \(W\) are exactly \(E_\sigma\)-cubes of that automatic numerical word, with complete-block length \(n=\lfloor d/m\rfloor+O(1)\).

## Theorem X — The Cassaigne word is not automatic
**Status: PROVED** (standard frequency test applied to \(\varphi\)). Novelty: ELEMENTARY/LIKELY KNOWN as a remark; not located as a numbered CCSS theorem.

The Cassaigne incidence matrix (letters \(0,1,3,4\)) is primitive (\(M^6>0\)), with column sums \(2,2,1,2\) and characteristic polynomial \(x^4-x^3-2x^2+2x-1\). This polynomial has no rational roots and does not factor into monic integer quadratics, and \(p(1)=-1\), \(p(2)=3\), so the Perron eigenvalue lies in \((1,2)\) and is irrational. Primitive morphic letter frequencies are the normalised PF eigenvector, hence not all rational. Automatic sequences with existing letter frequencies have rational frequencies. Therefore the Cassaigne fixed point is not \(k\)-automatic for any \(k\ge 2\).

Independent checks: \(\det(M)=-1\), \(\mathrm{tr}(M)=1\), \(|\varphi^k(0)|=1,2,3,5,8,14,23,40,66\) (not \(2^k\)). Dual C++/Python.

## Theorem Y — Same-word uniformization of Cassaigne is impossible
**Status: PROVED.** Novelty: ELEMENTARY from Theorem X.

A letter-to-letter coding of a \(k\)-uniform morphism is \(k\)-automatic. By Theorem X the Cassaigne word is not such a coding. In particular higher-block encodings that remain constant-length cannot produce the same infinite word.

Padding \(\varphi(3)=1\) to a length-2 image produces a different 2-uniform word over \(\{0,1,3,4\}\). All eight pads, all four seeds, cap 200: cubes; best ACF prefix 48 (C++ matches Python). This is a special case of the exhaustive 2-uniform failure on that alphabet (Theorem F analogue on \(B\)).

## Theorem Z — Krylov expansion does not eject cubes from the T-window
**Status: PROVED** by explicit cubes. Novelty: UNVERIFIED NOVELTY as a packaged countertheorem; the coordinates are REPRODUCED.

If \(T_2=a\cdot 1+b\cdot T+R\) with \(R\neq 0\), then on \(L=\ker(1)\cap\ker(T)\) one has \(T_2\cdot v=R\cdot v\). Growth of \(T_2\cdot v\) is compatible with \(T\cdot v\) remaining in the finite defect window.

Famous witness: first cube \(v=(1,-1,1,-1)\), \(T\cdot v=2\), \(T_2\cdot v=-4\). On 442 cubes of the length-2048 iterate, \(T\cdot v\in[-4,3]\) while \(T_2\cdot v\in[-10,8]\) and \(\|v\|_1\le 18\). The proposed obstruction “a nonzero expanding Krylov component forces T-weight out of every bounded defect window” is false.

See `TEMPLATE_REPORT.md`.

---

## Theorem AA — Synchronized summatory iff bounded discrepancy
**Status: PROVED.** Novelty: DIRECT COROLLARY of Shallit, *The Logical Approach to Automatic Sequences*, Theorem 10.6.1, plus rational letter frequencies of primitive constant-length substitutions.

Let \(U\) be a primitive \(m\)-uniform word over a finite integer alphabet, \(a=T\circ U\), \(S(n)=\sum_{j<n}a_j\), and \(\mu\in\mathbb{Q}\) the mean of \(a\). Write \(\Delta(n)=S(n)-\mu n\).

A \((m,m)\)-synchronized function which is \(o(n)\) is bounded (Shallit 10.6.1 with \(\ell=m\)). Primitive uniform maps have a simple Perron eigenvalue \(m\), so \(\Delta=o(n)\). The identity function \(n\mapsto n\) is \(m\)-synchronized, hence so is \(\mu n\). Thus if \(S\) is \(m\)-synchronized then \(\Delta\) is too, hence bounded.

Conversely, bounded \(\Delta\) means \(S(n)=\mu n+O(1)\). A bounded \(m\)-regular sequence is \(m\)-automatic, and the graph of \(\mu n\) plus an automatic sequence is an \(m\)-synchronized graph. So \(S\) is \(m\)-synchronized.

Bounded discrepancy is equivalent to bounded additive complexity of \(a\), which forces additive \(k\)-powers for all \(k\) (Ardal–Brown–Jungić–Sahasrabudhe, Integers 2012, Thm 5). Therefore first-order / Walnut methods that require a synchronized summatory see only a class in which cubes are already forced.

The famous map has spectrum \(\{2,\sqrt{2},-\sqrt{2},0\}\), so \(\Delta\) is unbounded of order \(n^{1/2}\) (\(\max_{n<4096}\lvert S(n)-3n\rvert=63\)). Its summatory is 2-regular, not 2-synchronized.

Cassaigne is not uniform, hence not a counterexample to the constant-length hypothesis.

---

## Theorem AB — Aligned kernel of \(S\) is the Krylov orbit
**Status: PROVED.** Novelty: ELEMENTARY.

Let \(\sigma\) be \(m\)-uniform with incidence matrix \(M\), \(U=\sigma^\omega(a)\), \(\psi(n)=\mathrm{Parikh}(U[0:n))\), \(T_1=T\) the vector of image sums, \(T_{k+1}=M^\top T_k\). Then \(U[0:mn)=\sigma(U[0:n))\), so \(\psi(mn)=M\psi(n)\) and
\[
S(mn)=T\cdot\psi(mn)=T_2\cdot\psi(n).
\]
Inductively \(S(m^r n)=T_{r+1}\cdot\psi(n)\). The linear forms appearing in the aligned \(m\)-kernel of \(S\) are exactly the Krylov orbit of \(T\) under \(M^\top\).

The remainder of the kernel uses incomplete images:
\[
\psi(mn+r)=M\psi(n)+\mathrm{Parikh}(\sigma(U[n])[0:r]),
\]
which is an affine recurrence on \(\mathbb{Z}^{|A|}\) with finite control state \(U[n]\). This is not a finite-state representation of the integer \(S(n)\).

Dual check on the famous map: \(S(2n)=T_2\cdot\psi(n)\) for all \(n\le 400\), \(T_2=(3,5,7,9)\).

---

## Theorem AC — Famous prefix T-squares are 4-adic
**Status: PROVED** (the scaling identity is a \(4\times 4\) multiplication; the seed zero \(L\cdot\psi(70)=0\) is a finite Parikh count). Novelty: UNVERIFIED NOVELTY as a packaged additive-square statement; the linear algebra is elementary.

Let \(L=2T-T_2=(7,3,-3,-7)\). Then \(D_1(0,n)=L\cdot\psi(n)\), so prefix T-squares are the zeros of \(L\cdot\psi\). Direct multiplication gives \(LM^2=2L\). Combined with \(\psi(4n)=M^2\psi(n)\),
\[
L\cdot\psi(4n)=2L\cdot\psi(n).
\]
Hence the zero set is invariant under \(n\mapsto 4n\). Independent C++/Python: \(L\cdot\psi(70)=0\), therefore \(L\cdot\psi(70\cdot 4^k)=0\) for every \(k\ge 0\). This is an infinite 2-recognizable family of prefix additive **squares** in the T-word.

It does **not** produce prefix T-cubes: \(\chi_{\mathrm{cu}}\) is the zero sequence on the length-4096 prefix. Cubes use \(\psi(3n)\), which is not in the orbit of \(M\).

Cassaigne has no prefix squares in length 2048 (\(n_{\mathrm{sq}}=0\)) and is not uniform, so this mechanism uses constant length.

---

## Theorem G — High complexity of Up-and-Down words
**Status: COMPUTATIONALLY VERIFIED**

The ACF word of length 65986 produced by Up-and-Down with period 1000 contains every ACF factor of length \(\le 5\) and 3134 of the 3152 ACF factors of length 6. Its factor complexity satisfies \(p(10)=37591\).

A purely morphic infinite word has \(p(n)=O(n)\). Therefore this finite word (and, by the same mechanism, Liétard’s 70.88M word of the same construction type) is **not** a prefix of a low-complexity purely morphic word. Phase 5 question “is the long finite construction secretly a small morphism?” — **no**, not for the Up-and-Down family.

---

## Non-theorems / failed conjectures

See `COUNTEREXAMPLES.md` and `CONJECTURES.md`.
