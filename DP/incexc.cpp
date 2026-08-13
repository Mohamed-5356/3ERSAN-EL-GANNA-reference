// ============================================================================
// DIVISOR INVERSION / MOBIUS  -- template reference
// ----------------------------------------------------------------------------
// WHEN TO REACH FOR THIS
//   You count objects tagged by an invariant g that is FORCED to divide n
//   (gcd of parts, gcd of a chosen subset, period of a string, ...).
//   "invariant is a MULTIPLE of d" is easy to count; "invariant is EXACTLY d"
//   is not. The exact-classes partition the whole set, so the easy quantity is
//   their divisor-sum -> invert.
//
//   Tell: your first instinct is "subtract one term per prime divisor".
//   The moment you write that you owe an inclusion-exclusion layer.
//
// TWO DUAL FORMS -- PICK BEFORE WRITING ANYTHING
//   Wrong direction still passes small tests. Decide explicitly.
//
//   FORM A -- divisor sum      F(n) = sum_{d | n} f(d)
//             have: F at each divisor of ONE n
//             iterate divisors ASCENDING
//             e.g. CF 900D Unusual Sequences
//
//   FORM B -- multiple sum     G(d) = sum_{d | m} f(m)
//             have: G[d] for ALL d <= N
//             iterate d from N DOWN to 1
//             e.g. CF 803F Coprime Subsequences
// ============================================================================

#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
#define fio ios_base::sync_with_stdio(0); cin.tie(0); cout.tie(0);

const int mod = 1'000'000'007;
int add(int a, int b) { return (a += b) < mod ? a : a - mod; }
int subt(int a, int b) { return (a -= b) < 0 ? a + mod : a; }
int mult(int a, int b) { return 1ll * a * b % mod; }

// exponent is ll: in form A the exponent is n-1 with n up to 1e9, and callers
// pass raw divisors -- do not narrow this to int without checking the caller.
int fp(int b, ll e) {
    int r = 1;
    for (; e; b = mult(b, b), e >>= 1)
        if (e & 1) r = mult(r, b);
    return r;
}
int divi(int a, int b) { return mult(a, fp(b, mod - 2)); }

// ============================================================================
// SNIPPET 1 -- FORM A, single n, recursive inclusion-exclusion
// ----------------------------------------------------------------------------
// Identity:  F(n) = sum_{d | n} f(d)
// Isolate:   f(n) = F(n) - sum_{d | n, d < n} f(d)
//
// Invariant: divisors sorted ascending => every proper divisor of dv[j] sits at
// some index k < j, so f is already correct there when j is processed.
//
// Complexity: O(sqrt n) to enumerate + O(D^2) to combine + O(D log n) powers.
//             D = d(n) <= 1344 for n <= 1e9  =>  D^2 ~ 1.8M, fine.
// Space: O(D).
//
// TO ADAPT: replace F(). Nothing else changes.
//   F(m) = "unconstrained count at the shrunk instance of size m"
//   i.e. count of objects at scale m with no gcd/period restriction at all.
// ============================================================================

// problem-specific. Below: # compositions of m into positive parts = 2^(m-1).
int F(ll m) { return fp(2, m - 1); }

// returns (divisors ascending, f at each). f.back() == f(n).
pair<vector<ll>, vector<int>> invert_divisors(ll n) {
    vector<ll> dv;
    for (ll i = 1; i * i <= n; i++)
        if (n % i == 0) {
            dv.push_back(i);
            // the partner n/i is distinct from i iff i*i != n. FULL STOP.
            // an extra guard like (n/i) % i silently drops valid divisors.
            if (i * i != n) dv.push_back(n / i);
        }
    sort(dv.begin(), dv.end());

    int D = dv.size();
    vector<int> f(D);
    for (int j = 0; j < D; j++) {
        f[j] = F(dv[j]);
        for (int k = 0; k < j; k++)
            if (dv[j] % dv[k] == 0) f[j] = subt(f[j], f[k]);
    }
    return {dv, f};
}

// n == 1: dv = {1}, f[0] = F(1) = 2^0 = 1. Correct, no special case needed --
// but this is exactly the kind of boundary worth asserting in a sweep.

// ============================================================================
// SNIPPET 2 -- FORM A via Mobius closed form (same answer, faster)
// ----------------------------------------------------------------------------
// Unrolling snippet 1 collapses the coefficient of F(n/d) to mu(d):
//     f(n) = sum_{d | n} mu(d) * F(n/d)
//     mu(1)=1;  mu(d)=0 if a squared prime divides d;  mu(d)=(-1)^k otherwise.
// Only SQUAREFREE divisors survive -> 2^omega(n) terms, omega <= 9 for n <= 1e9
// => at most 512 terms vs ~1.8M above.
//
// Use snippet 1 when you need f at EVERY divisor; snippet 2 when only f(n).
// ============================================================================

int invert_mobius(ll n) {
    vector<ll> p;
    ll m = n;                      // m is destroyed by factorization
    for (ll i = 2; i * i <= m; i++)
        if (m % i == 0) {
            p.push_back(i);
            while (m % i == 0) m /= i;
        }
    if (m > 1) p.push_back(m);     // leftover prime > sqrt(n)

    int k = p.size(), res = 0;
    for (int mask = 0; mask < (1 << k); mask++) {
        ll d = 1;
        for (int b = 0; b < k; b++)
            if (mask >> b & 1) d *= p[b];
        int t = F(n / d);          // n, NOT m -- do not collapse these two vars
        res = (__builtin_popcount(mask) & 1) ? subt(res, t) : add(res, t);
    }
    return res;
}

