#!/usr/bin/env python3
"""Independent Python brute-force verifier, used to cross-check the C++ core."""
from itertools import product
import sys


def has_additive_cube_brute(w):
    n = len(w)
    for d in range(1, n // 3 + 1):
        for i in range(0, n - 3 * d + 1):
            s1 = sum(w[i : i + d])
            s2 = sum(w[i + d : i + 2 * d])
            s3 = sum(w[i + 2 * d : i + 3 * d])
            if s1 == s2 == s3:
                return True, i, d, s1
    return False, None, None, None


def is_acf(w):
    return not has_additive_cube_brute(w)[0]


def count_acf(n, alphabet=(0, 1, 2, 3)):
    c = 0
    for tup in product(alphabet, repeat=n):
        if is_acf(tup):
            c += 1
    return c


HAND_EXAMPLES = {
    "": True,
    "0": True,
    "01": True,
    "000": False,  # ordinary cube
    "111": False,
    "012": True,  # sums 0,1,2 distinct
    "010": False,  # 0,1,0 — wait d=1: 0,1,0 different. length 3 only d=1. ACF!
    "012012012": False,  # three blocks sum 3
    "0123": True,
    "01230123": False,  # two copies is not enough; actually 12|30|12 is a cube
    "001001001": False,
    "012301230123": False,  # three copies of 0123, sum 6 each
    "03": True,
    "031430110343430310110110314303434303434303143011031011011": None,  # Cassaigne prefix; checked by C++
}


def main():
    # Hand examples that we can decide locally
    local = {
        "": True,
        "0": True,
        "01": True,
        "000": False,
        "111": False,
        "012": True,
        "010": True,  # 0,1,0 is not an additive cube
        "0110": True,
        "012012012": False,
        "0123": True,
        "01230123": False,  # 12|30|12 sums 3
        "001001001": False,
        "012301230123": False,
        "123123123": False,
        "012012021": False,  # 012 012 021 sums 3,3,3
        "0011": True,
        "0001": True,  # 000 is a cube in the prefix!
    }
    # Fix 0001: contains 000
    local["0001"] = False
    local["1000"] = False

    failed = 0
    for s, expect in local.items():
        w = [int(c) for c in s]
        got = is_acf(w)
        if got != expect:
            print(f"FAIL hand {s!r}: got {got} expected {expect}")
            failed += 1
        else:
            print(f"OK   hand {s!r} -> {got}")

    print("counts n=0..6 over {0,1,2,3}:")
    counts = [1]  # empty
    for n in range(1, 7):
        c = count_acf(n)
        counts.append(c)
        print(f"  n={n}  {c}  (total 4^{n}={4**n}, fraction={c/4**n:.4f})")

    if failed:
        print(f"{failed} failures")
        sys.exit(1)
    print("python brute_verify: all hand tests passed")
    # write counts for C++ comparison
    with open("/agent/experiments/py_counts_n6.txt", "w") as f:
        for n, c in enumerate(counts):
            f.write(f"{n} {c}\n")


if __name__ == "__main__":
    main()
