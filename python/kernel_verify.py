#!/usr/bin/env python3
"""Independent checks for kernel/regular identities.

Does not import kernel_regular, automatic_kernel_size, or gaussian_rank.
Recomputes DFAO, S(2n)=T2·ψ(n), D1(0,n)=L·ψ(n), kernel fingerprints,
Thue–Morse discrepancy, and Cassaigne 2-kernel growth from first principles.
"""
from __future__ import annotations

import os
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from desub_geometry import FAMOUS, find_cube, prefix_sums  # noqa: E402

H = {0: (3, 2), 1: (3, 1), 2: (2, 0), 3: (0, 1)}
T = (5, 4, 2, 1)
T2 = (3, 5, 7, 9)
L = (7, 3, -3, -7)  # 2T - T2


def letter_at(n: int) -> int:
    if n == 0:
        return 2
    digits = []
    x = n
    while x:
        digits.append(x & 1)
        x >>= 1
    s = 2
    for d in reversed(digits):
        s = H[s][d]
    return s


def kernel_fps(seq, m, max_r, prefix):
    seen = set()
    for r in range(0, max_r + 1):
        stride = m**r
        for q in range(stride):
            key = []
            ok = True
            for n in range(prefix):
                j = stride * n + q
                if j >= len(seq):
                    ok = False
                    break
                key.append(seq[j])
            if ok:
                seen.add(tuple(key))
    return len(seen)


def parse_cert(path):
    d = {}
    with open(path) as f:
        for line in f:
            line = line.strip()
            if not line:
                continue
            k, _, rest = line.partition(" ")
            d[k] = rest
    return d


