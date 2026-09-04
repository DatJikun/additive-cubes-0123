# Bounded additive complexity / \(B(m)\) attack

**Status of the infinite problem:** OPEN.  
**ENDGAME: C** — computational fixed-base theorem: \(B(3)=1647<\infty\) for primitive prolongable 3-uniform maps on \(\{0,1,2,3\}\). Not a proof for every \(m\). Conjecture A is not a new mechanism (Corollary AH).

Control A is the famous 2-uniform map \(0\mapsto 32,\ 1\mapsto 31,\ 2\mapsto 20,\ 3\mapsto 01\). Control B is Cassaigne (non-uniform, not automatic). Control C: Thue–Morse / paperfolding / Rudin–Shapiro. Control D: \(\{0,1,4\}\) map \(0\mapsto 01,\ 1\mapsto 14,\ 4\mapsto 40\). Control E: the \(m=3\) maximizer and random/exhaustive 3-uniform maps.

---

## 1. Executive verdict

Proved or computationally verified:

- **Theorem AD (ELEMENTARY).** For fixed \(m\) and finite \(A\), the substitution space is finite, so \(B(m)<\infty\) if and only if there is no infinite primitive \(m\)-automatic ACF fixed point. Compactness of changing substitutions cannot produce a new object at fixed \(m\).
- **Theorem AE (ELEMENTARY).** Over a finite integer alphabet, unbounded \(p_T\) iff unbounded diameter. Sliding-window steps are bounded by \(\max A-\min A\).
- **Theorem AF (ELEMENTARY).** Weight colourings plus van der Waerden do not force additive cubes: the VdW difference need not equal the block length \(n\). Cassaigne \(n=8\): 524 monochromatic 3-APs, **0** with \(d=n\).
- **Theorem AG (COMPUTATIONALLY VERIFIED).** Exhaustive \(4^{12}\) census: every primitive prolongable 3-uniform map on \(\{0,1,2,3\}\) with \(\ge 3\) image sums has an additive cube by length 1648. \(B(3)=1647\). Dual C++/Python on the whole cap-243 tail (86 maps, all prolongable seeds) and on the maximizer cube \((982,222,229)\).
- **Corollary AH (DIRECT COROLLARY of Ardal).** Conjecture A + Ardal \(\Leftrightarrow\) no primitive automatic ACF word. The conjecture is the open uniform problem, not a stepping stone.

Falsified as forcing theorems:

- ACF \(\Rightarrow\) bounded \(p_T\) (Cassaigne).
- Unbounded diameter + uniform recurrence \(\Rightarrow\) cubes (Cassaigne).
- ACF \(\Rightarrow\) bounded discrepancy, so that a large modulus converts modular zeros to integer zeros (Ardal contrappositive).
- Expanding non-Perron spectrum \(\Rightarrow\) no long ACF prefix (maximizer has \(\lvert\lambda_2\rvert=1+\sqrt{2}>1\) and ACF prefix 1647).
- Van der Waerden on finitely many weight colours \(\Rightarrow\) additive cubes (Theorem AF).

Not achieved:

- A proof of Conjecture A for automatic words.
- An infinite automatic ACF word, or even a primitive uniform ACF word of unbounded length.
- \(B(m)<\infty\) for all \(m\), or any closed-form \(f(m)\).
- A decision procedure for the bivariate integer zero set of \((D_1,D_2)\). (Forbidden abstraction; not pursued.)

The original infinite problem is not solved.

---

## 2. Main theorem / counterexample

**Conjecture A is false without automaticity, and with automaticity it is the problem itself.**

Cassaigne’s morphism over \(\{0,1,3,4\}\) is uniformly recurrent, ACF, and has unbounded additive complexity (\(p_{\max}=66\) already on a length-2048 prefix, \(\mathrm{diam}(64)=95\), holes present, 0 equal-weight triples for \(n\le 64\)). That kills “ACF \(\Rightarrow\) bounded \(p_T\)” and kills “unbounded diameter + uniform recurrence \(\Rightarrow\) cubes”.

