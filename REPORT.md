# Additive cubes over {0,1,2,3}: a computational attack

**Date:** 2026-09-04  
**Problem status:** OPEN (confirmed independently; see `NOVELTY_AUDIT.md`).  
**This document’s claims are tagged** PROVED / COMPUTATIONALLY VERIFIED / STRONGLY CONJECTURED / ALREADY KNOWN / NEW.

---

## 1. Problem statement

An *additive cube* in a word \(a_0 a_1 \cdots\) over \(\mathbb{Z}\) is a factor of three consecutive blocks of equal length and equal sum: there exist \(i\ge 0\), \(d\ge 1\) with
\[
\sum_{t=0}^{d-1} a_{i+t}
= \sum_{t=0}^{d-1} a_{i+d+t}
= \sum_{t=0}^{d-1} a_{i+2d+t}.
\]
**Question (Rao 2015; Rao–Rosenfeld 2018; Liétard–Rosenfeld 2020).**  
Does there exist an infinite additive-cube-free (ACF) word over the alphabet \(\{0,1,2,3\}\)?

Green’s Open Problem 15 (Fox, 2005) asks for a Lipschitz \(f:\mathbb{N}\to\mathbb{Z}\) whose graph in \(\mathbb{Z}^2\) has no 3-term arithmetic progression. That is the *additive square* problem. Additive cubes correspond to 4-APs in the graph of the partial-sum function. The 4-AP problem is solved on every difference alphabet except the affine class of \(\{0,1,2,3\}\). This report attacks that exceptional case. It does **not** resolve Green 15.

---

## 2. Literature status (Phase 0)

**ALREADY KNOWN.** As of 2026 the infinite problem is open. Liétard–Rosenfeld proved ACF infinitude over every 4-element number alphabet not affine-equivalent to \(\{0,1,2,3\}\). Cassaigne–Currie–Schaeffer–Shallit gave the seed morphism on \(\{0,1,3,4\}\). Rao constructed many 3-letter examples and a finite word of length \(\sim 1.4\times 10^5\) over \(\{0,1,2,3\}\). Liétard (2020 thesis) constructed length **70,880,000** by “Up-and-Down” priority reversal and reported that a Cupisca morphism on this alphabet, if it exists, has size at least 7. No subsequent record was found. Full references: `NOVELTY_AUDIT.md`.

---

## 3. Experimental methodology

- Prefix-sum verifier, incremental cubes-ending-at-\(n\), independently matched to a Python brute-force triple loop on all words of length \(\le 6\) and on random longer words. Sanity check: 20,000-letter prefix of the Cassaigne morphism over \(\{0,1,3,4\}\) is ACF.
- Exact DFS enumeration of the ACF trie.
- Chronological backtracking with fixed and reversing letter orders (Liétard’s engine).
- Exhaustive morphism scans (uniform size 1–2; Cassaigne shapes) and random evolutionary search for larger size.
- Adversarial loop: any morphism that survives a cap is immediately pushed until a cube appears.

Code: `src/`. Commands: `SEARCH_ALGORITHMS.md`. Tests: `./bin/acf_tool test` and `python3 python/brute_verify.py`.

---

## 4. Strongest computational results

### 4.1 NEW — shortest right-crucial ACF words have length 14
**COMPUTATIONALLY VERIFIED.**

Every ACF word of length \(\le 13\) over \(\{0,1,2,3\}\) has an ACF one-letter extension. There are exactly **8170** ACF words of length 14 with no ACF extension (4083 orbits under reversal and complement \(a\mapsto 3-a\)).

Witness, verified by both implementations:
\[
w = 00103010301100.
\]
Appending \(0,1,2,3\) creates cubes of block lengths \(1,2,5,4\) respectively. The \(+0\) obstruction is an ordinary cube `000`; the other three are genuinely additive.

List: `data/deadends14.txt`.

**Novelty.** Abelian-cube crucial words over 4 letters have minimal length 20 (Glen–Simpson–Smyth). No published table of additive-cube crucial words over \(\{0,1,2,3\}\) was found. The comparison 14 vs 20 is the expected direction: additive cubes are weaker prohibitions, so the trie dies earlier.

### 4.2 NEW — enumeration through length 12
**COMPUTATIONALLY VERIFIED.**

\(c_n =\) number of ACF words of length \(n\):
\[
\begin{align*}
c_0&=1,\ c_1=4,\ c_2=16,\ c_3=60,\ c_4=228,\ c_5=864,\\
c_6&=3152,\ c_7=11468,\ c_8=42070,\ c_9=150560,\\
c_{10}&=538214,\ c_{11}=1924738,\ c_{12}=6772220.
\end{align*}
\]
**PROVED:** \(c_n\) equals the number of ordinary cubefree quaternary words (OEIS A051043) for \(n\le 5\), because the only possible additive cubes have \(d=1\). First divergence: \(c_6=3152\) vs \(3264\). Growth rate \(c_{n}/c_{n-1}\approx 3.52\) at \(n=11\).

### 4.3 NEW as a certificate — every 2-uniform morphism fails by length 119
**COMPUTATIONALLY VERIFIED.** Sharpening of Liétard’s size \(\ge 7\).

