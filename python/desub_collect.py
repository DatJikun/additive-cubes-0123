#!/usr/bin/env python3
"""Collect cube anatomies for structured uniform morphisms and for ALL cubes
in long iterates (the object is the cube + boundary geometry, not a longer ACF word).

Independent of C++: uses python/desub_geometry.py.
"""
from __future__ import annotations

import csv
import json
import os
import sys
from collections import Counter, defaultdict
from itertools import product

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from desub_geometry import (
    UniformMorph,
    analyze_cube,
    ancestor_of,
    cube_type_key,
    defect_set,
    enumerate_lattice,
    find_cube,
    first_acf_len,
    in_lattice,
    induced_pairs,
    morph_from_strings,
    morph_from_words,
    n_complete_closed_form,
    parikh,
    peel,
    prefix_sums,
    primitive_lattice,
    residue_table,
    two_fragment_defects,
    vdot,
    view_interval,
    vl1,
    vlinf,
    vgcd,
    FAMOUS,
)

ALPH_A = (0, 1, 2, 3)
ALPH_B = (0, 1, 3, 4)


def all_cubes(w, limit=None):
    """List all (i,d,sum) additive cubes. Optional cap on number found."""
    n = len(w)
    S = prefix_sums(w)
    out = []
    for i in range(0, n - 2):
        dmax = (n - i) // 3
        for d in range(1, dmax + 1):
            s1 = S[i + d] - S[i]
            s2 = S[i + 2 * d] - S[i + d]
            s3 = S[i + 3 * d] - S[i + 2 * d]
            if s1 == s2 == s3:
                out.append((i, d, s1))
                if limit and len(out) >= limit:
                    return out
    return out


def iterate_levels(sigma, seed, max_len):
    """Return (W, U) where W = sigma(U) and |W| >= max_len or growth stopped."""
    u = [seed]
    while len(u) * sigma.m < max_len:
        nxt = sigma.apply(u)
        if len(nxt) <= len(u):
            break
        u = nxt
    w = sigma.apply(u)
    return w, u


def anatomy_row(sigma, an, extra=None):
    row = {
        "m": sigma.m,
        "alph": "".join(map(str, sigma.alph)),
        "code": sigma.code(),
        "T": ",".join(map(str, sigma.T)),
        "n_sums": sigma.n_sums,
        "i": an.i,
        "d": an.d,
        "sum": an.block_sum,
        "r": an.X.r,
        "s": an.X.s,
        "k": an.X.k,
        "aligned": int(an.aligned),
        "nX": an.X.n_complete,
        "nY": an.Y.n_complete,
        "nZ": an.Z.n_complete,
        "v_xy": ",".join(map(str, an.v_xy)),
        "v_yz": ",".join(map(str, an.v_yz)),
        "l1_xy": vl1(an.v_xy),
        "l1_yz": vl1(an.v_yz),
        "db_xy": an.db_xy,
        "db_yz": an.db_yz,
        "defX": an.X.defect,
        "defY": an.Y.defect,
        "defZ": an.Z.defect,
        "in_LA": int(in_lattice(an.v_xy, sigma.alph)),
        "in_LT": int(sum(an.v_xy) == 0 and vdot(sigma.T, an.v_xy) == 0),
        "Tdot_v": vdot(sigma.T, an.v_xy),
        "anc_acf": int(an.ancestor_acf),
        "anc_Tcube": int(an.ancestor_has_T_cube),
        "eq_ok": int(an.eq_xy_ok and an.eq_yz_ok and an.recon_ok),
        "type": f"{an.X.r},{an.X.s}",
        "types": f"{an.X.r}:{an.X.s}|{an.Y.r}:{an.Y.s}|{an.Z.r}:{an.Z.s}",
    }
    if extra:
        row.update(extra)
    return row


def analyze_all_cubes_of_word(sigma, seed, max_len, cube_limit=200):
    W, U = iterate_levels(sigma, seed, max_len)
    cubes = all_cubes(W[:max_len], limit=cube_limit)
    rows = []
    for c in cubes:
        try:
            an = analyze_cube(W, sigma, c, U)
        except Exception:
            continue
        rows.append(anatomy_row(sigma, an, {"seed": seed, "Wlen": min(len(W), max_len)}))
    return rows, W, U, cubes


