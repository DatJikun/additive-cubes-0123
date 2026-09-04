#!/usr/bin/env python3
"""Independent reconstruction identity (ceil/floor split, defects read from W).

This is a second implementation of
    sum(W[p:p+d]) == T·psi(complete ancestor letters) + sum(boundary fragments)
and does not import view_interval from desub_geometry.
"""
from __future__ import annotations

import os
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from desub_geometry import FAMOUS, analyze_cube, ancestor_of, find_cube, morph_from_strings


def recon_independent(W, U, T_map, m, p, d):
    """T_map: letter -> image sum. Defects are raw W-slices, not image lookups."""
    start_full = (p + m - 1) // m
    end_full = (p + d) // m
    total = 0
    psi = {a: 0 for a in T_map}
    for j in range(start_full, end_full):
        a = U[j]
        psi[a] += 1
        total += T_map[a]
    left_end = start_full * m
    if left_end > p:
        total += sum(W[p : min(left_end, p + d)])
    right_start = end_full * m
    if p + d > right_start:
        total += sum(W[max(right_start, p) : p + d])
    return total, psi


def main():
    sigma = FAMOUS
    W = sigma.iterate(2, 512)
    m = sigma.m
    T_map = {a: sigma.T_of(a) for a in sigma.alph}
    cube = find_cube(W)
    assert cube is not None
    i, d, sm = cube
    U = ancestor_of(W, sigma, i + 3 * d)
    fails = 0
    for p, length, expect in [(i, d, sm), (i + d, d, sm), (i + 2 * d, d, sm)]:
        got, _ = recon_independent(W, U, T_map, m, p, length)
        true = sum(W[p : p + length])
        if got != true or true != expect:
            print("FAIL famous", p, length, got, true, expect)
            fails += 1
    # random intervals
    import random

    rng = random.Random(1)
    U2 = ancestor_of(W, sigma, len(W))
    W2 = sigma.apply(U2)
    for _ in range(500):
        p = rng.randrange(0, len(W2) - 1)
        length = rng.randrange(1, len(W2) - p + 1)
        got, _ = recon_independent(W2, U2, T_map, m, p, length)
        true = sum(W2[p : p + length])
        if got != true:
            fails += 1
    # cross-check analyze_cube vs independent on famous cube
    an = analyze_cube(W, sigma, cube, U)
    xi, _ = recon_independent(W, U, T_map, m, i, d)
    if xi != an.X.reconstructed_sum:
        print("FAIL cross", xi, an.X.reconstructed_sum)
        fails += 1
    print(f"desub_verify failures={fails} famous_cube=({i},{d},{sm}) reconX={xi}")
    if fails:
        sys.exit(1)


if __name__ == "__main__":
    main()
