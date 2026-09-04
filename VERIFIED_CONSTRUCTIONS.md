# Verified constructions

All words below were checked with the prefix-sum verifier in `src/acf.hpp`. The incremental builder was used during search; a full `find_cube` scan was run on the finished word except where noted.

## Known infinite construction (not over {0,1,2,3}) — sanity check

**Cassaigne–Currie–Schaeffer–Shallit morphism** on `{0,1,3,4}`:
```
0 → 03
1 → 43
3 → 1
4 → 01
```
Prefix of length 20,000 of the fixed point starting at 0: **ACF**. Command: `./bin/acf_tool test`.

## Reconstructed finite constructions over {0,1,2,3}

### 1. Fixed-order greedy stall (Liétard, reconstructed)

- **Length:** 24,396
- **Alphabet:** {0,1,2,3}
- **Generator:** chronological backtracking, letter priority always `(0,1,2,3)`, budget 2,000,000 extension attempts
- **Verification:** full `find_cube` after construction, ACF
- **Source of the number:** Liétard thesis Ch. 6 states that the classical (non-reversing) program produces length 24,396 and then stalls. We obtained **the same length independently**.
- **File:** `data/word_fixed_p1000_n24396.txt`
- **Frequencies:** 0: 43.93%, 1: 27.53%, 2: 17.22%, 3: 11.32%. Mean 0.959.
- **Command:** `./bin/acf_tool grow fixed 2000000 30000`

### 2. Up-and-Down, period 1000

- **Length:** 65,986 (**right-crucial / dead-end**, not budget-limited)
- **Generator:** reverse priority every 1000 letters, starting with `(3,2,1,0)`
- **Verification:** full `find_cube`, ACF as a finite word; tail \(q=0\) by unique-vote (`./bin/core_scan word`) and Python `q_vote`
- **File:** `data/word_updown_p1000_n65986.txt`
- **Command:** `./bin/acf_tool grow updown 2000000 200000 1000`

### 3. Up-and-Down, period 2000, cap 400,000

- **Length:** 400,000 (hit the cap; **not** a stall: tail \(q=1\))
- **Generator:** `updown` period 2000, budget 20,000,000, cap 400,000
- **Verification:** full `find_cube` in the grow command, ACF. Independent `analyze` confirms ACF.
- **File:** `data/word_updown_p2000_n400000.txt`
- **Mean:** 1.50364. Frequencies: 0: 27.90%, 1: 21.95%, 2: 22.04%, 3: 28.12%.
- **Discrepancy:** max |S(k)−μk| = 2022 (order of the reversal period).
- **Factor complexity:** p(6)=3152 = **all** ACF 6-letter words; p(7)=11452 / 11468; p(10)=143258.
- **Command:** `./bin/acf_tool grow updown 20000000 400000 2000`
- **Time:** 242 s on this machine.
- **This is NOT a new record.** Liétard’s published length is 70,880,000. This is an independent reconstruction of the same mechanism at a shorter, fully archived length.

### 4. Up-and-Down, period 5000, cap 400,000

- **Length:** 400,000 (hit cap; tail \(q=3\))
- **File:** `data/word_updown_p5000_n400000.txt`
- **Mean:** 1.50097
- **Command:** `./bin/acf_tool grow updown 20000000 400000 5000`
- **Time:** 69 s (less backtracking than period 2000)

## Liétard 70,880,000 word

**Not retrieved.** The thesis points to a URL on p. 13; `https://members.loria.fr/FLietard/phdProgramsAndDocumentation/` is a landing page without a live download in this audit. The generating mechanism (Up-and-Down) was reconstructed instead.

## Morphic candidates that are NOT constructions

See `COUNTEREXAMPLES.md`. No morphism found whose iterate is ACF beyond a few hundred letters.

## Crucial (dead-end) words

These are finite ACF words that are **not** constructions of long words; they are obstructions to extension. See `RESULTS.md` and `data/deadends14.txt` (8170 words of length 14).
