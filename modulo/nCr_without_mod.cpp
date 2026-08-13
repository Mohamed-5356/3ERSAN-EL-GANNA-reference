int nCr[N][N] = {};
for(int i = 0; i < N; ++i)
{
  nCr[i][0] = 1;
  for(int j = 1; j <= i; ++j)
  {
    nCr[i][j] = nCr[i - 1][j - 1] + nCr[i - 1][j];
  }
}

int nPr[N][N] = {};
for(int i = 0; i < N; ++i)
{
  nPr[i][0] = 1;
  for(int j = 1; j <= i; ++j)
  {
    nPr[i][j] = j * nPr[i - 1][j - 1] + nPr[i - 1][j]; // overflow
  }
}
