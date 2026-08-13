// =========================================================================
// TARJAN — one file
//
// DIRECTED   : SCC + condensation DAG
//              add_dir(u, v) → tarjan(i) for each unvisited i → build_dag(n)
//              outputs: comp_id[], comp[], compsz[], dag[], id
//                       comp ids are in REVERSE topological order
//
// UNDIRECTED : bridges + articulation + bridge tree
//              add_und(u, v) → dfs_und(i, -1) for each unvisited i → build_bridge_tree(n)
//              outputs: is_bridge[edge_id], is_art[v], bcc_id[v], btree[c]
//
// SHARED     : g[], dfs_num[], low_link[], dfn, n_edges
// =========================================================================

#include <bits/stdc++.h>
#define el '\n'
#define ll long long
#define ld long double
#define ToshToshTroshToshTosh ios_base::sync_with_stdio(0); cin.tie(0); cout.tie(0);
using namespace std;

const int N = 200000 + 10;
const int M = 200000 + 10;

// ---- shared scaffolding -------------------------------------------------
int dfs_num[N], low_link[N], dfn = 0;
vector<pair<int,int>> g[N];   // (neighbor, edge_id); edge_id paired across both halves of an undirected edge
int n_edges = 0;

void add_dir(int u, int v) {
    g[u].push_back({v, n_edges++});
}
void add_und(int u, int v) {
    g[u].push_back({v, n_edges});
    g[v].push_back({u, n_edges});
    n_edges++;
}

// =========================================================================
// SCC (directed) — Tarjan + condensation
// =========================================================================

int comp_id[N], compsz[N], id = 0;
bool in_stack[N];
stack<int> st;
vector<int> comp[N], dag[N];

void tarjan(int u) {
    dfs_num[u] = low_link[u] = ++dfn;
    st.push(u);
    in_stack[u] = 1;
    for (auto &[v, e] : g[u]) {
        if (dfs_num[v] == 0) {
            tarjan(v);
            low_link[u] = min(low_link[u], low_link[v]);
        } else if (in_stack[v]) {
            low_link[u] = min(low_link[u], dfs_num[v]);
        }
    }
    if (dfs_num[u] == low_link[u]) {
        while (true) {
            int x = st.top(); st.pop();
            in_stack[x] = false;
            comp_id[x] = id;
            comp[id].push_back(x);
            compsz[id]++;
            if (x == u) break;
        }
        id++;
    }
}

// Iterative Tarjan — escape hatch for n ≥ 2·10⁵ with chain-shape risk.
// Drop-in replacement for tarjan(i): call tarjan_iter(i) instead.
void tarjan_iter(int s) {
    stack<pair<int,int>> call;       // (u, next-neighbor-index into g[u])
    auto enter = [&](int u) {
        dfs_num[u] = low_link[u] = ++dfn;
        st.push(u); in_stack[u] = 1;
        call.push({u, 0});
    };
    enter(s);
    while (!call.empty()) {
        auto &[u, i] = call.top();   // reference: i must persist across iterations
        if (i < (int)g[u].size()) {
            auto [v, e] = g[u][i++];
            if (dfs_num[v] == 0) enter(v);
            else if (in_stack[v]) low_link[u] = min(low_link[u], dfs_num[v]);
        } else {
            int ud = u;
            call.pop();
            if (!call.empty()) {
                int p = call.top().first;
                low_link[p] = min(low_link[p], low_link[ud]);
            }
            if (dfs_num[ud] == low_link[ud]) {
                while (true) {
                    int x = st.top(); st.pop();
                    in_stack[x] = false;
                    comp_id[x] = id;
                    comp[id].push_back(x);
                    compsz[id]++;
                    if (x == ud) break;
                }
                id++;
            }
        }
    }
}

void build_dag(int n) {
    for (int u = 1; u <= n; u++)
        for (auto &[v, e] : g[u])
            if (comp_id[u] != comp_id[v])
                dag[comp_id[u]].push_back(comp_id[v]);
    for (int c = 0; c < id; c++) {
        sort(dag[c].begin(), dag[c].end());
        dag[c].erase(unique(dag[c].begin(), dag[c].end()), dag[c].end());
    }
}

// =========================================================================
// Undirected — bridges, articulation, bridge tree
// =========================================================================

bool is_art[N], is_bridge[M];
int n_bridges = 0;

void dfs_und(int u, int pe) {        // pe = entering edge id; -1 for root
    dfs_num[u] = low_link[u] = ++dfn;
    int children = 0;
    for (auto &[v, e] : g[u]) {
        if (e == pe) continue;       // skip parent EDGE (multi-edge safe)
        if (dfs_num[v] == 0) {
            children++;
            dfs_und(v, e);
            low_link[u] = min(low_link[u], low_link[v]);
            if (low_link[v] > dfs_num[u]) {       // STRICT → bridge
                is_bridge[e] = 1;
                n_bridges++;
            }
            if (pe != -1 && low_link[v] >= dfs_num[u])  // NON-STRICT → articulation
                is_art[u] = 1;
        } else {
            low_link[u] = min(low_link[u], dfs_num[v]);
        }
    }
    if (pe == -1 && children > 1)    // root special case
        is_art[u] = 1;
}

int bcc_id[N], n_bcc = 0;
vector<int> btree[N];

void color_bcc(int u, int c) {
    bcc_id[u] = c;
    for (auto &[v, e] : g[u]) {
        if (bcc_id[v] || is_bridge[e]) continue;
        color_bcc(v, c);
    }
}

void build_bridge_tree(int n) {
    for (int i = 1; i <= n; i++)
        if (!bcc_id[i])
            color_bcc(i, ++n_bcc);
    for (int u = 1; u <= n; u++)
        for (auto &[v, e] : g[u])
            if (is_bridge[e])
                btree[bcc_id[u]].push_back(bcc_id[v]);
    // Each bridge is traversed once from each endpoint → every tree edge ends up
    // exactly once in each adjacency list. Add sort+unique only if input may
    // contain parallel bridges between the same component pair.
}

// =========================================================================
// Driver — pick ONE block
// =========================================================================

int main() {
    ToshToshTroshToshTosh
    int n, m; cin >> n >> m;

    // ---- Directed (SCC) -------------------------------------------------
    // for (int u, v, i = 0; i < m; i++) { cin >> u >> v; add_dir(u, v); }
    // for (int i = 1; i <= n; i++) if (!dfs_num[i]) tarjan(i);     // or tarjan_iter(i)
    // build_dag(n);

    // ---- Undirected (bridges / articulation / bridge tree) --------------
    for (int u, v, i = 0; i < m; i++) { cin >> u >> v; add_und(u, v); }
    for (int i = 1; i <= n; i++) if (!dfs_num[i]) dfs_und(i, -1);
    build_bridge_tree(n);

    return 0;
}
