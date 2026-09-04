# Additive-cube research archive

Autonomous attack on additive-cube-free words over `{0,1,2,3}`.  
The infinite problem is **open**. Structural results: `STRUCTURAL_REPORT.md`.

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

- `data/deadends14.txt` — 8170 right-crucial ACF words of length 14
- `data/word_fixed_p1000_n24396.txt` — Liétard stall, reconstructed
- `data/word_updown_p2000_n400000.txt` — 400k Up-and-Down word (not a record)
