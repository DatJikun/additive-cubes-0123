#!/usr/bin/env python3
"""Independent checks for the ACF-core attack. Does not import core_scan."""
from __future__ import annotations

import os
import sys
from collections import Counter, defaultdict

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from desub_geometry import find_cube, prefix_sums  # noqa: E402


def vote_x(S, n: int, d: int):
    N = n + 1
    i1, i2, i3 = N - 3 * d, N - 2 * d, N - d
    if i1 < 0:
        return None
    s1 = S[i2] - S[i1]
    s2 = S[i3] - S[i2]
    if s1 != s2:
        return None
    last = S[n] - S[i3]
    x = s1 - last
    if x < 0 or x > 3:
        return -1
    return int(x)


def q_vote(w) -> int:
    S = prefix_sums(w)
    n = len(w)
    mask = 0
    for d in range(1, (n + 1) // 3 + 1):
        x = vote_x(S, n, d)
        if x is not None and x >= 0:
            mask |= 1 << x
    return 4 - bin(mask).count("1")


def q_brute(w) -> int:
    q = 0
    for a in range(4):
        ww = list(w) + [a]
        if find_cube(ww) is None:
            q += 1
    return q


def q_alphabet(w, alph):
    S = prefix_sums(w)
    n = len(w)
    forbidden = set()
    for d in range(1, (n + 1) // 3 + 1):
        N = n + 1
        i1, i2, i3 = N - 3 * d, N - 2 * d, N - d
        if i1 < 0:
            continue
        s1 = S[i2] - S[i1]
        s2 = S[i3] - S[i2]
        if s1 != s2:
            continue
        last = S[n] - S[i3]
        x = s1 - last
        if x in alph:
            forbidden.add(x)
    return len(alph) - len(forbidden)


def load_digits(path, cap=None):
    with open(path) as f:
        s = f.read()
    w = [int(c) for c in s if c in "0123"]
    if cap is not None:
        w = w[:cap]
    return w


def q_along(w):
    S = prefix_sums(w)
    qs = []
    for n in range(1, len(w) + 1):
        mask = 0
        for d in range(1, (n + 1) // 3 + 1):
            x = vote_x(S, n, d)
            if x is not None and x >= 0:
                mask |= 1 << x
        qs.append(4 - bin(mask).count("1"))
    return qs


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


def is_acf_py(w):
    return find_cube(w) is None


def main():
    fails = 0
    nwords = 0
    for n in range(0, 7):
        total = 4**n
        for mask in range(total):
            w = []
            m = mask
            for _ in range(n):
                w.append(m % 4)
                m //= 4
            if find_cube(w) is not None:
                continue
            nwords += 1
            if n == 0:
                continue
            qv, qb = q_vote(w), q_brute(w)
            if qv != qb:
                print("FAIL q", w, qv, qb)
                fails += 1
    print("q_vote vs brute ACF n<=6 words", nwords, "fails", fails)

    de = "00103010301100"
    wde = [int(c) for c in de]
    if find_cube(wde) is not None or q_vote(wde) != 0 or q_brute(wde) != 0:
        print("FAIL dead-end q", de, q_vote(wde), q_brute(wde), find_cube(wde))
        fails += 1
    else:
        print("OK Candidate A killed at n=14 q=0", de)

    min_q1 = None
    ex = None
    for n in range(1, 10):
        total = 4**n
        for mask in range(total):
            w = []
            m = mask
            for _ in range(n):
                w.append(m % 4)
                m //= 4
            if find_cube(w) is not None:
                continue
            if q_vote(w) == 1:
                min_q1 = n
                ex = "".join(map(str, w))
                break
        if min_q1 is not None:
            break
    print("min_n_q1", min_q1, "ex", ex)
    if min_q1 != 8:
        print("FAIL expected min_n_q1=8, got", min_q1)
        fails += 1
    if ex != "20010100":
        # lex-smallest in the 4-adic enumeration (least-significant letter first)
        print("NOTE q1 enumeration order ex", ex)

    # Theorem A / AI: uuu is an ordinary cube, hence additive.
    for u in ([0], [0, 1], [2, 0, 1], [1, 0, 2, 3], [3, 1, 0, 2, 1]):
        w = u + u + u
        c = find_cube(w)
        if c is None or c[1] != len(u):
            print("FAIL uuu", u, c)
            fails += 1
        else:
            print("OK uuu d", c[1], "ulen", len(u))

    # BBB of an L-block has d=L.
    B = [0, 1, 2, 1, 0]
    bbb = B + B + B
    c = find_cube(bbb)
    if c is None or c[1] != 5:
        print("FAIL BBB", c)
        fails += 1
    else:
        print("OK BBB d", c[1])

    # Nested palindrome W x W^R dies immediately on many seeds (ordinary-ish join).
    seed = [0, 1]
    nxt = seed + [0] + list(reversed(seed))
    c = find_cube(nxt)
    print("WxWR seed=01 x=0 word", "".join(map(str, nxt)), "cube", c)
    if c is None:
        print("NOTE first palindrome fold of 01,0 is ACF (length 5)")
    # Iterate until cube
    w = seed
    died = False
    for r in range(12):
        nxt = w + [0] + list(reversed(w))
        if find_cube(nxt) is not None:
            print("OK WxWR 01 x=0 dies round", r + 1, "len", len(nxt), "cube", find_cube(nxt))
            died = True
            break
        w = nxt
    if not died:
        print("FAIL WxWR 01 x=0 survived")
        fails += 1

    # WW always cubes by round 2: WWWW contains (WW) as period-2|W| wait:
    # W W is not yet a cube. Next: (WW)(WW) = W^4 contains W^3.
    w = [0, 1]
    w2 = w + w
    w4 = w2 + w2
    c = find_cube(w4)
    if c is None:
        print("FAIL WW iterate")
        fails += 1
    else:
        print("OK WW^2 contains cube", c)

    # Deterministic 1-state: constant 0-word cubes at length 3.
    w = [0, 0, 0]
    if find_cube(w) is None:
        print("FAIL 000")
        fails += 1
    else:
        print("OK detfsm constant-0 cubes at 3")

    # Cassaigne native q: infinite ACF word still has q=1 positions.
    cass = cassaigne(400)
    if find_cube(cass) is not None:
        print("FAIL cassaigne not ACF", find_cube(cass))
        fails += 1
    else:
        print("OK cassaigne prefix 400 ACF on {0,1,3,4}")
    alph = {0, 1, 3, 4}
    qs = [q_alphabet(cass[:k], alph) for k in range(1, len(cass) + 1)]
    hist = Counter(qs)
    print("cassaigne400 qhist", dict(sorted(hist.items())), "first_q1", qs.index(1) + 1 if 1 in qs else None)
    if 0 in hist:
        print("FAIL cassaigne q=0 on an ACF infinite-construction prefix")
        fails += 1
    if 1 not in hist:
        print("NOTE cassaigne400 has no q=1 (unexpected but possible)")

    # Two-branch R: a q=1 word should have R>=2 if it extends.
    if ex:
        w = [int(c) for c in ex]
        desc1 = []
        for a in range(4):
            ww = w + [a]
            if find_cube(ww) is None:
                desc1.append(ww)
        print("q1_ex n1_desc", len(desc1))
        desc2 = []
        for ww in desc1:
            for a in range(4):
                www = ww + [a]
                if find_cube(www) is None:
                    desc2.append(www)
        print("q1_ex n2_desc", len(desc2))
        if len(desc1) != 1:
            print("FAIL q1 word should have 1 descendant")
            fails += 1
        if len(desc2) < 2:
            print("FAIL q1 word at n=8 should two-branch by r=2")
            fails += 1
        else:
            print("OK Candidate B at this q=1 8-mer: two-branch r=2")

    # Candidate B globally false: length-14 dead-end has no descendant.
    if q_brute(wde) != 0:
        print("FAIL de not q=0")
        fails += 1
    else:
        print("OK Candidate B killed globally by n=14 dead-end (no descendant at any R)")

    # inject n=5: every 5-mer should have some good extensions? compute independently
    def all_acf(n):
        out = []

        def rec(cur):
            if len(cur) == n:
                if find_cube(cur) is None:
                    out.append(list(cur))
                return
            for a in range(4):
                cur.append(a)
                if find_cube(cur) is None:
                    rec(cur)
                cur.pop()

        rec([])
        return out

    words5 = all_acf(5)
    with2 = with1 = with0 = 0
    for w in words5:
        good = 0
        for a in range(4):
            ww = w + [a]
            if find_cube(ww) is not None:
                continue
            if q_brute(ww) >= 2:
                good += 1
        if good >= 2:
            with2 += 1
        elif good == 1:
            with1 += 1
        else:
            with0 += 1
    print("inject n=5 words", len(words5), "ge2", with2, "eq1", with1, "eq0", with0)
    if len(words5) != 864 or with2 != 864:
        print("FAIL c5/inject", len(words5), with2)
        fails += 1

    # basin: 13-prefixes of dead-ends
    path = "data/deadends14.txt"
    if os.path.exists(path):
        prefs = defaultdict(int)
        nlines = 0
        with open(path) as f:
            for line in f:
                w = "".join(c for c in line if c in "0123")
                if len(w) != 14:
                    continue
                nlines += 1
                prefs[w[:13]] += 1
        n_multi = sum(1 for v in prefs.values() if v > 1)
        print("basin dead14", nlines, "pref13", len(prefs), "multi", n_multi)
        if nlines != 8170 or n_multi != 0 or len(prefs) != 8170:
            print("FAIL dead14 count/basin", nlines, len(prefs), n_multi)
            fails += 1

    path = "data/word_fixed_p1000_n24396.txt"
    if os.path.exists(path):
        w = load_digits(path, 400)
        qs = q_along(w)
        hist = Counter(qs)
        print("fixed400 qhist", dict(sorted(hist.items())), "qmin20", min(qs[19:]), "n_q1", qs.count(1))
        qb = q_brute(w)
        qv = qs[-1]
        if qb != qv:
            print("FAIL tail q", qb, qv)
            fails += 1
        else:
            print("OK tail q brute", qb)
        # Full-word tail q (vote only, O(N^2) in d-loop is O(N^2) total... O(N * N/3) = O(N^2))
        wfull = load_digits(path)
        qt = q_vote(wfull)
        print("fixed_full_tail_q", qt, "N", len(wfull))
        if qt != 0:
            print("NOTE fixed 24396 tail q is", qt, "(expected 0 if stall is a dead-end)")

    path = "data/word_updown_p1000_n65986.txt"
    if os.path.exists(path):
        wfull = load_digits(path)
        qt = q_vote(wfull)
        print("updown1000_full_tail_q", qt, "N", len(wfull))

    # Uniform operators: no pair of distinct letters extends EVERY 4-mer.
    words4 = all_acf(4)
    best = 0
    for a in range(4):
        for b in range(a + 1, 4):
            ok = 0
            for w in words4:
                if find_cube(w + [a]) is None and find_cube(w + [b]) is None:
                    ok += 1
            if ok > best:
                best = ok
    print("ops n=4 r=1 best_letter_pair", best, "/", len(words4))
    if best == len(words4):
        print("FAIL unexpected uniform letter pair on all 4-mers")
        fails += 1
    else:
        print("OK no uniform two-letter operator on all ACF 4-mers")

    w38s = "00100100200100112001020011003010130300"
    w38 = [int(c) for c in w38s]
    if len(w38) != 38 or find_cube(w38) is not None or q_vote(w38) != 0 or q_brute(w38) != 0:
        print("FAIL n38", find_cube(w38), q_vote(w38), q_brute(w38))
        fails += 1
    else:
        print("OK n=38 q=0", w38s)
    for a, expect_d in [(0, 1), (1, 4), (2, 5), (3, 2)]:
        c = find_cube(w38 + [a])
        if c is None or c[1] != expect_d:
            print("FAIL n38 ext", a, c)
            fails += 1
    print("OK n=38 four extensions cube")

    w28s = "0010010020010011200102001100"
    w28 = [int(c) for c in w28s]
    if len(w28) != 28 or q_vote(w28) != 1 or q_brute(w28) != 1:
        print("FAIL n28", q_vote(w28), q_brute(w28), find_cube(w28))
        fails += 1
    else:
        print("OK n=28 q=1")

    path = "data/word_fixed_p1000_n24396.txt"
    if os.path.exists(path) and q_vote(load_digits(path)) != 0:
        print("FAIL fixed tail not q=0")
        fails += 1
    path = "data/word_updown_p1000_n65986.txt"
    if os.path.exists(path) and q_vote(load_digits(path)) != 0:
        print("FAIL updown p1000 tail not q=0")
        fails += 1

    print("core_verify_fails", fails)
    if fails:
        sys.exit(1)


if __name__ == "__main__":
    main()
