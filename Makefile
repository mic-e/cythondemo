PYTHON=python3.4m
PYTHONLIBDIR=/usr/lib/x86_64-gnu
PYTHONINCDIR=/usr/include/$(PYTHON)
CXX=g++
CXXFLAGS=-std=c++11 -Wall -Wextra -pedantic -O1 -g -I$(PYTHONINCDIR)
LDFLAGS=-l$(PYTHON) -L$(PYTHONLIBDIR)
CYTHON=cython

.PHONY: all
all: main

interface.cpp interface.h: interface.pyx
	$(CYTHON) --gdb --cplus -3 interface.pyx

test.o: test.cpp interface.h test.h
interface.o: interface.cpp interface.h test.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

main: test.o interface.o
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $^ -o $@

.PHONY: clean
clean:
	rm -rf interface.cpp interface.h *.o main cython_debug
