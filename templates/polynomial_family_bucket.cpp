#include <bits/stdc++.h>
#ifdef ALGO
#include "el_psy_congroo.hpp"
#else
#define DUMP(...) 1145141919810
#define CHECK(...) (__VA_ARGS__)
#endif

template<int MOD>
struct Integral {
  int v_ = 0;

  template<typename T> Integral(T v) : v_(norm(v)) {  // Implicit conversion is allowed.
    static_assert(std::is_integral<T>::value, "input should be an integral.");
  }
  Integral() = default;
  ~Integral() = default;

  template<typename T> T norm(T v) const {
    if constexpr(std::is_same<long long, T>::value) {
      v %= MOD;
      if (v < 0) v += MOD;
    } else {
      if (v >= MOD) v -= MOD;
      if (v < 0) v += MOD;
      if (v >= MOD || v < 0) {
        v %= MOD;
        if (v < 0) v += MOD;
      }
    }
    return v;
  }

  int val() const { return v_; }
  Integral& operator += (const Integral& rhs) { v_ += rhs.val(); if (v_ >= MOD) v_ -= MOD; return *this; }
  Integral& operator -= (const Integral& rhs) { v_ += MOD - rhs.val(); if (v_ >= MOD) v_ -= MOD; return *this; }
  Integral& operator *= (const Integral& rhs) { v_ = v_ * 1LL * rhs.val() % MOD; return *this; }
  Integral& operator /= (const Integral& rhs) { v_ = v_ * 1LL * rhs.inv().val() % MOD; return *this; }
  Integral operator + (const Integral& rhs) const { Integral ret = *this; return ret += rhs; }
  Integral operator - (const Integral& rhs) const { Integral ret = *this; return ret -= rhs; }
  Integral operator * (const Integral& rhs) const { Integral ret = *this; return ret *= rhs; }
  Integral operator / (const Integral& rhs) const { Integral ret = *this; return ret /= rhs; }
  bool operator == (const Integral& rhs) const { return val() == rhs.val(); }
  bool operator != (const Integral& rhs) const { return !(*this == rhs); }
  const Integral operator - () const { return Integral(-val()); }
  const Integral& operator ++ () { v_ += 1; if (v_ >= MOD) v_ -= MOD; return *this; }
  const Integral operator ++ (int) { Integral ret = *this; ++(*this); return ret; }
  const Integral& operator -- () { v_ += MOD - 1; if (v_ >= MOD) v_ -= MOD; return *this; }
  const Integral operator -- (int) { Integral ret = *this; --(*this); return ret; }

  Integral power(long long b) const {
    long long ret = 1 % MOD, a = v_;
    for ( ; b; b >>= 1, a = a * a % MOD) if (b & 1) ret = ret * a % MOD; return ret;
  }
  Integral inv() const { return power(MOD - 2); }
};

template<int MOD>
std::string to_string(const Integral<MOD>& v) {
  return std::string("{") + std::to_string(v.val()) + "}";
}

template<int MOD, bool kAllowBruteForce = false>
struct Binomial {
  std::vector<Integral<MOD>> factor, inv_factor;

  explicit Binomial(int n = 0) : factor(n + 1), inv_factor(n + 1) {
    factor[0] = 1;
    for (int i = 1; i <= n; ++i) factor[i] = factor[i - 1] * i;
    inv_factor[n] = factor[n].inv();
    for (int i = n; i >= 1; --i) inv_factor[i - 1] = inv_factor[i] * i;
  }
  ~Binomial() = default;

  template<typename T>
  Integral<MOD> operator () (T a, T b) const {
    if (a < b || b < 0) return 0;
    if (a < factor.size()) return factor[a] * inv_factor[b] * inv_factor[a - b];
    if constexpr(!kAllowBruteForce) {
      throw std::out_of_range("Binomial");
    } else {
      b = std::min(b, a - b);
      Integral<MOD> ret = 1;
      for (T i = 1; i <= b; ++i) ret = ret * (a + 1 - i) / i;
      return ret;
    }
  }
};

template<int MOD>
struct PowerTable : public std::vector<Integral<MOD>> {
  PowerTable(int n, const Integral<MOD>& g) {
    static_assert(sizeof(PowerTable) == sizeof(std::vector<Integral<MOD>>), "");
    this->resize(n + 1);
    this->at(0) = 1;
    this->at(1) = g;
    for (int i = 2; i < this->size(); ++i) this->at(i) = this->at(i - 1) * this->at(1);
  }
};

const int MOD = 998244353;
using Mint = Integral<MOD>;
using Binom = Binomial<MOD>;

// Binom binom(200000);
// PowerTable<MOD> pw2(200000, 2);

