# Novelty Audit: Additive Cubes over {0,1,2,3}

**Audit date:** 2026-09-04  
**Status of the original problem:** **OPEN** (not solved).  
**Do not stop.** Proceed to Phases 1–11.

This audit is the gate for all later claims. Anything not listed under **Already known** may later be labelled new only after a second, discovery-specific literature check (Phase 11).

---

## 1. Exact problem statements

### 1.1 Combinatorics-on-words form (this project’s central question)

**Alphabet.** \(A = \{0,1,2,3\} \subset \mathbb{Z}\).

**Additive cube.** A nonempty word \(XYZ\) with \(|X|=|Y|=|Z|=d\ge 1\) and
\[
\sum X = \sum Y = \sum Z.
\]
Equivalently, a word \(a_0 a_1 \cdots a_{n-1}\) contains an additive cube if there exist \(i\ge 0\) and \(d\ge 1\) with \(i+3d\le n\) and
\[
\sum_{t=0}^{d-1} a_{i+t}
= \sum_{t=0}^{d-1} a_{i+d+t}
= \sum_{t=0}^{d-1} a_{i+2d+t}.
\]
In prefix-sum form, writing \(S(0)=0\) and \(S(k)=a_0+\cdots+a_{k-1}\),
\[
S(i+d)-S(i) = S(i+2d)-S(i+d) = S(i+3d)-S(i+2d),
\]
i.e. the four points \((i,S(i)),\ (i+d,S(i+d)),\ (i+2d,S(i+2d)),\ (i+3d,S(i+3d))\) form a 4-term arithmetic progression in \(\mathbb{Z}^2\).

**Open question (Rao 2015; Rao–Rosenfeld 2018; Liétard–Rosenfeld 2020/2021).**  
Does there exist an *infinite* word over \(\{0,1,2,3\}\) containing no additive cube?

This is equivalent, by König’s lemma on the finitely branching tree of finite additive-cube-free (ACF) words, to: do ACF words of arbitrarily large finite length exist?

### 1.2 Green’s Open Problem 15 (related, **not identical**)

Green, *100 Open Problems* (2024), Problem 15, originally heard from Jacob Fox (2005):

> Does there exist a Lipschitz function \(f:\mathbb{N}\to\mathbb{Z}\) whose graph \(\Gamma=\{(n,f(n)):n\in\mathbb{N}\}\subset\mathbb{Z}^2\) is free of 3-term arithmetic progressions?

A 3-AP in the graph of a Lipschitz integer function is equivalent to an **additive square** in the sequence of first differences \(a_n = f(n)-f(n-1)\) (two consecutive equal-length equal-sum blocks). Green notes that the **4-AP** analogue is settled by Cassaigne–Currie–Schaeffer–Shallit via an additive-cube-free word over \(\{0,1,3,4\}\), taking \(f(n)=\sum_{i=1}^n x_i\).

**Relation to this project.**

| Graph APs of the partial-sum function | Difference-word pattern |
|---|---|
| 3-term AP (Green 15 / Fox) | additive **square** — open over every finite subset of \(\mathbb{Z}\) |
| 4-term AP | additive **cube** — settled except for alphabets affine-equivalent to \(\{0,1,2,3\}\) |

Attacking additive cubes over \(\{0,1,2,3\}\) therefore attacks the **exceptional 4-AP / Lipschitz-graph** case, not Green 15 itself. Green 15 remains the additive-square problem (Pirillo–Varricchio / Halbeisen–Hungerbühler / Fox).

### 1.3 Fox’s original problem

Independently of Pirillo–Varricchio (1994, semigroup language: is \(\mathbb{N}^+\) uniformly 2-repetitive?), Halbeisen–Hungerbühler (2000) and Fox (c. 2005, via Green) asked whether additive squares are avoidable over a finite subset of \(\mathbb{Z}\). **Still open.**

### 1.4 Affine equivalence

Alphabets \(A,B\subset\mathbb{C}\) are equivalent if \(B = \{\alpha x+\beta : x\in A\}\) for \(\alpha\neq 0\). Additive-cube avoidability is invariant under this relation (Liétard–Rosenfeld). The exceptional 4-letter class is exactly the 4-term arithmetic progressions, represented by \(\{0,1,2,3\}\).

---

## 2. Precise definitions used below

