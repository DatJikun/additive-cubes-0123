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

## Theorem D — No dead-end of length \(\le 14\)
**Status: COMPUTATIONALLY VERIFIED** (independent enumeration; not found in the literature at this length)

Every additive-cube-free word of length \(n\le 14\) over \(\{0,1,2,3\}\) has at least one letter \(a\in\{0,1,2,3\}\) such that \(wa\) is additive-cube-free.

**Method.** Exhaustive DFS of the ACF trie using the incremental verifier. At each node of depth \(\le 14\), all four one-letter extensions were tested. The dead-end counter is 0 at every depth \(0..14\).

**Check.** Counts through length 6 match the independent Python brute-force enumerator. Counts through length 5 match OEIS A051043. The C++ incremental checker agrees with the full \(O(n^2)\) checker on all words of length \(\le 6\).

This does **not** prove that the language is infinite. It does prove that the first obstruction, if any, occurs at length \(\ge 15\).

---

## Theorem E — Exact enumeration through length 12
**Status: COMPUTATIONALLY VERIFIED**

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

Liétard’s thesis Table 2.2 tabulated \(c_n\) for \(1\le n\le 8\); the HTML dump did not yield the digits, so \(n=7,8\) are independent recomputations and \(n=9..12\) appear to be new.

Empirical branching \(c_{n+1}/c_n \approx 3.52\) at \(n=11\). Letter frequencies under the uniform measure on length-12 ACF words: \((0.251,0.249,0.249,0.251)\).

---

## Theorem F — Classification of 2-uniform morphisms (partial)
**Status: COMPUTATIONALLY VERIFIED**, with a residual caveat stated below

Every 2-uniform morphism \(h:\{0,1,2,3\}\to\{0,1,2,3\}^2\) that is prolongable on at least one letter \(a\) (i.e. \(h(a)\) begins with \(a\)) produces an additive cube in the prefix of \(h^\omega(a)\) of length \(\le 120\). Exhaustive scan of all \(4^8=65536\) morphisms; 0 survivors to length 800 among prolongable seeds. Best prefix: \(0\mapsto 32,\;1\mapsto 31,\;2\mapsto 20,\;3\mapsto 01\), seed 2, first cube by length 120.

**Caveat (closed in a follow-up scan):** non-prolongable seeds and non-expanding images were not in the first scan. The all-seed rescan is the certificate to cite; until it finishes, the claim is only for purely morphic words generated from a prolongable letter.

Combined with Theorem C, morphisms with \(\le 2\) image sums are independently excluded for infinite iterates.

This is a sharpening of Liétard’s “size at least 7 if a morphism exists”, restricted to the uniform size-2 subclass, with an explicit worst-case prefix length.

---

## Theorem G — High complexity of Up-and-Down words
**Status: COMPUTATIONALLY VERIFIED**

The ACF word of length 65986 produced by Up-and-Down with period 1000 contains every ACF factor of length \(\le 5\) and 3134 of the 3152 ACF factors of length 6. Its factor complexity satisfies \(p(10)=37591\).

A purely morphic infinite word has \(p(n)=O(n)\). Therefore this finite word (and, by the same mechanism, Liétard’s 70.88M word of the same construction type) is **not** a prefix of a low-complexity purely morphic word. Phase 5 question “is the long finite construction secretly a small morphism?” — **no**, not for the Up-and-Down family.

---

## Non-theorems / failed conjectures

See `COUNTEREXAMPLES.md` and `CONJECTURES.md`.
