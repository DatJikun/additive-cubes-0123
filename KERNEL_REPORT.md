# Kernel / regular attack on uniform additive cubes

**Status of the infinite problem:** OPEN.  
**ENDGAME: C** — structural obstruction, stronger than the previous template Endgame C: the m-kernel chain produces an explicit finite-dimensional regular representation of block-sum differences whose **integer** zero set is not finite-state in the unbounded-discrepancy class, while the only class in which that zero set is FO / synchronized is the already-killed bounded-\(p_T\) class. Doubling (additive squares on prefixes) **does** close under the kernel; the third block \(3d\) does not.

Control A is the famous 2-uniform map \(0\mapsto 32,\ 1\mapsto 31,\ 2\mapsto 20,\ 3\mapsto 01\). Control B is Cassaigne (non-uniform, not automatic).

---

## 1. Executive verdict

Proved:

- The T-coded sequence \(a_i=T(U[i])\) of a primitive \(m\)-uniform fixed point is \(m\)-automatic. Its prefix-sum \(S(n)=\sum_{j<n}a_j\) is \(m\)-regular, not automatic (\(S(n)\sim\mu n\)).
- Explicit linear representation: \(\psi(n)=\mathrm{Parikh}(U[0:n))\) obeys the kernel recurrence \(\psi(mn+r)=M\psi(n)+\mathrm{Parikh}(\sigma(U[n])[0:r])\), and \(S(n)=T\cdot\psi(n)\). Aligned doubling: \(S(mn)=T_m\cdot\psi(n)\) with \(T_m=(M^\top)^{m\text{-powers}}T\). For \(m=2\), \(S(2n)=T_2\cdot\psi(n)\).
- Prefix additive squares of the T-word are the zeros of the linear form \(L=(2T-T_2)\cdot\psi(n)\). On the famous map, \(LM^2=2L\), so those zeros are invariant under \(n\mapsto 4n\). Combined with the explicit zero \(n=70\), there is an infinite 4-adic family of prefix T-squares.
- \(S\) is \(m\)-synchronized if and only if the discrepancy \(\Delta(n)=S(n)-\mu n\) is bounded (Shallit 10.6.1 + rational frequencies of primitive uniform maps). That class has bounded additive complexity, hence additive cubes by Ardal–Brown–Jungić–Sahasrabudhe. Walnut / FO / synchronized cube detection cannot see the interesting class.
- Modular relations \(D_1\equiv D_2\equiv 0\pmod M\) are automatic and strictly larger than integer zeros (famous: 2554 pairs mod 2 vs 6 integer zeros in a 7800-pair window).
- Candidate D is false: 140 lengths \(d<200\) are not T-cube lengths on the famous word.
- No automatic ACF candidate over \(\{0,1,2,3\}\) survived a structural \(m=3\) sample. Exhaustive prolongable 2-uniform 3-letter maps on \(\{0,1,2\}\) all cube by length 26; the affine-inequivalent alphabet \(\{0,1,4\}\) reaches ACF prefix 108 then cubes.

Falsified as forcing theorems: Candidates A, C (nonempty-zero half), D, E. Candidate B is **true on the prefix-square diagonal** for the famous map and does **not** produce cubes (0 prefix T-cubes in length 4096).

The original infinite problem is not solved.

---

## 2. Main theorem / counterexample

**Theorem AA** (DIRECT COROLLARY of Shallit, *Logical Approach*, Thm 10.6.1).  
Let \(U\) be a primitive \(m\)-uniform numerical word, \(a=T\circ U\), \(S\) its summatory, \(\mu\in\mathbb{Q}\) the mean. Then \(S\) is \(m\)-synchronized if and only if \(\Delta(n)=S(n)-\mu n\) is bounded, if and only if \(p_T\) is bounded. In that class additive cubes exist (Ardal et al. 2012).

**Theorem AB** (ELEMENTARY).  
For an \(m\)-uniform fixed point, \(\psi(mn)=M\psi(n)\) and \(S(mn)=T_2\cdot\psi(n)\) with \(T_2=M^\top T\). In particular the Krylov orbit \(T,T_2,T_3,\ldots\) **is** the aligned kernel of \(S\).

