#!/usr/bin/env python3
"""p_T / ACF attack: Conjecture A, level sets, controls. Exact integers.

Does NOT try to decide generic (D1,D2) zeros.
"""
from __future__ import annotations

import os
import sys
from collections import defaultdict

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from desub_geometry import (  # noqa: E402
    FAMOUS,
    UniformMorph,
    find_cube,
    first_acf_len,
    morph_from_words,
    prefix_sums,
)
from kernel_regular import (  # noqa: E402
    cassaigne_iterate,
    is_primitive,
    t_seq,
)


def additive_profile(w, nmax: int) -> list[dict]:
    S = prefix_sums(w)
    N = len(S) - 1
    out = []
    for n in range(1, nmax + 1):
        if n > N:
            break
        weights = []
        for i in range(0, N - n + 1):
            weights.append(S[i + n] - S[i])
        vals = set(weights)
        L = min(weights)
        H = max(weights)
        # equal-weight adjacent pairs / triples of block length n
        npair = ntrip = 0
        for i in range(0, N - 2 * n + 1):
            if weights[i] == weights[i + n]:
                npair += 1
        for i in range(0, N - 3 * n + 1):
            if weights[i] == weights[i + n] == weights[i + 2 * n]:
                ntrip += 1
        holes = (H - L + 1) - len(vals)
        out.append(
            {
                "n": n,
                "p": len(vals),
                "L": L,
                "H": H,
                "diam": H - L,
                "holes": holes,
                "npair": npair,
                "ntrip": ntrip,
            }
        )
    return out


def level_set_gaps(w, n: int, c: int) -> dict:
    S = prefix_sums(w)
    N = len(S) - 1
    pos = [i for i in range(0, N - n + 1) if S[i + n] - S[i] == c]
    has_ap = any((i + n in pos and i + 2 * n in pos) for i in pos if i + 2 * n <= N - n)
    gaps = [pos[j + 1] - pos[j] for j in range(len(pos) - 1)] if len(pos) > 1 else []
    return {
        "n": n,
        "c": c,
        "size": len(pos),
        "has_i_n_2n": has_ap,
        "min_gap": min(gaps) if gaps else None,
        "max_gap": max(gaps) if gaps else None,
        "n_eq_n": sum(1 for g in gaps if g == n),
    }


def vtm(cap: int) -> list[int]:
    imgs = {0: (0, 1, 2), 1: (0, 2), 2: (1,)}
    w = [0]
    while len(w) < cap:
        nxt = []
        for a in w:
            nxt.extend(imgs[a])
        if len(nxt) <= len(w):
            break
        w = nxt
    return w[:cap]


def tm_like(cap: int) -> list[int]:
    """0→01, 1→12, 2→20. Uniform, unbounded additive complexity (FSTTCS Thm 28)."""
    sigma = morph_from_words([[0, 1], [1, 2], [2, 0]], (0, 1, 2))
    return sigma.iterate(0, cap)


def paperfolding(cap: int) -> list[int]:
    """Paperfolding sequence: 2-automatic, {0,1}."""
    out = []
    for n in range(1, cap + 1):
        x = n
        while x % 2 == 0:
            x //= 2
        out.append(1 if (x % 4 == 1) else 0)
    return out


def rudin_shapiro(cap: int) -> list[int]:
    """Rudin–Shapiro on {0,1}: r_n = popcount of 11-pairs in binary, mod 2."""
    out = []
    for n in range(cap):
        x = n
        c = 0
        while x:
            if (x & 3) == 3:
                c ^= 1
            x >>= 1
        out.append(c)
    return out


def summarize(name, w, nmax: int) -> dict:
    cube = find_cube(w)
    acf = first_acf_len(w) if cube else len(w)
    prof = additive_profile(w, nmax)
    pmax = max(r["p"] for r in prof)
    dmax = max(r["diam"] for r in prof)
    holes = max(r["holes"] for r in prof)
    n_with_trip = sum(1 for r in prof if r["ntrip"] > 0)
    n_with_pair = sum(1 for r in prof if r["npair"] > 0)
    return {
        "name": name,
        "len": len(w),
        "cube": cube,
        "acf": acf,
        "p_max": pmax,
        "diam_max": dmax,
        "holes_max": holes,
        "n_with_pair": n_with_pair,
        "n_with_trip": n_with_trip,
        "p_at": [(r["n"], r["p"], r["diam"], r["holes"]) for r in prof if r["n"] in (1, 2, 4, 8, 16, 24, 32, 48)],
    }


