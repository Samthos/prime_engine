// Copyright 2017 Alex Doran

#include "prime_engine.h"

#include <fstream>

int main() {
  std::ofstream output("prime_q_test.output");
  PrimeEngine<int64_t> primeEngine(100);

  for (int64_t i = 1; i < 100; i++) {
    if (primeEngine.PrimeQ(i)) {
      output << i << std::endl;
    }
  }

  output.close(); }
