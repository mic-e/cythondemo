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
all: libtest.so if_math.so if_main.so if_strings.so

# pure c++ files
py_functions.o: py_functions.cpp py_functions.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

main.o: main.cpp main.h py_functions.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

libtest.so: main.o py_functions.o
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $^ -o $@

# cython files
if_main.cpp if_main.h: if_main.pyx
	$(CYTHON) $(CYTHONFLAGS) $<

if_main.o: if_main.cpp main.h py_functions.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

if_main.so: if_main.o libtest.so
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $^ -o $@ $(MODLDFLAGS)

if_math.cpp: if_math.pyx
	$(CYTHON) $(CYTHONFLAGS) $<

if_math.o: if_math.cpp main.h py_functions.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

if_math.so: if_math.o libtest.so
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $^ -o $@ $(MODLDFLAGS)

if_strings.cpp: if_strings.pyx
	$(CYTHON) $(CYTHONFLAGS) $<

if_strings.o: if_strings.cpp main.h py_functions.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

if_strings.so: if_strings.o libtest.so
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $^ -o $@ $(MODLDFLAGS)

.PHONY: clean
clean:
	rm -rf if_main.cpp if_math.cpp if_strings.cpp *.o *.so cython_debug
