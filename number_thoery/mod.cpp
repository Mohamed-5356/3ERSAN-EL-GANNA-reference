#include <bits/stdc++.h>
typedef long long ll;
typedef long double ld;
#define el '\n'
#define fio ios_base::sync_with_stdio(0); cin.tie(0); cout.tie(0);
using namespace std;
const int N = 2'000'123, mod = //1'000'000'007;
int fac[N], ifac[N], n;
int add(int  a, int b){ return (a += b) < mod? a : a - mod; }
int subt(int a, int b){ return (a -= b) < 0? a + mod : a; }
int mult(int a, int b){ return 1ll*a*b%mod; }
int fp(int b, int e){
  int r = 1;
  for(; e; b = mult(b, b), e >>= 1)
    if(e&1) r = mult(r, b); 
  return r;
}
int divi(int a, int b){ return mult(a, fp(b, mod-2)); }
int ncr(int n, int r){
  if(n < r || r < 0) return 0;
  return mult(fac[n], mult(ifac[n - r], ifac[r]));
}

void solve(){
}

int main(){ 
  fio 
  ifac[0] = fac[0] = 1;
  for(int i = 1; i < N; i++)
    fac[i] = mult(fac[i-1], i);
  ifac[N-1] = fp(fac[N-1], mod-2);
  for(int i = N-2; i > 0; i--)
    ifac[i] = mult(ifac[i+1], i+1);
  int t = 1; /*cin >> t;*/ while(t--) solve(); return 0; }











