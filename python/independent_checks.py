#!/usr/bin/env python3
"""Independent checks: unique-vote lemma, Cassaigne recoding deaths, n>=11 bound."""
from itertools import product


def prefix_sums(w):
    S = [0]
    for a in w:
        S.append(S[-1] + a)
    return S


def is_acf(w):
    n = len(w)
    S = prefix_sums(w)
    for d in range(1, n // 3 + 1):
        for i in range(0, n - 3 * d + 1):
            s1 = S[i + d] - S[i]
            s2 = S[i + 2 * d] - S[i + d]
            s3 = S[i + 3 * d] - S[i + 2 * d]
            if s1 == s2 == s3:
                return False
    return True


def forbidden_letters(w):
    """Unique-vote computation of letters that create a cube ending at n+1."""
    n = len(w)
    S = prefix_sums(w)
    N = n + 1
    votes = {}  # d -> x
    for d in range(1, N // 3 + 1):
        i1 = n + 1 - 3 * d
        i2 = n + 1 - 2 * d
        i3 = n + 1 - d
        if i1 < 0:
            continue
        s1 = S[i2] - S[i1]
        s2 = S[i3] - S[i2]
        if s1 != s2:
            continue
        last = S[n] - S[i3]
        x = s1 - last
        votes[d] = x
    forb = set()
    for d, x in votes.items():
        if x in (0, 1, 2, 3):
            forb.add(x)
    return votes, forb


def brute_forbidden(w):
    f = set()
    for a in range(4):
        if not is_acf(list(w) + [a]):
            f.add(a)
    return f


def test_unique_vote(nmax=7):
    fails = 0
    nwords = 0
    for n in range(0, nmax + 1):
        for tup in product(range(4), repeat=n):
            if not is_acf(tup):
                continue
            nwords += 1
            votes, forb = forbidden_letters(tup)
            # at most one x per d already in dict
            brute = brute_forbidden(tup)
            if forb != brute:
                print("FAIL vote", tup, "vote", forb, "brute", brute, "votes", votes)
                fails += 1
    print(f"unique-vote vs brute on all ACF words n<= {nmax}: {nwords} words, {fails} fails")
    return fails == 0


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


def first_acf_len(src, code, cap):
    w = []
    for i, a in enumerate(src[:cap]):
        w.append(code[a])
        if not is_acf(w):
            return i  # length of ACF prefix
    return len(w)


def main():
    assert test_unique_vote(6)
    src = cassaigne(80)
    # native Cassaigne prefix should be ACF
    assert is_acf(src[:60])
    naive = {0: 0, 1: 1, 3: 2, 4: 3}
    L = first_acf_len(src, naive, 40)
    print("naive recode 0,1,3,4 -> 0,1,2,3 ACF prefix", L)
    assert L == 9, L
    best_bi = {0: 1, 1: 0, 3: 3, 4: 2}
    L2 = first_acf_len(src, best_bi, 50)
    print("bijection 0->1,1->0,3->3,4->2 ACF prefix", L2)
    assert L2 == 35, L2
    # n>=11 bound: floor((n+1)/3) < 4 for n<=10
    for n in range(0, 11):
        assert (n + 1) // 3 < 4
    print("python independent checks passed")


if __name__ == "__main__":
    main()