// ============================================================================
// SNIPPET 3 -- FORM B, range inversion. The contest workhorse.
// ----------------------------------------------------------------------------
// in : g[d] = # objects whose invariant is a MULTIPLE of d,  1 <= d <= N
// out: g[d] = # objects whose invariant is EXACTLY d          (in place)
//
// DESCENDING is what makes it correct: on reaching d, every g[m] with d | m,
// m > d has already been converted to exact, so subtracting them is legal.
//
// Complexity: O(N log N)  (harmonic sum).
// ============================================================================

void invert_multiples(vector<int>& g, int N) {
    for (int d = N; d >= 1; d--)
        for (int m = 2 * d; m <= N; m += d)
            g[d] = subt(g[d], g[m]);
}

// Companion you almost always need first: cnt[d] = # of a_i divisible by d.
// Same harmonic loop, ASCENDING order is irrelevant here (pure accumulation).
// NOTE: plain int addition, no mod -- these are counts, not residues.
vector<int> divisor_counts(const vector<int>& a, int N) {
    vector<int> cnt(N + 1, 0);
    for (int v : a) cnt[v]++;
    for (int d = 1; d <= N; d++)
        for (int m = 2 * d; m <= N; m += d)
            cnt[d] += cnt[m];
    return cnt;
}

// ============================================================================
// SNIPPET 4 -- mu over a range (linear sieve)
// ----------------------------------------------------------------------------
// ONLY needed when a problem wants mu explicitly, e.g. sum_d mu(d)*h(d).
// Plain inversion (snippets 1/3) never needs this.
// Complexity: O(N).
// ============================================================================

vector<int> mu_sieve(int N) {
    vector<int> mu(N + 1, 1), pr;
    vector<char> comp(N + 1, 0);
    for (int i = 2; i <= N; i++) {
        if (!comp[i]) { pr.push_back(i); mu[i] = -1; }
        for (int p : pr) {
            if (1LL * i * p > N) break;   // cast: i*p overflows int near 2e9
            comp[i * p] = 1;
            if (i % p == 0) { mu[i * p] = 0; break; }   // squared factor
            mu[i * p] = -mu[i];
        }
    }
    return mu;
}

// ============================================================================
// WORKED CHECK -- n = 6 (smallest n with two distinct primes; the smallest
// case where a WRONG inclusion-exclusion actually differs from a right one)
// ----------------------------------------------------------------------------
// G(d) = # compositions of 6 with all parts divisible by d = 2^(6/d - 1)
//   G(1)=32   G(2)=4: (6),(4,2),(2,4),(2,2,2)
//   G(3)=2 : (6),(3,3)                      G(6)=1: (6)
//
// NAIVE TRAP: 32 - 4 - 2 = 26.  WRONG -- (6) sits in both G(2) and G(3) and was
// removed twice. Add it back: 32 - 4 - 2 + 1 = 27. That sign pattern IS mu.
//
// Snippet 1: f(1)=1, f(2)=2^1-f(1)=1, f(3)=2^2-f(1)=3, f(6)=32-1-1-3=27.
// Snippet 2: 2^5 - 2^2 - 2^1 + 2^0 = 27.  Agree.
//
// Spot-check the small ones by hand:
//   f(2)=1 -> (1,1)                  [ (2) has gcd 2, excluded ]
//   f(3)=3 -> (1,1,1),(2,1),(1,2)
// ============================================================================

// ============================================================================
// DRIVER -- CF 900D Unusual Sequences   https://codeforces.com/problemset/problem/900/D
// ----------------------------------------------------------------------------
// gcd(a_i) = x and sum = y. Divide through by x: compositions of n = y/x with
// gcd exactly 1. y % x != 0 -> answer 0.
// ============================================================================

int main() {
    fio
    ll x, y;
    cin >> x >> y;
    if (y % x) { cout << 0 << "\n"; return 0; }
    cout << invert_divisors(y / x).second.back() << "\n";
    // equivalently: cout << invert_mobius(y / x) << "\n";
    return 0;
}

// ============================================================================
// ADAPTATION CHECKLIST -- run this before writing code on a new problem
// ----------------------------------------------------------------------------
//   1. Name the invariant. Confirm it divides n (form A) or ranges 1..N (B).
//   2. Write G(d) = "count with invariant a multiple of d" in CLOSED FORM.
//      Can't? The technique does not apply yet -- go back to modelling.
//   3. Pick form A or B from the table at the top of this file.
//   4. Swap F() (form A) or build g[] (form B). Nothing else changes.
//   5. Hand-trace n = 6 against brute force before submitting.
//
// THIRD SHAPE REMINDER: if you need f at ALL values 1..N, do NOT build a
// divisor list per value. Use snippet 3. O(N log N) beats O(N * sqrt N).
//
// SIBLINGS (ratings from memory, NOT verified in search -- treat as approximate)
//   CF 803F   Coprime Subsequences        ~1900  form B, sieve shape
//             https://codeforces.com/problemset/problem/803/F
//   ABC 162 E Sum of gcd of Tuples (Hard)        same inversion, weight each
//             https://atcoder.jp/contests/abc162/tasks/abc162_e
//                                                exact-class by d
//   CF 900D   Unusual Sequences           ~1800  form A (this file)
//             https://codeforces.com/problemset/problem/900/D
//
// TRANSFERABLE HEURISTIC: when an "exact" count resists you, define the
// "at least / multiple of" version first. It is almost always a one-liner and
// inversion is mechanical from there. The hard step is never the inversion --
// it is spotting the invariant that is forced to divide n.
// ============================================================================