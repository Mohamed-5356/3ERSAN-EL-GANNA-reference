abaab
sa  2 3 0 4 1 
lcp 0 1 2 0 1
// === Suffix Array (radix doubling) + Kasai LCP + RMQ ===
// O(n log n) construction. Comfortable up to n ≈ 5e5.
// No sentinel needed — virtual -1 padding is built into rank update.

const int MAXN  = 5e5 + 10;   // raise per-problem; cnt[] must hold ≥ max(SIGMA, n)
const int LOG   = 25;         // ceil(log2(MAXN))
const int SIGMA = 256;        // raise if alphabet > byte

int n;                        // working length
int sa[MAXN];                 // sa[i]: start index of i-th lex suffix
int rk[MAXN];                 // rk[i]: rank of suffix at i (= sa^{-1})
int tmp[MAXN];                // scratch (old rank / order buffer)
int cnt[MAXN];                // counting buckets; sized for max(SIGMA, n)
int lcp[MAXN];                // lcp[i] = LCP(sa[i-1], sa[i]); lcp[0] = 0
int st[LOG][MAXN];            // sparse table over lcp[]
int lg[MAXN];                 // floor log2

string s;                     // input

// Does: build sa[], rk[] for s.
// In:   s of length n ≥ 1; chars in [0, SIGMA).
// Out:  sa[0..n-1] permutation; rk[0..n-1] inverse.
// Time: O(n log n). Space: O(n + SIGMA).
// Pitfalls: cast to (unsigned char) when indexing — signed chars > 127 break cnt[].
//           Raise SIGMA for non-byte alphabets and rebuild cnt[] sizing.
//           Sets `n = s.size()` — caller doesn't.
void buildSA() {
    n = (int)s.size();
    int m = SIGMA;
    fill(cnt, cnt + m, 0);
    for (int i = 0; i < n; i++) cnt[rk[i] = (unsigned char)s[i]]++;
    for (int i = 1; i < m; i++) cnt[i] += cnt[i-1];
    for (int i = n - 1; i >= 0; i--) sa[--cnt[rk[i]]] = i;

    for (int k = 1; k < n; k <<= 1) {
        int p = 0;
        for (int i = n - k; i < n; i++) tmp[p++] = i;
        for (int i = 0; i < n; i++) if (sa[i] >= k) tmp[p++] = sa[i] - k;

        fill(cnt, cnt + m, 0);
        for (int i = 0; i < n; i++) cnt[rk[tmp[i]]]++;
        for (int i = 1; i < m; i++) cnt[i] += cnt[i-1];
        for (int i = n - 1; i >= 0; i--) sa[--cnt[rk[tmp[i]]]] = tmp[i];

        for (int i = 0; i < n; i++) tmp[i] = rk[i];
        rk[sa[0]] = 0;
        p = 1;
        for (int i = 1; i < n; i++) {
            int a = sa[i], b = sa[i-1];
            bool same = tmp[a] == tmp[b] &&
                        (a + k < n ? tmp[a + k] : -1) == (b + k < n ? tmp[b + k] : -1);
            rk[a] = same ? p - 1 : p++;
        }
        if (p == n) break;
        m = p;
    }
}

// Does: Kasai's LCP between consecutive SA entries.
// In:   requires buildSA().
// Out:  lcp[1..n-1]; lcp[0] = 0 by convention.
// Time: O(n). Space: O(1) extra.
// Pitfalls: lcp[0] has no meaning (no predecessor) — never query it.
//           After this call, k may walk off the string — guarded by `i+k<n && j+k<n`.
void buildLCP() {
    lcp[0] = 0;
    for (int i = 0, k = 0; i < n; i++) {
        if (rk[i] == 0) { k = 0; continue; }
        int j = sa[rk[i] - 1];
        while (i + k < n && j + k < n && s[i + k] == s[j + k]) k++;
        lcp[rk[i]] = k;
        if (k) k--;
    }
}

// Does: sparse table over lcp[] for O(1) range-min.
// In:   requires buildLCP().
// Out:  st[][], lg[] filled.
// Time: O(n log n) build. Space: O(n log n).
// Pitfalls: memory — LOG·MAXN ints. With LOG=20, MAXN=5e5: ~40 MB. Shrink LOG/MAXN
//           if the judge gives ≤ 256 MB and you have other large structures.
void buildSparse() {
    lg[1] = 0;
    for (int i = 2; i <= n; i++) lg[i] = lg[i/2] + 1;
    for (int i = 0; i < n; i++) st[0][i] = lcp[i];
    for (int j = 1; (1 << j) <= n; j++)
        for (int i = 0; i + (1 << j) <= n; i++)
            st[j][i] = min(st[j-1][i], st[j-1][i + (1 << (j-1))]);
}

