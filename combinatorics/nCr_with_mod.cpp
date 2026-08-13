#include <bits/stdc++.h>
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
int main(){ 
  ifac[0] = fac[0] = 1;
  for(int i = 1; i < N; i++)
    fac[i] = mult(fac[i-1], i);
  ifac[N-1] = fp(fac[N-1], mod-2);
  for(int i = N-2; i > 0; i--)
    ifac[i] = mult(ifac[i+1], i+1);
}
/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////

int factorial[n + 1] = {}, factorial_inverse[n + 1] = {};
int mult(long long a, int b) { // be careful a , b < mod
  return a * b % mod;
}
int fp(int b, int p) {
  int result = 1;
  while (p > 0)
  {
    if (p & 1) {
      result = mult(result, b);
    }
    b = mult(b, b);
    p /= 2;
  }
  return result;
}
void init(int n) {
  factorial[0] = 1;
  for(int i = 1; i <= n; ++i) {
    factorial[i] = mult(factorial[i - 1], i);
  }
  factorial_inverse[n] = fp(factorial[n], mod - 2);
  for(int i = n - 1; i >= 0; --i) {
    factorial_inverse[i] = mult(factorial_inverse[i + 1], (i + 1));
  }
}
int nCr(int n, int r) {
  if (r < 0 || r > n) return 0;
  return mult(mult(factorial[n], factorial_inverse[r]), factorial_inverse[n - r]);
}