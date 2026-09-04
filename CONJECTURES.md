# Conjectures

Format: statement, status, attacks performed, remaining tests.

---

## C1. (Main open problem; NOT ours)
There exists an infinite additive-cube-free word over {0,1,2,3}.

**Status:** OPEN (literature). We did not resolve it.

**Evidence for:** unbounded finite lengths are known to at least 70.88M (Liétard) and independently to 400,000 here; exponential growth of the finite language through n=12 with λ≈3.52; no dead-ends before length 14, and 8170 dead-ends at 14 still leave millions of extendable length-14 words (c₁₂=6,772,220 already).

**Evidence against:** no morphism, including exhaustive size ≤2 and a large size-3 Cassaigne-shape class, produces a long ACF iterate; Liétard–Rosenfeld found morphisms “immediately” on every other 4-letter alphabet.

**We do not conjecture a sign.**

---

## C2. The language of finite ACF words over {0,1,2,3} has positive exponential growth rate
**Status:** STRONGLY CONJECTURED, not proved.

Empirical \(c_{n+1}/c_n \approx 3.52\) at n=11, and \(c_n\) is increasing through n=12. A proof would imply an infinite ACF word by König’s lemma.

**Attack:** look for a collapse of branching. Through n=13 the dead-end fraction at length 14 is 8170 / c₁₄. We did not compute c₁₄ exactly; c₁₂=6.77e6 and branching ~3.5 suggest c₁₄ is tens of millions, so the dead-end fraction at 14 is small (~10⁻⁴). Not a collapse.

---

## C3. There is no purely morphic infinite ACF word over {0,1,2,3}
**Status:** STRONGLY CONJECTURED for small size; open in general.

**Attacks:**
- All 2-uniform morphisms fail by length 119 (exhaustive).
- Cassaigne shapes 2,2,1,2 and 2,2,2,1 fail by length 112 and 81.
- Shape 3,3,1,3: one prefix of length 300, killed at 378.
- Random size 7–16: fail by length <90.

**Remaining:** non-uniform size ≥7 with a Cassaigne-like spectrum (at most two expanding eigenvalues). That is exactly Liétard’s leftover. Our random search is not a proof.

---

## C4. The minimal length of a right-crucial ACF word over {0,1,2,3} is 14
**Status:** COMPUTATIONALLY VERIFIED (this project). See RESULTS.md.

**Attack:** exhaustive trie through depth 14. Zero crucial words of length ≤13; 8170 of length 14; eight examples independently verified by a Python brute-force checker.

This is the analogue, for additive cubes, of the “minimal crucial word” theorems of Evdokimov–Kitaev / Glen–Simpson–Smyth for *abelian* powers. Over 4 letters, the abelian-cube crucial length is 20. Additive cubes, being a weaker prohibition, produce earlier dead-ends.

---

## C5. Every infinite ACF word over {0,1,2,3} has Cesàro mean 3/2
**Status:** CONJECTURED. Finite counterexamples to the unrestricted mean statement exist (length 24396, mean 0.959), so the conjecture is only for infinite words.

**Attack:** the finite counterexample is why the repaired form is needed. An infinite word with mean ≠ 3/2 has linear discrepancy, hence arbitrarily long monotone stretches of the walk S(n)−μn; we did not prove that those force a 4-AP in S.

---

## C6. Uniform morphisms of length k=3 over {0,1,2,3} all fail
**Status:** NOT SETTLED. 4¹²=16,777,216 morphisms; we did not exhaust. Random samples and the related shape 3,3,1,3 failed. Liétard’s “size ≥ 7” if true already implies C6.

---

## C7. (Green 15 / Fox / Pirillo–Varricchio)
There exists a Lipschitz f:ℕ→ℤ whose graph has no 3-AP.

**Status:** OPEN. Equivalent to additive *squares* over a finite integer alphabet. **Out of reach of this project’s cube-focused computation.** We did not attack it beyond recording the reduction.

---

## C8. Up-and-Down with a sufficiently large reversal period produces ACF words of unbounded length
**Status:** CONJECTURED by the construction’s behaviour, **not proved**. Hitting a cap of 400,000 with periods 2000 and 5000 is consistent with Liétard’s 70.88M but does not imply infinitude. The method has no finite mathematical rule independent of backtracking, so even an infinite run would not be an “explicit infinite word” in the sense of Phase 7.

