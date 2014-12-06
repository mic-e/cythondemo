This demo project evalutes designs for a two-way Python/C++ interface for projects that have C++ main loops and are mainly written in C++.

The interface design was created for [openage](https://github.com/sfttech/openage), but it is general-purpose. My Thanks go to `#Python` on irc.freenode.net, whose advice was vital for finding sane design decisions.

Basic design
------------

- The application becomes a library (`libtest.so`).
- The entry point (main method) is written in Python.
- `.pxd` files are generated from annotated `.h` files (see `pxdgen.py`; `.pxd` files are the Cython equivalent of header files).
- The interface is written in `.pyx` files, which are compiled to `.cpp` files by Cython; those `.cpp` files can then be compiled to Python extension modules; they are linked against `libtest.so`.
- Pure python modules can import those extension modules at run-time; that way they can access any C++ functions and objects that are wrapped there.
- Function pointers are used to call Python functions from C++; they are defined in the C++ code (`py_functions.cpp`) and made available to Python through `py_functions.h`. `.pyx` modules that want to provide a function define a `cdef` function with a matching signature and `except*`, and assign it to the C++ function pointer during initialization.
- Before making calls into the C++ code, all `.pyx` modules that provide `C++` function pointers  __must__ be imported
- C++ methods that are called from Python must be declared `except+` to enable proper exception translation (automatically done by Cython).
- Exceptions raised by Python methods that are called from C++ are translated to C++ exceptions by the `PyFunc` function pointer wrapper that is defined in and used by `py_functions.h`.

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
    g++ -std=c++11 -fPIC -I/usr/include/python3.4m --shared main.cpp py_functions.cpp -o libtest.so
    g++ -std=c++11 -fPIC -I/usr/include/python3.4m --shared if_main.cpp libtest.so -o if_main.so
    g++ -std=c++11 -fPIC -I/usr/include/python3.4m --shared if_math.cpp libtest.so -o if_math.so
    g++ -std=c++11 -fPIC -I/usr/include/python3.4m --shared if_strings.cpp libtest.so -o if_strings.so
