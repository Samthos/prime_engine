#include <algorithm>
#include <iostream>
#include <cassert>
#include <cstdint>
#include <vector>

#include "prime_engine.h"

template <class INT>
PrimeEngine<INT>::PrimeEngine(INT maxPrime) : maxPrime_(maxPrime) {
  generatePrimeQ();
  generatePrimes();
}

template <class INT>
void PrimeEngine<INT>::generatePrimeQ() {
  primeQ_.resize(maxPrime_);
  std::fill(primeQ_.begin(), primeQ_.end(), true);
  std::fill(primeQ_.begin(), primeQ_.begin()+2, false);

  auto it = primeQ_.begin();

  // screen out even number
  it = std::find(it+1, primeQ_.end(), true);
  INT i = std::distance(primeQ_.begin(), it);
  for (auto j = i * i; j < maxPrime_; j += i) {
    primeQ_[j] = false;
  }

  // run seive on remaining primes
  while (i * i < maxPrime_) {
    it = std::find(it+1, primeQ_.end(), true);
    i = std::distance(primeQ_.begin(), it);
    for (auto j = i * i; j < maxPrime_; j += i*2) {
      primeQ_[j] = false;
    }
  }
}

template <class INT>
void PrimeEngine<INT>::generatePrimes() {
  primes_.resize(std::count(primeQ_.begin(), primeQ_.end(), true));

  auto jt = primes_.begin();
  for (auto it = primeQ_.begin(); it != primeQ_.end(); it++) {
    if (*(it)) {
      *(jt) = std::distance(primeQ_.begin(), it);
      jt++;
    }
  }
}

template <class INT>
bool PrimeEngine<INT>::PrimeQ(INT n) const {
  assert(n < maxPrime_);
  return primeQ_[n];
}

template <class INT>
bool PrimeEngine<INT>::PrimeQForce(INT n) const {
  assert(sqrt(n) < maxPrime_);
  if (n < maxPrime_) {
    return primeQ_[n];
  } else {
    int index = 0;
    while (primes_[index] < sqrt(n)) {
      if (0 == n % primes_[index]) {
        return false;
      }
      index++;
    }
    return true;
  }
}

template <class INT>
INT PrimeEngine<INT>::PrimePi(INT n) const {
  assert(n < maxPrime_);
  auto it = std::upper_bound(primes_.begin(), primes_.end(), n);
  return std::distance(primes_.begin(), it);
}

template <class INT>
INT PrimeEngine<INT>::Prime(INT index) const {
  assert(index < numPrime_);
  return primes_[index-1];
}

template <class INT>
INT PrimeEngine<INT>::numPrimes() const {
  return numPrime_;
}

template <class INT>
std::vector<std::pair<INT,INT>> PrimeEngine<INT>::FactorInteger(INT n) const {
  INT count;
  std::vector<std::pair<INT,INT>> returnList;

  assert(sqrt(n) < maxPrime_);

  auto it = primes_.begin();
  auto stop = std::upper_bound(primes_.begin(), primes_.end(), static_cast<INT>(sqrt(n)));
  while (n > maxPrime_) {
    it = std::find_if(it, stop, [n](INT p) {return ((n%p)==0);});
    if (it == stop) {
      break;
    }

    count = 0;
    do {
      count++;
      n /= *(it);
    } while (0==(n % *(it)));
    returnList.push_back({*(it),count});
  }

  if (it != stop) {
    while (n != 1 && !primeQ_[n]) {
      it = std::find_if(it, stop, [n](INT p) {return ((n%p)==0);});
      if (it == stop) {
        break;
      }

      count = 0;
      do {
        count++;
        n /= *(it);
      } while (0==(n % *(it)));
      returnList.push_back({*(it),count});
    }
  }

  if (n != 1) {
    returnList.push_back({n,1});
  }
  return returnList;
}