def main():
    fails = 0
    cap = 4096
    w = [2]
    while len(w) < cap:
        nxt = []
        for a in w:
            nxt.extend(H[a])
        w = nxt
    U = w[:cap]
    dfao_fail = sum(1 for n in range(512) if letter_at(n) != U[n])
    if dfao_fail:
        print("FAIL dfao", dfao_fail)
        fails += 1
    else:
        print("OK dfao")

    a = [T[x] for x in U]
    S = prefix_sums(a)
    pr = [0, 0, 0, 0]
    S2n_fail = L_fail = 0
    Labs = 0
    for n in range(0, min(2001, len(U) + 1)):
        if n:
            pr[U[n - 1]] += 1
        t2 = sum(T2[i] * pr[i] for i in range(4))
        Lf = sum(L[i] * pr[i] for i in range(4))
        Labs = max(Labs, abs(Lf))
        if n <= 400 and 2 * n < len(S) and t2 != S[2 * n]:
            S2n_fail += 1
        if 1 <= n <= 400 and 2 * n < len(S):
            if 2 * S[n] - S[2 * n] != Lf:
                L_fail += 1
    if S2n_fail or L_fail:
        print("FAIL identities", S2n_fail, L_fail)
        fails += 1
    else:
        print("OK S(2n)=T2·ψ and D1(0,n)=L·ψ", "L_absmax", Labs)

    a_fp = kernel_fps(a, 2, 8, 48)
    U_fp = kernel_fps(U, 2, 8, 48)
    chi = [1 if S[2 * d] == 2 * S[d] else 0 for d in range(1, len(S) // 2)]
    sq_fp = kernel_fps(chi, 2, 7, 32)
    print("kernel fps a,U,sq", a_fp, U_fp, sq_fp)
    if a_fp < 1 or a_fp > 64:
        print("FAIL a_kernel unexpected", a_fp)
        fails += 1
    else:
        print("OK a_kernel finite on this window", a_fp)
    if sq_fp < 1:
        print("FAIL sq_kernel empty")
        fails += 1
    else:
        print("OK prefix-square fingerprints", sq_fp, "vs letter kernel", a_fp)

    c = find_cube(U)
    if c != (45, 25, 35):
        print("FAIL famous cube", c)
        fails += 1
    else:
        print("OK famous cube", c)

    # L M^2 = 2L and L·ψ(70)=0 (generator of the 4-adic prefix-square family)
    M = [
        [0, 0, 1, 1],
        [0, 1, 0, 1],
        [1, 0, 1, 0],
        [1, 1, 0, 0],
    ]
    M2 = [[sum(M[i][k] * M[k][j] for k in range(4)) for j in range(4)] for i in range(4)]
    LM2 = tuple(sum(L[i] * M2[i][j] for i in range(4)) for j in range(4))
    if LM2 != tuple(2 * x for x in L):
        print("FAIL LM2", LM2)
        fails += 1
    else:
        print("OK L M^2 = 2L")
    pr70 = [0, 0, 0, 0]
    for x in U[:70]:
        pr70[x] += 1
    L70 = sum(L[i] * pr70[i] for i in range(4))
    if L70 != 0:
        print("FAIL L·ψ(70)", L70)
        fails += 1
    else:
        print("OK L·ψ(70)=0")

    imgs = {0: (0, 1), 1: (1, 4), 4: (4, 0)}
    w = [0]
    while len(w) < 256:
        nxt = []
        for x in w:
            nxt.extend(imgs[x])
        w = nxt
    w = w[:256]
    c014 = find_cube(w)
    if c014 != (2, 63, 105):
        print("FAIL 01440 cube", c014)
        fails += 1
    else:
        print("OK 01440 cube", c014)

    # Thue–Morse
    tm = [(bin(n).count("1") & 1) for n in range(2048)]
    tmS = prefix_sums(tm)
    dmax = max(abs(2 * tmS[n] - n) for n in range(len(tmS)))
    tmc = find_cube(tm)
    print("tm Delta_max", dmax, "cube", tmc)
    if tmc is None:
        print("FAIL TM must have an additive cube (Dekking)")
        fails += 1
    else:
        print("OK TM cube")
    if dmax > 8:
        print("NOTE TM discrepancy sample", dmax, "(bounded class if O(1))")

    cass_imgs = {0: (0, 3), 1: (4, 3), 3: (1,), 4: (0, 1)}
    w = [0]
    while len(w) < 2048:
        nxt = []
        for x in w:
            nxt.extend(cass_imgs[x])
        w = nxt
    cass = w[:2048]
    cass_fp2 = kernel_fps(cass, 2, 7, 32)
    cass_fp3 = kernel_fps(cass, 3, 5, 24)
    print("cassaigne 2ker,3ker", cass_fp2, cass_fp3)
    if cass_fp2 <= 8:
        print("FAIL Cassaigne 2-kernel should grow (not automatic)")
        fails += 1
    else:
        print("OK Cassaigne letter kernel grows", cass_fp2)

    # Compare cert files if present
    root = os.path.join(os.path.dirname(os.path.abspath(__file__)), "..")
    py_path = os.path.join(root, "data/kernel_cert.txt")
    cpp_path = os.path.join(root, "data/kernel_cert_cpp.txt")
    if os.path.exists(py_path):
        py = parse_cert(py_path)
        keys = [
            "dfao_fail",
            "S2n_fail",
            "L_fail",
            "a_kernel_fp",
            "sq_kernel_fp",
            "n_missing_lt200",
            "tm_Delta_max",
            "cass_2ker",
        ]
        for k in keys:
            if k not in py:
                print("FAIL missing py cert key", k)
                fails += 1
                continue
            print(" py", k, py[k])
        if py.get("dfao_fail") != "0" or py.get("S2n_fail") != "0" or py.get("L_fail") != "0":
            print("FAIL python cert identities")
            fails += 1
        if py.get("a_kernel_fp") != str(a_fp):
            print("FAIL a_kernel_fp mismatch", py.get("a_kernel_fp"), a_fp)
            fails += 1
        else:
            print("OK python a_kernel_fp")
        if py.get("sq_kernel_fp") != str(sq_fp):
            print("FAIL sq_kernel_fp mismatch", py.get("sq_kernel_fp"), sq_fp)
            fails += 1
        else:
            print("OK python sq_kernel_fp")
        if int(py.get("cass_2ker", "0")) != cass_fp2:
            print("FAIL cass_2ker mismatch", py.get("cass_2ker"), cass_fp2)
            fails += 1
        else:
            print("OK python cass_2ker")
        if py.get("LM2_fail") != "0" or py.get("L_psi_70") != "0":
            print("FAIL python LM2/L70", py.get("LM2_fail"), py.get("L_psi_70"))
            fails += 1
        else:
            print("OK python LM2 and L_psi_70")

    if os.path.exists(cpp_path):
        cpp = parse_cert(cpp_path)
        if cpp.get("dfao_fail") != "0" or cpp.get("S2n_fail") != "0" or cpp.get("L_fail") != "0":
            print("FAIL cpp identities", cpp.get("dfao_fail"), cpp.get("S2n_fail"), cpp.get("L_fail"))
            fails += 1
        else:
            print("OK cpp identities")
        if cpp.get("a_kernel_fp") != str(a_fp):
            print("FAIL cpp a_kernel_fp", cpp.get("a_kernel_fp"), a_fp)
            fails += 1
        else:
            print("OK cpp a_kernel_fp")
        if cpp.get("sq_kernel_fp") != str(sq_fp):
            print("FAIL cpp sq_kernel_fp", cpp.get("sq_kernel_fp"), sq_fp)
            fails += 1
        else:
            print("OK cpp sq_kernel_fp")
        if cpp.get("cass_2ker") != str(cass_fp2):
            print("FAIL cpp cass_2ker", cpp.get("cass_2ker"), cass_fp2)
            fails += 1
        else:
            print("OK cpp cass_2ker")
        if cpp.get("L_absmax") != str(Labs):
            print("FAIL L_absmax", cpp.get("L_absmax"), Labs)
            fails += 1
        else:
            print("OK L_absmax dual", Labs)
        if cpp.get("LM2_fail") != "0":
            print("FAIL cpp LM2", cpp.get("LM2_fail"))
            fails += 1
        else:
            print("OK cpp LM2")
        if cpp.get("L_psi_70") != "0":
            print("FAIL cpp L_psi_70", cpp.get("L_psi_70"))
            fails += 1
        else:
            print("OK cpp L_psi_70")
        if cpp.get("cube_01440") != "2 63 105":
            print("FAIL cpp 01440", cpp.get("cube_01440"))
            fails += 1
        else:
            print("OK cpp 01440")

    if fails:
        print("FAILED", fails)
        sys.exit(1)
    print("ALL KERNEL CHECKS PASSED")
    return 0


if __name__ == "__main__":
    main()
