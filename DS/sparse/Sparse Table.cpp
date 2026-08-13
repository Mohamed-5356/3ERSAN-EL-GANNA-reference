
#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
typedef long double ld;
#define el '\n'
#define fio ios_base::sync_with_stdio(0); cin.tie(0); cout.tie(0);
const int N = 2e5+9;
ll t[18][N], a[N];
// n ? 
void build(int n) {
    for (int i = 1; i < n; i++) t[0][i] = a[i];
    for (int k = 1; k < 18; k++) {
        for (int i = 1; i + (1 << k)-1<=n; i++) {
            t[k][i] = gcd(t[k-1][i], t[k-1][i + (1 << (k-1))]);
        }
    }
}
ll query(int l, int r) {
    int k = 31 - __builtin_clz(r-l+1);
    return gcd(t[k][l], t[k][r-(1<<k)+1]);
}
void solve() {
    int n;
    cin >> n;
    vector<ll> v(n);
    for (auto &i : v)
        cin >> i;
    for (int i = 1; i < n; i++)
        a[i] = v[i] - v[i-1];
    int out = 0;
    build(n);
    for (int i = 1; i < n; i++) {
        int l = i, r = n - 1, md, ans{};
        while (l <= r) {
            md = l + r >> 1;
            if (query(i, md) ^ 1) {
                l = md + 1;
                ans = md-i+1;
            }
            else r = md - 1;
        }
        out = max(out, ans);
    }
    cout << out+1 << el;
}