For **primitive automatic** words, Ardal’s theorem says bounded \(p_T\) already produces cubes. So an automatic ACF word, if one exists, **must** have unbounded \(p_T\). Conjecture A asserts the opposite. The only way both can hold is if no such word exists. Exhaustive \(m=2\) (previous work, \(B(2)=119\)) and exhaustive \(m=3\) (this run, \(B(3)=1647\)) are computational instances of that nonexistence, not a general proof.

**Theorem AG.** \(B(3)=1647\). Maximizer
\[
0\mapsto 100,\ 1\mapsto 102,\ 2\mapsto 132,\ 3\mapsto 332,
\]
seed 1, \(T=(1,3,6,8)\), cube \((982,222,229)\). Expanding spectrum: eigenvalues \(3,1\pm\sqrt{2},1\).

No automatic ACF candidate with unbounded \(p_T\) survived, because no automatic ACF candidate survived.

---

## 3. Conjecture A and the Ardal trap

Ardal–Brown–Jungić–Sahasrabudhe 2012, Theorem 5: bounded additive complexity implies additive \(k\)-powers. FSTTCS 2024, Proposition 30 applies the contrappositive to Cassaigne.

| hypothesis | \(p_T\) | cubes | automatic? |
|---|---|---|---|
| Cassaigne | unbounded (66 on \(n\le 64\)) | none | no (Thm X) |
| vtm \(0\mapsto 012,\ 1\mapsto 02,\ 2\mapsto 1\) | bounded (\(p=3\)) | \((0,3,3)\), ACF prefix 8 | no (non-uniform) |
| TM-like \(0\mapsto 01,\ 1\mapsto 12,\ 2\mapsto 20\) | logarithmic (FSTTCS Thm 28; \(p_{\max}=15\) here) | \((0,132,132)\), ACF 18 | yes |
| Thue–Morse | bounded (\(p=3\)) | \((0,2,1)\), ACF 5 | yes |
| paperfolding | \(p_{\max}=8\) | \((0,3,2)\), ACF 7 | yes |
| Rudin–Shapiro | growing (\(p_{\max}=23\)) | \((0,1,0)\), ACF 2 | yes |
| famous U, ACF prefix 119 | already \(p_{\max}=18\) | none yet | yes |
| famous U, length 256 | \(p_{\max}=26\) | \((45,25,35)\) | yes |
| \(\{0,1,4\}\) `01 14 40` | \(p_{\max}=27\) | \((2,63,105)\), ACF 108 | yes |
| \(m=3\) maximizer ACF prefix 1647 | \(p(64)=81\), \(p_{\max}(n\le 80)=107\) | none yet | yes |
| \(m=3\) maximizer length 1648 | — | \((982,222,229)\) | yes |

Every automatic control either has bounded \(p_T\) and cubes (Ardal) or unbounded \(p_T\) and still cubes. Cassaigne is the only ACF + unbounded \(p_T\) object, and it is not automatic.

Implication chain attempted and status:

| claim | status |
|---|---|
| A. unbounded \(p_T\) \(\Rightarrow\) unbounded diameter | TRUE (Thm AE) |
| B. unbounded diameter \(\Rightarrow\) equal-weight adjacent blocks | not a cube theorem; additive squares are a different open problem |
| C. unbounded diameter + uniform recurrence \(\Rightarrow\) cubes | FALSE (Cassaigne) |
| ACF \(\Rightarrow\) bounded discrepancy for a modular-to-integer bridge | FALSE (Ardal contrappositive) |

---

## 4. Level sets and \(G(i,n)\)

For fixed \(n,c\),
\[
F_{n,c}=\{i:S(i+n)-S(i)=c\}.
\]
An additive cube is a 3-term arithmetic progression of **difference exactly \(n\)** inside some \(F_{n,c}\).