---

## C9. Large cubes in a primitive uniform word have ancestor v in a finite union of rank-2 affine lattices
**Status:** CONJECTURED (surviving remnant of the killed desubstitution dream).

For fixed `m`-uniform `σ`, cube types `(r,s)` are finite and `v=ψ_X−ψ_Y` satisfies `T·v=−Δδ` with `Δδ` from a finite defect set (Theorems K, L, **proved**). The conjecture is that this is the *only* complexity: `‖v‖₁` may grow (witness: `‖v‖₁=38` at `d=1767`) but the direction stays in `Λ(Δn,Δδ)`. This does **not** imply that a cube must occur.

**Killed companion:** “`v` itself has bounded `L¹`” and “`v∈L(A)`”. See `DESUBSTITUTION_REPORT.md`.

---

## C12. There is no k-automatic infinite ACF word over a 4-letter integer alphabet
**Status:** STRONGLY CONJECTURED for 2-automatic 4-letter maps (exhaustive m=2); open in general.

This is the uniform problem (Theorems V, W). Cassaigne is morphic and not automatic (Theorem X), so it is not a counterexample. Sampled 2-uniform maps on 5, 6, 8 letters coded onto `{0,1,3,4}` failed by length 27 (40 each, not a theorem). Affine / Pisot / two-sum strata are already excluded.

**Attack:** Theorem X/Y kill same-word uniformization. They do not kill every automatic candidate.

Kernel/regular attack (Theorems AA–AC): synchronized summatory is exactly bounded \(p_T\) (already Ardal cubes). Prefix T-squares of the famous map form an infinite 4-adic family and still do not yield cubes. Structured m=3 4-letter samples (3953+4012 primitive 3-sum maps) all cubed by length 160. Still not a proof for \(m\ge 3\).

---

## C13. The CCSS length-compensation mechanism cannot run on a uniform morphism
**Status:** PROVED (Theorems V, X, Y). Not a conjecture.

Equal image length forces equal ancestor length for complete images. The Cassaigne identity `|φ(w)|=2|w|-|w|_3` has no uniform analogue. This does **not** prove that no uniform ACF morphism exists.

---

## C10. Quotienting by `ker(T)` produces a finite-state obstruction
**Status:** KILLED (Theorems M–P; `QUOTIENT_REPORT.md`). The surviving exact state is the Krylov package of `T` under `M^⊤`, typically of dimension 3 or 4, plus finite fragments. On cubes the first two coordinates `(1·v, T·v)` are bounded; the extra `T_2·v` takes many values (famous: `[-10,8]` on 442 cubes) and is **not** an obstruction (Theorem Z).

---

## C11. Shared-middle triples yield a finite T-weighted factor-graph obstruction
**Status:** KILLED (`TRIPLE_REPORT.md`). \(\omega(X)=\omega(Y)=\omega(Z)\) is the original cube condition. Finite extra data: \(m^2\) types and \(\le 4\) fragment letters. Unbounded: \(\omega\). Coupling filters squares to cubes but is not an obstruction (Cassaigne has squares, no cubes; famous has both).

---

## C14. The bivariate zero set of \((D_1,D_2)\) is m-automatic for every primitive uniform map
**Status:** KILLED as a general claim; OPEN for this special pair.

Generic k-regular zeros are not regular (Allouche–Shallit 5.2 / Krenn–Shallit). Famous prefix \(D_1(0,n)=0\) *does* look 2-automatic (kernel 9). Prefix \(D_1=D_2=0\) is empty in length 4096. Modular \((D_1,D_2)\) is automatic and larger than the integer set. No finite automaton for integer cubes was constructed.

## C15. Every primitive 3-letter uniform numerical word has additive cubes (Layer B)
**Status:** COMPUTATIONALLY SUPPORTED for m=2 (exhaustive) and m=3 cap 81; not proved. Best ACF prefix 108 on \(\{0,1,4\}\). Rao’s 3-letter ACF word is non-uniform, so it is not a counterexample to the uniform statement.
