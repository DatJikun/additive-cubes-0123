#!/usr/bin/env python3
"""m-kernel / k-regular analysis of T-coded uniform words.

Objects distinguished:
  U  letter word (m-automatic if σ is m-uniform)
  a_i = T(U[i])   T-coded automatic numerical word
  W = σ(U)        equals U on a pure fixed point

No factor graph. Exact integer arithmetic. Gaussian rank over Q, not
combinatorial minors (those hang in dimension > 4).
"""
from __future__ import annotations

import os
import random
import sys
from fractions import Fraction
from math import gcd
from typing import Optional, Sequence, Tuple

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from desub_geometry import (  # noqa: E402
    FAMOUS,
    UniformMorph,
    find_cube,
    first_acf_len,
    morph_from_words,
    prefix_sums,
)
from quotient_dyn import krylov_package  # noqa: E402


def cassaigne_iterate(cap: int) -> list[int]:
    imgs = {0: (0, 3), 1: (4, 3), 3: (1,), 4: (0, 1)}
    w = [0]
    while len(w) < cap:
        nxt = []
        for a in w:
            nxt.extend(imgs[a])
        if len(nxt) <= len(w):
            break
        w = nxt
    return w[:cap]


def gaussian_rank(rows: Sequence[Sequence[int]]) -> int:
    """Exact rank over Q via Gaussian elimination with fractions."""
    A = [[Fraction(int(x)) for x in r] for r in rows if any(int(x) != 0 for x in r)]
    if not A:
        return 0
    n, m = len(A), len(A[0])
    h = k = 0
    while h < n and k < m:
        piv = None
        for i in range(h, n):
            if A[i][k] != 0:
                piv = i
                break
        if piv is None:
            k += 1
            continue
        A[h], A[piv] = A[piv], A[h]
        pv = A[h][k]
        for i in range(h + 1, n):
            if A[i][k] == 0:
                continue
            f = A[i][k] / pv
            for j in range(k, m):
                A[i][j] -= f * A[h][j]
        h += 1
        k += 1
    return h


def dfao_transitions(sigma: UniformMorph) -> dict:
    tr = {}
    for a in sigma.alph:
        tr[a] = tuple(sigma.images[a][r] for r in range(sigma.m))
    return tr


def letter_at(sigma: UniformMorph, seed: int, n: int) -> int:
    """U[n] via MSB base-m digits, starting from seed."""
    if n == 0:
        return seed
    m = sigma.m
    digits = []
    x = n
    while x:
        digits.append(x % m)
        x //= m
    digits.reverse()
    s = seed
    for d in digits:
        s = sigma.images[s][d]
    return s


def verify_dfao(sigma: UniformMorph, seed: int, cap: int = 256) -> int:
    W = sigma.iterate(seed, cap)
    fails = 0
    for n in range(min(cap, len(W))):
        if letter_at(sigma, seed, n) != W[n]:
            fails += 1
    return fails


def t_seq(U: Sequence[int], T: Sequence[int], alph: Sequence[int]) -> list[int]:
    idx = {a: i for i, a in enumerate(alph)}
    return [T[idx[a]] for a in U]


def parikh_list(word: Sequence[int], alph: Sequence[int]) -> list[int]:
    idx = {a: i for i, a in enumerate(alph)}
    c = [0] * len(alph)
    for x in word:
        c[idx[x]] += 1
    return c


def psi_rec(sigma: UniformMorph, seed: int, n: int, memo=None) -> list[int]:
    """Prefix Parikh ψ(n) by the LSB kernel recurrence, O(log n) with memo."""
    if memo is None:
        memo = {0: [0] * len(sigma.alph)}
    if n in memo:
        return memo[n][:]
    m = sigma.m
    q, r = divmod(n, m)
    v = [0] * len(sigma.alph)
    Mv = matvec(sigma.incidence(), psi_rec(sigma, seed, q, memo))
    frag = sigma.images[letter_at(sigma, seed, q)][:r]
    add = parikh_list(frag, sigma.alph)
    out = [Mv[i] + add[i] for i in range(len(sigma.alph))]
    memo[n] = out
    return out[:]


def matvec(M, v):
    k = len(v)
    return [sum(M[i][j] * v[j] for j in range(k)) for i in range(k)]


def Mtv(M, T):
    k = len(T)
    return tuple(sum(M[j][i] * T[j] for j in range(k)) for i in range(k))


