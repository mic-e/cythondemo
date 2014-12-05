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
    usage: __main__.py [-h] [--thatnumber THATNUMBER]

    optional arguments:
    -h, --help            show this help message and exit
    --thatnumber THATNUMBER
    mic@mic ~/git/cythondemo $ ./run
    LD_LIBRARY_PATH=. python3 -m demo
    exception during exctest: PyErr_Occured
    square of 17: 289
    type p to enter interactive python interp, anything else to add strings
    > p
    >>> 
    > testinput
    processed input: testinput
    > p
    >>> if_strings.callbacks.append(lambda s: s.upper())
    >>> 
    > testinput
    processed input: TESTINPUT
    > p
    >>> if_strings.callbacks.append("this is not a function.")
    >>> 
    > testinput
    error processing line: PyErr_Occured
    > p
    > kthxbai
