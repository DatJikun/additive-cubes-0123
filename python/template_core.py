#!/usr/bin/env python3
"""Non-affine uniform templates: length rigidity, E_σ, automaticity, T-complexity.

Does NOT build a finite-state cube graph. The interior problem is whether an
m-automatic T-coded word can keep consecutive n-factor sums outside a finite
defect pair set E_σ.
"""
from __future__ import annotations

import csv
import os
import random
import sys
from collections import Counter, defaultdict
from itertools import product
from math import gcd
from typing import Optional, Sequence, Tuple

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from desub_geometry import (  # noqa: E402
    FAMOUS,
    UniformMorph,
    analyze_cube,
    ancestor_of,
    find_cube,
    first_acf_len,
    interval_pieces,
    morph_from_words,
    n_complete_closed_form,
    prefix_sums,
    vdot,
    vl1,
)
from quotient_dyn import (  # noqa: E402
    Mtv,
    _det,
    integer_rank,
    krylov_package,
    in_span,
)

Vec4 = Tuple[int, int, int, int]
ONES = (1, 1, 1, 1)

# ---------------------------------------------------------------------------
# Cassaigne morphism (non-uniform)
# ---------------------------------------------------------------------------

CASSAIGNE_IMGS = {0: (0, 3), 1: (4, 3), 3: (1,), 4: (0, 1)}
CASSAIGNE_ALPH = (0, 1, 3, 4)
# Incidence M[i][j] = count of alph[i] in image of alph[j], columns = images.
CASSAIGNE_M = (
    (1, 0, 0, 1),
    (0, 0, 1, 1),
    (1, 1, 0, 0),
    (0, 1, 0, 0),
)


def cassaigne_apply(w: Sequence[int]) -> list[int]:
    out: list[int] = []
    for a in w:
        out.extend(CASSAIGNE_IMGS[a])
    return out


def cassaigne_iterate(seed: int = 0, cap: int = 800) -> list[int]:
    w = [seed]
    while len(w) < cap:
        nxt = cassaigne_apply(w)
        if len(nxt) <= len(w):
            break
        w = nxt
    return w[:cap]


def cassaigne_length(w: Sequence[int]) -> int:
    """Exact: |φ(w)| = 2|w| - |w|_3."""
    return 2 * len(w) - sum(1 for a in w if a == 3)


def image_len_phi(a: int) -> int:
    return len(CASSAIGNE_IMGS[a])


# ---------------------------------------------------------------------------
# Exact polynomials / primitivity
# ---------------------------------------------------------------------------

def mat_pow(M, k: int):
    n = len(M)
    acc = [[1 if i == j else 0 for j in range(n)] for i in range(n)]
    base = [list(row) for row in M]
    while k:
        if k & 1:
            acc = mat_mul(acc, base)
        base = mat_mul(base, base)
        k >>= 1
    return acc


def mat_mul(A, B):
    n = len(A)
    m = len(B[0])
    p = len(B)
    C = [[0] * m for _ in range(n)]
    for i in range(n):
        for k in range(p):
            aik = A[i][k]
            if aik == 0:
                continue
            for j in range(m):
                C[i][j] += aik * B[k][j]
    return C


def mat_positive(M) -> bool:
    return all(M[i][j] > 0 for i in range(len(M)) for j in range(len(M)))


def charpoly_det_at(M, x: int) -> int:
    n = len(M)
    A = [[(x if i == j else 0) - M[i][j] for j in range(n)] for i in range(n)]
    return _det(A)