All \(65536\) two-uniform morphisms, all seeds, cap 250: no ACF iterate of length 250. Worst prefix length 119.

### 4.4 Known mechanism, reconstructed — Up-and-Down
**ALREADY KNOWN** as a method; **COMPUTATIONALLY VERIFIED** reconstruction.

| generator | length | mean | note |
|---|---|---|---|
| fixed order 0>1>2>3 | 24,396 | 0.959 | exact match to Liétard’s stall |
| Up-and-Down period 1000 | 65,986 | 1.494 | budget-limited |
| Up-and-Down period 2000 | 400,000 | 1.504 | hit cap; ACF |
| Up-and-Down period 5000 | 400,000 | 1.501 | hit cap; ACF |

Record remains **70,880,000** (Liétard). We did not retrieve that file.

A 400,000-letter Up-and-Down word contains **every** ACF factor of length 6 (\(p(6)=3152=c_6\)). **COMPUTATIONALLY VERIFIED** that these objects are not prefixes of low-complexity morphic words.

---

## 5. Proofs (human-readable)

### Theorem A (PROVED, ALREADY KNOWN)
Every eventually periodic infinite word contains an ordinary cube \(uuu\), hence an additive cube. Infinite ACF words, if they exist, are aperiodic.

### Theorem B (PROVED)
For \(n<6\), ACF \(\Leftrightarrow\) ordinary cubefree. Proof: \(3d\le n\) forces \(d=1\).

### Theorem C (PROVED, elementary)
A uniform morphism whose images take at most two distinct sums cannot have an infinite ACF iterate: the block-sum sequence is an infinite word over a 2-letter integer alphabet, hence contains an abelian cube (Dekking 1979), which lifts to an aligned additive cube.

Details: `proofs/THEOREMS.md`.

---

## 6. Failed hypotheses and repairs

| Hypothesis | Fate |
|---|---|
| Random size-7–12 morphisms survive \(10^3\) letters | Killed (best \(\sim 80\)) |
| \(\varphi_0\) with \(4\mapsto 2\) is Cupisca | Killed (size 2 exhaustive) |
| Shape \(0\to001,1\to031,2\to1,3\to233\) is Cupisca | Survived 300, killed at 378 (\(d=59\), unaligned) |
| Long Up-and-Down words are hidden morphisms | Killed by factor complexity |
| Finite ACF words have mean \(3/2\) | Killed (mean \(0.96\) at length 24396). **Repaired:** infinite words have mean \(3/2\) (open) |
| Short reversal period is best | Killed (period 100 worse than fixed order) |

Full writeup: `COUNTEREXAMPLES.md`.

---

## 7. Distinction: known vs new

| Item | Tag |
|---|---|
| Problem open; 70.88M construction; size \(\ge 7\) if morphic | ALREADY KNOWN |
| Eventually periodic obstruction | ALREADY KNOWN (Theorem A) |
| Length 24,396 stall | ALREADY KNOWN, independently reproduced |
| Up-and-Down method | ALREADY KNOWN; 400k is reconstruction, not a record |
| \(c_n\) for \(n\le 5\) | ALREADY KNOWN (A051043) |
| \(c_6=3152\) and \(c_n\) for \(n=7..12\) | NEW (computational) |
| Crucial length exactly 14, 8170 words | NEW (computational) |
| 2-uniform exhaustive certificate, max prefix 119 | NEW as a reproducible certificate |
| Best (3,3,1,3) morphism dies at 378, \(d=59\) | NEW obstruction data |
| High complexity of Up-and-Down words | NEW diagnosis of a known construction type |
| Infinite ACF word over {0,1,2,3} | STILL OPEN |
| Green 15 | STILL OPEN, not attacked beyond the reduction |

---

## 8. Reproducibility

```bash
make
./bin/acf_tool test
python3 python/brute_verify.py
./bin/acf_tool characterize 12
./bin/dump_deadends                          # writes data/deadends14.txt
./bin/acf_tool grow fixed 2000000 30000
./bin/acf_tool grow updown 20000000 400000 2000
./bin/acf_tool uniform 2 250
./bin/acf_tool shape 3,3,1,3 300
./bin/run_candidate                          # kills 0→001,1→031,2→1,3→233 at 378
./bin/acf_tool analyze data/word_updown_p2000_n400000.txt
```

Environment used: g++ 13.3, C++17, `-O3`, 4 cores, 15 GB RAM.

---

## 9. Answer to the governing question

**What, exactly, did we discover that was not already known?**

The ACF language over \(\{0,1,2,3\}\) is extendable from every word of length \(\le 13\), and the first right-crucial words appear at length **14**, where there are **8170** of them. Combined with the first public terms of the counting sequence past the ordinary-cubefree regime (\(c_6=3152\), through \(c_{12}=6772220\)) and an exhaustive certificate that no 2-uniform morphism produces an ACF iterate of length 250, this is a structural description of the finite problem. It does not produce an infinite word, does not beat 70.88M, and does not decide Green’s Problem 15.

The main problem remains open. The correct next targets, if one continues, are (i) a human description of the 8170 crucial words, (ii) a growth-rate proof, and (iii) morphisms of size \(\ge 7\) with Cassaigne-like spectrum — not another unstructured long backtrack.
