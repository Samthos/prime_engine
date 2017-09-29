#include <cassert>
#include <vector>
#include <cstdint>
#include <cmath>
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
  PrimeQ_.resize(maxPrime_);
  std::fill(PrimeQ_.begin(), PrimeQ_.end(), true);
  std::fill(PrimeQ_.begin(), PrimeQ_.begin()+2, false);

  auto it = PrimeQ_.begin();

  // screen out even number
  it = std::find(it+1, PrimeQ_.end(), true);
  T i = std::distance(PrimeQ_.begin(), it);
  for (auto j = i * i; j < maxPrime_; j += i) {
    PrimeQ_[j] = false;
  }

  // run seive on remaining primes
  while (i * i < maxPrime_) {
    it = std::find(it+1, PrimeQ_.end(), true);
    i = std::distance(PrimeQ_.begin(), it);
    for (auto j = i * i; j < maxPrime_; j += i*2) {
      PrimeQ_[j] = false;
    }
  }
}

template <class T>
void PrimeEngine<T>::generatePrimes() {
  primes_.resize(std::count_if(PrimeQ_.begin(), PrimeQ_.end(), [](bool i){return i;}));

  auto jt = primes_.begin();
  for (auto it = PrimeQ_.begin(); it != PrimeQ_.end(); it++) {
    if (*(it)) {
      *(jt) = std::distance(PrimeQ_.begin(), it);
      jt++;
    }
  }
}

template <class T>
bool PrimeEngine<T>::PrimeQ(T n) {
  assert(n < maxPrime_);
  return PrimeQ_[n];
}

template <class T>
T PrimeEngine<T>::PrimePi(T n) {
  assert(n < maxPrime_);
  auto it = std::upper_bound(primes_.begin(), primes_.end(), n);
  return std::distance(primes_.begin(), it);
}

template <class T>
T PrimeEngine<T>::Prime(unsigned index) {
  assert(index < primes_.size());
  return primes_[index];
}

template <class T>
T PrimeEngine<T>::numPrimes() {
  return primes_.size();
}

template <class T>
T PrimeEngine<T>::lower_bound_n_primes_below(T val) {
	// This is a lower bound on the number of primes that occur below val
	assert(val >= 55 && "This formula only valid for val >= 55");
	double v = val / (log(val) + 2);
	return static_cast<T>(floor(v));
}

template <class T>
std::vector<T> PrimeEngine<T>::n_primes(T n) {
	std::vector<T> local_primes(n);

	if (n > numPrimes()) {
		if (n <= 25) {
			maxPrime_ = 100;
		} else if (n > 25) {
			maxPrime_ = 100;
			T n_primes_below = lower_bound_n_primes_below(maxPrime_);
			T temp = maxPrime_;
			for (temp = maxPrime_; n_primes_below < n; ++temp)
				n_primes_below = lower_bound_n_primes_below(temp); 
			maxPrime_ = temp;
		}
		generatePrimeQ();
		generatePrimes();
	}
	std::copy_n(primes_.begin(), n, local_primes.begin());
	return local_primes;
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
    while (n != 1 && !PrimeQ_[n]) {
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
  
  T v;
  unsigned i;
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
      for (T j = 0; j <= pf[i].second; j++) {
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
    for (T i = 0; i <= it.second; i++) {
      v *= it.first;
    }
    v -= 1;
    rVal *= v / (it.first - 1);
  }
  return rVal;
}

#ifdef WITH_PYTHON
	#include <pybind11/pybind11.h>
	#include <pybind11/stl.h>
	// Note Python can only bind explicitly declared templates.
	typedef PrimeEngine<int> pe;
	namespace py = pybind11;
	PYBIND11_MODULE(prime_engine, m) {
		py::class_<pe>(m, "PrimeEngine")
			.def(py::init<int>())
			.def("is_prime", &pe::PrimeQ, 
				"Return True if prime, false otherwise.")
			.def("PrimePi", &pe::PrimePi,
				"Alex please add I don't know what this one does.")
			.def("Prime", &pe::Prime,
				"docstring")
			.def("numPrimes", &pe::numPrimes, 
				"Return the current number of primes stored.")
			.def("FactorInteger_tuples", &pe::FactorInteger, 
				"Return pairs of (factor, occurances) of the factorization.")
			.def("Divisors", &pe::Divisors, 
				"Return list of divisors of given integer")
			.def("DivisorSum", &pe::DivisorSum, 
				"Return the sum of all divisors of given integer")
			.def("EulerPhi", &pe::EulerPhi, 
				"Return the value of euler's Phi function, the number of positive integers < n coprime to n.")
			.def("n_primes", &pe::n_primes,
				"Return the first n prime numbers");
	}
#endif // WITH_PYTHON