- **Ordinary \(k\)-power:** \(X^k\) with \(X\neq\varepsilon\).
- **Abelian \(k\)-power:** \(X_1\cdots X_k\) with \(|X_i|\) equal and equal Parikh vectors.
- **Additive \(k\)-power:** \(X_1\cdots X_k\) with equal lengths and equal sums.
- Over a 2-letter integer alphabet, additive \(=\) abelian.
- **Cupisca** (Liétard): a morphism whose infinite fixed point is additive-cube-free. (Name used in the 2020 thesis.)
- **Size of a morphism:** \(\max_a |h(a)|\).
- **Uniform morphism:** all images have equal length.

---

## 3. Known results (settled theorems)

### 3.1 Ordinary and abelian background

| Pattern | Smallest alphabet for an infinite avoiding word | Source |
|---|---|---|
| Ordinary squares | 3 | Thue |
| Ordinary cubes | 2 | Thue |
| Abelian squares | 4 | Keränen 1992 |
| Abelian cubes | 3 | Dekking 1979 |
| Abelian 4-powers | 2 | Dekking 1979 |

### 3.2 Additive 4-powers

Over 2 letters, additive 4-powers \(=\) abelian 4-powers, hence avoidable (Dekking). Thus additive 4-powers are avoidable over every integer alphabet of size \(\ge 2\).

### 3.3 Additive cubes — settled alphabets

1. **Cassaigne, Currie, Schaeffer, Shallit (arXiv:1106.5204, J. ACM 2014 / Adv. Appl. Math. 56 (2014)).**  
   The morphism
   \[
   \varphi_0:\quad 0\mapsto 03,\quad 1\mapsto 43,\quad 3\mapsto 1,\quad 4\mapsto 01
   \]
   has an infinite fixed point over \(\{0,1,3,4\}\) with no additive cube.  
   Consequence: \(\mathbb{N}^+\) is not uniformly 3-repetitive.  
   Consequence for Green 15: Lipschitz graphs **without 4-APs** exist.

2. **Rao (2015), “On some generalizations of abelian power avoidability”.**  
   Using substitutions from \(\{0,1,3,4\}\), additive cubes are avoidable over many 3-letter alphabets (e.g. \(\{0,1,5\},\{0,1,6\},\ldots\)).  
   Conjecture: avoidable over every \(\{0,i,j\}\) with \(\gcd(i,j)=1\) and \(j\ge 6\).  
   **Left open:** \(\{0,1,2\},\{0,1,3\},\{0,1,4\},\{0,2,5\}\).  
   Also left open: \(\{0,1,2,3\}\). Longest word Rao obtained over \(\{0,1,2,3\}\): \(\sim 1.4\times 10^5\).

3. **Liétard–Rosenfeld (DLT 2020 / Discrete Mathematics / LIRMM preprint “Additive cubes are avoidable over all but one set of four numbers”).**  
   Additive cubes are avoidable over **every** 4-element alphabet of numbers that is **not** affine-equivalent to \(\{0,1,2,3\}\). Hence over every alphabet of size \(\ge 5\).  
   The exceptional case \(\{0,1,2,3\}\) is left open; they “do not dare conjecture anything.”

4. **Liétard, PhD thesis, Université de Lorraine, 12 Nov 2020.**  
   Extends the Cassaigne et al. kernel/lattice argument to an infinite family of morphisms similar to \(\varphi_0\). Independent computational construction of a finite ACF word of length **70,880,000** over \(\{0,1,2,3\}\) (Proposition 6.4.1).  
   Additional structural observations in the thesis (see §5–6 below).

5. **Rao–Rosenfeld (SIAM J. Discrete Math. 2018).**  
   Additive **squares** are avoidable over a finite subset of \(\mathbb{Z}^2\) (not of \(\mathbb{Z}\)).

6. **Andrade–Mol (arXiv:2408.15390, 2024/2025).**  
   Infinite additive 5-power-free *rich* word over \(\{0,1\}\); infinite additive 4-power-free rich word over \(\{0,1,2\}\).  
   They restate that an infinite ACF word over \(\{0,1,2,3\}\) is still unknown.

### 3.4 Additive squares

Open over every finite subset of \(\mathbb{Z}\). Partial negative results:

- Freedman: if \(a+d=b+c\), longest additive-square-free word over \(\{a,b,c,d\}\) has length \(\le 60\).
- Brown: every infinite word over a finite subset of \(\mathbb{Z}\) contains factors “close” to additive squares, and \(k\) consecutive blocks of equal *average* for every \(k\).
- Au–Robertson–Shallit: related modular and frequency constraints.

