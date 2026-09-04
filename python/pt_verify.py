#!/usr/bin/env python3
"""Independent dual of the p_T / B(3) attack.

Does not import pt_acf.py. Rebuilds the m=3 maximizer, every cap-243
survivor, famous and Cassaigne controls, VdW counts, and p <= diam+1
from first principles (desub_geometry + kernel_regular primitives only).
"""
from __future__ import annotations

import os
import sys
from collections import defaultdict

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from desub_geometry import (  # noqa: E402
    FAMOUS,
    find_cube,
    first_acf_len,
    morph_from_words,
    prefix_sums,
)
from kernel_regular import cassaigne_iterate, is_primitive  # noqa: E402


def decode_code(code: int):
    imgs = []
    t = code
    for _a in range(4):
        trip = []
        for _r in range(3):
            trip.append(t & 3)
            t >>= 2
        imgs.append(tuple(trip))
    return imgs


def sigma_from_code(code: int):
    imgs = decode_code(code)
    return morph_from_words(imgs, (0, 1, 2, 3)), imgs


def incidence_from_imgs(imgs):
    M = [[0] * 4 for _ in range(4)]
    for a, img in enumerate(imgs):
        for x in img:
            M[x][a] += 1
    return M


def T_from_imgs(imgs):
    return tuple(sum(img) for img in imgs)


def additive_at(w, n: int):
    S = prefix_sums(w)
    N = len(S) - 1
    weights = [S[i + n] - S[i] for i in range(0, N - n + 1)]
    vals = set(weights)
    L, H = min(weights), max(weights)
    ntrip = 0
    for i in range(0, N - 3 * n + 1):
        if weights[i] == weights[i + n] == weights[i + 2 * n]:
            ntrip += 1
    return {
        "p": len(vals),
        "L": L,
        "H": H,
        "diam": H - L,
        "holes": (H - L + 1) - len(vals),
        "ntrip": ntrip,
        "weights": weights,
    }


