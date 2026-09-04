#!/usr/bin/env python3
"""Independent classification of length-14 dead-ends: for each extension letter,
report the shortest additive cube created. Cross-checks C++ taxonomy."""
import sys
from collections import Counter


def cubes_ending_at(w):
    n = len(w)
    hits = []
    for d in range(1, n // 3 + 1):
        i = n - 3 * d
        if i < 0:
            continue
        s1 = sum(w[i : i + d])
        s2 = sum(w[i + d : i + 2 * d])
        s3 = sum(w[i + 2 * d : i + 3 * d])
        if s1 == s2 == s3:
            hits.append((i, d, s1))
    hits.sort(key=lambda t: t[1])  # shortest d
    return hits


def classify(s):
    w = [int(c) for c in s]
    # w itself must be ACF
    n = len(w)
    for d in range(1, n // 3 + 1):
        for i in range(0, n - 3 * d + 1):
            s1 = sum(w[i : i + d])
            s2 = sum(w[i + d : i + 2 * d])
            s3 = sum(w[i + 2 * d : i + 3 * d])
            if s1 == s2 == s3:
                raise SystemExit(f"not ACF: {s} cube i={i} d={d}")
    ds = []
    for a in range(4):
        hits = cubes_ending_at(w + [a])
        if not hits:
            raise SystemExit(f"extendable {s}+{a}")
        ds.append(hits[0][1])
    return tuple(ds)


def main():
    path = sys.argv[1] if len(sys.argv) > 1 else "data/deadends14.txt"
    quad = Counter()
    n = 0
    with open(path) as f:
        for line in f:
            s = line.strip()
            if not s or s[0] == "#" or any(c not in "0123" for c in s):
                continue
            q = classify(s)
            quad[q] += 1
            n += 1
            if n <= 5:
                print("ex", s, "d", q)
    print("n", n)
    print("distinct d-quads", len(quad))
    for q, c in quad.most_common(20):
        print(c, q)


if __name__ == "__main__":
    main()
