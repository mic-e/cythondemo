from decl cimport PyObject
from decl cimport square as c_square
from decl cimport add_callback as c_add_callback
from decl cimport Args as c_Args
from decl cimport inputstrings as c_inputstrings
from decl cimport callbacks as c_callbacks
from libcpp.string cimport string


cdef public void py_setup_path():
    import sys
    sys.path.insert(0, '')


cdef public c_Args py_handle_args(int c_argc, char **c_argv):
    cdef c_Args retval

    argv = list()

    for i in range(c_argc):
        argv.append(c_argv[i].decode())

    import sys
    sys.argv = argv

    import pymod
    try:
        args = pymod.handle_args()
        retval.exit = 0
        retval.thatnumber = args.thatnumber
    except SystemExit as e:
        retval.exit = 1
        retval.exitcode = e.args[0]

    return retval


cdef public void py_interact():
    import pymod
    pymod.interact()


cdef public void py_print_square(int thatnumber):
    print("square of {}: {}".format(thatnumber, c_square(thatnumber)))


def get_strings():
    for s in c_inputstrings:
        yield s


def append_string(s):
    c_inputstrings.push_back(s)


def add_callback(f):
    if not callable(f):
        raise Exception("not callable")

    c_callbacks.push_back(<PyObject *> f)


cdef public void py_invoke_callback(PyObject *f, string s):
    try:
        (<object> f)(s)
    except Exception as e:
        print(e)