**Theorem AC** (PROVED for the famous map; the identity \(LM^2=2L\) is a 4×4 multiplication).  
Let \(L=2T-T_2\). Prefix T-squares are exactly \(\{n:L\cdot\psi(n)=0\}\). On the famous morphism \(LM^2=2L\), hence \(L\cdot\psi(4n)=2L\cdot\psi(n)\). Since \(L\cdot\psi(70)=0\), \(L\cdot\psi(70\cdot 4^k)=0\) for every \(k\ge 0\). Infinite 2-recognizable family of prefix additive squares. Prefix additive cubes: none in the length-4096 T-prefix.

**Obstruction.** Cubes require also \(S(i+3d)-S(i+2d)=S(i+2d)-S(i+d)\). The index \(i+3d\) is not an \(m\)-power shift of \(i+d\). The doubling automaton does not constrain \(\psi(3n)\).

---

## 3. m-kernel structure

Famous DFAO (MSB, start 2): \(0\to(3,2),\ 1\to(3,1),\ 2\to(2,0),\ 3\to(0,1)\). Independent iterate check: 0 failures on 512 indices. LSB recurrence \(U[2n+r]=\sigma(U[n])[r]\) is the same map.

| sequence | 2-kernel fingerprints (prefix 48 / 32) | growth |
|---|---|---|
| \(U\) letters | saturates at **37** | r=6,7,8 all 37 |
| \(a=T\circ U\) | saturates at **37** | identical |
| \(\chi_{\mathrm{sq}}\) (prefix T-squares) | saturates at **9** | r=6,7 both 9 |
| \(\chi_{\mathrm{cu}}\) (prefix T-cubes) | **1** | the zero sequence |
| Thue–Morse | saturates at **2** | |
| period-doubling | saturates at **4** | |
| Cassaigne letters, base 2 | **127** = data limit | \(1,3,7,15,31,63,127\) |
| Cassaigne letters, base 3 | **121** = data limit | not automatic |

Cassaigne’s letter kernel grows until the sample is exhausted; famous saturates. That is the automaticity distinction (Theorem X already: irrational PF). Do not claim 37 is the minimised DFAO size; it is a lower bound from length-48 windows that has stopped growing.

Incidence
\[
M=\begin{pmatrix}0&0&1&1\\0&1&0&1\\1&0&1&0\\1&1&0&0\end{pmatrix},\quad
T=(5,4,2,1),\quad T_2=(3,5,7,9),\quad L=(7,3,-3,-7).
\]
Krylov rank of \(\mathrm{span}\{1,T,T_2,\ldots\}=3\). Sampled \(\mathbb{Q}\)-rank of 127 kernel windows of \(S\) (length 48) is **7**: the kernel module of \(S\) is strictly larger than the 4-dimensional Parikh space, because offsets \(q\neq 0\) adjoin the automatic fragment \(T(\sigma(U[n])[0:r])\).

---

## 4. Block-sum representation

\[
P(i,d)=S(i+d)-S(i)=T\cdot\bigl(\psi(i+d)-\psi(i)\bigr),
\]
\[
D_1(i,d)=2S(i+d)-S(i)-S(i+2d),\qquad
D_2(i,d)=2S(i+2d)-S(i+d)-S(i+3d).
\]
Clean form: \(D_2(i,d)=P(i+d,d)-P(i+2d,d)\).

On the diagonal \(i=0\), \(m=2\):
\[
D_1(0,n)=L\cdot\psi(n),\qquad L=2T-T_2.
\]
Dual C++/Python: 0 failures for \(n\le 400\). \(\psi\) itself matches the iterate Parikh for \(n\le 400\) (Python LSB recurrence vs iterate).

This is **affine finite-state** of dimension 4 (Parikh) plus a 4-state DFAO, not a finite automaton on the integer value of \(D_1\). Finite-state modulo \(M\) is the reduction of Parikh modulo \(M\).

---

## 5. Zero-set analysis

