#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
#define el '\n'
#define fio ios_base::sync_with_stdio(0); cin.tie(0); cout.tie(0);
const int N = 100123;
vector<int> cg[N], g[N];
int par[N], sz[N], n;
bool vis[N];
int getsz(int u, int p = -1) {
    if (vis[u]) return 0;
    sz[u] = 1;
    for (int v : g[u])
        if (v != p) sz[u] += getsz(v,u);
    return sz[u];
}
int getc(int u, int p, int n) {
    for (int v : g[u])
        if (v!=p&&!vis[v]&&sz[v]> n/2)
            return getc(v,u, n);
    return u;
}
vector<pair<int, int>> dist[N];
bool vis2[N];
void initc(int u = 0, int p = -1) {
    getsz(u);
    int c = getc(u,-1,sz[u]);
    queue<pair<int, int>> q; q.push({c, 0});
    vector<int> vc;
    vis[c] = 1;
    while (q.size()) {
        auto [u, lv] = q.front();
        q.pop();
        for (auto &v : g[u]) {
            if (!vis[v] && !vis2[v]) {
                q.push({v, lv+1});
                vis2[v] = 1;
                vc.push_back(v);
                dist[v].push_back({c, lv+1});
            }
        }
    }
    for (auto i : vc) vis2[i] = 0;
    vc.clear();
    par[c] = p;
    if (p >- 1)
        cg[p].push_back(c);
    for (int v : g[c])
        if (!vis[v])
            initc(v,c);
}
int ans[N];
void setnode(int u) {
    ans[u] = 0;
    for (auto &[c, dit] : dist[u])
        ans[c] = min(ans[c], dit);
}
int getans(int u) {
    int rt = ans[u];
    for (auto &[c, dit] : dist[u])
        rt = min(rt, ans[c]+dit);
    return rt;
}
int main() {
    fio
    int q;
    cin >> n >> q;
    for (int u,v,i = 1; i < n; i++) {
        cin >> u >> v; u--, v--;
        g[u].push_back(v);
        g[v].push_back(u);
    }
    initc();
    for (int i = 0; i < n; i++)
        ans[i] = 1e9;
    setnode(0);
    for (int tp, u; q--;) {
        cin >> tp >> u; u--;
        if (tp & 1)
            setnode(u);
        else cout << getans(u) << el;
    }
    return 0;
}