def first_cube_anatomy(sigma, seed, cap):
    W, U = iterate_levels(sigma, seed, cap)
    w = W[:cap]
    c = find_cube(w)
    if c is None:
        return None, W, U
    an = analyze_cube(W, sigma, c, U)
    L = first_acf_len(w)
    return anatomy_row(sigma, an, {"seed": seed, "acf_prefix": L}), W, U


# ---- structured morphism families (not uniform random) ----

def family_powers_of_famous():
    """sigma^k as a 2^k-uniform morphism, same infinite word."""
    out = []
    sigma = FAMOUS
    # build sigma^k images of each letter
    for k in range(1, 7):
        imgs = []
        for a in ALPH_A:
            w = [a]
            for _ in range(k):
                w = sigma.apply(w)
            imgs.append(w)
        out.append(morph_from_words(imgs, ALPH_A))
    return out


def family_two_sums(m, alph=ALPH_A):
    """Images with only two distinct sums: 0,1 -> low, 2,3 -> high."""
    lo = [alph[0]] * m
    hi = [alph[-1]] * m
    # mix so not constant words: lo = 0^{m-1}1, hi = 3^{m-1}2 (mapped)
    lo = [alph[0]] * (m - 1) + [alph[1]]
    hi = [alph[-1]] * (m - 1) + [alph[-2]]
    return morph_from_words([lo, lo, hi, hi], alph)


def family_three_sums(m, alph=ALPH_A):
    imgs = [
        [alph[0]] * m,
        [alph[0]] * (m - 1) + [alph[1]],
        [alph[-1]] * (m - 1) + [alph[0]],
        [alph[-1]] * m,
    ]
    return morph_from_words(imgs, alph)


def family_four_sums_spread(m, alph=ALPH_A):
    """Maximal image-sum separation, injective T."""
    imgs = []
    for a in alph:
        imgs.append([a] * m)
    return morph_from_words(imgs, alph)


def family_permutation_cycle(m, alph=ALPH_A):
    """a -> (a, a+1, ..., ) cycling through alphabet, length m."""
    k = len(alph)
    imgs = []
    for i, a in enumerate(alph):
        imgs.append([alph[(i + t) % k] for t in range(m)])
    return morph_from_words(imgs, alph)