| object | classification |
|---|---|
| \(a_i\) | 2-automatic |
| \(S(n)\) | 2-regular, not automatic, not 2-synchronized (famous \(\lvert S(n)-3n\rvert\) reaches 63 inside length 4096, order \(\sqrt{n}\)) |
| \(D_1,D_2\) as functions of \((i,d)\) | 2-regular (composition of regular \(S\) with synchronized addition) |
| \(\{n:D_1(0,n)=0\}\) famous | 2-automatic on this evidence (kernel 9, closed under \(\times 4\)) |
| \(\{n:D_1(0,n)=D_2(0,n)=0\}\) famous | empty in length 4096 (no prefix T-cubes) |
| \(\{(i,d):D_1=D_2=0\}\) | nonempty (ordinary cubes exist); not shown to be 2-automatic; modular projections are 2-automatic and strictly larger |
| \(\bigcup_n R_\sigma(n)\times\{n\}\) | **not** claimed semilinear. Diameter of \(R_\sigma(n)\) grows (\(\mathrm{diam}(R(64))=32\)), which is *compatible* with semilinear slices (sublinear). The projection \(\{n:(0,0)\in R_\sigma(n)\}\) has 140 gaps below 200 and is not ultimately periodic on that window; 1-dimensional semilinearity of cube lengths is therefore not supported. Cassaigne has growing diam and **no** cubes: diam growth is not an automaticity-specific obstruction. |
| generic \(k\)-regular zero set | emptiness **undecidable** (Allouche–Shallit 1992, Thm 5.2); whether a fibre is regular is undecidable (Krenn–Shallit 2022). Endgame D is blocked at this generality. Block-sum \(D_1\) is a special regular sequence; we did not produce a decision procedure for it. |

Thue–Morse: \(\lvert 2S(n)-n\rvert\le 1\), synchronized summatory, kernel of \(\chi_{\mathrm{sq}}\) saturates at 3, additive cube at \((i,d)=(0,2)\). Period-doubling: running sum not 2-synchronized (Burns arXiv:2405.17536); still has a cube at \((0,3)\).

---

## 6. Main proof or obstruction

**Where the chain closes.**  
m-kernel of \(U\) \(\to\) Parikh recurrence \(\to\) \(S(n)=T\cdot\psi(n)\) \(\to\) \(D_1(0,n)=L\cdot\psi(n)\) \(\to\) for famous, \(LM^2=2L\) \(\to\) infinite 4-adic prefix squares.

**Where it breaks for cubes.**  
A cube is \(D_1(i,d)=D_2(i,d)=0\) (aligned; unaligned: the pair lands in finite \(E_\sigma\), Theorem W). \(D_2\) uses the index \(i+3d\). Multiplication by 3 is synchronized, so \(D_1,D_2\) remain regular, but their **joint integer zero set** is not the language of the doubling automaton. The 4-adic family \(70\cdot 4^k\) consists of T-squares, not T-cubes.

**Why Walnut cannot finish the job.**  
FO[\(n\)]\(_m\) decides first-order properties of \(m\)-synchronized sequences. Theorem AA: \(S\) is synchronized iff \(\Delta\) is bounded iff \(p_T\) is bounded iff (Ardal) cubes already exist. The ACF-relevant uniform maps have unbounded \(\Delta\sim n^{\log_m\lvert\lambda_2\rvert}\) (famous: \(\lvert\lambda_2\rvert=\sqrt{2}\)).

**Why modular pigeonhole fails.**  
\(D_1,D_2\bmod M\) automatic. Famous window of 7800 pairs: 2554 zeros mod 2, 6 integer zeros. Inverse limit over all moduli is the integer zero set and is not a single finite automaton.

**Cassaigne as Control B.**  
Any theorem that only uses “unbounded \(p_T\) / growing \(\mathrm{diam} R(n)\) / regular summatory” also applies after dropping constant length, and Cassaigne is ACF. The 4-adic closure uses \(M^2\) and therefore uses uniformity. The letter-kernel growth uses non-automaticity. Those two are the automatic-specific facts.

---

## 7. Smallest counterexamples

