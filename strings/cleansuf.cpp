#include <bits/stdc++.h>
#define el '\n'
typedef long long ll;
#define fio ios_base::sync_with_stdio(0); cin.tie(0); cout.tie(0);
using namespace std;
//abaab
//sa  2 3 0 4 1
//lcp 0 1 2 0 1
const int N  = 5e5 + 10, LG = 25, sigma = 256;
int n, sa[N], rk[N], tmp[N], cnt[N], lcp[N], st[LG][N], lg[N];

string s;
void buildSA() {
    n = (int)s.size();
    int m = sigma;
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
void buildSparse() {
    lg[1] = 0;
    for (int i = 2; i <= n; i++) lg[i] = lg[i/2] + 1;
    for (int i = 0; i < n; i++) st[0][i] = lcp[i];
    for (int j = 1; (1 << j) <= n; j++)
        for (int i = 0; i + (1 << j) <= n; i++)
            st[j][i] = min(st[j-1][i], st[j-1][i + (1 << (j-1))]);
}
// C++20
#include <bit>
int log2_floor(unsigned long i) {
    return std::bit_width(i) - 1;
}

// pre C++20
int log2_floor(unsigned long long i) {
    return i ? __builtin_clzll(1) - __builtin_clzll(i) : -1;
}
int lcpQuery(int i, int j) {
    if (i == j) return n - i;
    int l = rk[i], r = rk[j];
    if (l > r) swap(l, r);
    int len = r - l, k = lg[len];
    return min(st[k][l + 1], st[k][r - (1 << k) + 1]);
}
long long distinctSubstrings() {
    long long N = n, total = N * (N + 1) / 2;
    for (int i = 1; i < n; i++) total -= lcp[i];
    return total;
}
pair<int,int> longestRepeated() {
    int best = 0, pos = 0;
    for (int i = 1; i < n; i++)
        if (lcp[i] > best) { best = lcp[i]; pos = sa[i]; }
    return {best, pos};
}
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

int cmpSub(int a, int la, int b, int lb) {
    int common = lcpQuery(a, b);
    int mn = min(la, lb);
    if (common >= mn) return la == lb ? 0 : (la < lb ? -1 : 1);
    return s[a + common] < s[b + common] ? -1 : 1;
}
void solve() {
    
}

signed main() { fio int t=1; /*cin >> t;*/ while (t--) solve(); return 0; }









