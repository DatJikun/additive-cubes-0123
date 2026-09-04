# Additive-cube research archive

Autonomous attack on additive-cube-free words over `{0,1,2,3}`.  
The infinite problem is **open**. Structural results: `STRUCTURAL_REPORT.md`. Unaligned desubstitution: `DESUBSTITUTION_REPORT.md`. Quotient dynamics of `T·v+Δδ=0`: `QUOTIENT_REPORT.md`. Triple / \(\omega\)-graph: `TRIPLE_REPORT.md`. Non-affine uniform templates: `TEMPLATE_REPORT.md`. m-kernel / k-regular block sums: `KERNEL_REPORT.md`. Bounded \(p_T\) / exhaustive \(B(3)\): `PT_REPORT.md`.

## Build and test

```bash
make test
```

Auxiliary binaries:
```bash
g++ -std=c++17 -O3 -o bin/dump_deadends src/dump_deadends.cpp
g++ -std=c++17 -O3 -o bin/run_candidate src/run_candidate.cpp
g++ -std=c++17 -O3 -o bin/mutate_candidate src/mutate_candidate.cpp
```

## Documents

| File | Contents |
|---|---|
| `PT_REPORT.md` | Conjecture A, \(B(3)=1647\), compactness at fixed \(m\), VdW failure, dual census |
| `KERNEL_REPORT.md` | m-kernel of the T-word, regular block sums, 4-adic prefix squares, synchronized iff bounded \(\Delta\) |
| `TRIPLE_REPORT.md` | Shared-middle triples, \(\omega\)-identity, junction covering; no finite factor-graph obstruction |
| `QUOTIENT_REPORT.md` | Defect equation modulo `ker(T)`: quotient is infinite; Krylov package; killed finite-state `Q` |
| `DESUBSTITUTION_REPORT.md` | Unaligned cubes: exact geometry, T+defect identity, killed dream theorem |
| `UNIFORM_MORPHIC_REPORT.md` | Uniform morphisms: Cassaigne lattice vs AP, Pisot-uniform impossibility, 2-uniform forensics |
| `STRUCTURAL_REPORT.md` | Unique-vote lemma, dead-end taxonomy, Cassaigne recoding |
| `NOVELTY_AUDIT.md` | Literature status as of 2026 |
| `REPORT.md` | Final mathematical report |
| `RESULTS.md` | Known vs new, with novelty checks |
| `proofs/THEOREMS.md` | Proofs, tagged PROVED / COMPUTATIONALLY VERIFIED |
| `VERIFIED_CONSTRUCTIONS.md` | Words we actually built |
| `COUNTEREXAMPLES.md` | Killed hypotheses |
| `CONJECTURES.md` | Conjectures and attacks |
| `SEARCH_ALGORITHMS.md` | Methods and commands |
| `RESEARCH_LOG.md` | Chronological log |

## Data

- `data/pt_scan3.txt` / `data/pt_verify_cert.txt` / `data/pt_verify_cpp.txt` / `data/pt_acf_cert.txt` — exhaustive m=3 census and dual \(p_T\) / ACF certificates
- `data/kernel_cert.txt` / `data/kernel_cert_cpp.txt` — dual kernel / regular identities, 3-letter scans
- `data/template_cert.txt` — dual C++/Python numbers for length rigidity, Cassaigne spectrum, pads, T-word
- `data/deadends14.txt` — 8170 right-crucial ACF words of length 14
- `data/scan2_summary.txt` — exhaustive 2-uniform first-cube types on `{0,1,2,3}` and `{0,1,3,4}`
- `data/krylov_census_{A,B}.txt` — 2-uniform Krylov / `L`-invariance census (C++ matches Python)
- `data/triple_counts.txt` — pair vs triple counts on famous and Cassaigne (C++ matches Python)
- `data/famous_all_cubes.csv` — all additive cubes in the length-2048 witness iterate
- `data/word_fixed_p1000_n24396.txt` — Liétard stall, reconstructed
- `data/word_updown_p2000_n400000.txt` — 400k Up-and-Down word (not a record)