def famous_acf_prefix_profile():
    U = FAMOUS.iterate(2, 256)
    acf = first_acf_len(U)
    pref = U[:acf]
    a = t_seq(pref, FAMOUS.T, FAMOUS.alph)
    return {
        "acf": acf,
        "letter": summarize("famous_U_acf_prefix", pref, min(40, acf // 3)),
        "Tword": summarize("famous_T_acf_prefix", a, min(40, acf // 3)),
        "full_U": summarize("famous_U_256", U, 48),
        "full_T": summarize("famous_T_256", t_seq(U, FAMOUS.T, FAMOUS.alph), 48),
    }


def vdW_failure_demo(w, n: int) -> dict:
    """Color i by ω(w[i:i+n)). Count monochromatic 3-APs with difference d vs d=n."""
    S = prefix_sums(w)
    N = len(S) - 1
    color = [S[i + n] - S[i] for i in range(0, N - n + 1)]
    by = defaultdict(list)
    for i, c in enumerate(color):
        by[c].append(i)
    n_ap_any = 0
    n_ap_diff_n = 0
    for c, pos in by.items():
        st = set(pos)
        for i in pos:
            for d in range(1, (N - n - i) // 2 + 1):
                if i + d in st and i + 2 * d in st:
                    n_ap_any += 1
                    if d == n:
                        n_ap_diff_n += 1
                    break  # count colors/starts, one d enough per i
    return {
        "n": n,
        "n_colors": len(by),
        "n_starts_with_some_mono_3AP": n_ap_any,
        "n_starts_with_diff_exactly_n": n_ap_diff_n,
    }


def main():
    recs = []
    recs.append(summarize("cassaigne", cassaigne_iterate(2048), 64))
    recs.append(summarize("vtm", vtm(2000), 64))
    recs.append(summarize("tm_like_012", tm_like(2048), 64))
    recs.append(summarize("thue_morse", [(bin(n).count("1") & 1) for n in range(2048)], 64))
    recs.append(summarize("paperfolding", paperfolding(2048), 64))
    recs.append(summarize("rudin_shapiro", rudin_shapiro(2048), 64))
    sigma014 = morph_from_words([[0, 1], [1, 4], [4, 0]], (0, 1, 4))
    recs.append(summarize("01440", sigma014.iterate(0, 256), 48))

    print("=== CONTROLS ===")
    for r in recs:
        print(r)

    print("\n=== FAMOUS ACF PREFIX vs FULL ===")
    fam = famous_acf_prefix_profile()
    for k, v in fam.items():
        print(k, v)

    print("\n=== VdW FAILURE (famous full, n=8 and n=25) ===")
    U = FAMOUS.iterate(2, 512)
    print(vdW_failure_demo(U, 8))
    print(vdW_failure_demo(U, 25))

    print("\n=== CASSAIGNE VdW n=8 ===")
    print(vdW_failure_demo(cassaigne_iterate(800), 8))

    # p <= diam+1 check
    fails = 0
    for r in recs:
        for n, p, diam, holes in r["p_at"]:
            if p > diam + 1:
                fails += 1
    print("\np_le_diam+1_fails", fails)

    os.makedirs("data", exist_ok=True)
    with open("data/pt_acf_cert.txt", "w") as f:
        for r in recs:
            f.write(
                f"{r['name']} len={r['len']} acf={r['acf']} cube={r['cube']} "
                f"pmax={r['p_max']} diam={r['diam_max']} holes={r['holes_max']} "
                f"pairs={r['n_with_pair']} trips={r['n_with_trip']}\n"
            )
        f.write(f"famous_acf {fam['acf']}\n")
        f.write(f"p_le_diam_fails {fails}\n")
        f.write(f"vtm_cube {recs[1]['cube']}\n")
        f.write(f"tmlike_pmax {recs[2]['p_max']}\n")
        f.write(f"cassaigne_pmax {recs[0]['p_max']}\n")
        f.write(f"cassaigne_cube {recs[0]['cube']}\n")


if __name__ == "__main__":
    main()
