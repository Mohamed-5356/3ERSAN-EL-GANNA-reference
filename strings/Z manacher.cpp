#include <bits/stdc++.h>
typedef long long ll;
typedef double ld;
#define el '\n'
#define fio ios_base::sync_with_stdio(0); cin.tie(0); cout.tie(0);
using namespace std;
//int len   = p[i] - 1;
//int start = (i +1- p[i]) / 2;
//int end   = start + len - 1;
// d[2i] = 2dev[i]+1, d[2i+1] = 2dod[i]
vector<int> manacher(string s) {
    string t = "$#";
    for (auto &c : s) t += c,t += '#';
    t += '^';
    s = t;
    int n = (int)s.size() - 2;
    vector<int>p(n+2);
    for (int i = 1, l = 0, r = 1; i <= n; i++) {
        p[i] = max(0, min(r-i, p[l+r-i]));
        while (s[i-p[i]] == s[i+p[i]])p[i]++;
        if (i+p[i] > r) l = i - p[i], r = i + p[i];
    }
    return vector<int>(p.begin()+2, p.end()-2);
}
vector<int> zfun(string s) {
    int n = s.size();
    vector<int>z(n);
    for (int i = 1, l{}, r{}; i < n; i++) {
        z[i] = max(0, min(r-i, z[i-l]));
        while (i+z[i]<n && s[i+z[i]] == s[z[i]]) z[i]++;
        if (i+z[i] > r) l = i, r = i + z[i];
    }
    return z;
}
void solve() {

}

signed main() {
    fio
    int t = 1;
    cin >> t;
    while(t--)
        solve();
    return 0;
}
