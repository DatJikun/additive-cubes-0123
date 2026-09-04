#!/usr/bin/env python3
"""Triple geometry of additive cubes under uniform substitution.

ω(F) := T·ψ_F + δ_F equals the numerical sum of the corresponding interval
in W = σ(U).  Hence X,Y,Z consecutive equal-length factors form an additive
cube iff ω(X)=ω(Y)=ω(Z).  That identity is Theorem K; this module records
the *shared-middle* structure that pairwise Q-quotients cannot see:

* exactly m² residue triple types, indexed by (r,s);
* at most four ancestor fragment letters, with junctions shared;
* complementary fragments of one image sum to T(a_junc).

The concatenation graph at step d is a disjoint union of d paths on
positions; after identifying equal d-factors it is finite for each d
but not d-independent.  Cubes are equal-weight paths of length 2.
"""
from __future__ import annotations

import os
import sys
from collections import Counter, defaultdict
from dataclasses import dataclass
from typing import Optional, Sequence, Tuple

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from desub_geometry import (  # noqa: E402
    FAMOUS,
    UniformMorph,
    analyze_cube,
    ancestor_of,
    find_cube,
    interval_pieces,
    prefix_sums,
    view_interval,
    vdot,
)

Vec4 = Tuple[int, int, int, int]


@dataclass(frozen=True)
class TripleType:
    """Residue/fragment slot signature of (X,Y,Z), determined by (m,r,s)."""

    m: int
    r: int
    s: int
    rY: int
    rZ: int
    rEnd: int
    x_left: int   # length; 0 if absent
    x_right: int
    y_left: int
    y_right: int
    z_left: int
    z_right: int
    xy_shared: bool
    yz_shared: bool
    n_fragment_letters: int

    def slot_key(self) -> Tuple:
        return (
            self.x_left, self.x_right, self.y_left, self.y_right,
            self.z_left, self.z_right, self.xy_shared, self.yz_shared,
            self.n_fragment_letters,
        )


def triple_type(m: int, r: int, s: int) -> TripleType:
    rY = (r + s) % m
    rZ = (r + 2 * s) % m
    rEnd = (r + 3 * s) % m
    x_left = 0 if r == 0 else m - r
    x_right = rY  # prefix length at XY junction; 0 if aligned
    y_left = 0 if rY == 0 else m - rY
    y_right = rZ
    z_left = 0 if rZ == 0 else m - rZ
    z_right = rEnd
    xy_shared = rY != 0
    yz_shared = rZ != 0
    n_let = (1 if x_left else 0) + (1 if xy_shared else 0) + (1 if yz_shared else 0) + (1 if z_right else 0)
    return TripleType(
        m=m, r=r, s=s, rY=rY, rZ=rZ, rEnd=rEnd,
        x_left=x_left, x_right=x_right, y_left=y_left, y_right=y_right,
        z_left=z_left, z_right=z_right,
        xy_shared=xy_shared, yz_shared=yz_shared,
        n_fragment_letters=n_let,
    )


def all_triple_types(m: int) -> list[TripleType]:
    return [triple_type(m, r, s) for r in range(m) for s in range(m)]


def type_census(m: int) -> dict:
    types = all_triple_types(m)
    slots = Counter(t.slot_key() for t in types)
    nlet = Counter(t.n_fragment_letters for t in types)
    return {
        "m": m,
        "n_residue_types": len(types),  # always m²
        "n_distinct_slot_signatures": len(slots),
        "fragment_letter_histogram": dict(nlet),
        "max_fragment_letters": max(nlet),
        "types": types,
    }


def junction_cover_ok(sigma: UniformMorph, letter: int, prefix_len: int) -> bool:
    """prefix of length t plus complementary suffix equals the full image sum."""
    m = sigma.m
    img = sigma.images[letter]
    t = prefix_len
    return sum(img[:t]) + sum(img[t:]) == sum(img) == sigma.T_of(letter)