template<int MOD = 998244353, int kPrimRoot = 3>
void ntt(Integral<MOD> A[], int n, int inv) {
  // inv == 1: ntt, == -1: intt

  // MOD == a * b ^ k + 1, n <= b ^ k.
  // 998244353 == (7 * 17) * 2 ^ 23 + 1.
  // This code works only when b == 2.
  Integral<MOD> w = 1, d = Integral<MOD>(kPrimRoot).power((MOD - 1) / n), t;
  int i, j, c, s;
  if (inv == -1) {
    for (i = 1, j = n - 1; i < j; ++i, --j) std::swap(A[i], A[j]);
    for (t = Integral<MOD>(n).inv(), i = 0; i < n; ++i) A[i] = A[i] * t;
  }
  for (s = n >> 1; s; s >>= 1, w = 1, d = d * d) {
    for (c = 0; c < s; ++c, w = w * d) {
      for (i = c; i < n; i += s << 1) {
        A[i | s] = (A[i] - (t = A[i | s])) * w;
        A[i] += t;
      }
    }
  }
  for (i = 1; i < n; ++i) {
    for (j = 0, s = i, c = n >> 1; c; c >>= 1, s >>= 1) j = j << 1 | (s & 1);
    if (i < j) std::swap(A[i], A[j]);
  }
}

template<typename T>
using Polynomial = std::vector<T>;

template<typename T>
void norm(Polynomial<T>& p) {
  while (p.size() > 1 && p.back() == T(0)) p.pop_back();
}

template<typename T>
Polynomial<T> operator + (const Polynomial<T>& lhs, const Polynomial<T>& rhs) {
  Polynomial<T> ret = lhs;
  ret.resize(std::max(lhs.size(), rhs.size()), T(0));
  for (int i = 0; i < rhs.size(); ++i) ret[i] += rhs[i];
  norm(ret);
  return ret;
}

template<typename T>
Polynomial<T> operator - (const Polynomial<T>& lhs, const Polynomial<T>& rhs) {
  Polynomial<T> ret = lhs;
  ret.resize(std::max(lhs.size(), rhs.size()), T(0));
  for (int i = 0; i < rhs.size(); ++i) ret[i] -= rhs[i];
  norm(ret);
  return ret;
}

template<typename T>
Polynomial<T> mod_len(Polynomial<T> poly, int n) {
  // poly % x^n
  if (poly.size() > n) {
    poly.resize(n);
  }
  norm(poly);
  return poly;
}

int binary_upper_bound(int w) {
  if (w <= 0) return 1;
  const int highbit = 31 - __builtin_clz(w);
  return (1 << highbit) << ((1 << highbit) != w);
}

template<typename T>
int get_ntt_len(const Polynomial<T>& lhs, const Polynomial<T>& rhs) {
  return binary_upper_bound((int)lhs.size() + (int)rhs.size() - 1);
}

template<typename T>
Polynomial<T> operator * (Polynomial<T> lhs, Polynomial<T> rhs) {
  CHECK(lhs.size() + rhs.size() > 0);
  const int L = get_ntt_len(lhs, rhs);
  lhs.resize(L, 0);
  rhs.resize(L, 0);
  ntt(&lhs[0], L, 1);
  ntt(&rhs[0], L, 1);
  for (int i = 0; i < L; ++i) {
    lhs[i] *= rhs[i];
  }
  ntt(&lhs[0], L, -1);
  norm(lhs);
  return lhs;
}

template<typename T>
Polynomial<T> operator * (T a, Polynomial<T> p) {
  for (int i = 0; i < p.size(); ++i) p[i] *= a;
  norm(p);
  return p;
}

template<typename T>
Polynomial<T> inv(Polynomial<T> poly) {
  CHECK(poly.size() >= 1 && poly[0] != 0);
  const int n = poly.size();
  const int L = binary_upper_bound(n);
  poly.resize(L, 0);
  Polynomial<T> w(L << 1), r(L << 1), saved(L << 1);
  w[0] = poly[0].inv();
  for (int len = 2; len <= L; len <<= 1) {
    std::copy(w.begin(), w.begin() + (len >> 1), r.begin());
    std::copy(poly.begin(), poly.begin() + len, saved.begin());
    ntt(&saved[0], len, 1);
    ntt(&r[0], len, 1);
    for (int i = 0; i < len; ++i) r[i] *= saved[i];
    ntt(&r[0], len, -1);
    for (int i = 0; i < (len >> 1); ++i) r[i] = 0;
    std::copy(w.begin(), w.begin() + len, saved.begin());
    ntt(&saved[0], len, 1);
    ntt(&r[0], len, 1);
    for (int i = 0; i < len; ++i) r[i] *= saved[i];
    ntt(&r[0], len, -1);
    for (int i = (len >> 1); i < len; ++i) w[i] = w[i] + w[i] - r[i];
  }
  return mod_len(w, n);
}

using Poly = Polynomial<Integral<MOD>>;

int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);
  std::istream& reader = std::cin;

  int n;
  reader >> n;
  Poly poly(n);
  for (int i = 0; i < n; ++i) {
    int x;
    reader >> x;
    poly[i] = x;
  }
  Poly Q = inv(poly);
  for (int i = 0; i < n; ++i) {
    printf("%d%c", Q[i].val(), " \n"[i + 1 == n]);
  }
}