def kernel_subseq(seq: Sequence[int], m: int, r: int, q: int) -> list[int]:
    stride = m**r
    out = []
    n = 0
    while True:
        j = stride * n + q
        if j >= len(seq):
            break
        out.append(seq[j])
        n += 1
    return out


def automatic_kernel_size(seq: Sequence[int], m: int, max_r: int = 8, prefix: int = 64) -> dict:
    seen = {}
    growth = []
    for r in range(0, max_r + 1):
        for q in range(m**r):
            sub = kernel_subseq(seq, m, r, q)
            if len(sub) < prefix:
                continue
            key = tuple(sub[:prefix])
            if key not in seen:
                seen[key] = (r, q)
        growth.append((r, len(seen)))
    return {"n_fingerprints": len(seen), "growth": growth, "prefix": prefix, "max_r": max_r}


def summatory_kernel_rank(S: Sequence[int], m: int, max_r: int = 5, n_sample: int = 40) -> dict:
    rows = []
    for r in range(0, max_r + 1):
        for q in range(min(m**r, 64)):
            vec = []
            ok = True
            for n in range(n_sample):
                j = m**r * n + q
                if j >= len(S):
                    ok = False
                    break
                vec.append(S[j])
            if ok and len(vec) == n_sample:
                rows.append(vec)
    rank = gaussian_rank(rows) if rows else 0
    return {"rank": rank, "n_rows": len(rows), "n_sample": n_sample, "max_r": max_r}


def P(S: Sequence[int], i: int, d: int) -> int:
    return S[i + d] - S[i]


def D1(S: Sequence[int], i: int, d: int) -> int:
    return P(S, i, d) - P(S, i + d, d)


def D2(S: Sequence[int], i: int, d: int) -> int:
    return P(S, i + d, d) - P(S, i + 2 * d, d)


