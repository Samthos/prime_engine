// Copyright 2017 Alex Doran
#include <vector>
#include <utility>
#include <cstdint>

#ifdef WITH_PYTHON
#endif // WITH_PYTHON

#ifndef PRIME_ENGINE_H_
#define PRIME_ENGINE_H_
template <class T>
class PrimeEngine{ 
 private:
  T maxPrime_;
  std::vector<T> primes_;
  std::vector<bool> PrimeQ_;

  void generatePrimeQ();
  void generatePrimes();


  T lower_bound_n_primes_below(T val);
 public:

  PrimeEngine(T);

  bool PrimeQ(T);
  T PrimePi(T);
  T Prime(unsigned);
  T numPrimes();

  std::vector<T> n_primes(T n);
  std::vector<std::pair<T,T>> FactorInteger(T);
  std::vector<T> Divisors(T);
  T DivisorSum(T);
  T EulerPhi(T);
};

template class PrimeEngine<int>;
template class PrimeEngine<int64_t>;
template class PrimeEngine<uint64_t>;


#endif  // PRIME_ENGINE_H_
