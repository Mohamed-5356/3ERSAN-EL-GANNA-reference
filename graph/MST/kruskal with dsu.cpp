#include <bits/stdc++.h>
typedef long long ll;
#define el '\n'
#define fio ios_base::sync_with_stdio(0); cin.tie(0); cout.tie(0);
using namespace std;
const int N = 3012;
int parent[N], r[N];
void make_set(int v) { parent[v] = v; r[v] = 0; }

int find_set(int v) {
    if (v == parent[v]) return v;
    return parent[v] = find_set(parent[v]);
}

void union_sets(int a, int b) {
    a = find_set(a);
    b = find_set(b);
    if (a != b) {
        if (r[a] < r[b])
            swap(a, b);
        parent[b] = a;
        if (r[a] == r[b])
            r[a]++;
    }
}
int weight[N][N], res[N][N];
void solve() {
    int n;
    cin >> n;
    for (int i = 0; i <= n; i++)
        make_set(i);
    vector<tuple<int, int, int>> e;
    for (int i = 1; i < n; i++) {
        for (int x, j = i+1; j <= n; j++) {
            cin >> weight[i][j];
            e.push_back({weight[i][j], i, j});
            weight[j][i] = weight[i][j];
        }
    }
    sort(e.begin(), e.end());
    vector<pair<int, int>> g[n+1];
    for (auto [w, i, j] : e) {
        if (find_set(i) != find_set(j)) {
            g[i].push_back({w, j});
            g[j].push_back({w, i});
            union_sets(i, j);
        }
    }
    for (int i = 1; i <= n; i++) {
        auto &c = res[i];
        queue<int> q;
        q.push(i);
        while (q.size()) {
            auto u = q.front();
            q.pop();
            for (auto &[cost, v] : g[u]) {
                if (c[v] == 0 && v != i)
                    q.push(v), c[v] = c[u] + cost;
            }
        }
    }
    bool vld = 1;
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= n; j++) {
            vld &= res[i][j] == weight[i][j];
        }
    }
    cout << (vld? "Yes\n" : "No\n");
}
signed main(){ fio int t = 1; /*cin >> t;*/ while(t--) solve(); return 0; }