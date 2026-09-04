#!/usr/bin/env python3
"""Independent checks for triple geometry. Does not import triple_type."""
from itertools import product
import os
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from desub_geometry import FAMOUS, ancestor_of, find_cube, interval_pieces, prefix_sums, analyze_cube, vdot


def n_fragment_letters(m, r, s):
    rY = (r + s) % m
    rZ = (r + 2 * s) % m
    rEnd = (r + 3 * s) % m
    return (1 if r else 0) + (1 if rY else 0) + (1 if rZ else 0) + (1 if rEnd else 0)


def first_square(w):
    S = prefix_sums(w)
    n = len(w)
    for i in range(n - 1):
        for d in range(1, (n - i) // 2 + 1):
            if S[i + d] - S[i] == S[i + 2 * d] - S[i + d]:
                return (i, d, S[i + d] - S[i])
    return None


def count_sq_cube(w):
    n = len(w)
    S = prefix_sums(w)
    nsq = ncu = npnt = 0
    first_sq = first_cu = None
    for d in range(1, n // 2 + 1):
        for i in range(0, n - 2 * d + 1):
            s1 = S[i + d] - S[i]
            s2 = S[i + 2 * d] - S[i + d]
            if s1 == s2:
                nsq += 1
                if first_sq is None:
                    first_sq = (i, d, s1)
            if i + 3 * d <= n:
                s3 = S[i + 3 * d] - S[i + 2 * d]
                if s1 == s2 == s3:
                    ncu += 1
                    if first_cu is None:
                        first_cu = (i, d, s1)
                elif s1 == s2:
                    npnt += 1
    return nsq, ncu, npnt, first_sq, first_cu


def cassaigne(n):
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


def main():
    fails = 0
    # m=2,3 residue count and letter counts
    hist2 = [n_fragment_letters(2, r, s) for r, s in product(range(2), repeat=2)]
    if sorted(hist2) != [0, 2, 2, 4]:
        print("FAIL m=2 letter hist", hist2)
        fails += 1
    # residues of Y,Z determined: 4 types not 8
    seen = {(r, (r + s) % 2, (r + 2 * s) % 2) for r, s in product(range(2), repeat=2)}
    if len(seen) != 4:
        print("FAIL m=2 residue triples", seen)
        fails += 1

    sigma = FAMOUS
    W = sigma.iterate(2, 512)
    cube = find_cube(W)
    if cube != (45, 25, 35):
        print("FAIL famous cube", cube)
        fails += 1
    U = ancestor_of(W, sigma, cube[0] + 3 * cube[1])
    an = analyze_cube(W, sigma, cube, U)
    if not (an.X.true_sum == an.Y.true_sum == an.Z.true_sum == 35):
        print("FAIL omega")
        fails += 1
    if vdot(sigma.T, an.v_xy) != an.Y.defect - an.X.defect:
        print("FAIL T v_xy")
        fails += 1
    psi_xz = tuple(a - b for a, b in zip(an.X.psi, an.Z.psi))
    vsum = tuple(a + b for a, b in zip(an.v_xy, an.v_yz))
    if psi_xz != vsum:
        print("FAIL v_xy+v_yz", psi_xz, vsum)
        fails += 1
    # Claim E: X-left block is not among Y's pieces
    i, d, _ = cube
    m = 2
    x_start_blk = i // m
    y_start_blk = (i + d) // m
    y_end_blk = (i + 2 * d) // m
    if i % m != 0 and x_start_blk >= y_start_blk and x_start_blk <= y_end_blk:
        # only fail if the left fragment block is inside Y's ancestor span
        print("FAIL expected Xleft outside Y", x_start_blk, y_start_blk, y_end_blk)
        fails += 1
    # predicted type (1,1): n_let=2
    if n_fragment_letters(2, 1, 1) != 2:
        print("FAIL n_let (1,1)")
        fails += 1
    # YZ junction complementary: i+2d = 95, 95%2=1, shared block 47
    junc = (i + 2 * d) // m
    # pieces of Y ending and Z starting at junc
    yp = interval_pieces(i + d, d, m)
    zp = interval_pieces(i + 2 * d, d, m)
    y_last = yp[-1]
    z_first = zp[0]
    if y_last[0] != z_first[0] or y_last[0] != junc:
        print("FAIL YZ block", y_last, z_first, junc)
        fails += 1
    if y_last[2] != z_first[1]:
        print("FAIL complementary off", y_last, z_first)
        fails += 1
    if y_last[2] - y_last[1] + z_first[2] - z_first[1] != m:
        print("FAIL cover m")
        fails += 1

    nsq, ncu, npnt, fsq, fcu = count_sq_cube(W)
    if cube != (45, 25, 35):
        print("FAIL find_cube lex", cube)
        fails += 1
    if fcu is None or ncu < 1 or nsq < ncu:
        print("FAIL squares/cubes", fsq, fcu, nsq, ncu)
        fails += 1

    C = cassaigne(800)
    if find_cube(C) is not None:
        print("FAIL cassaigne has cube")
        fails += 1
    sq = first_square(C)
    if sq is None:
        print("FAIL cassaigne no square")
        fails += 1
    nsqC, ncuC, npntC, _, _ = count_sq_cube(C)
    if ncuC != 0 or nsqC == 0:
        print("FAIL cassaigne sq/cu", nsqC, ncuC)
        fails += 1

    # ω unbounded: d=1 vs d=100 on famous 512
    S = prefix_sums(W)
    r1 = max(S[i + 1] - S[i] for i in range(len(W) - 1))
    r100 = max(S[i + 100] - S[i] for i in range(len(W) - 100))
    if r100 <= r1:
        print("FAIL omega not growing", r1, r100)
        fails += 1

    print(
        f"triple_verify failures={fails} famous_cube={cube} first_square_famous={fsq} "
        f"famous_sq={nsq} famous_cu={ncu} pair_not_triple={npnt} "
        f"cassaigne_square={sq} cassaigne_sq={nsqC} cassaigne_cu={ncuC}"
    )
    if fails:
        raise SystemExit(1)


if __name__ == "__main__":
    main()
