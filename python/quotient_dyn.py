#!/usr/bin/env python3
"""Quotient dynamics of the defect equation T·v + Δδ = 0.

Central algebraic facts, proved by exact integer linear algebra:

* Z^4 / ker(T) ≅ gcd(T) Z, hence infinite. Cubes occupy finitely many
  classes therein (T·v ∈ -D, D finite). That is Theorem L, not a
  finite-state dynamical reduction.
* M always preserves ker(1). M preserves L := ker(1)∩ker(T) iff
  M^T T lies in span{1, T}.
* The smallest M-invariant linear package containing the defect form T
  is the Krylov subspace K = span{(M^T)^j A : j≥0} + span{1}.
* The maximal lattice that can be quotiented at every scale is K^⊥.

This module computes those objects, the residue-type Δn, the extra
coordinate T2·v on cubes, and a 2-uniform census of Krylov ranks.
"""
from __future__ import annotations

import csv
import os
import sys
from collections import Counter
from itertools import combinations, product
from math import gcd
from typing import Optional, Sequence, Tuple

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from desub_geometry import (  # noqa: E402
    FAMOUS,
    UniformMorph,
    analyze_cube,
    ancestor_of,
    defect_set,
    find_cube,
    in_lattice,
    morph_from_words,
    n_complete_closed_form,
    two_fragment_defects,
    vdot,
    vgcd,
    vl1,
)

Vec4 = Tuple[int, int, int, int]
Mat = Tuple[Vec4, Vec4, Vec4, Vec4]


def _gcd_list(xs) -> int:
    g = 0
    for x in xs:
        g = gcd(g, abs(int(x)))
    return g


def mv(M: Sequence[Sequence[int]], x: Sequence[int]) -> Vec4:
    return tuple(sum(M[i][j] * x[j] for j in range(4)) for i in range(4))  # type: ignore[return-value]


def Mtv(M: Sequence[Sequence[int]], x: Sequence[int]) -> Vec4:
    return tuple(sum(M[j][i] * x[j] for j in range(4)) for i in range(4))  # type: ignore[return-value]


def integer_rank(rows: Sequence[Sequence[int]]) -> int:
    """Rank over Q via gcd of maximal minors (exact)."""
    rows = [tuple(int(x) for x in r) for r in rows if any(r)]
    if not rows:
        return 0
    n = len(rows)
    m = len(rows[0])
    kmax = min(n, m)
    for k in range(kmax, 0, -1):
        for rs in combinations(range(n), k):
            for cs in combinations(range(m), k):
                det = _det([ [rows[i][j] for j in cs] for i in rs ])
                if det != 0:
                    return k
    return 0


def _det(A: list[list[int]]) -> int:
    n = len(A)
    if n == 1:
        return A[0][0]
    if n == 2:
        return A[0][0] * A[1][1] - A[0][1] * A[1][0]
    if n == 3:
        a, b, c = A[0]
        d, e, f = A[1]
        g, h, i = A[2]
        return a * (e * i - f * h) - b * (d * i - f * g) + c * (d * h - e * g)
    # n=4 Laplace
    s = 0
    for j in range(4):
        minor = [[A[r][c] for c in range(4) if c != j] for r in range(1, 4)]
        s += ((-1) ** j) * A[0][j] * _det(minor)
    return s


def in_span(target: Sequence[int], gens: Sequence[Sequence[int]]) -> bool:
    return integer_rank(list(gens) + [target]) == integer_rank(gens)


def ker_T_param(T: Vec4):
    """Integer parametrization of ker(T): rank 3 if T≠0."""
    # Find a coordinate with T_i != 0 of minimal |T_i| to solve
    g = _gcd_list(T)
    return g


def L_basis(T: Vec4) -> list[Vec4]:
    """Integer basis of ker(1)∩ker(T) by scanning short vectors then reducing."""
    found: list[Vec4] = []
    bound = 8
    for x0, x1, x2 in product(range(-bound, bound + 1), repeat=3):
        x3 = -x0 - x1 - x2
        v = (x0, x1, x2, x3)
        if v == (0, 0, 0, 0):
            continue
        if vdot(T, v) != 0:
            continue
        if vgcd(v) != 1:
            continue
        if not found:
            found.append(v)
            continue
        if integer_rank(found + [v]) > len(found):
            found.append(v)
        if len(found) == 2:
            break
    if len(found) != 2:
        # T parallel to 1 ⇒ L has rank 3
        if integer_rank([T, (1, 1, 1, 1)]) == 1:
            return [(1, -1, 0, 0), (1, 0, -1, 0), (1, 0, 0, -1)]
        raise RuntimeError(f"L-basis failed for T={T}: {found}")
    return found


