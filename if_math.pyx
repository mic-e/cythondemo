from .main cimport square as c_square


def square(unsigned number):
    return c_square(number)


cdef int print_square(int thatnumber) except*:
    print("square of {}: {}".format(thatnumber, c_square(thatnumber)))
    return 0


# register functions with C++
cimport py_functions
py_functions.print_square = print_square