// Does: LCP of suffixes starting at i and j in original s.
// In:   0 ≤ i, j < n. Requires buildSparse().
// Out:  length of longest common prefix of s[i..] and s[j..].
// Time: O(1).
// Pitfalls: query window is lcp[l+1 .. r] inclusive (NOT lcp[l..r]).
//           i == j must short-circuit to n - i; otherwise you query an empty range.
int lcpQuery(int i, int j) {
    if (i == j) return n - i;
    int l = rk[i], r = rk[j];
    if (l > r) swap(l, r);
    int len = r - l, k = lg[len];
    return min(st[k][l + 1], st[k][r - (1 << k) + 1]);
}

// Does: count of distinct non-empty substrings of s.
// In:   requires buildSA() and buildLCP().
// Out:  long long.
// Time: O(n). Space: O(1).
// Pitfalls: int64 — n=5e5 ⇒ ~1.25e11. Don't store in int.
//           No sentinel adjustment needed (we built without one).
long long distinctSubstrings() {
    long long N = n, total = N * (N + 1) / 2;
    for (int i = 1; i < n; i++) total -= lcp[i];
    return total;
}

// Does: longest substring occurring ≥ 2 times.
// In:   requires buildLCP().
// Out:  {length, start position in s}; {0, 0} if no repeat.
// Time: O(n).
// Pitfalls: ties broken by first SA encounter; if you need a specific occurrence
//           or all max occurrences, iterate lcp[] yourself.
pair<int,int> longestRepeated() {
    int best = 0, pos = 0;
    for (int i = 1; i < n; i++)
        if (lcp[i] > best) { best = lcp[i]; pos = sa[i]; }
    return {best, pos};
}

// Does: range [L, R) of SA indices whose suffix has p as a prefix.
// In:   |p| ≥ 1. Requires buildSA().
// Out:  {L, R}. R - L = occurrence count. sa[L..R-1] = match positions (unsorted).
// Time: O(|p| log n). Space: O(1) extra.
// Pitfalls: empty p returns {0, n} — every suffix has empty prefix. Guard upstream.
//           To enumerate matches in input order, sort sa+L .. sa+R or copy and sort.
pair<int,int> findRange(const string& p) {
    int m = (int)p.size();
    int lo = 0, hi = n;
    while (lo < hi) {
        int mid = (lo + hi) / 2;
        if (s.compare(sa[mid], m, p) < 0) lo = mid + 1; else hi = mid;
    }
    int L = lo;
    lo = 0; hi = n;
    while (lo < hi) {
        int mid = (lo + hi) / 2;
        if (s.compare(sa[mid], m, p) <= 0) lo = mid + 1; else hi = mid;
    }
    return {L, lo};
}

int countOcc(const string& p) { auto [L, R] = findRange(p); return R - L; }
vector<int> findAll(const string& p) {
    auto [L, R] = findRange(p);
    vector<int> v(sa + L, sa + R);
    sort(v.begin(), v.end());
    return v;
}
// Does: lex-compare s[a..a+la-1] vs s[b..b+lb-1].
// In:   0 ≤ a, b; a+la ≤ n; b+lb ≤ n; la, lb ≥ 0. Requires buildSparse().
// Out:  -1, 0, +1.
// Time: O(1).
// Pitfalls: empty substring (la or lb = 0) — caller guards if it matters.
//           Don't pass la, lb that overflow s — no bounds check inside.
int cmpSub(int a, int la, int b, int lb) {
    int common = lcpQuery(a, b);
    int mn = min(la, lb);
    if (common >= mn) return la == lb ? 0 : (la < lb ? -1 : 1);
    return s[a + common] < s[b + common] ? -1 : 1;
}

