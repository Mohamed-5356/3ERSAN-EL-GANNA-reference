#define mat vector<vector<ll>>
#define zero(n,m) mat(n , vector<ll>(m,0))
const int mod = /*1e9+7*/ /*119<<23|1*/;
int add(int a, int b){ return (a+=b) < mod? a : a - mod; }
int subt(int a, int b){ return (a-=b) < 0? a + mod : a; }
int mult(int a, int b){ return 1ll*a*b%mod;}
mat I(int n){
  mat r = zero(n, n);
  for(int i = 0; i < n; i++) r[i][i] = 1;
  return r;
}
mat mult(mat &a , mat &b){
  int n = a.size() , m = b[0].size();
  mat res = zero(n , m);
  for(int i = 0; i < n; i++) 
  for(int j = 0; j < m; j++) 
  for(int k = 0; k < n; k++) 
    res[i][j] = add(res[i][j], mult(a[i][k], b[k][j]));
  return res;
}
mat fp(mat &b , ll e){
  mat r = I(b.size());
  for(; e; b = mult(b, b), e >>= 1)
    if(e & 1) r = mult(r, b);
  return r;
}
void init(mat &T, mat &F, vector<int> &b, vector<int>&c){
  int k = b.size();
  for(int i = 0; i < k; i++)
    F[i][0] = b[k-i-1], T[0][i] = c[i];
  for(int i = 1; i < k; i++)
    T[i][i-1] = 1;
}
void solve() { 
  int k, n;
  cin >> k;
  vector<int> b(k), c(k);
  for(auto &i : b) cin >> i;
  for(auto &i : c) cin >> i;
  cin >> n;
  if(n <= k) return void(cout << b[n-1] << el);
  n -= k;
  mat T = zero(k, k), F = zero(k, 1);
  init(T, F, b, c);
  T = fp(T, n);
  F = mult(T, F);
  cout << F[0][0] << el;
}
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
#define row vector<long long>
#define matrix vector<row>
#define zero(n,m) vector<row>(n, row(m))
matrix mult(matrix& a, matrix& b) // a is a square matrix
{
  int n = a.size(), m = b[0].size();
  matrix result = zero(n, m);
  for(int i = 0; i < n; ++i)
  {
    for(int j = 0; j < m; ++j)
    {
      for(int k = 0; k < n; ++k)
      {
        result[i][j] += a[i][k] * b[k][j];
        result[i][j] %= mod;
      }
    }
  }
  return result;
}
matrix fp(matrix a, long long p)
{
  int n = a.size();
  matrix result = matrix(n, row(n));
  for(int i = 0; i < n; ++i) result[i][i] = 1;
  while(p > 0)
  {
    if (p & 1) result = mult(result, a);
    a = mult(a, a);
    p /= 2;
  }
  return result;
}
/*
F = mult(T, F)
*/