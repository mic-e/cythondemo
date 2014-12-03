from .main cimport square as c_square


def square(unsigned number):
    return c_square(number)


cdef void print_square(int thatnumber):
    print("square of {}: {}".format(thatnumber, c_square(thatnumber)))


# register functions with C++
cimport py_functions
py_functions.print_square = print_square
