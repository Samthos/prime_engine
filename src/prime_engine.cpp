#include <cassert>
#include <vector>
#include <cstdint>
#include <algorithm>
#include <iostream>

#include "prime_engine.h"

template <class T>
PrimeEngine<T>::PrimeEngine(T maxPrime) : maxPrime_(maxPrime) {
  generatePrimeQ();
  generatePrimes();
}

template <class T>
void PrimeEngine<T>::generatePrimeQ() {
  primeQ_.resize(maxPrime_);
  std::fill(primeQ_.begin(), primeQ_.end(), true);
  std::fill(primeQ_.begin(), primeQ_.begin()+2, false);

  auto it = primeQ_.begin();

  // screen out even number
  it = std::find(it+1, primeQ_.end(), true);
  T i = std::distance(primeQ_.begin(), it);
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

template <class T>
void PrimeEngine<T>::generatePrimes() {
  primes_.resize(std::count_if(primeQ_.begin(), primeQ_.end(), [](bool i){return i;}));

  auto jt = primes_.begin();
  for (auto it = primeQ_.begin(); it != primeQ_.end(); it++) {
    if (*(it)) {
      *(jt) = std::distance(primeQ_.begin(), it);
      jt++;
    }
  }
}

template <class T>
bool PrimeEngine<T>::PrimeQ(T n) {
  assert(n < maxPrime_);
  return primeQ_[n];
}

template <class T>
T PrimeEngine<T>::PrimePi(T n) {
  assert(n < maxPrime_);
  auto it = std::upper_bound(primes_.begin(), primes_.end(), n);
  return std::distance(primes_.begin(), it);
}

template <class T>
T PrimeEngine<T>::Prime(T index) {
  assert(index < primes_.size());
  return primes_[index];
}

template <class T>
T PrimeEngine<T>::numPrimes() {
  return primes_.size();
}

template <class T>
std::vector<std::pair<T,T>> PrimeEngine<T>::FactorInteger(T n) {
  T count;
  std::vector<std::pair<T,T>> returnList;

  assert(sqrt(n) < maxPrime_);

  auto it = primes_.begin();
  auto stop = std::upper_bound(primes_.begin(), primes_.end(), static_cast<T>(sqrt(n)));
  while (n > maxPrime_) {
    it = std::find_if(it, stop, [n](T p) {return ((n%p)==0);});
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
      it = std::find_if(it, stop, [n](T p) {return ((n%p)==0);});
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

template <class T>
std::vector<T> PrimeEngine<T>::Divisors(T n) {
  auto pf = FactorInteger(n);
  
  T i, v;
  std::vector<T> returnList;
  std::vector<T> indices(pf.size()+1);

  std::fill(indices.begin(), indices.end(), 0);

  v = 1;
  while (indices.back() == 0) {
    returnList.push_back(v);

    i = 0;
    indices[i] = indices[i] + 1;
    v *= pf[i].first;
    while (indices[i] == (pf[i].second+1)) {
      indices[i] = 0;
      for (int j = 0; j <= pf[i].second; j++) {
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

template <class T>
T PrimeEngine<T>::EulerPhi(T n) {
  auto pf = FactorInteger(n);
  for (auto &it : pf) {
    n /= it.first;
    n *= it.first - 1;
  }
  return n;
}

template <class T>
T PrimeEngine<T>::DivisorSum(T n) {
  T v, rVal = 1;
  auto pf = FactorInteger(n);
  for (auto &it : pf) {
    v = 1;
    for (int i = 0; i <= it.second; i++) {
      v *= it.first;
    }
    v -= 1;
    rVal *= v / (it.first - 1);
  }
  return rVal;
}
