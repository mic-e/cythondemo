PYTHON=python3.4m
PYTHONLIBDIR=/usr/lib/x86_64-gnu
PYTHONINCDIR=/usr/include/$(PYTHON)
CXX=g++
CXXFLAGS=-std=c++11 -Wall -Wextra -pedantic -fPIC -O1 -g -I$(PYTHONINCDIR)
LDFLAGS=--shared
MODLDFLAGS=-Wl,-rpath,.
CYTHON=cython
CYTHONFLAGS=--gdb --cplus -3 --fast-fail

.PHONY: all
all: interface.so libtest.so

interface.cpp interface.h: interface.pyx
	$(CYTHON) $(CYTHONFLAGS) $<

test.o: test.cpp test.h interface.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

interface.o: interface.cpp test.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

libtest.so: test.o
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $^ -o $@

interface.so: interface.o libtest.so
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $^ -o $@ $(MODLDFLAGS)

.PHONY: clean
clean:
	rm -rf interface.cpp interface.h interface.cpp *.o cython_debug *.so