def particular_solution(T: Vec4, dn: int, tdot: int) -> Optional[Vec4]:
    """Find x with 1·x=dn, T·x=tdot, preferring small L1."""
    ones = (1, 1, 1, 1)
    gdiff = _gcd_list([T[i] - T[j] for i in range(4) for j in range(i)])
    # image of (1,T): (dn, tdot) must be compatible with gcd of minors
    if gdiff and (tdot - T[0] * dn) % gdiff != 0 and False:
        pass
    best = None
    bound = 8
    while best is None and bound <= 16:
        for x0, x1, x2 in product(range(-bound, bound + 1), repeat=3):
            x3 = dn - x0 - x1 - x2
            v = (x0, x1, x2, x3)
            if vdot(T, v) == tdot:
                if best is None or vl1(v) < vl1(best):
                    best = v
        bound *= 2
    return best


def decompose_mod_L(T: Vec4, v: Vec4) -> dict:
    """v = particular(1·v, T·v) + L-combination, via brute on small coeffs then LS."""
    dn = sum(v)
    tdot = vdot(T, v)
    part = particular_solution(T, dn, tdot)
    basis = L_basis(T)
    rem = tuple(v[i] - (part[i] if part else 0) for i in range(4))
    # solve x b1 + y b2 = rem over Q, check integer
    b1, b2 = basis[0], basis[1]
    # 2x2 on first two independent coords
    coeffs = None
    for i, j in combinations(range(4), 2):
        det = b1[i] * b2[j] - b1[j] * b2[i]
        if det == 0:
            continue
        # Cramer's
        if (rem[i] * b2[j] - rem[j] * b2[i]) % det == 0 and (
            b1[i] * rem[j] - b1[j] * rem[i]
        ) % det == 0:
            x = (rem[i] * b2[j] - rem[j] * b2[i]) // det
            y = (b1[i] * rem[j] - b1[j] * rem[i]) // det
            recon = tuple(x * b1[t] + y * b2[t] for t in range(4))
            if recon == rem:
                coeffs = (x, y)
                break
    return {
        "dn": dn,
        "Tdot": tdot,
        "particular": part,
        "L_basis": basis,
        "L_coeffs": coeffs,
        "remainder": rem,
    }


def krylov_package(M, A, T) -> dict:
    ones = (1, 1, 1, 1)
    vecs_A = [ones, tuple(A), tuple(T)]
    vecs_T = [ones, tuple(T)]
    x = tuple(T)
    orbit = [tuple(T)]
    for _ in range(6):
        x = Mtv(M, x)
        orbit.append(x)
        vecs_A.append(x)
        vecs_T.append(x)
    ranks = {
        "rank_1": 1,
        "rank_1_T": integer_rank([ones, T]),
        "rank_1_T_T2": integer_rank([ones, T, orbit[1]]),
        "rank_1_A_T": integer_rank([ones, A, T]),
        "krylov_A": integer_rank(vecs_A),
        "krylov_T": integer_rank(vecs_T),
        "rank_orbit_T": integer_rank(orbit),
    }
    T2 = orbit[1]
    L_inv = in_span(T2, [ones, T])
    kerT_inv = in_span(T2, [T])  # M preserves ker(T) iff T2 ∥ T
    T_affine = in_span(T, [ones, A])
    return {
        "T": tuple(T),
        "T2": T2,
        "T3": orbit[2],
        "ranks": ranks,
        "L_invariant": L_inv,
        "kerT_invariant": kerT_inv,
        "T_affine": T_affine,
        "krylov_dim": ranks["krylov_A"],
        "krylov_A": ranks["krylov_A"],
        "krylov_T": ranks["krylov_T"],
    }


def delta_n_table(m: int, k: int = 3) -> list[tuple]:
    rows = []
    for r in range(m):
        for s in range(m):
            def n(rr):
                return n_complete_closed_form(m, rr, s, k)

            rY = (r + s) % m
            rZ = (r + 2 * s) % m
            nX, nY, nZ = n(r), n(rY), n(rZ)
            rows.append((r, s, nX - nY, nY - nZ, nX, nY, nZ))
    return rows


def analyze_sigma(sigma: UniformMorph) -> dict:
    M = sigma.incidence()
    T = sigma.T
    A = sigma.alph
    pkg = krylov_package(M, A, T)
    gT = _gcd_list(T)
    D1 = defect_set(sigma)
    D2 = two_fragment_defects(sigma)
    Dd = {x - y for x in D2 for y in D2}
    pkg.update(
        {
            "m": sigma.m,
            "gcd_T": gT,
            "Q_isomorphic_to": f"{gT}Z",
            "n_sums": sigma.n_sums,
            "defect_set": sorted(D1),
            "two_fragment": sorted(D2),
            "defect_diff_set": sorted(Dd),
            "n_defect_classes": len({d for d in Dd if d % gT == 0}),
            "L_basis": L_basis(T) if integer_rank([T, (1, 1, 1, 1)]) == 2 else "T∥1",
        }
    )
    return pkg


