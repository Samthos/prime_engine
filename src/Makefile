src = prime_engine.cpp
obj = $(src:.cpp=.o)

CXX = g++
CXXFLAGS = -Wall -O3 -std=c++11

libprime_engine.a: $(obj)
	ar -rvs $@ $^

.cpp.o:
	$(CXX) $(CXXFLAGS) -c -o $@ $^

clean:
	rm *.a *.o

.PHONY: clean
