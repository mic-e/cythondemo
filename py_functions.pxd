from libcpp.string cimport string

cdef extern from "py_functions.h" namespace "test::py_functions":
    void (*print_square)(int)
    void (*interact)()
    string (*invoke_callbacks)(string)
