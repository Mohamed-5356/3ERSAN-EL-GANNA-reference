using namespace std;
const int LG = 22;
struct BinaryTrie {
    struct Node {
        int child[2] = {};
        int f = 0;
    };
    vector<Node> trie;
    BinaryTrie() { trie.emplace_back(); }
    void insert(int x) {
        int node = 0;
        for (int bit = LG; bit >= 0; bit--) {
            int val = (x >> bit) & 1;
            if (!trie[node].child[val]) {
                trie[node].child[val] = (int)trie.size();
                trie.emplace_back();
            }
            node = trie[node].child[val];
            trie[node].f++;
        }
    }
    //get max
    int query(int x) {
        int node = 0;
        int val = 0;
        for (int bit = LG; bit >= 0; bit--) {
            int b = (x >> bit) & 1;
            if (trie[node].child[b ^ 1]) {
                val |= (1 << bit);
                node = trie[node].child[b ^ 1];
            } else {
                node = trie[node].child[b];
            }
        }
        return val;
    }
};
void solve(){
    int n;
    cin >> n;
    vector<int> a(n);
    for (int i = 1; i < n; i++)
        cin >> a[i], a[i] ^= a[i-1];
    BinaryTrie t;
    for (int i = 0; i < n; i++)
        t.insert(a[i]);
    int ans = 0;
    for (int i = 0; i < n; i++) {
        if (t.query(a[i]) == (n-1)) {
            ans = a[i];
            break;
        }
    }
    for (auto & j : a)
        j ^= ans, cout << j <<  ' ';
    cout << el;
}