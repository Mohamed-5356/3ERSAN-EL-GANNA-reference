bitset<N + 5>prime;
void sieve(long long N) {
  for(int i = 2; i < N; ++i) {
    prime[i] = 1;
  }
  for(long long i = 2; i * i <= N; ++i) {
    if (prime[i] == 0) {
      continue;
    }
    for(long long j = i * i; j <= N; j += i) {
      prime[j] = 0;
    }
  }
}