**Fixed \(n\).** The map \(i\mapsto S(i+n)-S(i)\) takes values in a finite interval of length \(O(n)\). It is a sliding-window sum of an \(m\)-automatic sequence, hence \(m\)-automatic. Emptiness of \(\{i:G(i,n)=G(i+n,n)=G(i+2n,n)\}\) is therefore decidable for each fixed \(n\) (Walnut / a product automaton). That is not a uniform procedure over all \(n\).

**Moving \(n\).** \(G(i,n)=S(i+n)-S(i)\). The sequence \(S\) is \(m\)-regular (previous kernel report). Addition \((i,n)\mapsto i+n\) is \(m\)-synchronized. Composition of a regular sequence along a synchronized path remains regular, so \(G\) is \(m\)-regular as a function of the pair \((i,n)\). The obstruction is not “\(G\) fails to be regular”. The obstruction is that the **integer** slice
\[
G(i,n)=G(i+n,n)=G(i+2n,n)
\]
is a bivariate regular zero set, and generic regular emptiness is undecidable (Allouche–Shallit 5.2). This run does not attempt to decide it. The special hypotheses (primitive, constant length, integer-valued, ACF) were used instead to force a global bound on \(p_T\), which failed as a theorem and succeeded as an exhaustive \(m=3\) classification.

**Geometry on controls.** On the maximizer, the level set at the cube length \(n=222\) **does** contain \(i,i+n,i+2n\) on the prefix of length 1648, and contains no such progression on the ACF prefix of length 1647. Cassaigne’s level sets for \(n\le 64\) have no such progression. Famous \(n=8\): the weight colouring has many monochromatic 3-APs, almost none with difference 8 (Theorem AF).

Return-word weights: not a function of length (previous report). This run did not find a semigroup obstruction that forces three consecutive equal-length equal-weight return words under primitivity. Cassaigne remains the negative control.

---

## 5. Spectral case split

Incidence of the \(m=3\) maximizer: eigenvalues \(3\) (Perron), \(1+\sqrt{2}\) (modulus \(>1\)), \(1\), \(1-\sqrt{2}\) (modulus \(<1\)). Case III, expanding transverse direction, longest ACF prefix in the census. Expansion does not prevent cubes; it also does not produce a Krylov-style forcing theorem (Theorem Z already).

Case I (all non-Perron \(\lvert\lambda\rvert<1\)): bounded discrepancy, bounded \(p_T\), Ardal cubes. Already killed as an ACF class.

Case II (roots of modulus 1, not the Perron root): the maximizer has eigenvalue \(1\). Combined with expansion it still cubes.

The desired contradiction “expansion + finite alphabet + uniform recurrence + no equal triple \(\Rightarrow\) impossible” is **not proved**. The maximizer is consistent with the implication being true (it does cube) but is not a proof.

---

## 6. Finite substitution space and \(B(m)\)

For each fixed \(m\) there are \(4^{4m}\) maps on a 4-letter alphabet. Theorem AD: \(B(m)\) is a maximum over a finite set, finite if and only if every primitive prolongable map cubes.

