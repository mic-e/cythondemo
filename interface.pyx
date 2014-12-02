from libcpp.string cimport string
from libcpp.vector cimport vector
from decl cimport square as c_square
from decl cimport Args as c_Args
from decl cimport inputstrings as c_inputstrings


cdef public void py_init(string modulepath):
    import sys
    sys.path.insert(0, modulepath.decode())


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


callbacks = []


cdef public string py_invoke_callbacks(string s):
    cdef string ret

    for cb in callbacks:
        try:
            ret = cb(s)
            if ret.size():
                s = ret
        except:
            import traceback
            print(traceback.format_exc())

    return s
