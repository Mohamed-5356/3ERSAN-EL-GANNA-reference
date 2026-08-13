#include <bits/stdc++.h>
using namespace std;
using ll = long long;

// Pool size = 2 * Q * log2(N) with margin. For N=1e9, Q=2e5: ~12M nodes.
const int POOL = 12'000'000;
int ls[POOL], rs[POOL], tot = 0;
ll  sm[POOL];
// Node 0 is the null sentinel. ls[0]=rs[0]=sm[0]=0 by default (global arrays).

int new_node() { return ++tot; }

// Update: add val at position pos; create nodes as needed.
void update(int &u, int l, int r, int pos, ll val) {
    if (!u) u = new_node();         // create node on first touch
    sm[u] += val;
    if (l == r) return;
    int mid = (l + r) >> 1;        // NOT l + r >> 1 (precedence bug — always parenthesise)
    if (pos <= mid) update(ls[u], l, mid, pos, val);
    else             update(rs[u], mid+1, r, pos, val);
}

// Query: sum in [ql, qr].
ll query(int u, int l, int r, int ql, int qr) {
    if (!u) return 0;               // null node = 0 contribution (key invariant)
    if (ql <= l && r <= qr) return sm[u];
    int mid = (l + r) >> 1;
    ll res = 0;
    if (ql <= mid) res += query(ls[u], l, mid, ql, qr);
    if (qr >  mid) res += query(rs[u], mid+1, r, ql, qr);
    return res;
}