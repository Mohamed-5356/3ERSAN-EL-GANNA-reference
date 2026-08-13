//const int N = 200'123;
int a[N];
struct Node {
    int prfincrease{};
    int sufincrease{};
    int first{}, sz{}, last{};
    ll ans{};
};
Node create(int n) {
    Node r{};
    r.first = r.last = n;
    r.prfincrease = r.sufincrease  = r.ans = r.sz = 1;
    return r;
}
Node add(Node a, Node b) {
    Node r{};
    if (a.first < 0) return b;
    if (b.first < 0) return a;
    r.prfincrease = a.prfincrease;
    r.sufincrease = b.sufincrease;
    if (a.last <= b.first) {
        if (a.sufincrease == a.sz)
            r.prfincrease = a.sz + b.prfincrease;
        if (b.prfincrease == b.sz)
            r.sufincrease = b.sz + a.sufincrease;
    }
    r.first = a.first, r.sz = a.sz + b.sz, r.last = b.last,
    r.ans = a.ans + b.ans + (a.last <= b.first? a.sufincrease * 1ll * b.prfincrease : 0ll);
    return r;
}
// [L, R]
struct ST{
    Node t[N << 2];
    ST(){};//memset(t, 0, sizeof(t));}
    void build(int n, int b, int e){
        if(b == e) return void(t[n] = create(a[b]));//////////////////////
        int m = (b + e) >> 1, l = n << 1, r = l | 1;
        build(l, b, m);
        build(r, m + 1, e);
        t[n] = add(t[l], t[r]);
    }
    void upd(int n, int b, int e, int i){
        if(b > i || e < i) return;
        if(b == e) return void(t[n] = create(a[i]));
        int m = (b + e) >> 1, l = n << 1, r = l|1;
        upd(l, b, m, i);
        upd(r, m + 1, e, i);
        t[n] = add(t[l], t[r]);
    }
    Node query(int n, int b, int e, int i, int j){
        if(e < i || b > j) return create(-1);
        if(i <= b && e <= j) return t[n];
        int m = (b + e) >> 1, l = n << 1, r = l|1;
        Node L = query(l, b, m, i, j);
        Node R = query(r, m + 1, e, i, j);
        return add(L, R);
    }
}t;

void solve() {
    int n, q;
    cin >> n >> q;
    for (int i = 1; i <= n; i++)
        cin >> a[i];
    t.build(1, 1, n);
    for (int op, x, y, l, r; q--;) {
        cin >> op;
        if (op&1) {
            cin >>  x >> y;
            a[x] = y;
            t.upd(1,1,n, x);
        }
        else {
            cin >> l >> r;
            cout << t.query(1,1,n, l, r).ans << el;
        }
    }
}

//////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////
struct Node {
  int val = 0; //initial
};
struct SegmentTree {
  int n = 1;
  Node skip; // initialize Identity element
  vector<Node> tree;

  SegmentTree(vector<int>& v) { // be careful datatype
    while (n < (int)v.size()) n <<= 1;
    this->skip.val = 0; // 
    tree.resize(n << 1);
    build(0, 0, n, v);
  }
  SegmentTree(int _n) { // be careful datatype
    while (n < _n) n <<= 1;
    tree.resize(n << 1);
  }

  Node single(int x) {
    Node r;
    r.val = x;
    return r;
  }

  Node merge(Node l, Node r) {
    Node result;
    result.val = l.val + r.val;
    return result;
  }
  void build(int x, int lx, int rx, vector<int>& v) {
    if (rx - lx == 1) {
      if (lx < (int)v.size()) {
        tree[x] = single(v[lx]);
      } 
      else {
        tree[x] = skip;
      }
      return;
    }
    int m = (lx + rx) >> 1;
    build(2 * x + 1, lx, m, v);
    build(2 * x + 2, m, rx, v);
    tree[x] = merge(tree[2 * x + 1], tree[2 * x + 2]);
  }

  void set(int i, int v, int x, int lx, int rx) {
    if (i >= rx || i < lx) return;
    if (rx - lx == 1) {
      tree[x] = single(v);
      return;
    }
    int m = (lx + rx) >> 1;
    if (i < m) {
      set(i, v, 2 * x + 1, lx, m);
    }
    else {
      set(i, v, 2 * x + 2, m, rx);
    }
    tree[x] = merge(tree[2 * x + 1], tree[2 * x + 2]);
  }

  Node query(int l, int r, int x, int lx, int rx) {
    if (lx >= l && rx <= r) {
      return tree[x];
    }
    if (lx >= r || rx <= l) {
      return skip;
    }
    int m = (lx + rx) >> 1;
    return merge(query(l, r, 2 * x + 1, lx, m), query(l, r, 2 * x + 2, m, rx));
  }

  //0-indexed half-open query [l, r)
  void set(int i, int v) {
    set(i, v, 0, 0, n);
  }

  Node query(int l, int r) {
    return query(l, r, 0, 0, n);
  }
};