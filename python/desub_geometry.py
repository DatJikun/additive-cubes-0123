#!/usr/bin/env python3
"""Exact desubstitution geometry for additive cubes in uniformly morphic words.

An interval [p, p+d) in W = sigma(U), with sigma m-uniform, decomposes into
at most two partial substitution-image fragments plus a run of complete
images.  This module derives the identity

    sum(interval) = T · psi + defect

and the two equal-sum equations of an additive cube as

    T · (psi_X - psi_Y) + (def_X - def_Y) = 0
    T · (psi_Y - psi_Z) + (def_Y - def_Z) = 0

with defect drawn from a finite set depending only on sigma (not on depth).
"""
from __future__ import annotations

from collections import Counter
from dataclasses import dataclass, field
from itertools import product
from math import gcd, inf
from typing import Iterable, Optional, Sequence, Tuple

Letter = int
Word = list[int]
Vec4 = Tuple[int, int, int, int]


def parikh(word: Sequence[int], alph: Sequence[int]) -> Vec4:
    idx = {a: i for i, a in enumerate(alph)}
    c = [0, 0, 0, 0]
    for x in word:
        c[idx[x]] += 1
    return (c[0], c[1], c[2], c[3])


def vadd(a: Vec4, b: Vec4) -> Vec4:
    return (a[0] + b[0], a[1] + b[1], a[2] + b[2], a[3] + b[3])


def vsub(a: Vec4, b: Vec4) -> Vec4:
    return (a[0] - b[0], a[1] - b[1], a[2] - b[2], a[3] - b[3])


def vdot(a: Sequence[int], b: Sequence[int]) -> int:
    return sum(x * y for x, y in zip(a, b))


def vl1(v: Sequence[int]) -> int:
    return sum(abs(x) for x in v)


def vlinf(v: Sequence[int]) -> int:
    return max(abs(x) for x in v)


def vgcd(v: Sequence[int]) -> int:
    g = 0
    for x in v:
        g = gcd(g, abs(x))
    return g


def prefix_sums(w: Sequence[int]) -> list[int]:
    S = [0]
    for a in w:
        S.append(S[-1] + a)
    return S


def find_cube(w: Sequence[int]) -> Optional[Tuple[int, int, int]]:
    """Lex-first additive cube: smallest start i, then smallest d. Returns (i,d,sum)."""
    n = len(w)
    if n < 3:
        return None
    S = prefix_sums(w)
    for i in range(0, n - 2):
        dmax = (n - i) // 3
        for d in range(1, dmax + 1):
            s1 = S[i + d] - S[i]
            s2 = S[i + 2 * d] - S[i + d]
            s3 = S[i + 3 * d] - S[i + 2 * d]
            if s1 == s2 == s3:
                return (i, d, s1)
    return None


def first_acf_len(w: Sequence[int]) -> int:
    """Length of the longest ACF prefix (O(n^2) incremental)."""
    S = [0]
    for n, a in enumerate(w, 1):
        S.append(S[-1] + a)
        dmax = n // 3
        for d in range(1, dmax + 1):
            s3 = S[n] - S[n - d]
            s2 = S[n - d] - S[n - 2 * d]
            if s3 != s2:
                continue
            s1 = S[n - 2 * d] - S[n - 3 * d]
            if s1 == s2:
                return n - 1
    return len(w)


# ---------------------------------------------------------------------------
# Uniform morphism
# ---------------------------------------------------------------------------

