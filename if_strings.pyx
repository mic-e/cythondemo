from libcpp.string cimport string

from .main cimport inputstrings as c_inputstrings

import traceback


def get_strings():
    for s in c_inputstrings:
        yield s


def append_string(s):
    c_inputstrings.push_back(s)


callbacks = []


cdef string invoke_callbacks(string s) except*:
    for cb in callbacks:
        s = cb(s.decode()).encode()

    return s


# register functions with C++
cimport py_functions
py_functions.invoke_callbacks = invoke_callbacks
