const int N = 2e6+10, SQ = 145;
ll n, q, ans = 0, k;
int a[N], frq[N];
struct query {
    int l, r, i;
    bool operator<(query q) {
        int b1 = l / SQ, b2 = q.l / SQ;
        if (b1 != b2) return b1 < b2;
        return (b1 & 1)? (r < q.r): (r > q.r);
    }
};
void add(int idx) {
    ans += frq[a[idx]^k];
    frq[a[idx]]++;
}
void rem(int idx) {
    frq[a[idx]]--;
    ans -= frq[a[idx]^k];
}
ll out[N];
void MO(vector<query> &qq) {
    int l = 0, r = -1;
    sort(qq.begin(), qq.end());
    for (auto &[ql, qr, qi] : qq) {
        while (r < qr) add(++r);
        while (l > ql) add(--l);
        while (r > qr) rem(r--);
        while (l < ql) rem(l++);
        out[qi] = ans;
    }
}
void solve() {
    cin >> n >> q >> k;
    for (int i = 1; i <= n; i++)
        cin >> a[i], a[i] ^= a[i-1];
    vector<query> qry(q);
    int idx{};
    for (auto &[ql, qr, qi] : qry)
        cin >> ql >> qr, qi = idx++,ql--;
    MO(qry);
    for (int i = 0; i < q; i++)
        cout << out[i] << el;
}