// contribution: each lcp[i] is the min of (i-PLE)*(NLE-i) subarrays of lcp[]
long long pairLcpSum(vector<int>& lcp) {           // lcp indexed 1..n-1; pass slice [1..n-1] as h[0..m-1]
    int m = lcp.size();
    vector<int> L(m), R(m);
    stack<int> st;
    for (int i = 0; i < m; i++) {                  // strict: previous strictly-less
        while (!st.empty() && lcp[st.top()] >= lcp[i]) st.pop();
        L[i] = st.empty() ? -1 : st.top();
        st.push(i);
    }
    while (!st.empty()) st.pop();
    for (int i = m - 1; i >= 0; i--) {             // non-strict: next less-or-equal
        while (!st.empty() && lcp[st.top()] > lcp[i]) st.pop();
        R[i] = st.empty() ? m : st.top();
        st.push(i);
    }
    long long s = 0;
    for (int i = 0; i < m; i++)
        s += 1LL * lcp[i] * (i - L[i]) * (R[i] - i);
    return s;
}

// max over windows of size k-1 of window-min on lcp[1..n-1]; deque increasing
int longestKRepeat(vector<int>& lcp, int k) {      // lcp valid on [1, n-1]
    int m = lcp.size(), w = k - 1, best = 0;       // window size k-1, NOT k
    if (w <= 0) return -1;                          // k=1 → whole string, handle outside
    deque<int> dq;
    for (int i = 1; i < m; i++) {
        while (!dq.empty() && lcp[dq.back()] >= lcp[i]) dq.pop_back();
        dq.push_back(i);
        if (dq.front() <= i - w) dq.pop_front();
        if (i - w >= 0) best = max(best, lcp[dq.front()]);
    }
    return best;
}

// largest rectangle on lcp histogram; width w bars = w+1 suffixes
long long bestLenTimesCount(vector<int>& lcp, int n) {  // n = string length
    int m = lcp.size();
    vector<int> h(lcp.begin() + 1, lcp.end());     // bars = lcp[1..n-1]
    h.push_back(0);                                 // sentinel flushes stack
    long long best = n;                             // baseline: whole string ×1
    stack<int> st;
    for (int i = 0; i < (int)h.size(); i++) {
        while (!st.empty() && h[st.top()] >= h[i]) {
            int hh = h[st.top()]; st.pop();
            int L = st.empty() ? -1 : st.top();
            long long wBars = i - L - 1;
            best = max(best, 1LL * hh * (wBars + 1)); // +1: suffixes = bars+1
        }
        st.push(i);
    }
    return best;
}


// f = contribution per distinct substring given its occurrence count c
long long bandSum(vector<int>& lcp, int n, function<long long(long long)> f) {
    int m = lcp.size();
    vector<int> h(lcp.begin() + 1, lcp.end());
    h.push_back(0);
    // distinct substrings occurring exactly once are NOT covered by bands:
    // handle via per-suffix "new characters": (n - sa[i]) - lcp[i] each contributes f(1)... 
    // actually those overlap bands only above lcp; total distinct = n(n+1)/2 - sum(lcp).
    long long res = 0;
    stack<int> st;
    for (int i = 0; i < (int)h.size(); i++) {
        while (!st.empty() && h[st.top()] >= h[i]) {
            int hh = h[st.top()]; st.pop();
            int hLow = max(st.empty() ? 0 : h[st.top()], h[i]); // floor after pop
            long long wBars = i - (st.empty() ? -1 : st.top()) - 1;
            res += 1LL * (hh - hLow) * f(wBars + 1 + 1 - 1);    // c = wBars+1 occurrences
        }
        st.push(i);
    }
    return res;   // covers substrings with c >= 2; add singles separately if f(1) != 0
}

pair<int,int> findRange(vector<int> &p) {
    int m = (int)p.size();
    int lo = 0, hi = n;
    while (lo < hi) {
        int mid = (lo + hi) / 2;
        int e = min((int)s.size(), sa[mid] + m);
        if (lexicographical_compare(s.begin()+sa[mid], s.begin()+e, p.begin(), p.end()))
            lo = mid + 1; else hi = mid;
    }
    int L = lo;
    lo = 0; hi = n;
    while (lo < hi) {
        int mid = (lo + hi) / 2;
        //  suffix <= p  ==  !(p < suffix)
        int e = min((int)s.size(), sa[mid] + m);
        if (!lexicographical_compare(p.begin(), p.end(), s.begin()+sa[mid], s.begin()+e))
            lo = mid + 1; else hi = mid;
    }
    return {L, lo};
}