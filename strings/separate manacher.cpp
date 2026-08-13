#include <bits/stdc++.h>
#define el '\n'
typedef long long ll;
#define fio ios_base::sync_with_stdio(0); cin.tie(0); cout.tie(0);
using namespace std;
#include <bits/stdc++.h>
using namespace std;
// Returns array d1 where d1[i] = maximum radius of odd-length palindrome centered at i.
// Radius is number of characters in the palindrome from center to one end (including center).
// Palindrome covers indices [i - d1[i] + 1, i + d1[i] - 1].
vector<int> manacher_odd(const string& s) {
    int n = s.size();
    vector<int> d1(n);
    for (int i = 0, l = 0, r = -1; i < n; ++i) {
        int k = (i > r) ? 1 : min(d1[l + r - i], r - i + 1);
        while (i - k >= 0 && i + k < n && s[i - k] == s[i + k]) ++k;
        d1[i] = k--;
        if (i + k > r) {
            l = i - k;
            r = i + k;
        }
    }
    return d1;
}

// Returns array d2 where d2[i] = maximum radius of even-length palindrome centered between i-1 and i.
// Radius is half-length (number of pairs). Palindrome covers indices [i - d2[i], i + d2[i] - 1].
vector<int> manacher_even(const string& s) {
    int n = s.size();
    vector<int> d2(n);
    for (int i = 0, l = 0, r = -1; i < n; ++i) {
        int k = (i > r) ? 0 : min(d2[l + r - i + 1], r - i + 1);
        while (i - k - 1 >= 0 && i + k < n && s[i - k - 1] == s[i + k]) ++k;
        d2[i] = k--;
        if (i + k > r) {
            l = i - k - 1;
            r = i + k;
        }
    }
    return d2;
}

// Returns two arrays: odd_end[i] = maximal end of an odd-length palindrome starting at i;
// even_end[i] = maximal end of an even-length palindrome starting at i (-1 if none).
pair<vector<int>, vector<int>> max_end_by_parity(const string& s) {
    int n = s.size();
    auto d1 = manacher_odd(s);
    auto d2 = manacher_even(s);

    vector<int> odd_end(n), even_end(n, -1);
    for (int i = 0; i < n; ++i) {
        odd_end[i] = i;                     // single char is an odd palindrome
        if (d1[i] > 0) {
            int L = i - d1[i] + 1;
            int R = i + d1[i] - 1;
            odd_end[L] = max(odd_end[L], R);
        }
        if (d2[i] > 0) {
            int L = i - d2[i];
            int R = i + d2[i] - 1;
            even_end[L] = max(even_end[L], R);
        }
    }

    // Propagate: a palindrome starting at i-1 and ending at E gives a palindrome starting at i ending at E-1.
    for (int i = 1; i < n; ++i) {
        odd_end[i] = max(odd_end[i], odd_end[i-1] - 1);
        if (even_end[i-1] != -1)
            even_end[i] = max(even_end[i], even_end[i-1] - 1);
    }

    return {odd_end, even_end};
}

// Returns max_end[i] = maximal end of any palindrome starting at i.
vector<int> max_end_start(const string& s) {
    auto [odd, even] = max_end_by_parity(s);
    vector<int> res(s.size());
    for (size_t i = 0; i < s.size(); ++i)
        res[i] = max(odd[i], even[i]);
    return res;
}
void solve() {

}
signed main() { fio  int t = 1; cin >> t; while (t--) solve(); return 0;}
/*
// Example usage
int main() {
    string s = "ababa";
    auto [odd_end, even_end] = max_end_by_parity(s);
    auto all_end = max_end_start(s);

    cout << "Odd max ends: ";
    for (int x : odd_end) cout << x << ' ';
    cout << "\nEven max ends: ";
    for (int x : even_end) cout << x << ' ';
    cout << "\nOverall max ends: ";
    for (int x : all_end) cout << x << ' ';
    cout << '\n';
    return 0;
}*/

