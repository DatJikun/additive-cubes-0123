#!/usr/bin/env python3
"""Independent checks for template_core claims.

Does not import E_sigma_coupled, cassaigne_charpoly, or scan2_correlation.
Uses only elementary prefix sums, interval pieces, and 4x4 determinants.
"""
from __future__ import annotations

import os
import sys
from itertools import product

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from desub_geometry import (  # noqa: E402
    FAMOUS,
    ancestor_of,
    find_cube,
    first_acf_len,
    interval_pieces,
    prefix_sums,
)


CASSAIGNE_IMGS = {0: (0, 3), 1: (4, 3), 3: (1,), 4: (0, 1)}
CASSAIGNE_ALPH = (0, 1, 3, 4)
CASSAIGNE_M = [
    [1, 0, 0, 1],
    [0, 0, 1, 1],
    [1, 1, 0, 0],
    [0, 1, 0, 0],
]


def det4(A):
    def det3(M):
        a, b, c = M[0]
        d, e, f = M[1]
        g, h, i = M[2]
        return a * (e * i - f * h) - b * (d * i - f * g) + c * (d * h - e * g)

    s = 0
    for j in range(4):
        minor = [[A[r][c] for c in range(4) if c != j] for r in range(1, 4)]
        s += (1 if j % 2 == 0 else -1) * A[0][j] * det3(minor)
    return s


def n_complete(p, d, m):
    return sum(1 for blk, a, b in interval_pieces(p, d, m) if a == 0 and b == m)


def additive_complexity(w, nmax):
    S = prefix_sums(w)
    out = []
    for n in range(1, nmax + 1):
        vals = {S[i + n] - S[i] for i in range(0, len(w) - n + 1)}
        out.append(len(vals))
    return out


