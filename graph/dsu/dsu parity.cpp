void make_set(int v) {
    parent[v] = make_pair(v, 0);
    rank[v] = 0;
    bipartite[v] = true;
}

pair<int, int> find_set(int v) {
    if (v != parent[v].first) {
        int parity = parent[v].second;
        parent[v] = find_set(parent[v].first);
        parent[v].second ^= parity;
    }
    return parent[v];
}

void add_edge(int a, int b) {
    pair<int, int> pa = find_set(a);
    a = pa.first;
    int x = pa.second;

    pair<int, int> pb = find_set(b);
    b = pb.first;
    int y = pb.second;

    if (a == b) {
        if (x == y)
            bipartite[a] = false;
    } else {
        if (rank[a] < rank[b])
            swap (a, b);
        parent[b] = make_pair(a, x^y^1);
        bipartite[a] &= bipartite[b];
        if (rank[a] == rank[b])
            ++rank[a];
    }
}

bool is_bipartite(int v) {
    return bipartite[find_set(v).first];
}
/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
struct DSU {
  public:
    DSU() : n(0) {}
    DSU(int _n) : n(_n), parent_or_size(_n, -1) {}
  
    int merge(int a, int b) {
      int x = leader(a);
      int y = leader(b);
      if (x == y) return x;
      
      if (-parent_or_size[x] < -parent_or_size[y]) std::swap(x, y);
      parent_or_size[x] += parent_or_size[y];
      parent_or_size[y] = x;
      return x;
    }
  
    bool same(int a, int b) {
      return leader(a) == leader(b);
    }
  
    int leader(int a) {
      if (parent_or_size[a] < 0) return a;
      return parent_or_size[a] = leader(parent_or_size[a]);
    }
  
    int size(int a) {
      return -parent_or_size[leader(a)];
    }
  
    vector<vector<int>> groups() {
      vector<int> leader_buf(n), group_size(n);
      for (int i = 0; i < n; i++) {
        leader_buf[i] = leader(i);
        group_size[leader_buf[i]]++;
      }
      vector<vector<int>> result(n);
      for (int i = 0; i < n; i++) {
        result[i].reserve(group_size[i]);
      }
      for (int i = 0; i < n; i++) {
        result[leader_buf[i]].push_back(i);
      }
      result.erase(
          remove_if(result.begin(), result.end(),
          [&](const vector<int>& v) { return v.empty(); }),
          result.end());
      return result;
    }
  
  private:
    int n;
    std::vector<int> parent_or_size; // leader with negative sign size , parent with positve sign
};