def prefix_square_char(S: Sequence[int]) -> list[int]:
    n = len(S)
    chi = []
    for d in range(1, n // 2):
        chi.append(1 if S[2 * d] == 2 * S[d] else 0)
    return chi


def cube_char_prefix(S: Sequence[int]) -> list[int]:
    n = len(S)
    chi = []
    for d in range(1, n // 3):
        s1 = S[d]
        s2 = S[2 * d] - S[d]
        s3 = S[3 * d] - S[2 * d]
        chi.append(1 if s1 == s2 == s3 else 0)
    return chi


def R_sigma_n(S: Sequence[int], n: int) -> set[tuple]:
    N = len(S)
    out = set()
    # Need S[i+3n], so i+3n <= N-1.
    for i in range(0, N - 3 * n):
        out.add((D1(S, i, n), D2(S, i, n)))
    return out


def diam_R(pairs: set[tuple]) -> int:
    if not pairs:
        return 0
    return max(max(abs(a), abs(b)) for a, b in pairs)


def discrepancy(S: Sequence[int], mu_num: int, mu_den: int) -> list[int]:
    return [mu_den * S[n] - mu_num * n for n in range(len(S))]


def is_primitive(M, rounds: int = 12) -> bool:
    k = len(M)
    acc = [r[:] for r in M]
    for _ in range(rounds):
        if all(acc[i][j] > 0 for i in range(k) for j in range(k)):
            return True
        nxt = [[0] * k for _ in range(k)]
        for i in range(k):
            for p in range(k):
                for j in range(k):
                    nxt[i][j] += acc[i][p] * M[p][j]
        acc = nxt
    return False


def famous_analysis(cap: int = 4096) -> dict:
    sigma = FAMOUS
    U = sigma.iterate(2, cap)
    a = t_seq(U, sigma.T, sigma.alph)
    S = prefix_sums(a)
    dfao_fail = verify_dfao(sigma, 2, min(512, cap))
    M = sigma.incidence()
    pkg = krylov_package(M, sigma.alph, sigma.T)
    T = sigma.T
    T2 = pkg["T2"]
    L = tuple(2 * T[i] - T2[i] for i in range(4))  # D1(0,d) = L · ψ(d)

    psi_fail = 0
    L_fail = 0
    S2n_fail = 0
    memo = {0: [0, 0, 0, 0]}
    for n in range(0, min(400, len(U) + 1)):
        pr = parikh_list(U[:n], sigma.alph)
        rec = psi_rec(sigma, 2, n, memo)
        if rec != pr:
            psi_fail += 1
        if 2 * n < len(S):
            t2dot = T2[0] * pr[0] + T2[1] * pr[1] + T2[2] * pr[2] + T2[3] * pr[3]
            if t2dot != S[2 * n]:
                S2n_fail += 1
        if n >= 1 and n < len(S) // 2:
            d1 = D1(S, 0, n)
            form = L[0] * pr[0] + L[1] * pr[1] + L[2] * pr[2] + L[3] * pr[3]
            if d1 != form:
                L_fail += 1

    s_rank = summatory_kernel_rank(S, 2, max_r=6, n_sample=48)
    a_ker = automatic_kernel_size(a, 2, max_r=8, prefix=48)
    u_ker = automatic_kernel_size(U, 2, max_r=8, prefix=48)
    chi_sq = prefix_square_char(S)
    chi_cu = cube_char_prefix(S)
    sq_ker = automatic_kernel_size(chi_sq, 2, max_r=7, prefix=32)
    cu_ker = automatic_kernel_size(chi_cu, 2, max_r=7, prefix=32)
    Lvals = []
    for n in range(1, min(len(U) + 1, 2000)):
        pr = parikh_list(U[:n], sigma.alph)
        Lvals.append(L[0] * pr[0] + L[1] * pr[1] + L[2] * pr[2] + L[3] * pr[3])
    L_absmax = max(abs(x) for x in Lvals)

    diams = []
    hits00 = []
    for n in range(1, 65):
        Rn = R_sigma_n(S, n)
        diams.append((n, len(Rn), diam_R(Rn), (0, 0) in Rn))
        hits00.append(int((0, 0) in Rn))

    sm = S[-1]
    N = len(a)
    g = gcd(sm, N)
    mu_num, mu_den = sm // g, N // g
    Delta = discrepancy(S, mu_num, mu_den)
    dmax = max(abs(x) for x in Delta)

    d1vals = set()
    for n in (1, 2, 4, 8, 16, 32, 64, 128):
        for i in range(0, min(400, len(S) - 2 * n)):
            d1vals.add(D1(S, i, n))

    mod_vs_int = {}
    for Mmod in (2, 3, 4, 5, 8):
        n_mod = n_int = n_tot = 0
        for n in range(1, 40):
            for i in range(0, min(200, len(S) - 3 * n)):
                x = D1(S, i, n)
                y = D2(S, i, n)
                n_tot += 1
                if x % Mmod == 0 and y % Mmod == 0:
                    n_mod += 1
                    if x == 0 and y == 0:
                        n_int += 1
        mod_vs_int[Mmod] = {"pairs": n_tot, "mod0": n_mod, "int0": n_int}

    cube_ds = set()
    for n in range(1, min(200, len(S) // 3)):
        for i in range(0, len(S) - 3 * n):
            if D1(S, i, n) == 0 and D2(S, i, n) == 0:
                cube_ds.add(n)
                break
    missing = [n for n in range(1, 200) if n not in cube_ds]

    # 1-dimensional semilinearity of cube lengths would force ultimate periodicity.
    # Record gap sequence of missing d.
    gaps = []
    prev = 0
    for n in sorted(cube_ds):
        if n - prev > 1:
            gaps.append((prev, n, n - prev - 1))
        prev = n

    pT = []
    for n in range(1, 65):
        vals = {S[i + n] - S[i] for i in range(0, len(S) - n)}
        pT.append(len(vals))

    return {
        "dfao_fail": dfao_fail,
        "psi_fail": psi_fail,
        "S2n_fail": S2n_fail,
        "L_fail": L_fail,
        "L": L,
        "T2": T2,
        "krylov_T": pkg["krylov_T"],
        "S_module_rank": s_rank,
        "a_kernel": a_ker,
        "U_kernel": u_ker,
        "prefix_square_kernel": sq_ker,
        "prefix_cube_kernel": cu_ker,
        "n_prefix_squares": sum(chi_sq),
        "n_prefix_cubes": sum(chi_cu),
        "L_absmax": L_absmax,
        "diam_R_sample": diams[::8],
        "diam_1": diams[0],
        "diam_16": diams[15],
        "diam_64": diams[63],
        "n_with_00_le64": sum(hits00),
        "Delta_max_prefix": dmax,
        "mu_prefix": (mu_num, mu_den),
        "n_distinct_D1_sampled": len(d1vals),
        "mod_vs_int": mod_vs_int,
        "n_cube_lengths_lt200": len(cube_ds),
        "missing_d_lt200_head": missing[:30],
        "n_missing_lt200": len(missing),
        "cube_length_gaps_head": gaps[:15],
        "pT_1": pT[0],
        "pT_16": pT[15],
        "pT_64": pT[63],
        "first_cube_a": find_cube(a),
        "first_cube_U": find_cube(U),
        "transitions": dfao_transitions(sigma),
        "incidence": M,
        "LM2": tuple(sum(L[i] * sum(M[i][k] * M[k][j] for k in range(4)) for i in range(4)) for j in range(4)),
        "L_psi_70": (lambda pr: L[0]*pr[0]+L[1]*pr[1]+L[2]*pr[2]+L[3]*pr[3])(parikh_list(U[:70], sigma.alph)),
    }


def cassaigne_kernel_growth(cap: int = 2048) -> dict:
    w = cassaigne_iterate(cap)
    S = prefix_sums(w)
    ker = automatic_kernel_size(w, 2, max_r=7, prefix=32)
    ker3 = automatic_kernel_size(w, 3, max_r=5, prefix=24)
    chi_sq = prefix_square_char(S)
    sq_ker = automatic_kernel_size(chi_sq, 2, max_r=6, prefix=24)
    diams = []
    for n in (1, 2, 4, 8, 16, 32, 64):
        Rn = R_sigma_n(S, n)
        diams.append((n, len(Rn), diam_R(Rn), (0, 0) in Rn))
    return {
        "letter_2kernel": ker,
        "letter_3kernel": ker3,
        "square_2kernel": sq_ker,
        "n_prefix_squares": sum(chi_sq),
        "n_prefix_cubes": sum(cube_char_prefix(S)),
        "diam_R": diams,
        "cube": find_cube(w),
        "len": len(w),
    }


def thue_morse_sums(cap: int = 4096) -> dict:
    tm = [(bin(n).count("1") & 1) for n in range(cap)]
    S = prefix_sums(tm)
    Delta = [2 * S[n] - n for n in range(len(S))]
    return {
        "cube": find_cube(tm),
        "S_rank": summatory_kernel_rank(S, 2, 6, 40),
        "a_kernel": automatic_kernel_size(tm, 2, 6, 40),
        "prefix_square_kernel": automatic_kernel_size(prefix_square_char(S), 2, 6, 24),
        "Delta_max": max(abs(x) for x in Delta),
        "Delta_unbounded_sample": len(set(Delta)),
    }


def period_doubling_sums(cap: int = 4096) -> dict:
    """Fixed point of 1→10, 0→11, starting at 1. Burns: running sum not 2-synchronized."""
    imgs = {1: (1, 0), 0: (1, 1)}
    w = [1]
    while len(w) < cap:
        nxt = []
        for a in w:
            nxt.extend(imgs[a])
        w = nxt
    w = w[:cap]
    S = prefix_sums(w)
    Delta = [3 * S[n] - 2 * n for n in range(len(S))]  # mean of {0,1} with more 1s
    return {
        "cube": find_cube(w),
        "a_kernel": automatic_kernel_size(w, 2, 6, 40),
        "S_rank": summatory_kernel_rank(S, 2, 5, 32),
        "prefix_square_kernel": automatic_kernel_size(prefix_square_char(S), 2, 6, 24),
        "Delta_max": max(abs(x) for x in Delta),
        "pT_32": len({S[i + 32] - S[i] for i in range(0, len(S) - 32)}),
    }


def scan_uniform(
    m: int,
    alph: Tuple[int, ...],
    cap: int = 120,
    max_morphs: Optional[int] = None,
    require_n_sums: int = 1,
    prolongable_only: bool = True,
) -> dict:
    k = len(alph)
    n_total = k ** (m * k)
    rng_limit = n_total if max_morphs is None else min(n_total, max_morphs)
    best_acf = -1
    best = None
    n_cube = 0
    n_surv = 0
    n_prim = 0
    n_tried = 0
    n_prol = 0
    for code in range(rng_limit):
        t = code
        imgs = []
        for _ in range(k):
            row = []
            for _ in range(m):
                row.append(alph[t % k])
                t //= k
            imgs.append(row)
        sigma = morph_from_words(imgs, alph)
        if sigma.n_sums < require_n_sums:
            continue
        seeds = [a for a in alph if sigma.prolongable(a)] if prolongable_only else list(alph)
        if not seeds:
            continue
        n_prol += 1
        n_tried += 1
        prim = is_primitive(sigma.incidence())
        if prim:
            n_prim += 1
        local_best = 0
        local_cube = None
        for seed in seeds:
            W = sigma.iterate(seed, cap)
            c = find_cube(W)
            L = first_acf_len(W) if c is not None else len(W)
            if c is None:
                L = len(W)
            if L > local_best:
                local_best = L
                local_cube = (seed, c, L)
        if local_cube and local_cube[1] is None:
            n_surv += 1
        else:
            n_cube += 1
        if local_best > best_acf:
            best_acf = local_best
            best = (sigma.code(), sigma.T, sigma.n_sums, prim, local_cube)
    return {
        "m": m,
        "alph": alph,
        "n_total_space": n_total,
        "n_codes_scanned": rng_limit,
        "n_tried": n_tried,
        "n_prolongable": n_prol,
        "n_primitive": n_prim,
        "n_cube": n_cube,
        "n_survive_cap": n_surv,
        "best_acf": best_acf,
        "best": best,
        "cap": cap,
        "require_n_sums": require_n_sums,
        "prolongable_only": prolongable_only,
    }


def scan_random_m3_4letter(n_samples: int = 8000, cap: int = 160, seed: int = 20260904) -> dict:
    """Structured random primitive prolongable 3-uniform maps, n_sums>=3."""
    rng = random.Random(seed)
    alph = (0, 1, 2, 3)
    best_acf = -1
    best = None
    n_prim = 0
    n_cube = 0
    n_surv = 0
    n_ok = 0
    for _ in range(n_samples):
        imgs = [[rng.randrange(4) for _ in range(3)] for _ in range(4)]
        # force at least one prolongable letter
        s0 = rng.randrange(4)
        imgs[s0][0] = s0
        sigma = morph_from_words(imgs, alph)
        if sigma.n_sums < 3:
            continue
        if not is_primitive(sigma.incidence()):
            continue
        n_prim += 1
        n_ok += 1
        local_best = 0
        local_cube = None
        for seed_a in alph:
            if not sigma.prolongable(seed_a):
                continue
            W = sigma.iterate(seed_a, cap)
            c = find_cube(W)
            L = first_acf_len(W) if c is not None else len(W)
            if c is None:
                L = len(W)
            if L > local_best:
                local_best = L
                local_cube = (seed_a, c, L)
        if local_cube is None:
            continue
        if local_cube[1] is None:
            n_surv += 1
        else:
            n_cube += 1
        if local_best > best_acf:
            best_acf = local_best
            best = (sigma.code(), sigma.T, sigma.n_sums, True, local_cube)
    return {
        "n_samples": n_samples,
        "n_ok_prim_3sums": n_ok,
        "n_primitive": n_prim,
        "n_cube": n_cube,
        "n_survive_cap": n_surv,
        "best_acf": best_acf,
        "best": best,
        "cap": cap,
        "seed": seed,
    }


def cert_lines(rec: dict, cass: dict, tm: dict, pd: dict, scans: list, rnd: dict) -> list[str]:
    lines = []
    lines.append(f"dfao_fail {rec['dfao_fail']}")
    lines.append(f"psi_fail {rec['psi_fail']}")
    lines.append(f"S2n_fail {rec['S2n_fail']}")
    lines.append(f"L_fail {rec['L_fail']}")
    lines.append(f"L {rec['L']}")
    lines.append(f"T2 {rec['T2']}")
    lines.append(f"krylov_T {rec['krylov_T']}")
    lines.append(f"S_module_rank {rec['S_module_rank']['rank']}")
    lines.append(f"a_kernel_fp {rec['a_kernel']['n_fingerprints']}")
    lines.append(f"U_kernel_fp {rec['U_kernel']['n_fingerprints']}")
    lines.append(f"sq_kernel_fp {rec['prefix_square_kernel']['n_fingerprints']}")
    lines.append(f"cu_kernel_fp {rec['prefix_cube_kernel']['n_fingerprints']}")
    lines.append(f"n_prefix_squares {rec['n_prefix_squares']}")
    lines.append(f"n_prefix_cubes {rec['n_prefix_cubes']}")
    lines.append(f"L_absmax {rec['L_absmax']}")
    lines.append(f"diam_64 {rec['diam_64']}")
    lines.append(f"n_with_00_le64 {rec['n_with_00_le64']}")
    lines.append(f"Delta_max {rec['Delta_max_prefix']}")
    lines.append(f"n_distinct_D1 {rec['n_distinct_D1_sampled']}")
    lines.append(f"n_cube_len_lt200 {rec['n_cube_lengths_lt200']}")
    lines.append(f"n_missing_lt200 {rec['n_missing_lt200']}")
    lines.append(f"missing_head {rec['missing_d_lt200_head']}")
    lines.append(f"pT_1 {rec['pT_1']}")
    lines.append(f"pT_16 {rec['pT_16']}")
    lines.append(f"pT_64 {rec['pT_64']}")
    lines.append(f"first_cube_a {rec['first_cube_a']}")
    lines.append(f"first_cube_U {rec['first_cube_U']}")
    lm2 = rec["LM2"]
    twoL = tuple(2 * x for x in rec["L"])
    lines.append(f"LM2_fail {0 if lm2 == twoL else 1}")
    lines.append(f"L_psi_70 {rec['L_psi_70']}")
    for Mmod, v in rec["mod_vs_int"].items():
        lines.append(f"mod{Mmod} {v['pairs']} {v['mod0']} {v['int0']}")
    lines.append(f"cass_2ker {cass['letter_2kernel']['n_fingerprints']}")
    lines.append(f"cass_3ker {cass['letter_3kernel']['n_fingerprints']}")
    lines.append(f"cass_cube {cass['cube']}")
    lines.append(f"tm_cube {tm['cube']}")
    lines.append(f"tm_Delta_max {tm['Delta_max']}")
    lines.append(f"tm_a_ker {tm['a_kernel']['n_fingerprints']}")
    lines.append(f"tm_sq_ker {tm['prefix_square_kernel']['n_fingerprints']}")
    lines.append(f"pd_cube {pd['cube']}")
    lines.append(f"pd_Delta_max {pd['Delta_max']}")
    lines.append(f"pd_sq_ker {pd['prefix_square_kernel']['n_fingerprints']}")
    for s in scans:
        lines.append(
            f"scan m={s['m']} A={s['alph']} cap={s['cap']} tried={s['n_tried']} "
            f"surv={s['n_survive_cap']} best={s['best_acf']} rec={s['best']}"
        )
    lines.append(
        f"rand_m3 n_ok={rnd['n_ok_prim_3sums']} surv={rnd['n_survive_cap']} "
        f"best={rnd['best_acf']} rec={rnd['best']}"
    )
    return lines


def main():
    print("=== DFAO vs iterate ===")
    print(" famous dfao fails", verify_dfao(FAMOUS, 2, 512))

    print("\n=== FAMOUS KERNEL / REGULAR ===")
    rec = famous_analysis(4096)
    for k, v in rec.items():
        print(f"  {k}: {v}")

    print("\n=== CASSAIGNE CONTROL (not automatic) ===")
    cass = cassaigne_kernel_growth(2048)
    print(" ", cass)

    print("\n=== THUE-MORSE CONTROL (binary automatic, synchronized summatory) ===")
    tm = thue_morse_sums(2048)
    print(" ", tm)

    print("\n=== PERIOD-DOUBLING CONTROL (automatic, summatory not synchronized) ===")
    pd = period_doubling_sums(2048)
    print(" ", pd)

    scans = []
    print("\n=== LAYER B: 2-uniform 3-letter exhaustive, prolongable, n_sums>=2 ===")
    for alph in ((0, 1, 2), (0, 1, 3), (0, 1, 4), (0, 2, 3)):
        rec3 = scan_uniform(2, alph, cap=80, require_n_sums=2, prolongable_only=True)
        print(" ", rec3)
        scans.append(rec3)

    print("\n=== 2-uniform 3-letter n_sums>=3 (all T distinct) ===")
    rec3b = scan_uniform(2, (0, 1, 2), cap=80, require_n_sums=3, prolongable_only=True)
    print(" ", rec3b)
    scans.append(rec3b)

    print("\n=== m=3 3-letter, n_sums>=2, cap 60, all 19683, prolongable ===")
    rec33 = scan_uniform(3, (0, 1, 2), cap=60, require_n_sums=2, prolongable_only=True)
    print(" ", rec33)
    scans.append(rec33)

    print("\n=== m=3 4-letter first 2500 codes, n_sums>=3, prolongable, cap 80 ===")
    rec34 = scan_uniform(3, (0, 1, 2, 3), cap=80, max_morphs=2500, require_n_sums=3, prolongable_only=True)
    print(" ", rec34)
    scans.append(rec34)

    print("\n=== random primitive m=3 4-letter, n_sums>=3 ===")
    rnd = scan_random_m3_4letter(8000, cap=160, seed=20260904)
    print(" ", rnd)

    os.makedirs("data", exist_ok=True)
    lines = cert_lines(rec, cass, tm, pd, scans, rnd)
    with open("data/kernel_cert.txt", "w") as f:
        f.write("\n".join(lines) + "\n")
    print("\n=== CERT ===")
    print("\n".join(lines))


if __name__ == "__main__":
    main()