| conjecture | smallest witness |
|---|---|
| Candidate A (3 symbols + unbounded \(p_T\) \(\Rightarrow\) cubes, without uniformity) | Rao 2015, non-uniform size 25 over 3 letters. **KNOWN.** Uniform version: no counterexample in exhaustive m=2 or m=3 cap 81; not proved. |
| Candidate B as “the square relation is not m-recognizable” | Famous prefix squares: kernel 9, closed under \(\times 4\). **False.** The cube intersection still does not follow. |
| Candidate C nonempty zeros of a regular set as a *forcing* theorem | Restates existence of cubes. Famous has cubes; Cassaigne T-analogue has none. |
| Candidate D: every large \(d\) is an \(E_\sigma\)-cube | Famous missing \(d\): 1,2,3,5,7,9,\ldots (140 values \(<200\)). Smallest missing: \(d=1\). |
| Candidate E | Restatement of aligned cubes. Not a theorem. |
| “\(\lvert\mathrm{kernel}(U)\rvert\le 4\)” | Famous saturates at 37 length-48 fingerprints. The 4-state MSB DFAO does not bound the kernel fingerprint count at 4. |
| “diam \(R(n)\sim\sqrt{n}\) obstructs semilinearity” | Sublinear growth is compatible with semilinear slices. Killed as an obstruction. Cassaigne diam\((R(64))=86\), no cubes. |
| Layer B, alphabet \(\{0,1,2\}\), m=2 | Best prolongable 3-sum map `01 12 20`, seed 1, first cube \((8,10,9)\), ACF prefix 26. 210 maps, 0 survivors at cap 80. |
| Layer B, alphabet \(\{0,1,4\}\), m=2 | `01 14 40`, seed 0, ACF prefix **108**, cube \((2,63,105)\). Not affine-equivalent to \(\{0,1,2\}\). Dies. |
| Layer B, m=3, \(\{0,1,2\}\) | Four cap-60 survivors, all cube by length 69. Best `001 122 021`, cube at cap 81: \((5,24,24)\). |
| 4-letter m=3 random primitive, 3 image-sums | Python seed 20260904: 3953 maps, 0 survive cap 160, best ACF 109 (`103 100 022 323`). C++ independent LCG: 4012 maps, best 142 (`200 100 312 332`). |

---

## 8. Computational certificates

Reproduce:
```
make bin/kernel_scan
./bin/kernel_scan | tee data/kernel_cert_cpp.txt
python3 python/kernel_regular.py
python3 python/kernel_verify.py
```

Matched dual integers (Python / C++, exact):

| key | value |
|---|---|
| dfao_fail, S2n_fail, L_fail, psi_fail | 0 |
| L | (7,3,-3,-7) |
| T2 | (3,5,7,9) |
| LM2_fail | 0 |
| L_psi_70 | 0 |
| a_kernel_fp, U_kernel_fp | 37 |
| sq_kernel_fp | 9 |
| n_prefix_squares | 13 (length-4096 T-prefix; 17 by length 8192) |
| n_prefix_cubes | 0 |
| L_absmax (n≤2000) | 124 |
| n_with_00_le64 | 24 |
| n_cube_len_lt200 / n_missing | 59 / 140 |
| pT(1), pT(16), pT(64) | 4, 20, 40 |
| first cube of U | (45,25,35) |
| first cube of a | (90,50,145) |
| mod2 / int0 | 2554 / 6 of 7800 |
| cass_2ker / cass_3ker | 127 / 121 |
| tm_Delta_max | 1 |
| tm_a_ker | 2 |
| cube_01440 | (2,63,105) |
| scan {0,1,2} m=2 n_sums≥2 | tried 480, surv 0, best 26 |
| scan {0,1,2} m=2 n_sums≥3 | tried 210, surv 0, best 26 |

Prefix T-squares in the length-8192 T-word:
`70, 74, 78, 280, 296, 312, 558, 622, 626, 630, 1120, 1184, 1248, 2232, 2488, 2504, 2520`.
Every listed \(d\) with \(8d\) in range scales: \(S(8d)=2S(4d)\). Failures: 0.

---

## 9. Literature audit

**KNOWN, used:**