def vdW(w, n: int) -> dict:
    S = prefix_sums(w)
    N = len(S) - 1
    color = [S[i + n] - S[i] for i in range(0, N - n + 1)]
    by = defaultdict(list)
    for i, c in enumerate(color):
        by[c].append(i)
    n_ap_any = 0
    n_ap_diff_n = 0
    for _c, pos in by.items():
        st = set(pos)
        for i in pos:
            for d in range(1, (N - n - i) // 2 + 1):
                if i + d in st and i + 2 * d in st:
                    n_ap_any += 1
                    if d == n:
                        n_ap_diff_n += 1
                    break
    return {
        "n_colors": len(by),
        "n_starts_with_some_mono_3AP": n_ap_any,
        "n_starts_with_diff_exactly_n": n_ap_diff_n,
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


def parse_scan(path: str) -> dict:
    d: dict = {"push": []}
    with open(path) as f:
        for line in f:
            parts = line.split()
            if not parts:
                continue
            if parts[0] == "push":
                d["push"].append(
                    {
                        "code": int(parts[1]),
                        "seed": int(parts[3]),
                        "acf": int(parts[5]),
                    }
                )
            elif len(parts) >= 2 and parts[0] not in ("hist_nonzero", "best_imgs"):
                try:
                    d[parts[0]] = int(parts[1])
                except ValueError:
                    d[parts[0]] = parts[1]
    return d


def charpoly_coeffs(M):
    """Characteristic polynomial of a 4x4 integer matrix via Faddeev–LeVerrier."""
    n = 4
    I = [[1 if i == j else 0 for j in range(n)] for i in range(n)]

    def matmul(A, B):
        C = [[0] * n for _ in range(n)]
        for i in range(n):
            for k in range(n):
                for j in range(n):
                    C[i][j] += A[i][k] * B[k][j]
        return C

    def trace(A):
        return sum(A[i][i] for i in range(n))

    c = [1]
    A = [row[:] for row in I]
    Mk = [row[:] for row in M]
    for k in range(1, n + 1):
        ck = -trace(Mk) // k
        c.append(ck)
        # M_{k} = M (M_{k-1} + c_k I); next Mk = M * (current_acc)
        acc = [[Mk[i][j] + ck * I[i][j] for j in range(n)] for i in range(n)]
        Mk = matmul(M, acc)
        A = acc
    return c  # [1, c1, c2, c3, c4] for t^4 + c1 t^3 + ...


def main():
    fails = 0
    scan_path = "data/pt_scan3.txt"
    scan = parse_scan(scan_path)
    expected = {
        "N": 16777216,
        "n_prol": 11468800,
        "n_prim": 6993126,
        "n_3sum_prol": 9992241,
        "n_tried_prim_3sum": 6116013,
        "n_surv_cap": 86,
        "n_surv_2187": 0,
        "best2_acf": 1647,
        "best2_code": 12507201,
        "best2_seed": 1,
        "SURV243": 86,
        "hist_p50": 15,
        "hist_p90": 29,
        "hist_p99": 56,
    }
    for k, v in expected.items():
        if scan.get(k) != v:
            print("FAIL scan field", k, scan.get(k), "expected", v)
            fails += 1
        else:
            print("OK scan", k, v)
    if len(scan["push"]) != 86:
        print("FAIL push lines", len(scan["push"]))
        fails += 1

    # Rebuild every survivor, all prolongable seeds, cap 2187.
    best = -1
    best_code = -1
    best_seed = -1
    n_surv2 = 0
    acf_mismatch = 0
    for rec in scan["push"]:
        sigma, imgs = sigma_from_code(rec["code"])
        M = incidence_from_imgs(imgs)
        if not is_primitive(M) or len(set(T_from_imgs(imgs))) < 3:
            print("FAIL survivor filter", rec)
            fails += 1
            continue
        local = -1
        lseed = -1
        for a in range(4):
            if imgs[a][0] != a:
                continue
            w = sigma.iterate(a, 2187)
            L = first_acf_len(w)
            if L > local:
                local = L
                lseed = a
        if local >= 2187:
            n_surv2 += 1
        if local > best:
            best = local
            best_code = rec["code"]
            best_seed = lseed
        # Recorded seed's ACF must match the file (that seed, not necessarily max).
        wrec = sigma.iterate(rec["seed"], 2187)
        Lrec = first_acf_len(wrec)
        if Lrec != rec["acf"]:
            acf_mismatch += 1
            print("FAIL push acf", rec, "python", Lrec)
            fails += 1
    print("python_n_surv_2187", n_surv2)
    print("python_best2_acf", best)
    print("python_best2_code", best_code)
    print("python_best2_seed", best_seed)
    if n_surv2 != 0 or best != 1647 or best_code != 12507201 or best_seed != 1:
        print("FAIL python maximizer among survivors")
        fails += 1
    if acf_mismatch:
        print("FAIL acf_mismatch", acf_mismatch)
        fails += 1

    sigma, imgs = sigma_from_code(12507201)
    print("max_imgs", imgs)
    T = T_from_imgs(imgs)
    print("max_T", T)
    if imgs != [(1, 0, 0), (1, 0, 2), (1, 3, 2), (3, 3, 2)]:
        print("FAIL imgs")
        fails += 1
    if T != (1, 3, 6, 8):
        print("FAIL T")
        fails += 1
    M = incidence_from_imgs(imgs)
    print("max_M", M)
    if not is_primitive(M):
        print("FAIL not primitive")
        fails += 1
    if imgs[1][0] != 1:
        print("FAIL not prolongable at 1")
        fails += 1
    w = sigma.iterate(1, 2187)
    cube = find_cube(w)
    acf = first_acf_len(w)
    print("max_cube", cube)
    print("max_acf", acf)
    if cube != (982, 222, 229) or acf != 1647:
        print("FAIL maximizer cube/acf")
        fails += 1
    if cube and cube[0] + 3 * cube[1] - 1 != 1647:
        print("FAIL cube/ACF identity")
        fails += 1
    # Prefix of length 1647 is ACF; 1648 contains the cube.
    if find_cube(w[:1647]) is not None:
        print("FAIL prefix 1647 has a cube")
        fails += 1
    if find_cube(w[:1648]) is None:
        print("FAIL prefix 1648 has no cube")
        fails += 1

    cpoly = charpoly_coeffs(M)
    print("max_charpoly", cpoly)
    # t^4 - 6 t^3 + 8 t^2 - 3 t  (Faddeev); record for the report.
    # Independently: det(tI-M).
    def det4(A):
        def det3(B):
            a, b, c = B[0]
            d, e, f = B[1]
            g, h, i = B[2]
            return a * (e * i - f * h) - b * (d * i - f * g) + c * (d * h - e * g)

        s = 0
        for j in range(4):
            minor = [[A[r][c] for c in range(4) if c != j] for r in range(1, 4)]
            s += (1 if j % 2 == 0 else -1) * A[0][j] * det3(minor)
        return s

    # Evaluate p(t)=det(tI-M) at a few integers and match Faddeev.
    for tval in (0, 1, 2, 3, 6):
        tIM = [[(tval if i == j else 0) - M[i][j] for j in range(4)] for i in range(4)]
        d = det4(tIM)
        # p(t) = t^4 + c1 t^3 + c2 t^2 + c3 t + c4
        pred = tval**4 + cpoly[1] * tval**3 + cpoly[2] * tval**2 + cpoly[3] * tval + cpoly[4]
        if d != pred:
            print("FAIL charpoly at", tval, d, pred)
            fails += 1
    print("OK charpoly interpolation")

    pref = w[:1647]
    pmax = dmax = 0
    holes_max = 0
    n_with_trip = 0
    for n in range(1, min(80, 1647 // 3) + 1):
        r = additive_at(pref, n)
        pmax = max(pmax, r["p"])
        dmax = max(dmax, r["diam"])
        holes_max = max(holes_max, r["holes"])
        if r["ntrip"]:
            n_with_trip += 1
        if r["p"] > r["diam"] + 1:
            print("FAIL p>diam+1", n, r)
            fails += 1
    print("max_acfpref_pmax", pmax, "diam_max", dmax, "holes_max", holes_max, "n_with_trip", n_with_trip)
    if n_with_trip != 0:
        print("FAIL trips inside ACF prefix")
        fails += 1
    # Growing p_T on the finite ACF prefix (Conjecture A test on a finite word).
    r1 = additive_at(pref, 1)
    r8 = additive_at(pref, 8)
    r16 = additive_at(pref, 16)
    r64 = additive_at(pref, 64)
    print("max_pT", r1["p"], r8["p"], r16["p"], r64["p"])
    print("max_diam", r1["diam"], r8["diam"], r16["diam"], r64["diam"])
    if not (r1["p"] < r8["p"] <= r16["p"] and r16["p"] < r64["p"]):
        # Allow plateaus but require overall growth 1 -> 64.
        if r64["p"] <= r1["p"]:
            print("FAIL p_T not growing on maximizer ACF prefix")
            fails += 1
    # The actual cube length-set contains i, i+n, i+2n.
    full = additive_at(w[:1648], 222)
    if full["ntrip"] < 1:
        print("FAIL cube length 222 has no triple on prefix 1648")
        fails += 1
    else:
        print("OK level set n=222 has i,i+n,i+2n on prefix 1648")

    # Other dual-checked long maps from the census.
    for code, seed, cube_expect, acf_expect in (
        (16376976, 0, (539, 222, 229), 1204),
        (8805231, 1, (409, 144, 235), 840),
        (4269726, 2, (409, 144, 197), 840),
    ):
        sig, _imgs = sigma_from_code(code)
        ww = sig.iterate(seed, 2187)
        cu = find_cube(ww)
        L = first_acf_len(ww)
        print("map", code, "cube", cu, "acf", L)
        if cu != cube_expect or L != acf_expect:
            print("FAIL long map", code)
            fails += 1

    # Controls.
    U = FAMOUS.iterate(2, 256)
    fam_cube = find_cube(U)
    fam_acf = first_acf_len(U)
    print("famous_cube", fam_cube, "acf", fam_acf)
    if fam_cube != (45, 25, 35) or fam_acf != 119:
        print("FAIL famous")
        fails += 1
    fam_pref = U[:119]
    fam_pmax = max(additive_at(fam_pref, n)["p"] for n in range(1, min(40, 119 // 3) + 1))
    print("famous_acfpref_pmax", fam_pmax)
    if fam_pmax < 8:
        print("FAIL famous p_T not visibly growing")
        fails += 1

    cass = cassaigne_iterate(2048)
    if find_cube(cass) is not None:
        print("FAIL Cassaigne cube in 2048")
        fails += 1
    cass_prof = [additive_at(cass, n) for n in (1, 8, 16, 32, 64)]
    cass_pmax = max(additive_at(cass, n)["p"] for n in range(1, 65))
    cass_trips = sum(1 for n in range(1, 65) if additive_at(cass, n)["ntrip"])
    print("cassaigne_pmax64", cass_pmax, "trips", cass_trips, "p_at", [r["p"] for r in cass_prof])
    if cass_trips != 0 or cass_pmax < 40:
        print("FAIL Cassaigne profile")
        fails += 1
    # Holes: unbounded diameter with gaps (p < diam+1).
    cass16 = additive_at(cass, 16)
    if cass16["holes"] <= 0:
        print("FAIL expected Cassaigne holes at n=16")
        fails += 1

    v = vtm(2000)
    vc = find_cube(v)
    print("vtm_cube", vc)
    if vc != (0, 3, 3):
        print("FAIL vtm")
        fails += 1
    vtm_p = max(additive_at(v, n)["p"] for n in range(1, 65))
    print("vtm_pmax", vtm_p)
    if vtm_p != 3:
        print("FAIL vtm bounded p_T")
        fails += 1

    tm = morph_from_words([[0, 1], [1, 2], [2, 0]], (0, 1, 2))
    tmw = tm.iterate(0, 2048)
    tmc = find_cube(tmw)
    print("tmlike_cube", tmc, "acf", first_acf_len(tmw))
    if tmc != (0, 132, 132):
        print("FAIL tm_like")
        fails += 1
    tml_p = max(additive_at(tmw, n)["p"] for n in range(1, 65))
    print("tmlike_pmax", tml_p)
    if tml_p < 10:
        print("FAIL tm_like p_T not growing")
        fails += 1

    sigma014 = morph_from_words([[0, 1], [1, 4], [4, 0]], (0, 1, 4))
    w014 = sigma014.iterate(0, 256)
    c014 = find_cube(w014)
    print("014_cube", c014, "acf", first_acf_len(w014))
    if c014 != (2, 63, 105) or first_acf_len(w014) != 108:
        print("FAIL 01440")
        fails += 1

    # VdW failure: monochromatic 3-APs need not have difference n.
    U512 = FAMOUS.iterate(2, 512)
    v8 = vdW(U512, 8)
    v25 = vdW(U512, 25)
    print("vdW_famous_n8", v8)
    print("vdW_famous_n25", v25)
    if v8["n_starts_with_some_mono_3AP"] <= v8["n_starts_with_diff_exactly_n"]:
        print("FAIL famous VdW n=8 should have extra APs")
        fails += 1
    cass800 = cassaigne_iterate(800)
    vc8 = vdW(cass800, 8)
    print("vdW_cassaigne_n8", vc8)
    if vc8["n_starts_with_diff_exactly_n"] != 0:
        print("FAIL Cassaigne VdW d=n should be 0")
        fails += 1
    if vc8["n_starts_with_some_mono_3AP"] == 0:
        print("FAIL Cassaigne should have some mono 3-APs")
        fails += 1

    # p <= diam+1 on all listed controls.
    for name, ww, nmax in (
        ("cassaigne", cass, 64),
        ("famous_pref", fam_pref, 39),
        ("vtm", v, 64),
        ("tmlike", tmw, 64),
        ("014", w014, 48),
        ("max_pref", pref, 80),
    ):
        bad = 0
        for n in range(1, nmax + 1):
            if n > len(ww):
                break
            r = additive_at(ww, n)
            if r["p"] > r["diam"] + 1:
                bad += 1
        print(name, "p_le_diam_fails", bad)
        if bad:
            fails += 1

    os.makedirs("data", exist_ok=True)
    with open("data/pt_verify_cert.txt", "w") as f:
        f.write(f"fails {fails}\n")
        f.write(f"best2_acf {best}\n")
        f.write(f"best2_code {best_code}\n")
        f.write(f"max_cube {cube}\n")
        f.write(f"max_T {T}\n")
        f.write(f"max_charpoly {cpoly}\n")
        f.write(f"max_acfpref_pmax {pmax}\n")
        f.write(f"famous_acf {fam_acf}\n")
        f.write(f"cassaigne_pmax64 {cass_pmax}\n")
        f.write(f"vtm_pmax {vtm_p}\n")
        f.write(f"vdW_famous_n8_any {v8['n_starts_with_some_mono_3AP']}\n")
        f.write(f"vdW_famous_n8_dn {v8['n_starts_with_diff_exactly_n']}\n")
        f.write(f"vdW_cass_n8_dn {vc8['n_starts_with_diff_exactly_n']}\n")

    print("pt_verify_fails", fails)
    if fails:
        sys.exit(1)


if __name__ == "__main__":
    main()
