src = prime_q_test.cpp prime_pi_test.cpp prime_test.cpp
exe = $(src:.cpp=.out)
CXX = g++
CXXFLAGS = -Wall -O3 -std=c++11

all: $(exe)

.cpp.out:
	$(CXX) $(CXXFLAGS) -o $@ $^ -I../src -L../src -lprime_engine

clean:
	-rm *.out *.output