def interpolate_monomial(xs: Sequence[int], ys: Sequence[int]) -> list[int]:
    """Newton interpolation converted to integer monomials. ys integer, monic expected."""
    n = len(xs)
    div = [list(ys)]
    for k in range(1, n):
        row = []
        for i in range(n - k):
            num = div[k - 1][i + 1] - div[k - 1][i]
            den = xs[i + k] - xs[i]
            if num % den != 0:
                raise RuntimeError(f"non-integral Newton entry {num}/{den}")
            row.append(num // den)
        div.append(row)
    # Newton: c0 + c1(x-x0) + c2(x-x0)(x-x1)+...
    # Convert to monomials a0 + a1 x + ... by multiplying polynomials.
    coeff = [0]
    term = [1]  # current Π(x-xs[j])
    for k in range(n):
        ck = div[k][0]
        coeff = poly_add(coeff, [ck * t for t in term])
        # term *= (x - xs[k])
        shifted = poly_add(poly_shift(term), [-xs[k] * t for t in term])
        term = shifted
    return coeff  # coeff[i] = coefficient of x^i


def poly_add(a, b):
    n = max(len(a), len(b))
    a = a + [0] * (n - len(a))
    b = b + [0] * (n - len(b))
    return [x + y for x, y in zip(a, b)]


def poly_shift(a):
    return [0] + list(a)


def cassaigne_charpoly() -> list[int]:
    xs = list(range(5))
    ys = [charpoly_det_at(CASSAIGNE_M, x) for x in xs]
    return interpolate_monomial(xs, ys)


def col_sums(M) -> Tuple[int, ...]:
    n = len(M)
    return tuple(sum(M[i][j] for i in range(n)) for j in range(n))


# ---------------------------------------------------------------------------
# Phase 1: D_{r,s} and E_σ
# ---------------------------------------------------------------------------

def slot_lengths(m: int, r: int, s: int) -> Tuple[int, int]:
    left = 0 if r == 0 else m - r
    right = (r + s) % m
    return left, right


def interval_defect_from_letters(
    sigma: UniformMorph, r: int, s: int, left_letter: Optional[int], right_letter: Optional[int]
) -> int:
    m = sigma.m
    left_len, right_len = slot_lengths(m, r, s)
    dlt = 0
    if left_len:
        assert left_letter is not None
        dlt += sum(sigma.images[left_letter][r:])
    if right_len:
        assert right_letter is not None
        dlt += sum(sigma.images[right_letter][:right_len])
    return dlt


def D_pair(sigma: UniformMorph, r: int, s: int) -> set[int]:
    """Possible δ_X - δ_Y if X,Y fragment letters are chosen independently.

    Overapproximation of the coupled set. Finite, depends only on σ and (r,s).
    """
    m = sigma.m
    rY = (r + s) % m
    alph = sigma.alph
    leftX, rightX = slot_lengths(m, r, s)
    leftY, rightY = slot_lengths(m, rY, s)
    LX = alph if leftX else (None,)
    RX = alph if rightX else (None,)
    LY = alph if leftY else (None,)
    RY = alph if rightY else (None,)
    out = set()
    for a, b, c, d in product(LX, RX, LY, RY):
        dx = interval_defect_from_letters(sigma, r, s, a, b)
        dy = interval_defect_from_letters(sigma, rY, s, c, d)
        out.add(dx - dy)
    return out


def E_sigma_pairwise(sigma: UniformMorph) -> dict:
    """Product overapproximation: (D_{r,s}) × (D_{rY,s})."""
    m = sigma.m
    by_type = {}
    all_pairs = set()
    for r, s in product(range(m), repeat=2):
        rY = (r + s) % m
        rZ = (r + 2 * s) % m
        Dxy = D_pair(sigma, r, s)
        Dyz = D_pair(sigma, rY, s)
        pairs = {(u, v) for u in Dxy for v in Dyz}
        by_type[(r, s)] = {
            "Dxy": sorted(Dxy),
            "Dyz": sorted(Dyz),
            "n_pairs": len(pairs),
            "nX_minus_nY": n_complete_closed_form(m, r, s, 3) - n_complete_closed_form(m, rY, s, 3),
            "nY_minus_nZ": n_complete_closed_form(m, rY, s, 3) - n_complete_closed_form(m, rZ, s, 3),
        }
        all_pairs |= pairs
    return {"by_type": by_type, "all_pairs": sorted(all_pairs), "n_all": len(all_pairs)}


def E_sigma_coupled(sigma: UniformMorph) -> dict:
    """Exact fragment-letter enumeration with shared junctions.

    Ancestor window (large k):
      [X-left suffix] [X-completes] [XY-split] [Y-completes] [YZ-split] [Z-completes] [Z-right]
    Fragment defects depend on at most four letters. Interiors are forgotten.
    A cube requires (T·ψ_X - T·ψ_Y, T·ψ_Y - T·ψ_Z) = (δ_Y-δ_X, δ_Z-δ_Y).
    """
    m = sigma.m
    alph = sigma.alph
    by_type = {}
    all_pairs = set()
    for r, s in product(range(m), repeat=2):
        rY = (r + s) % m
        rZ = (r + 2 * s) % m
        rEnd = (r + 3 * s) % m
        # letters: Lx (if r!=0), Jxy (if rY!=0), Jyz (if rZ!=0), Rz (if rEnd!=0)
        Lx_opts = alph if r != 0 else (None,)
        Jxy_opts = alph if rY != 0 else (None,)
        Jyz_opts = alph if rZ != 0 else (None,)
        Rz_opts = alph if rEnd != 0 else (None,)
        pairs = set()
        dxy_set = set()
        dyz_set = set()
        for Lx, Jxy, Jyz, Rz in product(Lx_opts, Jxy_opts, Jyz_opts, Rz_opts):
            # X: left = suffix of Lx from r, right = prefix of Jxy of length rY
            dx = interval_defect_from_letters(sigma, r, s, Lx, Jxy)
            dy = interval_defect_from_letters(sigma, rY, s, Jxy, Jyz)
            dz = interval_defect_from_letters(sigma, rZ, s, Jyz, Rz)
            pairs.add((dx - dy, dy - dz))
            dxy_set.add(dx - dy)
            dyz_set.add(dy - dz)
        by_type[(r, s)] = {
            "E": sorted(pairs),
            "n_E": len(pairs),
            "Dxy_coupled": sorted(dxy_set),
            "Dyz_coupled": sorted(dyz_set),
            "aligned": r == 0 and s == 0,
        }
        all_pairs |= pairs
    aligned_E = by_type[(0, 0)]["E"]
    return {
        "by_type": by_type,
        "all_pairs": sorted(all_pairs),
        "n_all": len(all_pairs),
        "aligned_E": aligned_E,
    }


def length_rigidity_table(m: int, k: int = 5) -> list[tuple]:
    rows = []
    for r, s in product(range(m), repeat=2):
        rY = (r + s) % m
        rZ = (r + 2 * s) % m
        nX = n_complete_closed_form(m, r, s, k)
        nY = n_complete_closed_form(m, rY, s, k)
        nZ = n_complete_closed_form(m, rZ, s, k)
        rows.append((r, s, nX, nY, nZ, nX - nY, nY - nZ))
    return rows


def max_abs_dn(m: int, k: int = 8) -> int:
    return max(max(abs(row[5]), abs(row[6])) for row in length_rigidity_table(m, k))


# ---------------------------------------------------------------------------
# Additive complexity of a numerical word
# ---------------------------------------------------------------------------

def additive_complexity(w: Sequence[int], nmax: int) -> list[int]:
    S = prefix_sums(w)
    N = len(w)
    out = []
    for n in range(1, nmax + 1):
        vals = set()
        for i in range(0, N - n + 1):
            vals.add(S[i + n] - S[i])
        out.append(len(vals))
    return out


def consecutive_diff_set(w: Sequence[int], n: int, bound: Optional[int] = None) -> Counter:
    """Values of (sX-sY, sY-sZ) for consecutive n-factors. If bound, only |di|<=bound."""
    S = prefix_sums(w)
    N = len(w)
    c = Counter()
    for i in range(0, N - 3 * n + 1):
        s1 = S[i + n] - S[i]
        s2 = S[i + 2 * n] - S[i + n]
        s3 = S[i + 3 * n] - S[i + 2 * n]
        pair = (s1 - s2, s2 - s3)
        if bound is not None and (abs(pair[0]) > bound or abs(pair[1]) > bound):
            continue
        c[pair] += 1
    return c


def T_word(U: Sequence[int], Tmap: Sequence[int], alph: Sequence[int]) -> list[int]:
    idx = {a: i for i, a in enumerate(alph)}
    return [Tmap[idx[a]] for a in U]


# ---------------------------------------------------------------------------
# Return words
# ---------------------------------------------------------------------------

def return_words(w: Sequence[int], letter: int, max_count: int = 4000) -> dict:
    pos = [i for i, a in enumerate(w) if a == letter]
    if len(pos) < 2:
        return {"n_occ": len(pos), "types": {}, "n_types": 0}
    types: Counter = Counter()
    weights: dict[tuple, int] = {}
    for a, b in zip(pos, pos[1:]):
        rw = tuple(w[a:b])
        types[rw] += 1
        if rw not in weights:
            weights[rw] = sum(rw)
        if len(types) > max_count:
            break
    lens = Counter(len(rw) for rw in types)
    wts = Counter(weights[rw] for rw in types)
    slopes = Counter((len(rw), weights[rw]) for rw in types)
    return {
        "n_occ": len(pos),
        "n_types": len(types),
        "length_multiset": dict(lens),
        "weight_multiset": dict(wts),
        "n_distinct_slopes": len(slopes),
        "sample": sorted(((len(rw), weights[rw], types[rw]) for rw in types), reverse=True)[:12],
    }


def return_words_to_factor(w: Sequence[int], fac: Sequence[int]) -> dict:
    n = len(fac)
    pos = [i for i in range(len(w) - n + 1) if list(w[i : i + n]) == list(fac)]
    types: Counter = Counter()
    for a, b in zip(pos, pos[1:]):
        types[tuple(w[a:b])] += 1
    return {
        "n_occ": len(pos),
        "n_types": len(types),
        "lengths": sorted({len(rw) for rw in types}),
        "weights": sorted({sum(rw) for rw in types}),
    }


# ---------------------------------------------------------------------------
# Uniformization of Cassaigne
# ---------------------------------------------------------------------------

def pad_cassaigne_2uniform(pad_letter: int, pad_left: bool) -> UniformMorph:
    """Replace φ(3)=1 by a length-2 image, leaving other images unchanged."""
    imgs = {
        0: (0, 3),
        1: (4, 3),
        3: (pad_letter, 1) if pad_left else (1, pad_letter),
        4: (0, 1),
    }
    return UniformMorph(m=2, alph=CASSAIGNE_ALPH, images=imgs)


def phi_power_images(n: int) -> dict[int, tuple]:
    out = {}
    for a in CASSAIGNE_ALPH:
        w: list[int] = [a]
        for _ in range(n):
            w = cassaigne_apply(w)
        out[a] = tuple(w)
    return out


def iterate_lengths(nmax: int = 12) -> list[tuple]:
    """(|φ^k(a)| for a in alph) as exact integer recurrence v_{k+1} = M^T v_k."""
    v = [1, 1, 1, 1]
    rows = [(0, tuple(v))]
    MT = list(zip(*CASSAIGNE_M))
    for k in range(1, nmax + 1):
        v = [sum(MT[i][j] * v[j] for j in range(4)) for i in range(4)]
        rows.append((k, tuple(v)))
    return rows


# ---------------------------------------------------------------------------
# Scan2 correlation (matrix Krylov vs first-cube d from CSV)
# ---------------------------------------------------------------------------

def morph2_from_code(code: int, alph=(0, 1, 2, 3)) -> UniformMorph:
    t = code
    imgs = []
    for _ in range(4):
        x = t % 4
        y = (t // 4) % 4
        t //= 16
        imgs.append([alph[x], alph[y]])
    return morph_from_words(imgs, alph)


def scan2_correlation(path: str, alph=(0, 1, 2, 3), limit: Optional[int] = None) -> dict:
    agg = defaultdict(lambda: Counter())
    n = 0
    max_d = 0
    max_row = None
    by_kT_d = defaultdict(list)
    by_aff_d = defaultdict(list)
    by_ns_d = defaultdict(list)
    with open(path) as f:
        for row in csv.DictReader(f):
            code = int(row["code"])
            d = int(row["d"])
            ns = int(row["n_sums"])
            sigma = morph2_from_code(code, alph)
            M = sigma.incidence()
            pkg = krylov_package(M, alph, sigma.T)
            kT = pkg["krylov_T"]
            aff = int(pkg["T_affine"])
            agg["kT"][kT] += 1
            agg["d_by_kT"][(kT, d)] += 1
            agg["aligned_by_kT"][(kT, int(row["aligned"]))] += 1
            by_kT_d[kT].append(d)
            by_aff_d[aff].append(d)
            by_ns_d[ns].append(d)
            if d > max_d:
                max_d = d
                max_row = (code, int(row["seed"]), d, kT, aff, ns, sigma.T)
            n += 1
            if limit and n >= limit:
                break

    def stats(xs):
        if not xs:
            return None
        xs = sorted(xs)
        return {
            "n": len(xs),
            "min": xs[0],
            "max": xs[-1],
            "mean_milli": (1000 * sum(xs)) // len(xs),
            "median": xs[len(xs) // 2],
        }

    return {
        "n": n,
        "max_d": max_d,
        "max_row": max_row,
        "kT_counts": dict(agg["kT"]),
        "stats_by_kT": {k: stats(by_kT_d[k]) for k in sorted(by_kT_d)},
        "stats_by_affine": {k: stats(by_aff_d[k]) for k in sorted(by_aff_d)},
        "stats_by_nsums": {k: stats(by_ns_d[k]) for k in sorted(by_ns_d)},
    }


# ---------------------------------------------------------------------------
# Random uniform samples m=3..8
# ---------------------------------------------------------------------------

def is_primitive_matrix(M, max_k: int = 16) -> bool:
    n = len(M)
    acc = [list(row) for row in M]
    for _ in range(max_k):
        if mat_positive(acc):
            return True
        acc = mat_mul(acc, M)
    return False


def random_uniform(m: int, alph=(0, 1, 2, 3), rng: Optional[random.Random] = None) -> UniformMorph:
    rng = rng or random.Random(0)
    imgs = [[rng.choice(alph) for _ in range(m)] for _ in range(4)]
    return morph_from_words(imgs, alph)


def sample_uniform(
    m: int,
    n_samples: int = 80,
    cap: int = 240,
    seed: int = 0,
    alph=(0, 1, 2, 3),
    require_primitive: bool = True,
) -> dict:
    rng = random.Random(seed + 17 * m)
    hits = []
    n_prim = 0
    n_tried = 0
    n_nonsurvive = 0
    while len(hits) < n_samples and n_tried < n_samples * 30:
        n_tried += 1
        sigma = random_uniform(m, alph, rng)
        M = sigma.incidence()
        prim = is_primitive_matrix(M)
        if require_primitive and not prim:
            continue
        n_prim += 1
        pkg = krylov_package(M, alph, sigma.T)
        # try all 4 seeds; take the longest ACF prefix among prolongable, else any
        best = None
        for a in alph:
            W = sigma.iterate(a, cap)
            cube = find_cube(W)
            acf = first_acf_len(W) if cube is not None else len(W)
            rec = {
                "seed": a,
                "cube": cube,
                "acf": acf,
                "prolongable": sigma.prolongable(a),
            }
            if best is None or rec["acf"] > best["acf"]:
                best = rec
        if best["cube"] is None:
            n_nonsurvive += 1
        hits.append(
            {
                "code": sigma.code(),
                "T": sigma.T,
                "n_sums": sigma.n_sums,
                "kT": pkg["krylov_T"],
                "kA": pkg["krylov_A"],
                "affine": pkg["T_affine"],
                "primitive": prim,
                **best,
            }
        )
    cubes = [h for h in hits if h["cube"] is not None]
    ds = [h["cube"][1] for h in cubes]
    acfs = [h["acf"] for h in hits]
    return {
        "m": m,
        "n_hits": len(hits),
        "n_tried": n_tried,
        "n_primitive_accepted": n_prim,
        "n_no_cube_in_cap": n_nonsurvive,
        "max_acf": max(acfs) if acfs else None,
        "min_acf": min(acfs) if acfs else None,
        "max_d": max(ds) if ds else None,
        "kT_of_max_acf": max(hits, key=lambda h: h["acf"]) if hits else None,
        "kT_counts": dict(Counter(h["kT"] for h in hits)),
        "affine_counts": dict(Counter(h["affine"] for h in hits)),
        "nsums_counts": dict(Counter(h["n_sums"] for h in hits)),
    }


def sample_coded_automatic(
    n_letters: int,
    n_samples: int = 60,
    cap: int = 200,
    seed: int = 1,
    target=(0, 1, 3, 4),
) -> dict:
    """2-uniform on a larger alphabet, coded onto target. Tests larger automatic kernels."""
    rng = random.Random(seed + 3 * n_letters)
    src = tuple(range(n_letters))
    best_acf = 0
    n_cube = 0
    n_surv = 0
    for _ in range(n_samples):
        imgs = [[rng.choice(src) for _ in range(2)] for _ in range(n_letters)]
        coding = [rng.choice(target) for _ in range(n_letters)]
        # iterate on source, then code
        s = rng.randrange(n_letters)
        w = [s]
        while len(w) < cap:
            nxt = []
            for a in w:
                nxt.extend(imgs[a])
            if len(nxt) <= len(w):
                break
            w = nxt
        coded = [coding[a] for a in w[:cap]]
        cube = find_cube(coded)
        if cube is None:
            n_surv += 1
            best_acf = max(best_acf, len(coded))
        else:
            n_cube += 1
            best_acf = max(best_acf, cube[0] + 3 * cube[1] - 1 if False else first_acf_len(coded))
    return {
        "n_letters": n_letters,
        "n_samples": n_samples,
        "cap": cap,
        "n_cube": n_cube,
        "n_survive_cap": n_surv,
        "best_acf": best_acf,
        "target": target,
    }


# ---------------------------------------------------------------------------
# Famous T-word / Krylov on cubes
# ---------------------------------------------------------------------------

def famous_T_analysis(cap: int = 1024, pmax: int = 40) -> dict:
    sigma = FAMOUS
    W = sigma.iterate(2, cap)
    U = ancestor_of(W, sigma, len(W))
    Tw = T_word(U, sigma.T, sigma.alph)
    cube_W = find_cube(W)
    cube_T = find_cube(Tw)
    cube_U = find_cube(U)
    U_cover = ancestor_of(W, sigma, cube_W[0] + 3 * cube_W[1]) if cube_W else U
    Tw_cover = T_word(U_cover, sigma.T, sigma.alph)
    cube_T_cover = find_cube(Tw_cover)
    cube_U_cover = find_cube(U_cover)
    pT = additive_complexity(Tw, min(pmax, len(Tw) // 4))
    pW = additive_complexity(W, min(pmax, len(W) // 4))
    # consecutive exact cubes in T-word vs defect window
    E = E_sigma_coupled(sigma)
    aligned_hits = 0
    defect_hits = 0
    Eset = set(E["all_pairs"])
    n_checked = 0
    first_E = None
    S = prefix_sums(Tw)
    N = len(Tw)
    for n in range(1, min(80, N // 3)):
        for i in range(0, N - 3 * n + 1):
            s1 = S[i + n] - S[i]
            s2 = S[i + 2 * n] - S[i + n]
            s3 = S[i + 3 * n] - S[i + 2 * n]
            pair = (s1 - s2, s2 - s3)
            n_checked += 1
            if pair == (0, 0):
                aligned_hits += 1
            if pair in Eset:
                defect_hits += 1
                if first_E is None:
                    first_E = (i, n, pair, s1, s2, s3)
    return {
        "cap": cap,
        "|W|": len(W),
        "|U|": len(U),
        "T": sigma.T,
        "cube_W": cube_W,
        "cube_U_nested": cube_U,
        "cube_U_cover": cube_U_cover,
        "|U_cover|": len(U_cover),
        "cube_Tword_nested": cube_T,
        "cube_Tword_cover": cube_T_cover,
        "pT": pT,
        "pW": pW,
        "pT_unbounded_evidence": pT[-1] > pT[0] and max(pT) >= 8,
        "aligned_T_cubes_count_nle80": aligned_hits,
        "E_hits_nle80": defect_hits,
        "first_E_hit": first_E,
        "n_checked": n_checked,
        "E_n_all": E["n_all"],
        "aligned_E": E["aligned_E"],
    }


def famous_cube_T2_from_csv(path="data/famous_all_cubes.csv") -> dict:
    sigma = FAMOUS
    M = sigma.incidence()
    T = sigma.T
    T2 = Mtv(M, T)
    Tdots = []
    T2dots = []
    l1s = []
    if not os.path.exists(path):
        return {"missing": True}
    with open(path) as f:
        for row in csv.DictReader(f):
            v = tuple(int(x) for x in row["v_xy"].split(","))
            Tdots.append(int(row["Tdot_v"]))
            T2dots.append(vdot(T2, v))
            l1s.append(vl1(v))
    return {
        "n": len(Tdots),
        "Tdot_range": (min(Tdots), max(Tdots)) if Tdots else None,
        "T2dot_range": (min(T2dots), max(T2dots)) if T2dots else None,
        "l1_range": (min(l1s), max(l1s)) if l1s else None,
        "n_Tdot_zero": sum(1 for x in Tdots if x == 0),
        "T2": T2,
    }


def cassaigne_complexity(cap: int = 800, pmax: int = 30) -> dict:
    w = cassaigne_iterate(0, cap)
    p = additive_complexity(w, min(pmax, len(w) // 4))
    cube = find_cube(w)
    # squares: consecutive equal sums of length n
    n_sq = 0
    S = prefix_sums(w)
    for n in range(1, min(40, len(w) // 2)):
        for i in range(0, len(w) - 2 * n + 1):
            if S[i + n] - S[i] == S[i + 2 * n] - S[i + n]:
                n_sq += 1
                break
    return {
        "len": len(w),
        "cube": cube,
        "p_add": p,
        "p_unbounded_evidence": max(p) >= 8,
        "n_lengths_with_a_square_nle40": n_sq,
        "phi_len_check_fail": sum(
            1
            for _ in range(30)
            for ww in [[random.Random(0).choice(CASSAIGNE_ALPH) for _ in range(8)]]
            if cassaigne_length(ww) != len(cassaigne_apply(ww))
        ),
    }


def main():
    print("=== LENGTH RIGIDITY ===")
    for m in range(2, 7):
        mdn = max_abs_dn(m)
        print(f"  m={m} max|Δn| (k=8) = {mdn}")
        assert mdn <= 1
    print("  Cassaigne |φ(w)|=2|w|-|w|_3 on random words:")
    rng = random.Random(1)
    fails = 0
    max_dn = 0
    for _ in range(200):
        w = [rng.choice(CASSAIGNE_ALPH) for _ in range(rng.randint(1, 20))]
        if cassaigne_length(w) != len(cassaigne_apply(w)):
            fails += 1
    print("  formula fails", fails)
    # equal image length with unequal ancestor length
    examples = []
    for n in range(2, 8):
        for _ in range(200):
            x = [rng.choice(CASSAIGNE_ALPH) for _ in range(n)]
            y = [rng.choice(CASSAIGNE_ALPH) for _ in range(n + rng.randint(1, n))]
            if len(cassaigne_apply(x)) == len(cassaigne_apply(y)) and len(x) != len(y):
                examples.append((len(x), len(y), len(cassaigne_apply(x))))
                break
    print("  equal-image unequal-ancestor examples", examples[:6], "n_found", len(examples))

    print("\n=== CASSAIGNE SPECTRUM / AUTOMATICITY ===")
    cp = cassaigne_charpoly()
    print("  charpoly coeffs a0..a4 (const to x^4)", cp)
    cs = col_sums(CASSAIGNE_M)
    print("  column sums", cs, "hence 1 ≤ λ_PF ≤ 2, not integer if primitive and not all cols equal")
    prim_k = None
    for k in range(1, 12):
        if mat_positive(mat_pow(CASSAIGNE_M, k)):
            prim_k = k
            break
    print("  primitive: M^k > 0 at k", prim_k)
    print("  iterate lengths |φ^k(a)|", iterate_lengths(8))
    imgs2 = phi_power_images(2)
    print("  |φ^2|", {a: len(v) for a, v in imgs2.items()})
    imgs3 = phi_power_images(3)
    print("  |φ^3|", {a: len(v) for a, v in imgs3.items()})

    print("\n=== E_σ FAMOUS ===")
    sigma = FAMOUS
    pair = E_sigma_pairwise(sigma)
    coup = E_sigma_coupled(sigma)
    print("  pairwise n_all", pair["n_all"])
    print("  coupled n_all", coup["n_all"], "aligned_E", coup["aligned_E"])
    for rs, info in coup["by_type"].items():
        print(f"    type {rs} n_E={info['n_E']} aligned={info['aligned']}")

    print("\n=== FAMOUS T-WORD ===")
    ft = famous_T_analysis(512, 24)
    for k in (
        "cube_W",
        "cube_U_nested",
        "cube_U_cover",
        "cube_Tword_nested",
        "cube_Tword_cover",
        "pT",
        "pW",
        "aligned_T_cubes_count_nle80",
        "E_hits_nle80",
        "first_E_hit",
        "E_n_all",
    ):
        print(f"  {k}: {ft[k]}")
    print("  T2 on cubes", famous_cube_T2_from_csv())

    print("\n=== CASSAIGNE ADDITIVE COMPLEXITY ===")
    print(" ", cassaigne_complexity(800, 24))

    print("\n=== RETURN WORDS ===")
    W = sigma.iterate(2, 1024)
    for a in sigma.alph:
        rw = return_words(W, a)
        print(f"  famous letter {a}: n_types={rw['n_types']} slopes={rw['n_distinct_slopes']} sample={rw['sample'][:5]}")
    Cw = cassaigne_iterate(0, 800)
    for a in CASSAIGNE_ALPH:
        rw = return_words(Cw, a)
        print(f"  Cassaigne letter {a}: n_types={rw['n_types']} slopes={rw['n_distinct_slopes']} lengths={rw['length_multiset']}")

    print("\n=== PAD CASSAIGNE ===")
    for pad in CASSAIGNE_ALPH:
        for left in (True, False):
            sm = pad_cassaigne_2uniform(pad, left)
            best = -1
            cube_best = None
            for seed in CASSAIGNE_ALPH:
                w = sm.iterate(seed, 200)
                c = find_cube(w)
                L = first_acf_len(w)
                if L > best:
                    best = L
                    cube_best = (seed, c, L, sm.T)
            print(f"  pad {'L' if left else 'R'}{pad} T={sm.T} best_acf={best} cube={cube_best}")

    print("\n=== SCAN2 CORRELATION (A) ===")
    path = "data/scan2_A.csv"
    if os.path.exists(path):
        corr = scan2_correlation(path)
        print("  n", corr["n"], "max_d", corr["max_d"], "max_row", corr["max_row"])
        print("  stats_by_kT", corr["stats_by_kT"])
        print("  stats_by_affine", corr["stats_by_affine"])
        print("  stats_by_nsums", corr["stats_by_nsums"])
        os.makedirs("data", exist_ok=True)
        with open("data/template_scan2_corr.txt", "w") as f:
            for k, v in corr.items():
                f.write(f"{k}: {v}\n")

    print("\n=== SAMPLE m=3..8 ===")
    samples = []
    for m in range(3, 9):
        rec = sample_uniform(m, n_samples=40, cap=180, seed=42)
        samples.append(rec)
        print(" ", rec)
    with open("data/template_sample_m.txt", "w") as f:
        for rec in samples:
            f.write(repr(rec) + "\n")

    print("\n=== CODED AUTOMATIC (larger kernel onto B) ===")
    for nlet in (5, 6, 8):
        rec = sample_coded_automatic(nlet, n_samples=40, cap=160, seed=7)
        print(" ", rec)

    print("\n=== IDENTITY CHECKS ===")
    assert max_abs_dn(2) <= 1 and max_abs_dn(5) <= 1
    assert coup["aligned_E"] == [(0, 0)]
    assert prim_k is not None
    assert cp[-1] == 1  # monic
    print("  ok")


if __name__ == "__main__":
    main()