---

## 4. Known constructions

### 4.1 Infinite morphic constructions (not over {0,1,2,3})

Canonical seed (Cassaigne et al.):
\[
\varphi_0(0)=03,\ \varphi_0(1)=43,\ \varphi_0(3)=1,\ \varphi_0(4)=01.
\]
Liétard–Rosenfeld work with the conjugate family
\[
\varphi_{a,b,c,d}(a)=ac,\ \varphi(b)=dc,\ \varphi(c)=b,\ \varphi(d)=ab,
\]
and prove the fixed point \(W_{a,b,c,d}\) is ACF for all 4-letter alphabets except the AP class (after finitely many remaining cases handled by other morphisms / literature).

Rao: size-25 (and other) substitutions producing ACF words over various 3-letter alphabets.

### 4.2 Finite constructions over {0,1,2,3}

| Length | Author | Method | Source |
|---|---|---|---|
| 1288 | Cassaigne et al. | DFS, over **{0,1,2}** (3 letters) | arXiv:1106.5204 |
| \(\sim 1.4\times 10^5\) | Rao (2015) | backtracking | TCS 2015 |
| 24,396 | Liétard, greedy fixed letter order | “classical” extension; then stalls (two extra hours to gain one letter) | Thesis Ch. 6 |
| **70,880,000** | Liétard 2020 | “Up and Down”: periodic reversal of letter-priority, OpenMP, prefix-sum verifier | Thesis Prop. 6.4.1 |

**No published improvement of 70,880,000 was found in 2021–2026 sources** (arXiv, DLT, theses, Andrade–Mol 2024, Green 2024, GitHub). Independent unpublished replication: word-structures lab (2026) confirmed that with budget \(10^6\), fixed order reaches 24,396 and alternating order reaches a 300,000 cap, i.e. the method works, but they did not claim a new record.

The 70,880,000 word itself was stated to be downloadable from a URL on p. 13 of the thesis. As of this audit, https://members.loria.fr/FLietard/phdProgramsAndDocumentation/ still exists as a landing page, but the word file was **not independently retrieved** in this audit. Reconstruction of the generating mechanism is mandatory in Phase 3.

Letter frequencies of the 70.88M word (Liétard):  
0: 28.320%, 1: 21.691%, 2: 21.689%, 3: 28.300%. Near-symmetry \(0\leftrightarrow 3\), \(1\leftrightarrow 2\).

---

## 5. Known records

- **Longest published finite ACF word over {0,1,2,3}:** 70,880,000 (Liétard 2020). Treat this as the record to beat or reconstruct.
- **Previous record:** \(\sim 1.4\times 10^5\) (Rao 2015).
- **Longest ACF word over {0,1,2} reported by Cassaigne et al.:** 1288 (not a 4-letter record).
- **No infinite construction** over {0,1,2,3} or any affine image.

---

## 6. Known impossibility / structural constraints

### 6.1 Trivial class: eventually periodic words

Every purely periodic infinite word \(u^\omega\) contains the ordinary cube \(uuu\), hence an additive cube. Every eventually periodic word \(vu^\omega\) contains \(uuu\) as a factor.  
**Therefore no eventually periodic infinite ACF word exists over any alphabet.**  
This is elementary and must not be advertised as a new theorem. It does, however, correctly force all infinite candidates to be aperiodic.

### 6.2 Binary alphabets

Additive cubes \(=\) abelian cubes, which are unavoidable over 2 letters (Dekking). So alphabet size \(\ge 3\) is necessary. Size 3 is open for several specific alphabets (Rao); size 4 is open only for the AP class.

### 6.3 Morphisms on {0,1,2,3} (Liétard, thesis)

- If a (finite) morphism over {0,1,2,3} producing an infinite ACF fixed point exists, its **size is at least 7**. (For {0,1,2,4}, size 2 is impossible but size 3 works.)
- All 2-uniform Cupisca morphisms found on integer 4-letter alphabets with \(0=a<b<c<d<26\) were similar to \(\varphi_0\). Liétard asks whether every size-2 Cupisca morphism is similar to \(\varphi_0\).
- Looking4Morphisms (C++, Grid5000, Jamet–Liétard) searched morphisms whose first 10,000 letters are ACF. On {0,1,2,3} this search does not produce the easy candidates that appear on every other 4-letter alphabet (Liétard–Rosenfeld: “running the same program 100000 times longer does not provide any candidate”).
- 288 size-2 morphisms on 4 letters have an abelian-cube-free prefix of length 10,000; 24 of them are the permutations of \(\varphi_0\).