@dataclass
class UniformMorph:
    m: int
    alph: Tuple[int, ...]
    images: dict[int, Tuple[int, ...]]

    def __post_init__(self):
        for a in self.alph:
            img = self.images[a]
            if len(img) != self.m:
                raise ValueError(f"image of {a} has length {len(img)} != {self.m}")
            for x in img:
                if x not in self.alph:
                    raise ValueError(f"image of {a} contains {x} not in alphabet")

    @property
    def T(self) -> Vec4:
        return tuple(sum(self.images[a]) for a in self.alph)  # type: ignore[return-value]

    @property
    def n_sums(self) -> int:
        return len(set(self.T))

    def T_of(self, a: int) -> int:
        return sum(self.images[a])

    def incidence(self) -> list[list[int]]:
        """M[i][j] = count of alph[i] in image of alph[j] (columns = images)."""
        k = len(self.alph)
        M = [[0] * k for _ in range(k)]
        idx = {a: i for i, a in enumerate(self.alph)}
        for j, a in enumerate(self.alph):
            for x in self.images[a]:
                M[idx[x]][j] += 1
        return M

    def apply(self, u: Sequence[int]) -> Word:
        out: Word = []
        for a in u:
            out.extend(self.images[a])
        return out

    def iterate(self, seed: int, cap: int) -> Word:
        w: Word = [seed]
        while len(w) < cap:
            nxt = self.apply(w)
            if len(nxt) <= len(w):
                break
            w = nxt
        return w[:cap]

    def prefix_sum(self, a: int, length: int) -> int:
        return sum(self.images[a][:length])

    def suffix_sum(self, a: int, start: int) -> int:
        return sum(self.images[a][start:])

    def prolongable(self, seed: int) -> bool:
        return self.images[seed][0] == seed

    def code(self) -> str:
        return " ".join("".join(str(x) for x in self.images[a]) for a in self.alph)


def morph_from_strings(imgs: Sequence[str], alph: Sequence[int] = (0, 1, 2, 3)) -> UniformMorph:
    m = len(imgs[0])
    images = {alph[i]: tuple(int(c) for c in imgs[i]) for i in range(4)}
    # For alphabet {0,1,3,4} the digit characters are still 0,1,3,4.
    if set(alph) != {0, 1, 2, 3}:
        images = {}
        for i, a in enumerate(alph):
            images[a] = tuple(alph[int(c)] if c.isdigit() and int(c) < 4 else int(c) for c in imgs[i])
            # Prefer writing images with the actual letter values as digits; 4 is one character.
    return UniformMorph(m=m, alph=tuple(alph), images=images)


def morph_from_words(imgs: Sequence[Sequence[int]], alph: Sequence[int] = (0, 1, 2, 3)) -> UniformMorph:
    m = len(imgs[0])
    images = {alph[i]: tuple(imgs[i]) for i in range(4)}
    return UniformMorph(m=m, alph=tuple(alph), images=images)


# ---------------------------------------------------------------------------
# Phase 1: interval geometry
# ---------------------------------------------------------------------------

@dataclass
class Piece:
    """One intersection of [p, p+length) with a substitution block."""
    block_index: int  # ancestor index
    start_off: int    # inclusive offset in the image
    end_off: int      # exclusive offset in the image

    @property
    def length(self) -> int:
        return self.end_off - self.start_off

    @property
    def complete(self) -> bool:
        return self.start_off == 0 and self.end_off == self.end_off  # filled in by caller with m


def interval_pieces(p: int, length: int, m: int) -> list[Tuple[int, int, int]]:
    """Cover [p, p+length) by (block_index, start_off, end_off) with 0 <= start < end <= m."""
    if length <= 0:
        return []
    pieces = []
    x = p
    end = p + length
    while x < end:
        blk = x // m
        off = x % m
        take = min(m - off, end - x)
        pieces.append((blk, off, off + take))
        x += take
    return pieces


def n_complete_closed_form(m: int, r: int, s: int, k: int) -> int:
    """Number of complete sigma-blocks in an interval of length k*m+s starting at offset r.

    Derived: complete indices j satisfy ceil(p/m) <= j <= floor((p+d)/m) - 1.
    Independent of q. For k large enough that the interval is not confined to one block:
        if r == 0: n = k
        if r > 0:  n = k + (r+s)//m - 1
    When k == 0 and s <= m-r the interval sits inside one block and n = 0.
    """
    # Use the same formula as interval_pieces for all k, including small.
    p = r  # translate so q = 0
    d = k * m + s
    pieces = interval_pieces(p, d, m)
    return sum(1 for blk, a, b in pieces if a == 0 and b == m)