def boundary_fragments(p: int, length: int, m: int, U: Sequence[int]):
    """Return (left_suffix, right_prefix) fragment triples (letter, a, b) or None.

    left_suffix: a != 0 (proper suffix of an image).
    right_prefix: b != m (proper prefix of an image).
    They may be the same piece when the interval sits inside one block.
    """
    left = None
    right = None
    for blk, a, b in interval_pieces(p, length, m):
        if a == 0 and b == m:
            continue
        letter = U[blk]
        if a != 0:
            left = (letter, a, b, blk)
        if b != m:
            right = (letter, a, b, blk)
    return left, right


def triple_letters(anatomy, U: Sequence[int], m: int) -> dict:
    """Ancestor letters actually used as fragments on a concrete cube."""
    X, Y, Z = anatomy.X, anatomy.Y, anatomy.Z
    Xl, Xr = boundary_fragments(X.p, X.length, m, U)
    Yl, Yr = boundary_fragments(Y.p, Y.length, m, U)
    Zl, Zr = boundary_fragments(Z.p, Z.length, m, U)
    xy_shared = Xr is not None and Yl is not None and Xr[3] == Yl[3]
    yz_shared = Yr is not None and Zl is not None and Yr[3] == Zl[3]
    letters = []
    if Xl:
        letters.append(("Xleft", Xl[0], Xl[3]))
    if xy_shared:
        letters.append(("XY", Xr[0], Xr[3]))
    elif Xr:
        letters.append(("Xright", Xr[0], Xr[3]))
    if yz_shared:
        letters.append(("YZ", Yr[0], Yr[3]))
    elif Yr:
        letters.append(("Yright", Yr[0], Yr[3]))
    if Zr:
        letters.append(("Zright", Zr[0], Zr[3]))
    Y_blocks = set(range(Y.pieces[0][0], Y.pieces[-1][0] + 1))
    Xleft_in_Y = Xl is not None and Xl[3] in Y_blocks
    return {
        "Xl": Xl, "Xr": Xr, "Yl": Yl, "Yr": Yr, "Zl": Zl, "Zr": Zr,
        "xy_shared": xy_shared,
        "yz_shared": yz_shared,
        "letters": letters,
        "n_letter_slots": len(letters),
        "Xleft_block_in_Y": Xleft_in_Y,
        "Xleft_letter": None if Xl is None else Xl[0],
    }


def famous_junction_check(anatomy, sigma, U) -> dict:
    """Verify complementary fragments at shared junctions sum to T(letter)."""
    m = sigma.m
    info = triple_letters(anatomy, U, m)
    fails = []
    if info["xy_shared"]:
        a = info["Xr"][0]
        t = info["Xr"][2] - info["Xr"][1]
        if not junction_cover_ok(sigma, a, t):
            fails.append("XY")
        if info["Yl"][0] != a:
            fails.append("XY_letter_mismatch")
        if t + (info["Yl"][2] - info["Yl"][1]) != m:
            fails.append("XY_len")
    if info["yz_shared"]:
        a = info["Yr"][0]
        t = info["Yr"][2] - info["Yr"][1]
        if not junction_cover_ok(sigma, a, t):
            fails.append("YZ")
        if info["Zl"][0] != a:
            fails.append("YZ_letter_mismatch")
        if t + (info["Zl"][2] - info["Zl"][1]) != m:
            fails.append("YZ_len")
    return {"fails": fails, "info": info}


def linear_relations(anatomy, sigma: UniformMorph) -> dict:
    X, Y, Z = anatomy.X, anatomy.Y, anatomy.Z
    v_xy, v_yz = anatomy.v_xy, anatomy.v_yz
    v_xz = tuple(a + b for a, b in zip(v_xy, v_yz))
    psi_xz = tuple(a - b for a, b in zip(X.psi, Z.psi))
    omega = (X.true_sum, Y.true_sum, Z.true_sum)
    return {
        "psiX_minus_psiZ_equals_vxy_plus_vyz": psi_xz == v_xz,
        "omega_equal": omega[0] == omega[1] == omega[2],
        "omega": omega,
        "T_vxy": vdot(sigma.T, v_xy),
        "deltaY_minus_deltaX": Y.defect - X.defect,
        "T_vyz": vdot(sigma.T, v_yz),
        "deltaZ_minus_deltaY": Z.defect - Y.defect,
        "dn_xy": X.n_complete - Y.n_complete,
        "dn_yz": Y.n_complete - Z.n_complete,
        "1_vxy": sum(v_xy),
        "1_vyz": sum(v_yz),
    }