### 6.4 No published theorem ruling out

- all morphic words over {0,1,2,3},
- all uniform morphisms of size \(\ge 7\),
- automatic / morphic / linearly recurrent / S-adic / Toeplitz classes,
- uniformly recurrent ACF words.

Absence of a morphism of size \(\le 6\) is a **computational** claim in Liétard’s thesis, not a human-readable obstruction theorem for the whole morphic class.

### 6.5 Why {0,1,2,3} is hard

Many additive relations: \(0+3=1+2\), \(1+1=0+2\), \(2+2=1+3\), \(0+0+3=1+1+1\), etc. Liétard notes that a nearby non-equivalent alphabet such as \(\{0,19,38,58\}\) is much easier for the same search program.

---

## 7. Known computational searches

| Work | What was searched | Outcome |
|---|---|---|
| Cassaigne et al. | DFS over {0,1,2} | length 1288 |
| Rao 2015 | long backtracking over {0,1,2,3} | \(\sim 1.4\times 10^5\); morphism search unsuccessful |
| Liétard Looking4Morphisms | size-2 (and some larger) morphisms, 10k prefix test, all 4-letter integer alphabets \(d<26\) and later \(d<100\) | plenty of candidates except on the AP class; size \(\ge 7\) if any exist on {0,1,2,3} |
| Liétard Up-and-Down | greedy extension with periodic priority reversal | 70,880,000 |
| Liétard verifier | prefix sums, cubes ending at the newest letter, OpenMP | \(O(n)\) per extension, \(O(n^2)\) full check |
| Andrade 2024 + `lgmol/Additive-Powers-Decision-Algorithm` | decision procedure for additive \(k\)-power-freeness of certain morphic words (Currie–Mol–Rampersad–Shallit) | used for rich-word theorems, **not** a {0,1,2,3} cube solution |
| word-structures/combinatorics-on-words-research (2026) | replicated alternating-priority on additive cubes with budget \(10^6\) | cap 300,000 vs 24,396 fixed order; no new record claimed |

**No published SAT/SMT/CP encoding** specifically for additive cubes over {0,1,2,3} was found. (SAT is used widely for ordinary/abelian avoidability; absence of a paper is not absence of the idea.)

---

## 8. Known morphisms (explicit)

**Cassaigne et al. \(\varphi_0\) on {0,1,3,4}:**  
`0 → 03`, `1 → 43`, `3 → 1`, `4 → 01`.

**Liétard–Rosenfeld template:**  
`a → ac`, `b → dc`, `c → b`, `d → ab`.

**Naive transport of \(\varphi_0\) onto {0,1,2,3}** (e.g. `0→03`, `1→23`, `2→1`, `3→01`) is **not** known to work and is the sort of candidate Liétard’s search already tries; it must be treated as already searched in the size-2 class (and size 2 is ruled out for Cupisca on this alphabet).

Rao’s 3-letter substitutions: see Table 2 of TCS 2015 (images of length 19, 25, …). If an infinite ACF word over {0,1,2,3} existed, Rao notes that certain substitutions would then yield ACF words over {0,1,4} and {0,2,5}.

---

## 9. OEIS and databases

- **A051043:** number of *ordinary* cubefree quaternary words of length \(n\). **Not** additive cubes.
- No OEIS sequence for the number of additive-cube-free words over {0,1,2,3} was found. Liétard’s Table 2.2 counts them for \(1\le n\le 8\) (exact numbers not cleanly extractable from the HTML thesis dump; they will be recomputed independently).
- Green’s problem is tracked in `google-deepmind/formal-conjectures` as `green_15` (3-APs, still `sorry`) and a solved 4-AP companion.

---

## 10. Code repositories

| Repo | Relevance |
|---|---|
| https://github.com/lgmol/Additive-Powers-Decision-Algorithm | morphic additive \(k\)-power decision (Andrade 2024) |
| https://github.com/word-structures/combinatorics-on-words-research | independent 2026 experiments; replicated Up-and-Down on this exact problem at modest budget |
| https://github.com/google-deepmind/formal-conjectures | Lean statement of Green 15 |
| https://members.loria.fr/FLietard/phdProgramsAndDocumentation/ | Liétard C++ (Looking4Morphisms, verifiers); retrieval of the 70.88M word not confirmed live |