- Allouche–Shallit, *The ring of k-regular sequences*, TCS 1992, Thm 5.2: emptiness of zeros of a k-regular sequence is undecidable. Summatory of k-regular is k-regular. Automatic ⇒ k-regular.
- Krenn–Shallit, TCS 2022: whether \(f^{-1}(q)\) is regular is undecidable.
- Shallit, *Logical Approach to Automatic Sequences*, Thm 10.6.1: (k,ℓ)-synchronized \(f=o(n^{\log_k \ell})\) is bounded. Carpi–Maggi synchronized sequences sit between automatic and regular.
- Burns, arXiv:2405.17536: Thue–Morse running sum is 2-synchronized; period-doubling running sum is not.
- Shallit WORDS 2021: TM running sum synchronized not automatic; \(\sum s_2(i)\) regular not synchronized.
- Dekking 1979: abelian=additive cubes unavoidable over 2 letters. **Layer A, no automaticity needed.**
- Ardal–Brown–Jungić–Sahasrabudhe, Integers 2012: bounded additive complexity ⇒ additive k-powers.
- Cassaigne–Currie–Schaeffer–Shallit: infinite ACF over {0,1,3,4}, non-uniform.
- Rao 2015: additive cubes avoidable over 3 letters by a size-25 **non-uniform** substitution. Does not give a uniform/automatic 3-letter ACF word.
- Liétard–Rosenfeld: 2-uniform exhaustive failure on {0,1,2,3}; this project reproduced it (Theorem F).
- FSTTCS 2024 / arXiv:2410.02409: additive complexity of k-automatic sequences conjectured k-regular. Does not force cubes.
- Currie–Mol–Rampersad–Shallit / Andrade–Mol: Walnut decision for additive powers in **Parikh-collinear / special numeration** morphisms. Not integer-base k for generic uniform maps with unbounded \(\Delta\).

**No source found** that primitive automatic + finite integer alphabet \(\Rightarrow\) additive cube. If that implication were known it would already close the uniform problem; it is not in the above.

**Novelty of this run:** Theorems AA–AB are DIRECT COROLLARY / ELEMENTARY. Theorem AC (4-adic prefix squares on the famous map) is **UNVERIFIED NOVELTY** as a packaged statement; the linear algebra is elementary and may be folklore in k-regular papers. The obstruction “doubling closes, \(3d\) does not” is **UNVERIFIED NOVELTY** as an explanation of why the kernel attack does not decide cubes; it is not a new decision algorithm and not a forcing theorem.

---

## 10. Why previous approaches fail

Previous Endgame C (templates): length rigidity + finite \(E_\sigma\) **restate** the cube condition on the T-word. This run uses that packaging (Theorem W) and asks what automatic structure the T-word actually has.

- Factor graphs / triples / residue types: forbidden (Rule 2); already the cube condition.
- Krylov rank: the Krylov orbit **is** the aligned kernel of \(S\) (Theorem AB). Rank 3 does not force cubes (Theorem Z, famous).
- Bounded \(p_T\): exactly the synchronized class (Theorem AA), already Ardal.
- Brute prefixes: 3-letter maps were classified by first cubes, not by “prefix = 200”.
- Semilinearity of \(R_\sigma(n)\times\{n\}\): diam growth does not obstruct; Cassaigne has the same qualitative diam growth without being automatic.

The new object that actually uses m-adic structure is \(L\cdot\psi(n)\) and \(LM^2=2L\).

---

## 11. Does this solve the original problem?

**NO.**

Remaining bottleneck: decide whether the bivariate regular pair \((D_1(i,d),D_2(i,d))\) is ever \((0,0)\) (aligned) or lands in finite \(E_\sigma\) (unaligned) for **every** primitive uniform \(\sigma\) over {0,1,2,3}, or produce one \(\sigma\) whose pair avoids \(E_\sigma\) for all \((i,d)\). Generic k-regular zero-emptiness is undecidable; this special pair is still not shown decidable. Exhaustive m=2 is already negative for ACF. Exhaustive m=3 on 4 letters was subsequently run: \(B(3)=1647\) (Theorem AG, `PT_REPORT.md`); not a decision procedure for \((D_1,D_2)\).

---

## 12. ENDGAME

**C**

Not A: no forcing theorem for a new infinite class of uniform maps (Layer B remains computational).  
Not B: no automatic ACF candidate that survives a serious cap (best 3-letter uniform ACF prefix 108; best sampled 4-letter m=3 prefix 142).  
Not D: Allouche–Shallit 5.2 blocks a generic regular-zero decision procedure; we did not specialise it to a finite algorithm for block-sum cubes.  
Not E: the obstruction is exact (doubling vs \(3d\), synchronized iff bounded \(\Delta\), modular vs integer), not a census.