# ---------------------------------------------------------------------------
# Sliding-window cubes / squares (prefix sums)
# ---------------------------------------------------------------------------

def pair_triple_counts(w: Sequence[int], nmax: Optional[int] = None) -> dict:
    """O(n²) count of additive squares and cubes on a prefix.

    square: ω_i = ω_{i+d}
    one_sided_middle: ω_i = ω_{i+d} but ω_{i+d} != ω_{i+2d} (and Z exists)
    cube: ω_i = ω_{i+d} = ω_{i+2d}
    """
    n = len(w) if nmax is None else min(len(w), nmax)
    S = prefix_sums(w[:n])
    n_sq = 0
    n_cube = 0
    n_pair_no_triple = 0  # XY equal, Z exists, YZ unequal
    n_yz_only = 0
    first_sq = None
    first_cube = None
    # middles Y=(j,d) with a left pair, right pair
    both = 0
    left_only = 0
    right_only = 0
    neither = 0
    n_middles = 0
    for d in range(1, n // 2 + 1):
        for i in range(0, n - 2 * d + 1):
            s1 = S[i + d] - S[i]
            s2 = S[i + 2 * d] - S[i + d]
            if s1 == s2:
                n_sq += 1
                if first_sq is None:
                    first_sq = (i, d, s1)
            if i + 3 * d <= n:
                s3 = S[i + 3 * d] - S[i + 2 * d]
                if s1 == s2 == s3:
                    n_cube += 1
                    if first_cube is None:
                        first_cube = (i, d, s1)
                elif s1 == s2:
                    n_pair_no_triple += 1
                elif s2 == s3:
                    n_yz_only += 1
    # occurrence-middles: Y starts at j, length d, both neighbors exist
    for d in range(1, n // 3 + 1):
        for j in range(d, n - 2 * d + 1):
            n_middles += 1
            sX = S[j] - S[j - d]
            sY = S[j + d] - S[j]
            sZ = S[j + 2 * d] - S[j + d]
            L = sX == sY
            R = sY == sZ
            if L and R:
                both += 1
            elif L:
                left_only += 1
            elif R:
                right_only += 1
            else:
                neither += 1
    return {
        "n": n,
        "n_squares": n_sq,
        "n_cubes": n_cube,
        "n_XY_equal_Z_exists_not_cube": n_pair_no_triple,
        "n_YZ_equal_not_XY": n_yz_only,
        "first_square": first_sq,
        "first_cube": first_cube,
        "middles": n_middles,
        "P_and_S": both,
        "P_only": left_only,
        "S_only": right_only,
        "neither": neither,
    }


def first_square(w: Sequence[int]) -> Optional[Tuple[int, int, int]]:
    n = len(w)
    S = prefix_sums(w)
    for i in range(0, n - 1):
        dmax = (n - i) // 2
        for d in range(1, dmax + 1):
            if S[i + d] - S[i] == S[i + 2 * d] - S[i + d]:
                return (i, d, S[i + d] - S[i])
    return None


def omega_range(w: Sequence[int], d: int) -> Tuple[int, int, int]:
    S = prefix_sums(w)
    vals = [S[i + d] - S[i] for i in range(0, len(w) - d + 1)]
    return (min(vals), max(vals), len(set(vals)))


def cassaigne_word(n: int) -> list[int]:
    h = {0: [0, 3], 1: [4, 3], 3: [1], 4: [0, 1]}
    w = [0]
    while len(w) < n:
        nxt = []
        for a in w:
            nxt.extend(h[a])
        if len(nxt) <= len(w):
            break
        w = nxt
    return w[:n]


def factor_sum_graph_stats(w: Sequence[int], d: int, offset: int = 0) -> dict:
    """Concatenation graph of non-overlapping d-blocks starting at `offset`.

    Vertices = distinct d-factors in this parsing. Edge u->v if uv occurs.
    Equal-weight length-2 paths = cubes aligned with this parsing.
    """
    n = len(w)
    blocks = []
    pos = offset
    while pos + d <= n:
        blk = tuple(w[pos : pos + d])
        blocks.append((blk, sum(blk)))
        pos += d
    verts = {}
    for blk, sm in blocks:
        verts.setdefault(blk, sm)
    edges = set()
    eq_edges = 0
    paths2_eq = 0
    for i in range(len(blocks) - 1):
        u, su = blocks[i]
        v, sv = blocks[i + 1]
        edges.add((u, v))
        if su == sv:
            eq_edges += 1
    for i in range(len(blocks) - 2):
        if blocks[i][1] == blocks[i + 1][1] == blocks[i + 2][1]:
            paths2_eq += 1
    return {
        "d": d,
        "offset": offset,
        "n_blocks": len(blocks),
        "n_vertices": len(verts),
        "n_edge_occurrences": max(0, len(blocks) - 1),
        "n_distinct_edges": len(edges),
        "n_equal_weight_consecutive": eq_edges,
        "n_equal_weight_2paths": paths2_eq,
        "n_distinct_sums": len(set(verts.values())),
    }


def main():
    print("=== PHASE 2: triple types ===")
    for m in range(2, 8):
        c = type_census(m)
        print(
            f"  m={m} residue_types={c['n_residue_types']} "
            f"slot_sigs={c['n_distinct_slot_signatures']} "
            f"n_letters={c['fragment_letter_histogram']} max={c['max_fragment_letters']}"
        )

    print("\n=== PHASE 1+5: famous cube ω and junctions ===")
    sigma = FAMOUS
    W = sigma.iterate(2, 512)
    cube = find_cube(W)
    U = ancestor_of(W, sigma, cube[0] + 3 * cube[1])
    an = analyze_cube(W, sigma, cube, U)
    rel = linear_relations(an, sigma)
    print("  cube", cube, "type", (an.X.r, an.X.s), "->", (an.Y.r, an.Y.s), (an.Z.r, an.Z.s))
    print("  relations", {k: v for k, v in rel.items() if k != "omega"}, "omega", rel["omega"])
    tt = triple_type(2, an.X.r, an.X.s)
    print("  predicted rY,rZ,n_let", tt.rY, tt.rZ, tt.n_fragment_letters)
    jc = famous_junction_check(an, sigma, U)
    print("  junction fails", jc["fails"], "letters", jc["info"]["letters"])
    print("  Xleft block in Y?", jc["info"]["Xleft_block_in_Y"], "Xleft", jc["info"]["Xleft_letter"])

    print("\n=== PHASE 4+6: pair vs triple on famous ===")
    W2 = sigma.iterate(2, 1024)
    st = pair_triple_counts(W2, 512)
    print("  famous prefix", st)

    print("\n=== CLAIM C: ω unbounded ===")
    for d in (1, 5, 10, 25, 50, 100):
        mn, mx, nd = omega_range(W2, d)
        print(f"  d={d} omega in [{mn},{mx}] n_distinct={nd}")

    print("\n=== CLAIM A/D: Cassaigne squares vs cubes ===")
    C = cassaigne_word(4000)
    print("  |C|", len(C), "first cube", find_cube(C))
    print("  first square", first_square(C))
    stc = pair_triple_counts(C, 800)
    print("  cassaigne prefix 800", {k: stc[k] for k in stc if k != "middles"})

    print("\n=== concatenation graph sample (famous d=8 offsets 0,1) ===")
    for off in range(2):
        print(" ", factor_sum_graph_stats(W2[:256], 8, off))

    print("\n=== A vs B pair/triple on 2-uniform first-cube scale ===")
    # famous A vs identity-like on B: Cassaigne is the B witness (non-uniform)
    print("  A famous 512: squares", st["n_squares"], "cubes", st["n_cubes"],
          "pair_not_triple", st["n_XY_equal_Z_exists_not_cube"])
    print("  B cassaigne 800: squares", stc["n_squares"], "cubes", stc["n_cubes"],
          "pair_not_triple", stc["n_XY_equal_Z_exists_not_cube"])


if __name__ == "__main__":
    main()
