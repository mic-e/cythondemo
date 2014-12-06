Demo project to demonstrate a Cython-powered `Python` <-> `C++` interface.

Basic design
------------

- The game becomes a library (`libtest.so`).
- For each `.h` file, a `.pxd` file is generated from header annotations (see `pxdgen.py`). The `.pxd` files can then be included from `.pyx` files.
- Each `.pyx` file is compiled to a `.cpp` file by Cython, then linked with `libtest.so` to a `.so` python extension module.
- Pure python modules can import `.pyx` extension modules at run-time; that way they can access any `C++` functions that are wrapped there.
- During their initialization, the `.pyx` modules store pointers to their `cdef` functions in `py_functions.cpp`. Before initializion, those function pointers hold dummies. All `cdef` functions must be declared `except*` to correctly pass the Python exception to the C++ Python exception translator.
- Due to the last point, all `.pyx` modules that register function pointers __MUST__ be manually imported before transferring control to `C++` code.
- In `C++`, the function pointers are wrapped in the `PyFunc` class, which checks whether the pointer has been correctly set, and whether a Python exception as occured; if one has, it translates that exception to a C++ exception.

Sample session
--------------

    mic@mic ~/git/cythondemo $ ./run --help
    LD_LIBRARY_PATH=. python3 -m demo --help
    usage: __main__.py [-h] [--thatnumber THATNUMBER]

    optional arguments:
    -h, --help            show this help message and exit
    --thatnumber THATNUMBER
    mic@mic ~/git/cythondemo $ ./run --thatnumber=17
    LD_LIBRARY_PATH=. python3 -m demo --thatnumber=17
    exception during exctest: python exception: <class 'Exception'>: your mom is fat
    square of 17: 289
    type p to enter interactive python interp, anything else to add strings
    > teststring
    processed input: teststring
    > p
    >>> if_strings.callbacks.append(lambda s: s.upper())
    >>> 
    > teststring
    processed input: TESTSTRING
    > p
    >>> if_strings.callbacks.append(lambda s: len(s)/0)
    >>> 
    > teststring
    error processing line: python exception: <class 'ZeroDivisionError'>: division by zero
    > kthxbai

Build process
-------------

    python3 pxdgen.py main.h -o main.pxd
    python3 pxdgen.py py_functions.h -o py_functions.pxd
    cython --gdb --cplus -3 --fast-fail if_math.pyx
    cython --gdb --cplus -3 --fast-fail if_main.pyx
    cython --gdb --cplus -3 --fast-fail if_strings.pyx
    g++ -std=c++11 -fPIC -I/usr/include/python3.4m -c main.cpp -o main.o
    g++ -std=c++11 -fPIC -I/usr/include/python3.4m -c py_functions.cpp -o py_functions.o
    g++ -std=c++11 -fPIC -I/usr/include/python3.4m --shared main.o py_functions.o -o libtest.so
    g++ -std=c++11 -fPIC -I/usr/include/python3.4m -c if_main.cpp -o if_main.o
    g++ -std=c++11 -fPIC -I/usr/include/python3.4m -c if_math.cpp -o if_math.o
    g++ -std=c++11 -fPIC -I/usr/include/python3.4m -c if_strings.cpp -o if_strings.o
    g++ -std=c++11 -fPIC -I/usr/include/python3.4m --shared if_main.o libtest.so -o if_main.so 
    g++ -std=c++11 -fPIC -I/usr/include/python3.4m --shared if_math.o libtest.so -o if_math.so 
    g++ -std=c++11 -fPIC -I/usr/include/python3.4m --shared if_strings.o libtest.so -o if_strings.so 
