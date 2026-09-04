#!/usr/bin/env python3
"""Independent minors check of the famous Krylov package, first-cube T2·v,
reachable T·v filling, and AP-interior counterexample.

Does not import quotient_dyn.rank; only elementary determinants plus CSV reads.
"""
from itertools import combinations
from math import gcd
import csv
import os
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from desub_geometry import FAMOUS, ancestor_of, in_lattice, vdot  # noqa: E402

ONES = (1, 1, 1, 1)
T = (5, 4, 2, 1)
T2 = (3, 5, 7, 9)
T3 = (16, 14, 10, 8)
V = (1, -1, 1, -1)
A = (0, 1, 2, 3)
B = (0, 1, 3, 4)


def det3(M):
    a, b, c = M[0]
    d, e, f = M[1]
    g, h, i = M[2]
    return a * (e * i - f * h) - b * (d * i - f * g) + c * (d * h - e * g)


def det4(M):
    s = 0
    for j in range(4):
        minor = [[M[r][c] for c in range(4) if c != j] for r in range(1, 4)]
        s += (1 if j % 2 == 0 else -1) * M[0][j] * det3(minor)
    return s


def reachable_Tdots(U, Tvec, nmax=80, window=8):
    """T-sum differences of consecutive equal-length factors."""
    ST = [0]
    for a in U:
        ST.append(ST[-1] + Tvec[a])
    N = len(U)
    hits = {d: 0 for d in range(-window, window + 1)}
    npairs = 0
    for n in range(1, nmax + 1):
        for i in range(0, N - 2 * n + 1):
            s1 = ST[i + n] - ST[i]
            s2 = ST[i + 2 * n] - ST[i + n]
            dlt = s1 - s2
            npairs += 1
            if -window <= dlt <= window:
                hits[dlt] += 1
    return hits, npairs


def main():
    fails = 0
    M = [[0, 0, 1, 1], [0, 1, 0, 1], [1, 0, 1, 0], [1, 1, 0, 0]]
    T2c = tuple(sum(M[j][i] * T[j] for j in range(4)) for i in range(4))
    T3c = tuple(sum(M[j][i] * T2c[j] for j in range(4)) for i in range(4))
    if T2c != T2 or T3c != T3:
        print("FAIL T2/T3", T2c, T3c)
        fails += 1
    tdot = sum(T[i] * V[i] for i in range(4))
    t2dot = sum(T2[i] * V[i] for i in range(4))
    if tdot != 2 or t2dot != -4:
        print("FAIL T dots", tdot, t2dot)
        fails += 1
    minors = []
    rows = [ONES, T, T2]
    for cs in combinations(range(4), 3):
        Mm = [[rows[i][j] for j in cs] for i in range(3)]
        minors.append(det3(Mm))
    if not any(minors):
        print("FAIL expected rank 3", minors)
        fails += 1
    M4 = [list(ONES), list(T), list(T2), list(T3)]
    if det4(M4) != 0:
        print("FAIL expected T3 in span", det4(M4))
        fails += 1
    g = 0
    for x in T:
        g = gcd(g, x)
    if g != 1:
        print("FAIL gcd T", g)
        fails += 1
    if tdot not in range(-6, 7):
        print("FAIL defect window")
        fails += 1
    # L basis action: T2·e_i = 2
    e1 = (1, 0, -4, 3)
    e2 = (0, 1, -3, 2)
    if vdot(T, e1) != 0 or vdot(T, e2) != 0 or sum(e1) != 0 or sum(e2) != 0:
        print("FAIL L basis not in ker")
        fails += 1
    if vdot(T2, e1) != 2 or vdot(T2, e2) != 2:
        print("FAIL T2 on L", vdot(T2, e1), vdot(T2, e2))
        fails += 1
    # AP interior smallest from geometry, independent of CSV
    vap = (0, -1, 2, -1)
    if not in_lattice(vap, A) or in_lattice(vap, B):
        print("FAIL AP vector lattice membership", vap)
        fails += 1

    path = "data/famous_all_cubes.csv"
    t2_range = None
    ap_smallest = None
    n_ap_notB = 0
    if os.path.exists(path):
        t2s = []
        with open(path) as f:
            for row in csv.DictReader(f):
                v = tuple(int(x) for x in row["v_xy"].split(","))
                t2s.append(vdot(T2, v))
                if in_lattice(v, A) and not in_lattice(v, B):
                    n_ap_notB += 1
                    key = (int(row["d"]), int(row["i"]), v)
                    if ap_smallest is None or key < ap_smallest:
                        ap_smallest = key
        t2_range = (min(t2s), max(t2s), len(set(t2s)))
        if ap_smallest is None or ap_smallest[0:2] != (27, 47):
            print("FAIL expected AP interior at d=27 i=47", ap_smallest)
            fails += 1

    path8 = "data/famous_all_cubes_8k.csv"
    t2_8k = None
    if os.path.exists(path8):
        t2s = []
        with open(path8) as f:
            for row in csv.DictReader(f):
                v = (int(row["v0"]), int(row["v1"]), int(row["v2"]), int(row["v3"]))
                t2s.append(vdot(T2, v))
        t2_8k = (min(t2s), max(t2s), len(set(t2s)), len(t2s))

    sigma = FAMOUS
    W = sigma.iterate(2, 2048)
    U = ancestor_of(W, sigma, len(W))
    Tmap = {a: sigma.T[i] for i, a in enumerate(sigma.alph)}
    Tw = tuple(Tmap[a] for a in range(4))
    hits, npairs = reachable_Tdots(U, Tw, nmax=80, window=8)
    missing = [d for d in range(-8, 9) if hits[d] == 0]
    if missing:
        print("FAIL reachable T·v missing", missing)
        fails += 1
    if npairs != 75520:
        # 1024 ancestor: sum_{n=1}^{80} (1025-2n) = 80*1025 - 2*(80*81/2) = 82000-6480=75520
        print("FAIL pair count", npairs)

    print(
        f"quotient_verify failures={fails} T·v={tdot} T2·v={t2dot} "
        f"minors={minors} det4={det4(M4)} t2_2048={t2_range} t2_8k={t2_8k} "
        f"ap_notB={n_ap_notB} ap_smallest={ap_smallest} "
        f"reachable_min={min(hits.values())} npairs={npairs}"
    )
    if fails:
        raise SystemExit(1)


if __name__ == "__main__":
    main()
