src = prime_engine.cpp
obj = $(src:.cpp=.o)
		
# For wrapping with pybind11
sobj = $(src:.cpp=.so)


CXX = g++
CXXFLAGS = -Wall -O3 -std=c++11
PYBIND11_FLAGS = -DWITH_PYTHON -shared -fPIC `python3 -m pybind11 --includes`

libprime_engine.a: $(obj)
	ar -rvs $@ $^

.cpp.o:
	$(CXX) $(CXXFLAGS) -c -o $@ $^

python: $(sobj)
	@echo "Python built!"

$(sobj): %.so : %.cpp
	$(CXX) $(CXXFLAGS) $(PYBIND11_FLAGS) $^ -o $@
	

clean:
	@-rm -f *.a *.o *.so

.PHONY: clean