@dataclass
class ResidueType:
    """Exact boundary configuration of one interval, depending only on (m,r,s) and k."""
    m: int
    r: int
    s: int
    left_len: int          # 0 if aligned at start
    right_len: int         # 0 if aligned at end
    n_complete_offset: int  # n_complete = k + n_complete_offset for k large
    intra_block: bool      # True when even large-k formula does not apply at k=0
    split_same_block: bool

    def n_complete(self, k: int) -> int:
        return n_complete_closed_form(self.m, self.r, self.s, k)


def classify_residue(m: int, r: int, s: int) -> ResidueType:
    assert 0 <= r < m and 0 <= s < m
    # Use a large dummy k so the interval spans many blocks.
    k = 3
    pieces = interval_pieces(r, k * m + s, m)
    completes = [(blk, a, b) for blk, a, b in pieces if a == 0 and b == m]
    partials = [(blk, a, b) for blk, a, b in pieces if not (a == 0 and b == m)]
    left_len = 0
    right_len = 0
    if partials:
        # first partial is a suffix of the start block (unless the whole interval is one partial)
        b0, a0, e0 = partials[0]
        if a0 != 0:
            left_len = e0 - a0 if len(completes) == 0 and len(partials) == 1 and e0 != m else (
                m - a0 if a0 != 0 else 0
            )
        # cleaner:
    left_len = 0 if r == 0 else (m - r)
    right_len = (r + s) % m
    # When the interval is longer than the first suffix, left_len is m-r.
    # For k>=1 this is always true except we still record the large-k type.
    n_large = n_complete_closed_form(m, r, s, k)
    offset = n_large - k
    intra = n_complete_closed_form(m, r, s, 0) == 0 and (r + s <= m)
    return ResidueType(
        m=m,
        r=r,
        s=s,
        left_len=0 if r == 0 else m - r,
        right_len=right_len,
        n_complete_offset=offset,
        intra_block=(s <= m - r),  # whole interval in one block when k=0
        split_same_block=False,
    )


def residue_table(m: int) -> list[ResidueType]:
    return [classify_residue(m, r, s) for r in range(m) for s in range(m)]


# ---------------------------------------------------------------------------
# Phase 2: apply geometry to a concrete word
# ---------------------------------------------------------------------------

@dataclass
class IntervalView:
    p: int
    length: int
    pieces: list[Tuple[int, int, int]]
    complete_letters: list[int]
    psi: Vec4
    defect: int
    T_contrib: int
    reconstructed_sum: int
    true_sum: int
    left: Optional[Tuple[int, int, int]]  # (letter, start_off, end_off)
    right: Optional[Tuple[int, int, int]]
    n_complete: int
    r: int
    s: int
    k: int


def view_interval(
    W: Sequence[int],
    U: Sequence[int],
    sigma: UniformMorph,
    p: int,
    length: int,
) -> IntervalView:
    m = sigma.m
    alph = sigma.alph
    pieces = interval_pieces(p, length, m)
    complete_letters: list[int] = []
    defect = 0
    left = None
    right = None
    partials: list[Tuple[int, int, int]] = []
    for blk, a, b in pieces:
        letter = U[blk]
        if a == 0 and b == m:
            complete_letters.append(letter)
        else:
            partials.append((letter, a, b))
            defect += sum(sigma.images[letter][a:b])
    if partials:
        left = partials[0]
        if len(partials) > 1:
            right = partials[-1]
    psi = parikh(complete_letters, alph)
    T_contrib = vdot(sigma.T, psi)
    recon = T_contrib + defect
    true = sum(W[p : p + length])
    r = p % m
    s = length % m
    k = length // m
    return IntervalView(
        p=p,
        length=length,
        pieces=pieces,
        complete_letters=complete_letters,
        psi=psi,
        defect=defect,
        T_contrib=T_contrib,
        reconstructed_sum=recon,
        true_sum=true,
        left=left,
        right=right,
        n_complete=len(complete_letters),
        r=r,
        s=s,
        k=k,
    )


