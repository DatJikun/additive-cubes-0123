#!/usr/bin/env python3
"""Lattice-vs-contracting-eigenvector computation for the Cassaigne-shaped morphism.

Incidence matrix M (and its contracting left eigenvector tau) is independent of
the numerical alphabet. The lattice L = ker[1 1 1 1; a b c d] ∩ Z^4 depends on
the alphabet. Liétard–Rosenfeld Lemma 3.3: an additive cube in W_{a,b,c,d} would
give nonzero x in L with |tau·x| < C ≈ 2.1758. Hence if every nonzero lattice
vector has |tau·x| >= C, the word is ACF.

This script computes the short-vector picture for {0,1,3,4} vs {0,1,2,3}.
"""
from math import sqrt

# tau3 from CCSS / Liétard–Rosenfeld (contracting left eigenvector)
TAU = [
    complex(0.5788, -0.5749),
    complex(-0.3219, 0.2183),
    complex(-0.0690, 0.6165),
    complex(-0.1662, -0.6810),
]
C = 2.175817


def tau_dot(v):
    return sum(TAU[i] * v[i] for i in range(4))


def in_L(v, alph):
    if sum(v) != 0:
        return False
    return sum(alph[i] * v[i] for i in range(4)) == 0


def scan_lattice(alph, name, bound=8):
    hits = []
    seen = set()
    for x0 in range(-bound, bound + 1):
        for x1 in range(-bound, bound + 1):
            for x2 in range(-bound, bound + 1):
                x3 = -x0 - x1 - x2
                v = (x0, x1, x2, x3)
                if v in seen:
                    continue
                if not in_L(v, alph):
                    continue
                seen.add(v)
                mag = abs(tau_dot(v))
                nrm = sqrt(sum(t * t for t in v))
                hits.append((mag, nrm, v))
    hits.sort()
    print(f"\n=== {name} alphabet {alph}  (vectors with ||x||_inf<={bound}) ===")
    print(f"lattice vectors found: {len(hits)}")
    below = [h for h in hits if h[0] < C and h[2] != (0, 0, 0, 0)]
    print(f"nonzero with |tau·x| < C={C}: {len(below)}")
    print("shortest |tau·x| among nonzero:")
    shown = 0
    for mag, nrm, v in hits:
        if v == (0, 0, 0, 0):
            continue
        flag = "  <C" if mag < C else ""
        print(f"  |tau|={mag:.5f}  ||x||={nrm:.3f}  x={v}{flag}")
        shown += 1
        if shown >= 12:
            break
    return hits


def main():
    print("Cassaigne/Liétard–Rosenfeld contracting test")
    print(f"C = {C}")
    print("tau =", TAU)
    scan_lattice((0, 1, 3, 4), "{0,1,3,4}")
    scan_lattice((0, 1, 2, 3), "{0,1,2,3}")
    # The AP exchange vector (1,-1,-1,1) lives in BOTH lattices.
    for alph, name in [((0, 1, 3, 4), "B"), ((0, 1, 2, 3), "A")]:
        v = (1, -1, -1, 1)
        print(f"\n(1,-1,-1,1) in L_{name}? {in_L(v, alph)}  |tau|={abs(tau_dot(v)):.5f}")
    vB = (1, -2, 2, -1)  # CCSS short vector for {0,1,3,4}
    print(f"(1,-2,2,-1) in L_B? {in_L(vB,(0,1,3,4))} |tau|={abs(tau_dot(vB)):.5f}")
    print(f"(1,-2,2,-1) in L_A? {in_L(vB,(0,1,2,3))} |tau|={abs(tau_dot(vB)):.5f}")
    # AP-typical extra generator
    vA = (1, -2, 1, 0)
    print(f"(1,-2,1,0) in L_A? {in_L(vA,(0,1,2,3))} |tau|={abs(tau_dot(vA)):.5f}")
    print(f"(1,-2,1,0) in L_B? {in_L(vA,(0,1,3,4))} |tau|={abs(tau_dot(vA)):.5f}")


if __name__ == "__main__":
    main()
