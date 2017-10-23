// Copyright 2017 Alex Doran
#include <type_traits>
#include <utility>
#include <cstdint>
#include <vector>

#ifndef PRIME_ENGINE_H_
#define PRIME_ENGINE_H_
template <class INT>
class PrimeEngine{ 
 private:
  typedef typename std::make_unsigned<INT>::type UINT;
  INT maxPrime_;
  INT numPrime_;
  std::vector<INT> primes_;
  std::vector<bool> primeQ_;

  void generatePrimeQ();
  void generatePrimes();

  bool RabinMiller(INT);
 public:
  PrimeEngine();
  PrimeEngine(INT);

  // Functions to return information directly related to primes
  bool PrimeQ(INT) const;
  bool PrimeQForce(INT) const;
  INT PrimePi(INT) const;
  INT Prime(INT) const;
  INT numPrimes() const;

  // Functions that rely heavily on knowing primes
  std::vector<std::pair<INT,INT>> FactorInteger(INT) const;
  INT IntegerFromFactors(const std::vector<std::pair<INT,INT>>&) const;

  std::vector<INT> Divisors(INT) const;
  std::vector<INT> Divisors(const std::vector<std::pair<INT,INT>>&) const;

  INT DivisorSum(INT) const;
  INT DivisorSum(const std::vector<std::pair<INT,INT>>&) const;
  INT DivisorSigma(INT, INT) const;
  INT DivisorSigma(const std::vector<std::pair<INT,INT>>&, INT) const;
  INT DivisorSigma_(const std::vector<std::pair<INT,INT>>&, INT) const;
  INT DivisorCount(INT) const;
  INT DivisorCount(const std::vector<std::pair<INT,INT>>&) const;

  INT EulerPhi(INT) const;
  INT EulerPhi(const std::vector<std::pair<INT,INT>>&) const;
  INT EulerPhi(INT n, const std::vector<std::pair<INT,INT>>&) const;

  // iterators
/*
  typedef typename std::vector<INT>::const_iterator const_iterator;
  typename std::vector<INT>::const_iterator begin() {
    return primes_.begin();
  }
  typename std::vector<INT>::const_iterator end() {
    return primes_.end();
  }
  typename std::vector<INT>::const_reverse_iterator rbegin() {
    return primes_.rbegin();
  }
  typename std::vector<INT>::const_reverse_iterator rend() {
    return primes_.rend();
  }
  typename std::vector<INT>::const_iterator PrimePiIter(INT n) {
    return std::upper_bound(primes_.begin(), primes_.end(), n);
  }
*/
};

template class PrimeEngine<int>;
template class PrimeEngine<int64_t>;
template class PrimeEngine<uint64_t>;
#endif  // PRIME_ENGINE_H_
