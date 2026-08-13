#include <bits/stdc++.h>
typedef long long ll;
typedef double ld;
#define el '\n'
#define fio ios_base::sync_with_stdio(0); cin.tie(0); cout.tie(0);
using namespace std;
const int N = 100123;
ll b0[N], b1[N], n, q;
void add(ll b[], ll i, ll x) {
    for (; i <= n; i += (i&-i))
        b[i] += x;
}
ll sum(ll b[], ll i) {
    ll r{};
    for (; i; i -= (i&-i))
        r += b[i];
    return r;
}
void add(ll l, ll r, ll x) {
    add(b0, l, x);
    add(b0, r+1, -x);
    add(b1, l, x*(l-1));
    add(b1, r+1, -x*r);
}
ll prf(ll i) {
    return sum(b0, i) * i - sum(b1, i);
}
ll sum(ll l, ll r) {
    return prf(r) - prf(l - 1);
}
void solve() {
    memset(b0, 0, sizeof(b0));
    memset(b1, 0, sizeof(b1));
    cin >> n >> q;
    for (ll op, l, r, x; q--;) {
        cin >> op >> l >> r;
        if (op == 0) {
            cin >> x;
            add(l, r, x);
        }
        else {
            cout << sum(l, r) << el;
        }
    }
}

signed main() { fio int t = 1; cin >> t; while (t--) solve(); return 0; }