@dataclass
class CubeAnatomy:
    i: int
    d: int
    block_sum: int
    X: IntervalView
    Y: IntervalView
    Z: IntervalView
    v_xy: Vec4
    v_yz: Vec4
    db_xy: int
    db_yz: int
    dn_xy: int
    dn_yz: int
    T: Vec4
    aligned: bool
    ancestor_acf: bool
    ancestor_has_T_cube: bool
    ancestor_span: Tuple[int, int]
    U_factor: list[int]
    T_word: list[int]
    # Does (v_xy, db_xy) satisfy the exact equation?
    eq_xy_ok: bool
    eq_yz_ok: bool
    recon_ok: bool


def ancestor_of(W: Sequence[int], sigma: UniformMorph, upto: int) -> Word:
    """Unique U such that sigma(U) covers W[:upto] (pad to full blocks)."""
    m = sigma.m
    n_blocks = (upto + m - 1) // m
    # Invert blockwise. W may be a prefix of sigma(U) for the true ancestor iterate.
    U: Word = []
    inv = {img: a for a, img in sigma.images.items()}
    # Images need not be unique. Prefer matching the actual iterate if provided via decoding
    # by reading W in chunks and matching any image; if collision, leave a sentinel and
    # require the caller to pass U. Here we try unique decode, else None via exception.
    for j in range(n_blocks):
        chunk = tuple(W[j * m : (j + 1) * m])
        if len(chunk) < m:
            # incomplete last block: still identify by prefix match
            matches = [a for a, img in sigma.images.items() if img[: len(chunk)] == chunk]
            if len(matches) != 1:
                raise ValueError("ambiguous incomplete block")
            U.append(matches[0])
            break
        matches = [a for a, img in sigma.images.items() if img == chunk]
        if len(matches) == 1:
            U.append(matches[0])
        elif len(matches) > 1:
            # repeated images: cannot uniquely decode; caller should pass U
            raise ValueError("non-injective images, pass U explicitly")
        else:
            raise ValueError(f"chunk {chunk} is not an image")
    return U


def analyze_cube(
    W: Sequence[int],
    sigma: UniformMorph,
    cube: Optional[Tuple[int, int, int]] = None,
    U: Optional[Word] = None,
) -> CubeAnatomy:
    if cube is None:
        cube = find_cube(W)
    if cube is None:
        raise ValueError("no cube")
    i, d, sm = cube
    m = sigma.m
    if U is None:
        U = ancestor_of(W, sigma, i + 3 * d)
    X = view_interval(W, U, sigma, i, d)
    Y = view_interval(W, U, sigma, i + d, d)
    Z = view_interval(W, U, sigma, i + 2 * d, d)
    v_xy = vsub(X.psi, Y.psi)
    v_yz = vsub(Y.psi, Z.psi)
    db_xy = X.defect - Y.defect
    db_yz = Y.defect - Z.defect
    eq_xy = vdot(sigma.T, v_xy) + db_xy == 0
    eq_yz = vdot(sigma.T, v_yz) + db_yz == 0
    recon_ok = (
        X.reconstructed_sum == X.true_sum == sm
        and Y.reconstructed_sum == Y.true_sum == sm
        and Z.reconstructed_sum == Z.true_sum == sm
    )
    first = min(X.pieces[0][0], Y.pieces[0][0], Z.pieces[0][0])
    last = max(X.pieces[-1][0], Y.pieces[-1][0], Z.pieces[-1][0])
    U_factor = list(U[first : last + 1])
    T_word = [sigma.T_of(a) for a in U_factor]
    ancestor_acf = find_cube(U_factor) is None
    # T-coded additive cube on the complete-letter words, ignoring defects
    ancestor_has_T_cube = False
    cx, cy, cz = X.complete_letters, Y.complete_letters, Z.complete_letters
    if cx and len(cx) == len(cy) == len(cz):
        ancestor_has_T_cube = sum(sigma.T_of(a) for a in cx) == sum(sigma.T_of(a) for a in cy) == sum(
            sigma.T_of(a) for a in cz
        )
    return CubeAnatomy(
        i=i,
        d=d,
        block_sum=sm,
        X=X,
        Y=Y,
        Z=Z,
        v_xy=v_xy,
        v_yz=v_yz,
        db_xy=db_xy,
        db_yz=db_yz,
        dn_xy=X.n_complete - Y.n_complete,
        dn_yz=Y.n_complete - Z.n_complete,
        T=sigma.T,
        aligned=(i % m == 0 and d % m == 0),
        ancestor_acf=ancestor_acf,
        ancestor_has_T_cube=ancestor_has_T_cube,
        ancestor_span=(first, last),
        U_factor=U_factor,
        T_word=T_word,
        eq_xy_ok=eq_xy,
        eq_yz_ok=eq_yz,
        recon_ok=recon_ok,
    )


