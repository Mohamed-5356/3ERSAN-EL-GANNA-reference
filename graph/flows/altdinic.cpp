
#include <bits/stdc++.h>
using namespace std;
typedef long long ll;

// ============================================================
// Dinic's Maximum Flow Algorithm
// Time: O(V^2 * E) general; O(E * sqrt(V)) for unit-capacity.
// ============================================================

struct Edge {
    int to;   // destination vertex
    int rev;  // index of reverse edge in graph[to]
    ll cap;   // remaining capacity (not original capacity)
};

struct Dinic {
    int n;                       // number of vertices
    vector<vector<Edge>> graph;  // adjacency list
    vector<int> level;           // BFS level of each vertex
    vector<int> ptr;             // current edge pointer for each vertex (the key optimization)

    Dinic(int n) : n(n), graph(n), level(n), ptr(n) {}

    // Add directed edge u->v with capacity cap.
    // Also adds reverse edge v->u with capacity 0 (residual bookkeeping).
    void add_edge(int u, int v, ll cap) {
        // graph[u].size() is the index where we're about to push the forward edge.
        // graph[v].size() is the index where the reverse edge will sit.
        graph[u].push_back({v, (int)graph[v].size(), cap});
        graph[v].push_back({u, (int)graph[u].size() - 1, 0LL});
        // Note: the reverse edge has capacity 0 initially.
        // When we push flow along u->v, we decrease graph[u][i].cap and
        // increase graph[v][rev].cap — that's how "un-routing" is enabled.
    }

    // For undirected edges: call add_edge(u, v, cap) AND add_edge(v, u, cap).
    // This creates TWO forward edges (each with capacity cap) and their reverses.
    // Be careful not to call this twice — it would double the capacity.
    void add_undirected_edge(int u, int v, ll cap) {
        graph[u].push_back({v, (int)graph[v].size(), cap});
        graph[v].push_back({u, (int)graph[u].size() - 1, cap}); // both directions get cap
    }

    // BFS to build the level graph (layered graph).
    // Returns true if t is reachable from s (i.e., augmenting paths still exist).
    bool bfs(int s, int t) {
        fill(level.begin(), level.end(), -1);
        queue<int> q;
        level[s] = 0;
        q.push(s);
        while (!q.empty()) {
            int u = q.front(); q.pop();
            for (auto& e : graph[u]) {
                // Only follow edges with remaining capacity, and only visit new vertices.
                if (e.cap > 0 && level[e.to] == -1) {
                    level[e.to] = level[u] + 1;
                    q.push(e.to);
                }
            }
        }
        return level[t] != -1; // t is reachable iff a path exists
    }

    // DFS to find and push a blocking flow in the level graph.
    // pushed: the maximum flow we can push from u to t (limited by bottleneck above u).
    // Returns the actual flow pushed.
    ll dfs(int u, int t, ll pushed) {
        if (u == t) return pushed; // reached sink, return flow amount

        // ptr[u] is the current edge index for u — the "current edge pointer" optimization.
        // We never revisit edges that lead to dead ends within this phase.
        for (int& cid = ptr[u]; cid < (int)graph[u].size(); cid++) {
            Edge& e = graph[u][cid];
            // Only traverse edges in the level graph: next level must be exactly level[u]+1.
            // Also, edge must have remaining capacity.
            if (e.cap <= 0 || level[e.to] != level[u] + 1) continue;

            ll d = dfs(e.to, t, min(pushed, e.cap));
            if (d > 0) {
                e.cap -= d;                    // consume capacity on forward edge
                graph[e.to][e.rev].cap += d;  // add capacity to reverse edge
                return d;                     // propagate flow upward
            }
            // If d == 0, the subtree from e.to is exhausted — ptr[u] auto-increments (cid++).
        }
        return 0; // no flow can be pushed from u
    }

    // Main Dinic's loop.
    ll max_flow(int s, int t) {
        ll flow = 0;
        while (bfs(s, t)) {               // Phase: build level graph
            fill(ptr.begin(), ptr.end(), 0); // Reset current edge pointers for this phase
            ll pushed;
            // Push blocking flow: keep finding augmenting paths in level graph until none remain.
            while ((pushed = dfs(s, t, LLONG_MAX)) > 0)
                flow += pushed;
        }
        return flow;
    }

    // After running max_flow, find the min cut.
    // Returns a list of edges (u, v) that are in the min cut (saturated AND cross the cut).
    // The S-side of the cut = all vertices reachable from s in the final residual graph.
    // The final BFS left level[] set for the last successful BFS; but we need a fresh reachability.
    vector<pair<int,int>> min_cut_edges(int s) {
        // Do one more BFS to find vertices reachable from s in residual.
        vector<bool> reachable(n, false);
        queue<int> q;
        q.push(s);
        reachable[s] = true;
        while (!q.empty()) {
            int u = q.front(); q.pop();
            for (auto& e : graph[u]) {
                if (e.cap > 0 && !reachable[e.to]) {
                    reachable[e.to] = true;
                    q.push(e.to);
                }
            }
        }
        vector<pair<int,int>> cut;
        for (int u = 0; u < n; u++) {
            if (!reachable[u]) continue;
            for (auto& e : graph[u]) {
                // Original edge u->v is in the min cut iff:
                // u is reachable (S-side), v is not (T-side), and the edge was originally present.
                // We detect "original" edges by checking if the reverse has cap > 0 (some flow used).
                // A cleaner way: check if graph[e.to][e.rev].cap > 0 (flow was sent).
                if (!reachable[e.to] && graph[e.to][e.rev].cap > 0)
                    cut.push_back({u, e.to});
            }
        }
        return cut;
    }
};

// ============================================================
// Main: reads n, m, s, t and edge list, prints max flow.
// ============================================================
int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n, m, s, t;
    cin >> n >> m >> s >> t;

    Dinic dinic(n);
    for (int i = 0; i < m; i++) {
        int u, v;
        ll cap;
        cin >> u >> v >> cap;
        dinic.add_edge(u, v, cap);
    }

    cout << dinic.max_flow(s, t) << "\n";

    // Optionally print min cut edges:
    // auto cut = dinic.min_cut_edges(s);
    // for (auto [u, v] : cut) cout << u << " -> " << v << "\n";

    return 0;
}