// Serewa style

#pragma GCC optimize("O3")

#include <algorithm>
#include <climits>
#include <cstdio>
#include <map>

using namespace std;

const int MAXK = 55;
const int MAXN = 35005;

struct Node {
    int st, l, r;
};

int cur = 1;
Node nodes[32 * MAXN];

int n, k;
int a[MAXN];
int root[MAXN], prev_i[MAXN], prev_x[MAXN];
int prev_ti[MAXN], prev_tx[MAXN];
int dp[MAXK][MAXN];
map<int, int> cache[MAXN]; 

inline int build(int l, int r)
{
    int t = cur++;
    if (l + 1 != r) {
        int m = (l + r) / 2;
        nodes[t].l = build(l, m);
        nodes[t].r = build(m, r);
    }
    return t;
}

inline int upd(int idx, int l, int r, int i)
{
    int t = cur++;
    nodes[t] = nodes[idx];
    if (l + 1 != r) {
        int m = (l + r) / 2;
        if (i < m) nodes[t].l = upd(nodes[t].l, l, m, i);
        else nodes[t].r = upd(nodes[t].r, m, r, i);
        nodes[t].st = nodes[nodes[t].l].st + nodes[nodes[t].r].st;
    }
    else nodes[t].st = 1;
    return t;
}

inline int query(int idx, int l, int r, int L, int R)
{
    if (l == L && r == R) return nodes[idx].st;
    int m = (l + r) / 2, res = 0;
    if (L < m) res += query(nodes[idx].l, l, m, L, min(m, R));
    if (R > m) res += query(nodes[idx].r, m, r, max(L, m), R);
    return res;
}

inline void build_pst()
{
    int t = 0;
    int cur_root = build(0, n);
    root[0] = cur_root;

    for (int i = 1; i <= n; i++) {
        prev_ti[i] = prev_tx[prev_i[i]];
        prev_tx[prev_i[i]] = i;
    }

    for (int i = 0; i <= n; i++) {
        int z = prev_tx[i];
        if (i > 0) root[i] = root[i-1];
        while (z) {
            cur_root = root[i] = upd(cur_root, 0, n + 1, z);
            z = prev_ti[z], t++;
        }
    }
}

inline int distinct(int l, int r)
{
    if (l > r) return 0;
    if (cache[l].count(r) == 0)
        cache[l][r] = query(root[l-1], 0, n + 1, l, r + 1);
    return cache[l][r];
}

inline void calc_state(int k, int l, int r, int opt_l, int opt_r)
{
    if (l > r) return;
    int m = (l + r) / 2, opt_m = -1;
    for (int t = opt_l; t <= min(m, opt_r); t++) {
        int val = dp[k-1][t] + distinct(t+1, m);
        if (val >= dp[k][m])
            dp[k][m] = val, opt_m = t;
    }
    if (l != r) {
        calc_state(k, l, m - 1, opt_l, opt_m);
        calc_state(k, m + 1, r, opt_m, opt_r);
    }
    //printf("%d %d => %d (opt between %d and %d, actual: %d)\n", k, m, dp[k][m], opt_l, opt_r, opt_m);
}

inline void calc_dp()
{
    for (int i = 1; i <= n; i++) {
        dp[1][i] = distinct(1, i);
    }
    for (int t = 2; t <= k; t++) {
        calc_state(t, 1, n, 0, n);
    }
}

int main()
{
    scanf("%d %d", &n, &k);
    for (int i = 1; i <= n; i++) {
        scanf("%d", a + i);
        prev_i[i] = prev_x[a[i]];
        prev_x[a[i]] = i;
    }

    build_pst();
    calc_dp();

    printf("%d\n", dp[k][n]);

    return 0;
}