def famous_cube_fiber():
    sigma = FAMOUS
    M = sigma.incidence()
    T = sigma.T
    T2 = Mtv(M, T)
    W = sigma.iterate(2, 2048)
    U = ancestor_of(W, sigma, len(W))
    # all cubes from csv if present else scan prefix 512
    cubes = []
    path = "data/famous_all_cubes.csv"
    if os.path.exists(path):
        with open(path) as f:
            for row in csv.DictReader(f):
                v = tuple(int(x) for x in row["v_xy"].split(","))
                cubes.append(
                    {
                        "i": int(row["i"]),
                        "d": int(row["d"]),
                        "v": v,
                        "db": int(row["db_xy"]),
                        "aligned": int(row["aligned"]),
                        "Tdot": int(row["Tdot_v"]),
                    }
                )
    else:
        from desub_collect import all_cubes, analyze_all_cubes_of_word

        rows, *_ = analyze_all_cubes_of_word(sigma, 2, 512, cube_limit=200)
        for row in rows:
            v = tuple(int(x) for x in row["v_xy"].split(","))
            cubes.append({"i": row["i"], "d": row["d"], "v": v, "db": row["db_xy"], "aligned": row["aligned"], "Tdot": row["Tdot_v"]})

    t2s = []
    L_coeffs = []
    inLA = 0
    mixing = []
    for c in cubes:
        v = c["v"]
        t2 = vdot(T2, v)
        t2s.append(t2)
        dec = decompose_mod_L(T, v)
        L_coeffs.append(dec["L_coeffs"])
        if in_lattice(v, sigma.alph):
            inLA += 1
        mixing.append((c["d"], vl1(v), t2, dec["L_coeffs"], v, c["Tdot"], c["db"]))
    return {
        "T2": T2,
        "n_cubes": len(cubes),
        "T2_dot_v_range": (min(t2s), max(t2s)) if t2s else None,
        "T2_unique": sorted(set(t2s)),
        "n_in_LA": inLA,
        "n_Tdot_zero": sum(1 for c in cubes if c["Tdot"] == 0),
        "n_aligned": sum(c["aligned"] for c in cubes),
        "worst_T2": max(mixing, key=lambda t: abs(t[2])) if mixing else None,
        "worst_L1": max(mixing, key=lambda t: t[1]) if mixing else None,
        "first": mixing[0] if mixing else None,
        "sample_L_coeffs": L_coeffs[:8],
        "pkg": analyze_sigma(sigma),
    }