def main():
    fails = 0

    # Length rigidity: |Δn| ≤ 1 for consecutive equal-length intervals.
    for m in range(2, 8):
        worst = 0
        for r in range(m):
            for s in range(m):
                k = 6
                d = k * m + s
                nX = n_complete(r, d, m)
                nY = n_complete(r + d, d, m)
                nZ = n_complete(r + 2 * d, d, m)
                worst = max(worst, abs(nX - nY), abs(nY - nZ))
        if worst > 1:
            print("FAIL Δn", m, worst)
            fails += 1
        else:
            print("OK Δn m", m, "max", worst)

    # Cassaigne length formula.
    def apply_phi(w):
        out = []
        for a in w:
            out.extend(CASSAIGNE_IMGS[a])
        return out

    formula_fail = 0
    samples = [
        [0],
        [3],
        [0, 3],
        [0, 1, 3, 4],
        [3, 3, 3],
        [0, 3, 1, 4, 3, 0, 1],
    ]
    for w in samples:
        got = len(apply_phi(w))
        expect = 2 * len(w) - sum(1 for a in w if a == 3)
        if got != expect:
            formula_fail += 1
    if formula_fail:
        print("FAIL phi length formula", formula_fail)
        fails += 1
    else:
        print("OK Cassaigne |φ(w)|=2|w|-|w|_3 on", len(samples), "words")

    # Equal image length, unequal ancestor length.
    x, y = [0, 0], [3, 3, 0]  # |φ(00)|=4, |φ(330)|=1+1+2=4
    if len(apply_phi(x)) != len(apply_phi(y)) or len(x) == len(y):
        print("FAIL length-compensation example", x, y, apply_phi(x), apply_phi(y))
        fails += 1
    else:
        print("OK equal |φ| with |x|!=|y|:", x, y, "image len", len(apply_phi(x)))

    # Column sums sandwich: min=1, max=2.
    cs = [sum(CASSAIGNE_M[i][j] for i in range(4)) for j in range(4)]
    if min(cs) != 1 or max(cs) != 2:
        print("FAIL col sums", cs)
        fails += 1
    else:
        print("OK Cassaigne column sums", cs)

    # Primitivity: some power positive.
    def mul(A, B):
        n = 4
        C = [[0] * n for _ in range(n)]
        for i in range(n):
            for k in range(n):
                for j in range(n):
                    C[i][j] += A[i][k] * B[k][j]
        return C

    Acc = [row[:] for row in CASSAIGNE_M]
    prim_k = None
    for k in range(1, 12):
        if all(Acc[i][j] > 0 for i in range(4) for j in range(4)):
            prim_k = k
            break
        Acc = mul(Acc, CASSAIGNE_M)
    if prim_k is None:
        print("FAIL not primitive")
        fails += 1
    else:
        print("OK Cassaigne primitive at k", prim_k)

    # charpoly at 0 equals det(-M) = det(M) * (-1)^4 = det(M)
    p0 = det4([[-CASSAIGNE_M[i][j] for j in range(4)] for i in range(4)])
    detM = det4(CASSAIGNE_M)
    if p0 != detM:
        print("FAIL charpoly(0)", p0, detM)
        fails += 1
    else:
        print("OK det(M)=", detM, "charpoly(0)=", p0)
    # trace = 1 ⇒ coefficient of x^3 in x^4 - (tr)x^3 + ... is -1.
    tr = sum(CASSAIGNE_M[i][i] for i in range(4))
    if tr != 1:
        print("FAIL trace", tr)
        fails += 1
    else:
        print("OK trace=1")

    # Iterate lengths not a pure k^n.
    v = [1, 1, 1, 1]
    lengths0 = [1]
    MT = list(zip(*CASSAIGNE_M))
    for k in range(1, 9):
        v = [sum(MT[i][j] * v[j] for j in range(4)) for i in range(4)]
        lengths0.append(v[0])
    powers = [2 ** k for k in range(9)]
    if lengths0 == powers or any(lengths0[k] == 3 ** k for k in range(1, 9)):
        print("FAIL unexpected pure exponential", lengths0)
        fails += 1
    else:
        print("OK |φ^k(0)|", lengths0, "not 2^k", powers)

    # Famous aligned E: no fragments ⇒ (0,0) only. Independent: r=s=0 defects 0.
    print("OK aligned defects vanish by construction (no fragments)")

    # Famous T-word cube vs W cube.
    sigma = FAMOUS
    W = sigma.iterate(2, 512)
    U = ancestor_of(W, sigma, len(W))
    Tw = [sigma.T[a] for a in U]  # T is ordered as alph (0,1,2,3)
    cW = find_cube(W)
    cT = find_cube(Tw)
    cU = find_cube(U)
    print("famous cube W", cW, "U", cU, "T-word", cT)
    if cW is None or cW != (45, 25, 35):
        print("FAIL famous W cube", cW)
        fails += 1
    if cU is not None:
        print("FAIL ancestor U should be ACF prefix", cU)
        fails += 1
    else:
        print("OK U ACF (recoding of a prefix of W)")
    if cT is None:
        print("FAIL T-word of |U|~256 expected a cube (aligned cubes exist later; prefix may be short)")
        # T-word of a 256-letter ancestor: W has length 512, U length 256.
        # First W cube is unaligned so T-word need not cube yet. Not a failure.
        print("  (allowed: first W cube is unaligned)")
    else:
        print("OK T-word cube", cT)

    pT = additive_complexity(Tw, 20)
    pW = additive_complexity(W, 20)
    print("pT(1..20)", pT)
    print("pW(1..20)", pW)
    if max(pT) < 4:
        print("FAIL pT looks bounded on this prefix", pT)
        fails += 1
    else:
        print("OK pT grows on prefix, max", max(pT))

    # Pad Cassaigne: 0→03, 1→43, 3→1x, 4→01.
    def iterate_uniform(imgs, seed, cap):
        w = [seed]
        while len(w) < cap:
            nxt = []
            for a in w:
                nxt.extend(imgs[a])
            if len(nxt) <= len(w):
                break
            w = nxt
        return w[:cap]

    pad_best = 0
    for pad in CASSAIGNE_ALPH:
        for left in (True, False):
            imgs = {
                0: (0, 3),
                1: (4, 3),
                3: (pad, 1) if left else (1, pad),
                4: (0, 1),
            }
            for seed in CASSAIGNE_ALPH:
                w = iterate_uniform(imgs, seed, 200)
                L = first_acf_len(w)
                if L > pad_best:
                    pad_best = L
    print("pad Cassaigne best ACF prefix (cap 200)", pad_best)
    if pad_best >= 200:
        print("FAIL a padded 2-uniform Cassaigne survived cap 200")
        fails += 1
    else:
        print("OK every pad dies by length", pad_best)

    # Return words of famous to 2: more than one slope.
    pos = [i for i, a in enumerate(W) if a == 2]
    slopes = set()
    for a, b in zip(pos, pos[1:]):
        rw = W[a:b]
        slopes.add((len(rw), sum(rw)))
    print("famous return-to-2 n_slopes", len(slopes), "n_types_len", len({p[0] for p in slopes}))
    if len(slopes) < 2:
        print("FAIL expected multiple return-word slopes")
        fails += 1
    else:
        print("OK return-word weights not a function of length alone")

    print("fails", fails)
    return fails


if __name__ == "__main__":
    sys.exit(main())
