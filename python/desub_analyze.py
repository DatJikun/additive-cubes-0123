#!/usr/bin/env python3
"""Summarise cube-anatomy CSVs: lattice occupancy, type automaton, A vs B."""
from __future__ import annotations

import csv
import os
import sys
from collections import Counter, defaultdict

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from desub_geometry import (
    FAMOUS,
    analyze_cube,
    ancestor_of,
    find_cube,
    in_lattice,
    primitive_lattice,
    vgcd,
    vl1,
    vdot,
)


def prim(v):
    g = vgcd(v)
    if g == 0:
        return (0, 0, 0, 0)
    p = tuple(x // g for x in v)
    for x in p:
        if x < 0:
            p = tuple(-t for t in p)
            break
        if x > 0:
            break
    return p


def load(path):
    with open(path) as f:
        return list(csv.DictReader(f))


def parse_v(row):
    if "v_xy" in row:
        return tuple(int(x) for x in row["v_xy"].split(","))
    return (int(row["v0"]), int(row["v1"]), int(row["v2"]), int(row["v3"]))


def summarise_cubes(rows, alph, title):
    print(f"\n=== {title} n={len(rows)} ===")
    if not rows:
        return
    aligned = sum(int(r["aligned"]) for r in rows)
    print(f"aligned {aligned} unaligned {len(rows)-aligned}")
    dirs = Counter()
    inLA = inLT = def0 = 0
    l1_by_k = []
    types = Counter()
    for r in rows:
        v = parse_v(r)
        dirs[prim(v)] += 1
        if int(r.get("in_LA", 0) or (1 if in_lattice(v, alph) else 0)):
            inLA += 1
        T = tuple(int(x) for x in r["T"].split(",")) if "T" in r and r["T"] else None
        if T is None and "T0" in r:
            T = (int(r["T0"]), int(r["T1"]), int(r["T2"]), int(r["T3"]))
        if T and sum(v) == 0 and vdot(T, v) == 0:
            inLT += 1
        db = int(r.get("db_xy", r.get("db", 0)))
        if db == 0:
            def0 += 1
        k = int(r.get("k", 0))
        l1_by_k.append((k, vl1(v), int(r["d"]), int(r["aligned"])))
        types[(int(r["r"]), int(r["s"]))] += 1
    print(f"in L(A) {inLA}  T-kernel-and-sum0 {inLT}  defect_xy=0 {def0}")
    print("residue types (r,s):", dict(types))
    print("primitive v directions (top 12):")
    for v, c in dirs.most_common(12):
        print(f"  {v}  count={c}  L1={vl1(v)}  in_L(A)={in_lattice(v, alph)}")
    unal = [t for t in l1_by_k if not t[3] and t[0] >= 2]
    if unal:
        worst = max(unal, key=lambda t: t[1])
        print(f"worst unaligned k>=2: L1={worst[1]} k={worst[0]} d={worst[2]}")
        # L1 vs k correlation: max L1 in k-buckets
        buckets = defaultdict(int)
        for k, l1, d, al in unal:
            buckets[k // 20] = max(buckets[k // 20], l1)
        print("max L1 by k/20:", dict(sorted(buckets.items())))


def famous_exact():
    sigma = FAMOUS
    W = sigma.iterate(2, 256)
    c = find_cube(W)
    U = ancestor_of(W, sigma, c[0] + 3 * c[1])
    an = analyze_cube(W, sigma, c, U)
    q = c[0] // 2
    print("\n=== Exact famous cube letters ===")
    print("i,d,sum", c)
    print("U[q-1:q+15]", "".join(map(str, U[q - 1 : q + 16])), "q=", q)
    print("X complete", "".join(map(str, an.X.complete_letters)))
    print("Y complete", "".join(map(str, an.Y.complete_letters)))
    print("Z complete", "".join(map(str, an.Z.complete_letters)))
    print("X left", an.X.left, "right", an.X.right, "def", an.X.defect)
    print("Y left", an.Y.left, "right", an.Y.right, "def", an.Y.defect)
    print("Z left", an.Z.left, "right", an.Z.right, "def", an.Z.defect)
    print("equations: T=(5,4,2,1)")
    print("  T·(1,-1,1,-1) + (1-3) = 2-2 = 0")
    print("  T·(-1,1,-1,1) + (3-1) = -2+2 = 0")


def main():
    famous_exact()
    if os.path.exists("data/famous_all_cubes.csv"):
        summarise_cubes(load("data/famous_all_cubes.csv"), (0, 1, 2, 3), "famous all cubes (python)")
    if os.path.exists("data/structured_first_cubes.csv"):
        rows = load("data/structured_first_cubes.csv")
        A = [r for r in rows if r["alph"] == "0123"]
        B = [r for r in rows if r["alph"] != "0123"]
        summarise_cubes(A, (0, 1, 2, 3), "structured first cubes A")
        summarise_cubes(B, (0, 1, 3, 4), "structured first cubes B")
        print("\nstructured A vs B first-cube alignment")
        print("  A aligned", sum(int(r["aligned"]) for r in A), "/", len(A))
        print("  B aligned", sum(int(r["aligned"]) for r in B), "/", len(B))
        print("  A in_LA", sum(int(r["in_LA"]) for r in A), "A in_LT", sum(int(r["in_LT"]) for r in A))
        print("  B in_LA", sum(int(r["in_LA"]) for r in B), "B in_LT", sum(int(r["in_LT"]) for r in B))
        print("  A anc_acf", sum(int(r["anc_acf"]) for r in A), "/", len(A))
        print("  B anc_acf", sum(int(r["anc_acf"]) for r in B), "/", len(B))
    for path, alph, name in [
        ("data/scan2_A.csv", (0, 1, 2, 3), "scan2 A"),
        ("data/scan2_B.csv", (0, 1, 3, 4), "scan2 B"),
    ]:
        if os.path.exists(path):
            summarise_cubes(load(path), alph, name)
    print("\n=== L(A) vs L(B) primitive inf<=4 ===")
    print("A", primitive_lattice((0, 1, 2, 3), 4)[:8])
    print("B", primitive_lattice((0, 1, 3, 4), 4)[:8])


if __name__ == "__main__":
    main()
