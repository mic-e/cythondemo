PYTHON=python3.4m
PYTHONLIBDIR=/usr/lib/x86_64-gnu
PYTHONINCDIR=/usr/include/$(PYTHON)
CXX=g++
CXXFLAGS=-std=c++11 -Wall -Wextra -pedantic -fPIC -O1 -g -I$(PYTHONINCDIR)
LDFLAGS=--shared #-l$(PYTHON) -L$(PYTHONLIBDIR) --shared
CYTHON=cython

.PHONY: all
all: main.so interface.so

main.cpp: main.pyx
	$(CYTHON) --gdb --cplus -3 $<

interface.cpp interface.h: interface.pyx
	$(CYTHON) --gdb --cplus -3 $<

test.o: test.cpp interface.h test.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

main.o: main.cpp test.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

interface.o: interface.cpp interface.h test.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

main.so: test.o main.o
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $^ -o $@

interface.so: interface.o
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $^ -o $@

.PHONY: clean
clean:
	rm -rf interface.cpp interface.h main.cpp *.o cython_debug *.so
