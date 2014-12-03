from libcpp.vector cimport vector
from libcpp.string cimport string


cdef extern from "main.h" namespace "test":
    unsigned square(unsigned num)

    struct Args:
        int thatnumber

    vector[string] inputstrings

    int main(Args args)
