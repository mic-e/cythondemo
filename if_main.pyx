from .main cimport main as c_main
from .main cimport Args as c_Args

import traceback


def main(args):
    cdef c_Args c_args
    c_args.thatnumber = args.thatnumber
    c_main(c_args)


cdef void interact():
    try:
        from .prompt import interact
        interact()
    except:
        traceback.print_exc()


# register functions with C++
cimport py_functions
py_functions.interact = interact

# import all other modules that will register functions with C++
from . import if_math
from . import if_strings