def m2_krylov_census(alph=(0, 1, 2, 3)):
    """All 65536 2-uniform morphisms: Krylov rank and L-invariance. No word generation."""
    ranks = Counter()
    ranksT = Counter()
    Linv = Counter()
    kerTinv = Counter()
    n_sums_by_kdim = Counter()
    n4_Linv_aff = 0
    n4_Linv_not = 0
    n3_Linv = 0
    nge3_Linv_notaff = 0
    nge3_kerT = 0
    examples = {2: None, 3: None, 4: None}
    for code in range(65536):
        t = code
        imgs = []
        for _ in range(4):
            a = t % 4
            b = (t // 4) % 4
            t //= 16
            imgs.append([alph[a], alph[b]])
        sigma = morph_from_words(imgs, alph)
        M = sigma.incidence()
        pkg = krylov_package(M, alph, sigma.T)
        kdim = pkg["krylov_A"]
        ranks[kdim] += 1
        ranksT[pkg["krylov_T"]] += 1
        Linv[pkg["L_invariant"]] += 1
        kerTinv[pkg["kerT_invariant"]] += 1
        n_sums_by_kdim[(sigma.n_sums, kdim)] += 1
        if sigma.n_sums == 4 and pkg["L_invariant"]:
            if pkg["T_affine"]:
                n4_Linv_aff += 1
            else:
                n4_Linv_not += 1
        if sigma.n_sums == 3 and pkg["L_invariant"]:
            n3_Linv += 1
        if sigma.n_sums >= 3 and pkg["L_invariant"] and not pkg["T_affine"]:
            nge3_Linv_notaff += 1
        if sigma.n_sums >= 3 and pkg["kerT_invariant"]:
            nge3_kerT += 1
        if examples.get(kdim) is None:
            examples[kdim] = (sigma.code(), sigma.T, pkg["T2"], pkg["L_invariant"])
    return {
        "alph": alph,
        "krylov_dim": dict(ranks),
        "krylov_T": dict(ranksT),
        "L_invariant": dict(Linv),
        "kerT_invariant": dict(kerTinv),
        "n_sums_x_kdim": {f"{a},{b}": c for (a, b), c in sorted(n_sums_by_kdim.items())},
        "n4_Linv_affine": n4_Linv_aff,
        "n4_Linv_not_affine": n4_Linv_not,
        "n3_Linv": n3_Linv,
        "nge3_Linv_notaff": nge3_Linv_notaff,
        "nge3_kerT": nge3_kerT,
        "examples": examples,
    }


def ap_in_interior_or_boundary(sigma: UniformMorph, v: Vec4, db: int, alph) -> str:
    """Where the AP lattice can appear."""
    inL = in_lattice(v, alph)
    # fragments realizing 0+2=1+1 would be a length-2 defect; single-letter defects cannot
    return "interior_L(A)" if inL else "not_L(A)_interior"


def main():
    print("=== PHASE 1: Z^4 / ker(T) ===")
    sigma = FAMOUS
    pkg = analyze_sigma(sigma)
    for k, val in pkg.items():
        print(f"  {k}: {val}")
    print("  Δn table m=2 (k=3):")
    for row in delta_n_table(2):
        print("   ", row)
    print("  Δn table m=3 distinct Δn values:", sorted({r[2] for r in delta_n_table(3)} | {r[3] for r in delta_n_table(3)}))

    print("\n=== PHASE 1b: M-action on L ===")
    M = sigma.incidence()
    T = sigma.T
    T2 = Mtv(M, T)
    for b in L_basis(T):
        Mb = mv(M, b)
        print(f"  b={b}  Mb={Mb}  1·Mb={sum(Mb)}  T·Mb={vdot(T, Mb)}  T2·b={vdot(T2, b)}")

    print("\n=== PHASE 2-3: famous cubes T2·v (unbounded extra coordinate) ===")
    fib = famous_cube_fiber()
    print("  T2", fib["T2"])
    print("  n_cubes", fib["n_cubes"], "in L(A)", fib["n_in_LA"], "T·v=0", fib["n_Tdot_zero"], "aligned", fib["n_aligned"])
    print("  T2·v range", fib["T2_dot_v_range"], "n_unique", len(fib["T2_unique"]))
    print("  first cube", fib["first"])
    print("  worst |T2·v|", fib["worst_T2"])
    print("  worst L1", fib["worst_L1"])

    print("\n=== PHASE 6: AP in interiors among famous cubes ===")
    path = "data/famous_all_cubes.csv"
    if os.path.exists(path):
        n_ap_int = 0
        n_ap_only = 0
        smallest = None
        smallest_only = None
        with open(path) as f:
            for row in csv.DictReader(f):
                v = tuple(int(x) for x in row["v_xy"].split(","))
                if in_lattice(v, (0, 1, 2, 3)):
                    n_ap_int += 1
                    key = (int(row["d"]), int(row["i"]), v)
                    if smallest is None or key < smallest:
                        smallest = key
                    if not in_lattice(v, (0, 1, 3, 4)):
                        n_ap_only += 1
                        if smallest_only is None or key < smallest_only:
                            smallest_only = key
        print("  cubes with v in L(A) (interior letter kernel):", n_ap_int, "smallest", smallest)
        print("  cubes with v in L(A)\\L(B):", n_ap_only, "smallest", smallest_only)

    print("\n=== PHASE 7: 2-uniform Krylov census (matrix only) ===")
    for name, alph in [("A", (0, 1, 2, 3)), ("B", (0, 1, 3, 4))]:
        c = m2_krylov_census(alph)
        print(f"  {name} krylov_A", c["krylov_dim"])
        print(f"  {name} krylov_T", c["krylov_T"])
        print(f"  {name} L_invariant", c["L_invariant"])
        print(f"  {name} kerT_invariant", c["kerT_invariant"])
        print(f"  {name} n4_Linv affine/not", c["n4_Linv_affine"], c["n4_Linv_not_affine"])
        print(f"  {name} n3_Linv", c["n3_Linv"], "nge3_Linv_notaff", c["nge3_Linv_notaff"], "nge3_kerT", c["nge3_kerT"])
        print(f"  {name} n_sums_x_kdim", c["n_sums_x_kdim"])
        print(f"  {name} examples", c["examples"])
        os.makedirs("data", exist_ok=True)
        with open(f"data/krylov_census_{name}.txt", "w") as f:
            for k, v in c.items():
                f.write(f"{k}: {v}\n")

    print("\n=== identity checks ===")
    # first cube T·v + db = 0 already known; T2 not constrained
    v = (1, -1, 1, -1)
    print("  first T·v", vdot(T, v), "T2·v", vdot(T2, v), "A·v", vdot(sigma.alph, v))
    print("  Q class of first cube is the integer T·v=2, one of finitely many cube classes in infinite Q")


if __name__ == "__main__":
    main()
