#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
const int N = 2e5 + 9, INF = 1e9;
#define el '\n'
// [0, n-1])
struct SegTree {
    int t[4*N], lz[4*N];
    void push(int v) {
        if (!lz[v]) return;
        for (int c : {v<<1, v<<1|1}) { t[c] += lz[v]; lz[c] += lz[v]; }
        lz[v] = 0;
    }
    void build(int v, int l, int r, int* a) {
        lz[v] = 0;
        if (l == r) { t[v] = a[l]; return; }
        int m = (l+r)>>1;
        build(v<<1, l, m, a); build(v<<1|1, m+1, r, a);
        t[v] = max(t[v<<1], t[v<<1|1]);
    }
    void upd(int v, int l, int r, int ql, int qr, int x) {
        if (qr < l || r < ql) return;
        if (ql <= l && r <= qr) { t[v] += x; lz[v] += x; return; }
        push(v); int m = (l+r)>>1;
        upd(v<<1, l, m, ql, qr, x); upd(v<<1|1, m+1, r, ql, qr, x);
        t[v] = max(t[v<<1], t[v<<1|1]);
    }
    int query(int v, int l, int r, int ql, int qr) {
        if (qr < l || r < ql) return -INF;
        if (ql <= l && r <= qr) return t[v];
        push(v); int m = (l+r)>>1;
        return max(query(v<<1, l, m, ql, qr), query(v<<1|1, m+1, r, ql, qr));
    }
} seg;

int n, dat[N];
int eu[N], ev[N], ew[N];                 // edge i: u,v,w
vector<int> g[N];
int par[N], dep[N], sz[N], hd[N], st[N], en[N], T;
void dfs(int u, int p = 0) {
    par[u] = p; dep[u] = dep[p]+1; sz[u] = 1;
    if (p) g[u].erase(find(g[u].begin(), g[u].end(), p));
    for (int& v : g[u]) {
        dfs(v, u); sz[u] += sz[v];
        if (sz[v] > sz[g[u][0]]) swap(v, g[u][0]);
    }
}
void dfs_hld(int u) {
    st[u] = T++;
    for (int v : g[u]) {
        hd[v] = (v == g[u][0] ? hd[u] : v);
        dfs_hld(v);
    }
    en[u] = T-1;
}

// Must be called AFTER dfs (par[] is needed).
int echild(int i) { return par[eu[i]] == ev[i] ? eu[i] : ev[i]; }

void path_upd(int u, int v, int x) {
    for (; hd[u] != hd[v]; u = par[hd[u]]) {
        if (dep[hd[u]] < dep[hd[v]]) swap(u, v);
        seg.upd(1, 0, n-1, st[hd[u]], st[u], x);
    }
    if (dep[u] > dep[v]) swap(u, v);
    if (u != v) seg.upd(1, 0, n-1, st[u]+1, st[v], x);  // +1: skip LCA slot
}
int path_query(int u, int v) {
    int res = -INF;
    for (; hd[u] != hd[v]; u = par[hd[u]]) {
        if (dep[hd[u]] < dep[hd[v]]) swap(u, v);
        res = max(res, seg.query(1, 0, n-1, st[hd[u]], st[u]));
    }
    if (dep[u] > dep[v]) swap(u, v);
    if (u != v) res = max(res, seg.query(1, 0, n-1, st[u]+1, st[v]));
    return res;
}
// Point-set edge i to new weight x.
// With lazy (range-add) segtree: add the delta; track current weight in ew[].
// If you strip lazy → replace with a direct leaf-set traversal.
void edge_set(int i, int x) {
    seg.upd(1, 0, n-1, st[echild(i)], st[echild(i)], x - ew[i]);
    ew[i] = x;
}

void solve() {
    cin >> n;
    fill(dat, dat+n, -INF);              // root slot = identity; never queried
    for (int i = 1; i < n; i++) {
        cin >> eu[i] >> ev[i] >> ew[i];
        g[eu[i]].push_back(ev[i]);
        g[ev[i]].push_back(eu[i]);
    }
    dfs(1); hd[1] = 1; dfs_hld(1);
    for (int i = 1; i < n; i++) dat[st[echild(i)]] = ew[i];
    seg.build(1, 0, n-1, dat);

    // point set:  edge_set(i, new_weight)
    // path max:   path_query(u, v)
    // path add:   path_upd(u, v, x)

    T = 0;
    for (int i = 0; i <= n; i++) g[i].clear();
}
int main() {
    ios_base::sync_with_stdio(0); cin.tie(0);
    int tc = 1; // cin >> tc;
    while (tc--) solve();
}