---

## 11. SAT / SMT / CP

**Already known:** none found as a published encoding for this exact problem.  
Using SAT/CP is therefore methodologically legitimate, but a SAT-found long word is **not** automatically new if it is shorter than 70.88M or is an implicit rediscovery of Up-and-Down.

---

## 12. Lower bounds on alphabet size for additive cubes

- Size 1, 2: impossible (abelian cubes unavoidable over 2 letters).
- Size 3: possible for many alphabets (Rao); four specific 3-letter alphabets open.
- Size 4: possible unless affine-equivalent to {0,1,2,3} (Liétard–Rosenfeld).
- Size \(\ge 5\): always possible.

So the **minimal alphabet size for some integer alphabet** is either 3 or 4, and is 3 if any of Rao’s remaining 3-letter cases is positive. The **minimal size among arithmetic-progression alphabets** is the {0,1,2,3} question (size 4 vs \(\infty\)).

---

## 13. Impossibility theorems for periodic / morphic / automatic classes

| Class | Status |
|---|---|
| Purely / eventually periodic | Impossible (ordinary cubes). Elementary, known. |
| Uniform morphisms of size \(\le 6\) on {0,1,2,3} | Computationally excluded by Liétard (thesis), not an independent published theorem with a full morphism-by-morphism table in a journal |
| Uniform morphisms of size \(\ge 7\) | Open |
| Non-uniform morphic | Open |
| Automatic | Open (automatic words are morphic) |
| Linearly recurrent / uniformly recurrent | Open |
| Toeplitz / S-adic / transducers | No literature found |

---

## 14. ALREADY KNOWN — do not claim as new

1. The problem is open as of 2026.
2. Infinite ACF words exist over every 4-letter number alphabet except the AP class {0,1,2,3}.
3. The Cassaigne et al. morphism over {0,1,3,4} and the Liétard–Rosenfeld extension.
4. Rao’s 3-letter constructions and the four remaining 3-letter open alphabets.
5. Finite length \(70{,}880{,}000\) via Up-and-Down priority reversal.
6. Finite length \(\sim 1.4\times 10^5\) by Rao.
7. Greedy fixed order stalls near length 24,396.
8. Morphism size \(\ge 7\) if a purely morphic infinite solution on {0,1,2,3} exists (Liétard).
9. Size-2 Cupisca morphisms found in large searches are similar to \(\varphi_0\); \(\varphi_0\) itself does not live on {0,1,2,3}.
10. Eventually periodic words are impossible.
11. Additive squares (Green 15 / Fox / Pirillo–Varricchio) are a strictly harder, still-open problem.
12. Prefix-sum \(O(n^2)\) full test and \(O(n)\) incremental test for cubes ending at the last letter.
13. Near-symmetry of letter frequencies in the long Up-and-Down word.
14. Equivalence of avoidability under affine alphabet maps.
15. König’s lemma equivalence: unbounded finite lengths \(\Leftrightarrow\) infinite word.
16. Over 2 letters, additive \(=\) abelian.
17. The decision algorithm of Currie–Mol–Rampersad–Shallit / Andrade for certain morphic additive powers.
18. Independent 2026 replication that alternating priority dramatically beats fixed order on this language (word-structures lab), without beating 70.88M.

---

## 15. What would count as new (gate for later phases)

Only after a second literature search on the specific object:

- A. An infinite ACF word over {0,1,2,3} with a finite explicit rule, preferably with a proof.
- B. A finite ACF word longer than 70,880,000, with independent verification and a described generator.
- C. An explicit morphism (size \(\ge 7\)) whose fixed point is ACF for a prefix much longer than previously tested, with obstruction analysis — especially if accompanied by a proof or a decision-procedure certificate.
- D. A theorem ruling out a genuine class (e.g. all uniform morphisms, all morphic words, all linearly recurrent words), not the trivial eventually-periodic class.
- E. A necessary condition that is not already in Liétard/Rao (frequency, balance, return words, modular invariants) and that survives adversarial testing.
- F. Exact enumeration of ACF words beyond Liétard’s \(n\le 8\), if no later table exists.
- G. A SAT/CP lower bound (no ACF word of length \(N\)) — currently no such finite-length impossibility is known, and 70.88M makes a SAT *impossibility* bound uncompetitive unless it is on a restricted subclass.

---

## 16. References (with links)

