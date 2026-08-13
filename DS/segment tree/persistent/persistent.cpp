#include <bits/stdc++.h>
using namespace std;

const int POOL = 8'000'000; // (n + Q) * log2(V) with margin
int ls[POOL], rs[POOL], cnt[POOL], tot = 0;
// cnt[0] = 0 by default (global).

int new_node() { return ++tot; }

// Build new version by inserting value pos into prev version.
// Returns new root.
int update(int prev, int l, int r, int pos) {
    int u = new_node();
    ls[u] = ls[prev]; rs[u] = rs[prev]; cnt[u] = cnt[prev] + 1;
    if (l == r) return u;           // leaf: only cnt differs from prev
    int mid = (l + r) >> 1;
    if (pos <= mid) ls[u] = update(ls[prev], l, mid, pos);
    else             rs[u] = update(rs[prev], mid+1, r, pos);
    return u;                       // internal: copy sibling from prev, update path
}

// Query kth smallest in a[l..r] via roots T[l-1] and T[r].
// [u, v] = [T[l-1], T[r]]; walk difference tree.
int kth(int u, int v, int l, int r, int k) {
    if (l == r) return l;           // answer is this coordinate
    int mid = (l + r) >> 1;
    int left_cnt = cnt[ls[v]] - cnt[ls[u]];
    if (k <= left_cnt) return kth(ls[u], ls[v], l, mid, k);
    return                 kth(rs[u], rs[v], mid+1, r, k - left_cnt);
}

// Setup for MKTHNUM-style problem:
// int roots[MAXN];  roots[0] = 0;  (empty tree)
// Coordinate-compress values to [1, V].
// for (int i = 1; i <= n; i++)
//     roots[i] = update(roots[i-1], 1, V, compressed[a[i]]);
// Answer: decompress(kth(roots[l-1], roots[r], 1, V, k));