| \(m\) | \(\#\) maps | \(B(m)\) | status |
|---|---|---|---|
| 2 | \(4^8=65536\) | **119** | exhaustive, previous run (famous map) |
| 3 | \(4^{12}=16{,}777{,}216\) | **1647** | exhaustive, this run (Thm AG) |
| 4 | \(4^{16}\approx 4.29\cdot 10^9\) | unknown | not exhausted |
| \(\ge 5\) | larger | unknown | not exhausted |

A theorem \(B(m)<\infty\) for every \(m\) would solve the primitive uniform subclass. We have it only for \(m=2,3\). No evidence was obtained that \(B(m)\) is unbounded as a function of \(m\): that would require a sequence of maps \(\sigma_m\) with ACF prefixes \(\to\infty\), which was not constructed. Compactness in \(m\) is not available from a finite list of two values.

Higher-block recoding \(\sigma^k\) produces an \(m^k\)-uniform map on the same alphabet. It does not uniformize Cassaigne (Theorem X/Y). It does not reduce the \(m=4\) search to \(m=3\).

---

## 7. Smallest counterexamples / maximizers

| claim | smallest / strongest witness |
|---|---|
| Conjecture A without automaticity | Cassaigne \(\varphi_0^\omega(0)\). **KNOWN.** |
| “unbounded diam + UR \(\Rightarrow\) cubes” | same |
| “ACF \(\Rightarrow\) bounded discrepancy” | same (Ardal contrappositive) |
| VdW \(d=n\) | Cassaigne \(n=8\): 524 mono 3-APs, 0 with \(d=n\) |
| automatic + unbounded \(p_T\) + still cubes | TM-like \(012\), ACF prefix 18; Rudin–Shapiro ACF 2 |
| automatic + bounded \(p_T\) + cubes | vtm (non-uniform), Thue–Morse |
| longest primitive 2-uniform ACF prefix | famous, 119 |
| longest primitive 3-uniform ACF prefix | code 12507201 seed 1, **1647** |
| second 3-uniform | 16376976 seed 0, 1204, cube \((539,222,229)\) |
| expanding-spectrum long survivor | the maximizer itself (\(\lvert\lambda_2\rvert=1+\sqrt{2}\)) |
| \(\{0,1,4\}\) 2-uniform | ACF 108, cube \((2,63,105)\) |

No pair (primitive automatic, no cube) survived a serious cap.

---

## 8. Computational certificates

Reproduce:
```
make bin/pt_verify
python3 python/pt_acf.py
./bin/pt_verify | tee data/pt_verify_cpp.txt
python3 python/pt_verify.py
```

The exhaustive census (`./bin/pt_scan`) is not part of `make test`; its output is `data/pt_scan3.txt`. The verifier recounts the 16{,}777{,}216-map **filters** (prolongable / primitive / 3-sum), rebuilds all 86 cap-243 survivors at cap 2187 trying every prolongable seed, and lex-first-cubes the maximizer.

Matched dual integers (Python / C++, exact):

| key | value |
|---|---|
| n_prol / n_prim / n_3sum_prol / n_tried | 11468800 / 6993126 / 9992241 / 6116013 |
| n_surv_243 / n_surv_2187 | 86 / 0 |
| best2_acf, best2_code, best2_seed | 1647, 12507201, 1 |
| maximizer images | 100 102 132 332 |
| maximizer T | (1,3,6,8) |
| maximizer cube | (982, 222, 229) |
| maximizer charpoly | \(t^4-6t^3+10t^2-2t-3\) |
| maximizer \(p_T(1,8,16,64)\) on ACF prefix | 4, 17, 35, 81 |
| maximizer diam(64) | 102 |
| famous cube / ACF | (45, 25, 35) / 119 |
| famous ACF-prefix \(p_{\max}\) | 18 |
| Cassaigne length 2048 | ACF, \(p_{\max}=66\), 0 triples |
| vtm cube / \(p_{\max}\) | (0, 3, 3) / 3 |
| TM-like cube / ACF / \(p_{\max}\) | (0, 132, 132) / 18 / 15 |
| 01440 cube / ACF | (2, 63, 105) / 108 |
| VdW famous \(n=8\) any / \(d=n\) | 372 / 2 |
| VdW Cassaigne \(n=8\) any / \(d=n\) | 524 / 0 |
| \(p\le\mathrm{diam}+1\) failures | 0 |

---

## 9. Literature audit

**KNOWN, used:**

- Ardal–Brown–Jungić–Sahasrabudhe, Integers 2012, Thm 5: bounded additive complexity \(\Rightarrow\) additive \(k\)-powers. Contrapositive: ACF \(\Rightarrow\) unbounded \(p_T\).
- Banero, *Abelian complexity and additive complexity* (2013): related complexity functions; no automatic-ACF theorem.
- FSTTCS 2024 / arXiv:2410.02409: TM-like \(0\mapsto 01,\ 1\mapsto 12,\ 2\mapsto 20\) has \(\rho_{\mathrm{add}}(n)=2\lfloor\log_2 n\rfloor+3\) (unbounded, 2-regular). vtm has **bounded** additive complexity (block sums in \(\{n-1,n,n+1\}\)) and unbounded abelian complexity, and is **not** uniform. Thm 29 is Ardal.
- Dekking 1979: abelian=additive cubes unavoidable over 2 letters.
- Cassaigne–Currie–Schaeffer–Shallit: infinite ACF over \(\{0,1,3,4\}\), non-uniform, not automatic (Thm X).
- Liétard–Rosenfeld / this project: exhaustive \(m=2\) failure, best prefix 119.
- Allouche–Shallit TCS 1992 Thm 5.2: generic \(k\)-regular zero emptiness undecidable. Not used as an attack.
- Shallit, *Logical Approach*, Thm 10.6.1: synchronized summatory iff bounded (previous Thm AA).

**No source found** for “automatic + additive-power avoidance \(\Rightarrow\) bounded additive complexity” except the tautology that ACF already forbids bounded \(p_T\) by Ardal. No source found for an exhaustive \(m=3\) classification on \(\{0,1,2,3\}\); the previous kernel report explicitly recorded that \(4^{12}\) was not run.

**Novelty of this run:** Theorems AD–AF are ELEMENTARY. Corollary AH is a DIRECT COROLLARY. Theorem AG (\(B(3)=1647\)) is **UNVERIFIED NOVELTY** as a packaged computational classification. Do not inflate to a proof for all \(m\), to a proof of Conjecture A, or to a solution of the infinite problem.

---

## 10. Why previous approaches fail, and why this one stops here

- Local dead-ends, extension graphs, desubstitution, quotients, triples, defect graphs, Krylov escape, two-image sums, Pisot-uniform, generic \(\mathrm{rank}(K_T)\), generic regular zeros: previous reports.
- Bounded \(p_T\) as a **sufficient** condition for cubes is Ardal, already known. Using it as a **necessary** condition under automatic ACF is Conjecture A, equivalent to nonexistence (AH).
- The \(m\)-kernel decides prefix squares on the famous map and does not decide cubes (Thm AC). This run did not chase the third block.
- Van der Waerden, modular-to-integer, and “recurrence + unbounded diameter” fail for documented reasons (AF, Ardal, Cassaigne).
- Compactness at fixed \(m\) is a pigeonhole, not a construction of an infinite ACF automatic word from a sequence of changing maps.

The remaining global question is still: does some primitive \(m\)-uniform map for \(m\ge 4\) avoid cubes forever? Finite classification is possible in principle for each \(m\) and infeasible at \(m=4\) without a new cut.

---

## 11. Does this solve the original problem?

**NO.**

Remaining bottleneck: either prove \(B(m)<\infty\) uniformly in \(m\) (or for all \(m\ge 4\) by a structural argument), or produce one primitive uniform ACF infinite word. Exhaustive \(m=3\) closes that base. Cassaigne remains the only known infinite ACF construction and is not automatic. The finite-language growth rate \(\lambda\approx 3.52\) still suggests infinite ACF words exist; they are not primitive uniform of base 2 or 3.

---

## 12. ENDGAME

**C**

Not A: no proof that every primitive uniform word over \(\{0,1,2,3\}\) has an additive cube (open for \(m\ge 4\)).  
Not B: Conjecture A was not proved; it is equivalent to A.  
Not D: compactness at fixed \(m\) is Theorem AD (elementary equivalence), not a new effective reduction.  
Not E: no infinite automatic ACF candidate; the maximizer cubes at 1648.  
Not F as a counterexample to automatic Conjecture A: no automatic ACF word with unbounded \(p_T\) was found. The non-automatic counterexample (Cassaigne) is KNOWN.  
Not G: \(B(3)<\infty\) with an exact value is a genuine fixed-base theorem, computationally verified, dual-checked.

The desired breakthrough — avoidance of equal block-sum triples forcing bounded additive complexity for primitive automatic words — did not survive as a theorem and did not die by an automatic counterexample. What survived is the finite classification of base 3.