# ---------------------------------------------------------------------------
# Phase 3: finite defect set
# ---------------------------------------------------------------------------

def defect_set(sigma: UniformMorph) -> set[int]:
    """All possible one-fragment defects: prefix/suffix sums of images, plus 0.

    A cube interval has at most two fragments, so interval defects lie in
    defect_set + defect_set (sumset). Independent of substitution depth.
    """
    vals = {0}
    for a in sigma.alph:
        img = sigma.images[a]
        acc = 0
        for t in range(1, sigma.m):
            acc += img[t - 1]
            vals.add(acc)  # prefix of length t
        acc = 0
        for t in range(1, sigma.m):
            acc += img[sigma.m - t]
            vals.add(acc)  # suffix of length t
    return vals


def two_fragment_defects(sigma: UniformMorph) -> set[int]:
    one = defect_set(sigma)
    return {x + y for x in one for y in one}


# ---------------------------------------------------------------------------
# Phase 7: AP lattices
# ---------------------------------------------------------------------------

def lattice_basis(alph: Sequence[int]) -> Tuple[Vec4, Vec4]:
    """Integer kernel of [1 1 1 1; a b c d]. Parametrize by (v2, v3).

    From sum v_i = 0 and A·v = 0:
        v1 = -((a2-a0) v2 + (a3-a0) v3) / (a1-a0)   if a1 != a0
    For consecutive integers this is integral. We brute-force a short basis.
    """
    # Scan small vectors and extract an HNF-like basis of rank 2.
    found: list[Vec4] = []
    bound = 6
    for x0, x1, x2 in product(range(-bound, bound + 1), repeat=3):
        x3 = -x0 - x1 - x2
        v = (x0, x1, x2, x3)
        if v == (0, 0, 0, 0):
            continue
        if vdot(alph, v) != 0:
            continue
        if vgcd(v) != 1:
            continue
        # keep if not in Z-span of already found (rank <= 2)
        if not found:
            found.append(v)
        elif len(found) == 1:
            # independent?
            a = found[0]
            # check if v is integer multiple of a
            indep = True
            for i in range(4):
                if a[i] != 0:
                    if v[i] % a[i] == 0:
                        k = v[i] // a[i]
                        if all(v[j] == k * a[j] for j in range(4)):
                            indep = False
                    else:
                        pass
                    break
            else:
                indep = True
            # more robust: 2x2 minors
            indep = False
            for i in range(4):
                for j in range(i + 1, 4):
                    if a[i] * v[j] - a[j] * v[i] != 0:
                        indep = True
                        break
                if indep:
                    break
            if indep:
                found.append(v)
        if len(found) == 2:
            break
    if len(found) != 2:
        raise RuntimeError(f"failed to find lattice basis for {alph}: {found}")
    return (found[0], found[1])


def enumerate_lattice(alph: Sequence[int], inf_bound: int = 6) -> list[Vec4]:
    out = []
    for x0, x1, x2 in product(range(-inf_bound, inf_bound + 1), repeat=3):
        x3 = -x0 - x1 - x2
        v = (x0, x1, x2, x3)
        if vdot(alph, v) != 0:
            continue
        if vlinf(v) > inf_bound:
            continue
        out.append(v)
    return out