1. J. Cassaigne, J. D. Currie, L. Schaeffer, J. Shallit, *Avoiding three consecutive blocks of the same size and same sum*, arXiv:1106.5204. https://arxiv.org/abs/1106.5204  
   Journal: *Adv. Appl. Math.* 56 (2014); also J. ACM 61 (2014).
2. M. Rao, *On some generalizations of abelian power avoidability*, Theoret. Comput. Sci. (2015). https://perso.ens-lyon.fr/michael.rao/publi/kab.pdf
3. F. Liétard, M. Rosenfeld, *Additive cubes are avoidable over all but one set of four numbers*. https://www.lirmm.fr/~mrosenfeld/LieRos.pdf  
   Conference: DLT 2020, LNCS 12086.
4. F. Liétard, *Évitabilité de puissances additives en combinatoire des mots*, PhD thesis, Université de Lorraine, 2020. http://theses.fr/2020LORR0259/document  
   HAL: https://hal.univ-lorraine.fr/tel-03203854
5. G. Pirillo, S. Varricchio, *On uniformly repetitive semigroups*, Semigroup Forum 49 (1994).
6. L. Halbeisen, N. Hungerbühler, *An application of van der Waerden’s theorem in additive number theory*, Integers (2000).
7. B. Green, *100 Open Problems* (2024), Problem 15. https://people.maths.ox.ac.uk/greenbj/papers/open-problems.pdf
8. J. Andrade, L. Mol, *Avoiding abelian and additive powers in rich words*, arXiv:2408.15390. https://arxiv.org/abs/2408.15390
9. J. Currie, L. Mol, N. Rampersad, J. Shallit, *Extending Dekking’s construction…*, arXiv:2111.07857.
10. M. Rao, M. Rosenfeld, *Avoiding two consecutive blocks of same size and same sum over \(\mathbb{Z}^2\)*, SIAM J. Discrete Math. 32 (2018). https://www.lirmm.fr/~mrosenfeld/M114937.pdf
11. T. C. Brown, V. Jungić, A. Poelstra, *On double 3-term arithmetic progressions*, Integers 14 (2014).
12. F. M. Dekking, *Strongly non-repetitive sequences and progression-free sets*, J. Combin. Theory Ser. A 27 (1979).
13. WORDS open problems: arXiv:1705.04055, Question 1.6.13.10 (Rao, WORDS 2013).
14. Y.-H. Au, A. Robertson, J. Shallit, *Van der Waerden’s theorem and avoidability in words*, Integers.
15. T. C. Brown, *A partial solution… additive squares*, Integers.
16. Additive-Powers-Decision-Algorithm: https://github.com/lgmol/Additive-Powers-Decision-Algorithm
17. Formal Green 15: https://github.com/google-deepmind/formal-conjectures
18. OEIS A051043 (ordinary cubefree, not additive): https://oeis.org/A051043
19. Liétard programs: https://members.loria.fr/FLietard/phdProgramsAndDocumentation/
20. word-structures lab (2026 computational notes): https://github.com/word-structures/combinatorics-on-words-research

---

## 17. Audit conclusion

- **Original infinite problem: OPEN.** Continue.
- **Green 15 (additive squares / 3-APs in Lipschitz graphs): OPEN and strictly harder.** This project’s legitimate target is the exceptional additive-cube alphabet, which is the remaining 4-AP Lipschitz-graph case for difference alphabet {0,1,2,3}.
- **Record to reconstruct:** Liétard Up-and-Down, length 70,880,000.
- **Morphism searches have already been done**, especially size \(\le 6\). New morphism work must start at size \(\ge 7\) or use a genuinely different class (S-adic, coding of larger alphabets, transducers), and must not rediscover \(\varphi_0\)-lookalikes on the wrong alphabet.
- **A long random computation without a finite mathematical rule is not success.**

---

## 18. Addendum (desubstitution attack, 2026-09-04)

Second search specifically for unaligned additive-cube desubstitution in uniform morphic words: CCSS, Rao 2015, Liétard thesis, Liétard–Rosenfeld, Andrade–Mol arXiv:2408.15390, WORDS list arXiv:1705.04055. **Not found:** the `(r,s)` classification, the identity `T·v+Δδ=0` as a cube criterion, or Theorem J. Label **UNVERIFIED NOVELTY**. Do **not** claim the AP lattice vector `(1,-2,1,0)` as new. Do **not** claim 2-uniform failure as new. See `DESUBSTITUTION_REPORT.md` §7.