template <class INT>
INT PrimeEngine<INT>::IntegerFromFactors(const std::vector<std::pair<INT,INT>>& pf) const {
  return std::accumulate(pf.begin(), pf.end(), 1, 
    [](INT total, std::pair<INT,INT> p) {
      for (INT i = 0; i < p.second; i++) {
        total *= p.first;
      }
      return total;
    });
}

template <class INT>
std::vector<INT> PrimeEngine<INT>::Divisors(INT n) const {
  auto pf = FactorInteger(n);
  return Divisors(pf);
}
  
template <class INT>
std::vector<INT> PrimeEngine<INT>::Divisors(const std::vector<std::pair<INT,INT>>& pf) const {
  INT v;
  UINT i;
  INT j;
  std::vector<INT> returnList;
  std::vector<INT> indices(pf.size()+1);

  std::fill(indices.begin(), indices.end(), 0);

  v = 1;
  while (indices.back() == 0) {
    returnList.push_back(v);

    i = 0;
    indices[i] = indices[i] + 1;
    v *= pf[i].first;
    while (indices[i] == (pf[i].second+1)) {
      indices[i] = 0;
      for (j = 0; j <= pf[i].second; j++) {
        v /= pf[i].first;
      }

      i++;
      indices[i] = indices[i] + 1;
      if (i == pf.size()) {
       break;
      }
      v *= pf[i].first;
    }
  }
  std::sort(returnList.begin(), returnList.end());
  return returnList;
}

template <class INT>
INT PrimeEngine<INT>::EulerPhi(INT n) const {
  auto pf = FactorInteger(n);
  return EulerPhi(n, pf);
}

template <class INT>
INT PrimeEngine<INT>::EulerPhi(const std::vector<std::pair<INT,INT>>& pf) const {
  return EulerPhi(IntegerFromFactors(pf), pf);
}

template <class INT>
INT PrimeEngine<INT>::EulerPhi(INT n, const std::vector<std::pair<INT,INT>>& pf) const {
  for (auto &it : pf) {
    n /= it.first;
    n *= it.first - 1;
  }
  return n;
}

template <class INT>
INT PrimeEngine<INT>::DivisorSum(INT n) const {
  auto pf = FactorInteger(n);
  return DivisorSum(pf);
}

template <class INT>
INT PrimeEngine<INT>::DivisorSum(const std::vector<std::pair<INT,INT>>& pf) const {
  INT v, rVal = 1;
  INT i;
  for (auto &it : pf) {
    v = 1;
    for (i = 0; i <= it.second; i++) {
      v *= it.first;
    }
    v -= 1;
    rVal *= v / (it.first - 1);
  }
  return rVal;
}

template <class INT>
INT PrimeEngine<INT>::DivisorCount(INT n) const {
  auto pf = FactorInteger(n);
  return DivisorCount(pf);
}

template <class INT>
INT PrimeEngine<INT>::DivisorCount(const std::vector<std::pair<INT,INT>>& pf) const {
  INT rVal = 1;
  for (auto &it : pf) {
    rVal *= (it.second + 1);
  }
  return rVal;
}

template <class INT>
INT PrimeEngine<INT>::DivisorSigma(INT n, INT m) const {
  auto pf = FactorInteger(n);
  assert(m >= 0);
  if (0 == m) {
    return DivisorCount(pf);
  } else if (1 == m) {
    return DivisorSum(pf);
  }
  return DivisorSigma_(pf, m);
}

template <class INT>
INT PrimeEngine<INT>::DivisorSigma(const std::vector<std::pair<INT,INT>>& pf, INT m) const {
  assert(m >= 0);
  if (0 == m) {
    return DivisorCount(pf);
  } else if (1 == m) {
    return DivisorSum(pf);
  }
  return DivisorSigma_(pf, m);
}

template <class INT>
INT PrimeEngine<INT>::DivisorSigma_(const std::vector<std::pair<INT,INT>>& pf, INT m) const {
  INT rVal = 1;
  for (auto &it : pf) {
    rVal *= (pow(it.first, (it.second+1)*m) - 1) / (pow(it.first, m) - 1);
  }
  return rVal;
}