def primitive_lattice(alph: Sequence[int], inf_bound: int = 6) -> list[Vec4]:
    vecs = []
    seen = set()
    for v in enumerate_lattice(alph, inf_bound):
        if v == (0, 0, 0, 0):
            continue
        g = vgcd(v)
        p = tuple(x // g for x in v)
        # canonical sign: first nonzero positive
        for x in p:
            if x < 0:
                p = tuple(-t for t in p)
                break
            if x > 0:
                break
        if p not in seen:
            seen.add(p)
            vecs.append(p)  # type: ignore[arg-type]
    vecs.sort(key=lambda v: (vl1(v), vlinf(v), v))
    return vecs


def in_lattice(v: Vec4, alph: Sequence[int]) -> bool:
    return sum(v) == 0 and vdot(alph, v) == 0


def induced_pairs(v: Vec4, alph: Sequence[int], max_len: int = 6) -> list[Tuple[str, str]]:
    """Equal-length equal-sum words with Parikh difference v (positive vs negative support)."""
    pos = []
    neg = []
    for a, c in zip(alph, v):
        if c > 0:
            pos.extend([a] * c)
        elif c < 0:
            neg.extend([a] * (-c))
    if not pos or not neg:
        return []
    # All distinct permutations of each, up to a cap
    from itertools import permutations

    pairs = []
    seen = set()
    for p in set(permutations(pos)):
        for q in set(permutations(neg)):
            if p == q:
                continue
            key = (p, q)
            if key in seen:
                continue
            seen.add(key)
            pairs.append(("".join(str(x) for x in p), "".join(str(x) for x in q)))
            if len(pairs) >= 8:
                return pairs
    return pairs


# ---------------------------------------------------------------------------
# Recursive desubstitution of complete interiors
# ---------------------------------------------------------------------------

def peel(anatomy: CubeAnatomy, sigma: UniformMorph) -> dict:
    """One desubstitution step: interiors become T-weighted ancestor factors."""
    X, Y, Z = anatomy.X, anatomy.Y, anatomy.Z
    return {
        "nX": X.n_complete,
        "nY": Y.n_complete,
        "nZ": Z.n_complete,
        "psiX": X.psi,
        "psiY": Y.psi,
        "psiZ": Z.psi,
        "v_xy": anatomy.v_xy,
        "v_yz": anatomy.v_yz,
        "defectX": X.defect,
        "defectY": Y.defect,
        "defectZ": Z.defect,
        "db_xy": anatomy.db_xy,
        "db_yz": anatomy.db_yz,
        "T_dot_v_xy": vdot(sigma.T, anatomy.v_xy),
        "T_dot_v_yz": vdot(sigma.T, anatomy.v_yz),
        "in_kerT_ker1_xy": sum(anatomy.v_xy) == 0 and vdot(sigma.T, anatomy.v_xy) == 0,
        "in_L_A_xy": in_lattice(anatomy.v_xy, sigma.alph),
        "in_L_T_xy": sum(anatomy.v_xy) == 0 and vdot(sigma.T, anatomy.v_xy) == 0,
        "aligned": anatomy.aligned,
        "type": (anatomy.X.r, anatomy.X.s),
        "types": ((X.r, X.s), (Y.r, Y.s), (Z.r, Z.s)),
        "l1_v_xy": vl1(anatomy.v_xy),
        "l1_v_yz": vl1(anatomy.v_yz),
        "ancestor_acf": anatomy.ancestor_acf,
        "ancestor_has_T_cube": anatomy.ancestor_has_T_cube,
        "eq_ok": anatomy.eq_xy_ok and anatomy.eq_yz_ok and anatomy.recon_ok,
    }


def cube_type_key(anatomy: CubeAnatomy) -> Tuple:
    """Finite state candidate: residue geometry + normalised (small) defects + length-diff of interiors."""
    return (
        anatomy.X.r,
        anatomy.X.s,
        anatomy.dn_xy,
        anatomy.dn_yz,
        anatomy.db_xy,
        anatomy.db_yz,
        anatomy.aligned,
    )


# ---------------------------------------------------------------------------
# Self-test / famous witness
# ---------------------------------------------------------------------------

FAMOUS = morph_from_strings(["32", "31", "20", "01"])  # 0->32, 1->31, 2->20, 3->01


def famous_word(cap: int = 256) -> Word:
    return FAMOUS.iterate(2, cap)


def _check_recon_identity(sigma: UniformMorph, U: Word, trials: int = 200) -> int:
    """Return number of failures of sum = T·psi + defect on random intervals."""
    W = sigma.apply(U)
    m = sigma.m
    n = len(W)
    fails = 0
    import random

    rng = random.Random(0)
    for _ in range(trials):
        p = rng.randrange(0, n)
        d = rng.randrange(1, max(2, n - p + 1))
        view = view_interval(W, U, sigma, p, d)
        if view.reconstructed_sum != view.true_sum:
            fails += 1
    return fails


def main():
    sigma = FAMOUS
    print("=== Famous 2-uniform morphism ===")
    print("images", {a: "".join(map(str, sigma.images[a])) for a in sigma.alph})
    print("T", sigma.T, "n_sums", sigma.n_sums)
    print("incidence", sigma.incidence())
    W = famous_word(256)
    L = first_acf_len(W)
    print("ACF prefix", L)
    cube = find_cube(W)
    print("first cube", cube)
    # U = decode of a covering prefix
    U = ancestor_of(W, sigma, cube[0] + 3 * cube[1])
    print("ancestor |U|", len(U), "U[:20]", "".join(map(str, U[:20])))
    print("U ACF?", find_cube(U) is None, "first_acf_len(U)", first_acf_len(U))
    an = analyze_cube(W, sigma, cube, U)
    print("aligned", an.aligned, "r,s,k", an.X.r, an.X.s, an.X.k)
    print("n_complete X,Y,Z", an.X.n_complete, an.Y.n_complete, an.Z.n_complete)
    print("psi X,Y,Z", an.X.psi, an.Y.psi, an.Z.psi)
    print("v_xy", an.v_xy, "v_yz", an.v_yz, "L1", vl1(an.v_xy), vl1(an.v_yz))
    print("defects X,Y,Z", an.X.defect, an.Y.defect, an.Z.defect)
    print("db_xy", an.db_xy, "db_yz", an.db_yz)
    print("T·v_xy", vdot(sigma.T, an.v_xy), "should be", -an.db_xy)
    print("T·v_yz", vdot(sigma.T, an.v_yz), "should be", -an.db_yz)
    print("eq_ok", an.eq_xy_ok, an.eq_yz_ok, "recon", an.recon_ok)
    print("in L(A)?", in_lattice(an.v_xy, sigma.alph), in_lattice(an.v_yz, sigma.alph))
    print("ancestor_acf", an.ancestor_acf, "T-cube interiors", an.ancestor_has_T_cube)
    print("left/right X", an.X.left, an.X.right)
    print("left/right Y", an.Y.left, an.Y.right)
    print("left/right Z", an.Z.left, an.Z.right)
    print("type key", cube_type_key(an))
    print("peel", peel(an, sigma))
    fails = _check_recon_identity(sigma, U, 300)
    print("random interval identity failures", fails)
    print("defect_set", sorted(defect_set(sigma)))
    print("two-fragment defects", sorted(two_fragment_defects(sigma)))
    print("\n=== Residue types m=2 ===")
    for t in residue_table(2):
        print(
            f"  (r,s)=({t.r},{t.s}) left={t.left_len} right={t.right_len} "
            f"n_complete = k + ({t.n_complete_offset})  k=0 in-block={t.intra_block} "
            f"n(k=0..3)={[t.n_complete(k) for k in range(4)]}"
        )
    print("\n=== L(A) primitive vectors inf<=4 ===")
    for v in primitive_lattice((0, 1, 2, 3), 4):
        print(" ", v, "L1", vl1(v), "pairs", induced_pairs(v, (0, 1, 2, 3))[:3])
    print("\n=== L(B) primitive vectors inf<=4 ===")
    for v in primitive_lattice((0, 1, 3, 4), 4):
        print(" ", v, "L1", vl1(v), "pairs", induced_pairs(v, (0, 1, 3, 4))[:3])


if __name__ == "__main__":
    main()