def family_imprimitive(m, alph=ALPH_A):
    """{0,1} and {2,3} are invariant (or first two / last two of alph)."""
    a, b, c, d = alph
    imgs = [
        [a, b] * (m // 2) + [a] * (m % 2),
        [b, a] * (m // 2) + [b] * (m % 2),
        [c, d] * (m // 2) + [c] * (m % 2),
        [d, c] * (m // 2) + [d] * (m % 2),
    ]
    return morph_from_words(imgs, alph)


def family_repeated_images(m, alph=ALPH_A):
    w = [alph[0], alph[-1]] * (m // 2) + [alph[0]] * (m % 2)
    return morph_from_words([w, w, w, w], alph)


def family_pisot_like(m, alph=ALPH_A):
    """All images are the same letter repeated except a permutation of a short tail.
    Incidence close to rank-1 (Pisot-ish / bounded discrepancy candidate)."""
    # 0 -> 0^{m-1}1, 1 -> 0^{m-1}2, 2 -> 0^{m-1}3, 3 -> 0^{m-1}0  (for A)
    imgs = []
    for i in range(4):
        imgs.append([alph[0]] * (m - 1) + [alph[(i + 1) % 4]])
    return morph_from_words(imgs, alph)


def family_cassaigne_padded(m, alph=ALPH_B):
    """Pad the Cassaigne shape 0->03, 1->43, 3->1, 4->01 to length m with a letter."""
    # Map alphabet B. Cassaigne: a->ac, b->dc, c->b, d->ab with (a,b,c,d)=(0,1,3,4)
    a, b, c, d = 0, 1, 3, 4
    base = {a: [a, c], b: [d, c], c: [b], d: [a, b]}
    imgs = []
    for x in alph:
        w = list(base[x])
        while len(w) < m:
            w.append(x)
        imgs.append(w[:m])
    return morph_from_words(imgs, alph)


def family_famous_on(alph):
    """The 2-uniform candidate, recoded onto alph by order-preserving map."""
    raw = ["32", "31", "20", "01"]
    imgs = []
    for s in raw:
        imgs.append([alph[int(ch)] for ch in s])
    return morph_from_words(imgs, alph)


def all_structured(ms=range(2, 11)):
    morphs = []
    # famous powers (A only)
    for k, sigma in enumerate(family_powers_of_famous(), 1):
        morphs.append((f"famous_power_k{k}", sigma, 2))
    for m in ms:
        morphs.append((f"two_sums_A_m{m}", family_two_sums(m, ALPH_A), 0))
        morphs.append((f"two_sums_B_m{m}", family_two_sums(m, ALPH_B), 0))
        morphs.append((f"three_sums_A_m{m}", family_three_sums(m, ALPH_A), 0))
        morphs.append((f"three_sums_B_m{m}", family_three_sums(m, ALPH_B), 0))
        morphs.append((f"spread_A_m{m}", family_four_sums_spread(m, ALPH_A), 0))
        morphs.append((f"spread_B_m{m}", family_four_sums_spread(m, ALPH_B), 0))
        morphs.append((f"cycle_A_m{m}", family_permutation_cycle(m, ALPH_A), 0))
        morphs.append((f"cycle_B_m{m}", family_permutation_cycle(m, ALPH_B), 0))
        morphs.append((f"impr_A_m{m}", family_imprimitive(m, ALPH_A), 0))
        morphs.append((f"impr_B_m{m}", family_imprimitive(m, ALPH_B), 0))
        morphs.append((f"repeat_A_m{m}", family_repeated_images(m, ALPH_A), 0))
        morphs.append((f"pisot_A_m{m}", family_pisot_like(m, ALPH_A), 0))
        morphs.append((f"pisot_B_m{m}", family_pisot_like(m, ALPH_B), 0))
        if m >= 2:
            morphs.append((f"cassaigne_pad_m{m}", family_cassaigne_padded(m, ALPH_B), 0))
    morphs.append(("famous_A", family_famous_on(ALPH_A), 2))
    morphs.append(("famous_B", family_famous_on(ALPH_B), 3))  # 3 is alph[2]? wait seed: images of B[2]=3 starts with B[2]=3 if "20" -> alph[2],alph[0]=3,0. seed 3? famous seed 2 maps to alph[2].
    return morphs


def write_csv(path, rows):
    if not rows:
        return
    os.makedirs(os.path.dirname(path), exist_ok=True)
    with open(path, "w", newline="") as f:
        w = csv.DictWriter(f, fieldnames=list(rows[0].keys()))
        w.writeheader()
        w.writerows(rows)


def m2_exhaustive_first_cubes(alph, cap=120, max_morphs=None):
    """All 2-uniform morphisms (4^8=65536). Prolongable seeds only.

    Images encoded as 8 letters in {0,1,2,3} then mapped onto alph.
    """
    rows = []
    n_surv = 0
    n_tried = 0
    for code in range(65536):
        if max_morphs is not None and n_tried >= max_morphs:
            break
        t = code
        imgs = []
        for _ in range(4):
            a = t % 4
            b = (t // 4) % 4
            t //= 16
            imgs.append([alph[a], alph[b]])
        sigma = morph_from_words(imgs, alph)
        for seed in alph:
            if sigma.images[seed][0] != seed:
                continue
            n_tried += 1
            W, U = iterate_levels(sigma, seed, cap)
            w = W[:cap]
            c = find_cube(w)
            if c is None:
                n_surv += 1
                rows.append(
                    {
                        "m": 2,
                        "alph": "".join(map(str, alph)),
                        "code": sigma.code(),
                        "T": ",".join(map(str, sigma.T)),
                        "n_sums": sigma.n_sums,
                        "seed": seed,
                        "i": -1,
                        "d": -1,
                        "survived": 1,
                        "acf_prefix": first_acf_len(w),
                    }
                )
                continue
            try:
                an = analyze_cube(W, sigma, c, U)
            except Exception:
                continue
            row = anatomy_row(sigma, an, {"seed": seed, "survived": 0, "acf_prefix": first_acf_len(w)})
            rows.append(row)
        if code % 4096 == 0 and code:
            print(f"  m2 {alph} code={code} rows={len(rows)}", flush=True)
    return rows, n_surv


def main():
    os.makedirs("data", exist_ok=True)
    print("=== Residue classification m=2..10 ===")
    with open("data/residue_types.txt", "w") as f:
        for m in range(2, 11):
            f.write(f"# m={m}\n")
            for t in residue_table(m):
                ns = [t.n_complete(k) for k in range(0, 4)]
                f.write(
                    f"m={m} r={t.r} s={t.s} left={t.left_len} right={t.right_len} "
                    f"offset={t.n_complete_offset} n_k0to3={ns}\n"
                )
            print(f"  m={m} types={m*m}")

    print("=== Famous word: ALL cubes up to length 2048 ===")
    rows, W, U, cubes = analyze_all_cubes_of_word(FAMOUS, 2, 2048, cube_limit=5000)
    write_csv("data/famous_all_cubes.csv", rows)
    print(f"  |W|={len(W)} cubes_recorded={len(rows)} first={cubes[0] if cubes else None}")
    l1s = [r["l1_xy"] for r in rows]
    ds = [r["d"] for r in rows]
    aligned = sum(r["aligned"] for r in rows)
    inLA = sum(r["in_LA"] for r in rows)
    inLT = sum(r["in_LT"] for r in rows)
    print(
        f"  d range {min(ds)}..{max(ds)}  L1(v) range {min(l1s)}..{max(l1s)}  "
        f"aligned={aligned}/{len(rows)} in_L(A)={inLA} in_L(T)={inLT}"
    )
    # largest L1 among unaligned with k>=2
    hard = [r for r in rows if not r["aligned"] and r["k"] >= 2]
    if hard:
        worst = max(hard, key=lambda r: r["l1_xy"])
        print("  worst unaligned L1", worst["l1_xy"], "d", worst["d"], "v", worst["v_xy"], "k", worst["k"])

    print("=== Famous powers as coarser morphisms, first cube (same word) ===")
    power_rows = []
    for k, sigma in enumerate(family_powers_of_famous(), 1):
        row, _, _ = first_cube_anatomy(sigma, 2, 256)
        if row:
            row["family"] = f"sigma^{k}"
            power_rows.append(row)
            print(
                f"  m={sigma.m} i={row['i']} d={row['d']} r,s={row['r']},{row['s']} "
                f"nX={row['nX']} L1={row['l1_xy']} aligned={row['aligned']} v={row['v_xy']}"
            )
    write_csv("data/famous_powers_first.csv", power_rows)

    print("=== Structured morphisms first cubes (A and B, m=2..10) ===")
    struct_rows = []
    for name, sigma, seed in all_structured():
        # try given seed, else any prolongable, else alph[0]
        # Only prolongable seeds: otherwise the iterate is not a prefix of a fixed point
        # and a short cap can report a cube from a throwaway finite word.
        seeds = []
        if seed in sigma.alph and sigma.prolongable(seed):
            seeds.append(seed)
        seeds += [a for a in sigma.alph if sigma.prolongable(a) and a not in seeds]
        got = None
        for s in seeds:
            cap = min(max(256, 16 * sigma.m), 1024)
            try:
                row, _, _ = first_cube_anatomy(sigma, s, cap)
            except Exception:
                continue
            if row:
                row["family"] = name
                struct_rows.append(row)
                got = row
                break
        if got is None:
            print(f"  {name} m={sigma.m} NO cube/cap fail")
        elif len(struct_rows) <= 8 or "famous" in name:
            print(f"  {name} m={sigma.m} d={got['d']} aligned={got['aligned']} L1={got['l1_xy']} inLA={got['in_LA']}")
    write_csv("data/structured_first_cubes.csv", struct_rows)
    print(f"  structured rows {len(struct_rows)}")

    print("=== Lattice primitive vectors ===")
    with open("data/lattice_vectors.txt", "w") as f:
        for name, alph in [("A", ALPH_A), ("B", ALPH_B)]:
            f.write(f"# L({name})={alph} primitive ||v||_inf<=6\n")
            for v in primitive_lattice(alph, 6):
                pairs = induced_pairs(v, alph)
                f.write(
                    f"{name} v={v} L1={vl1(v)} Linf={vlinf(v)} support={sum(x!=0 for x in v)} "
                    f"pairs={pairs[:4]}\n"
                )
    print("  wrote data/lattice_vectors.txt")

    # Identity check: every structured row has eq_ok
    bad = [r for r in struct_rows if r.get("eq_ok") == 0]
    print(f"structured eq_ok failures: {len(bad)}")
    bad2 = [r for r in rows if r.get("eq_ok") == 0]
    print(f"famous-all-cubes eq_ok failures: {len(bad2)}")


if __name__ == "__main__":
    main()
