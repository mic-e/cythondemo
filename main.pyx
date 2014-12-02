from test cimport main as c_main
from test cimport Args as c_Args

def main():
    cdef c_Args c_args

    import pymod
    args = pymod.handle_args()

    c_args.thatnumber = args.thatnumber

    c_main(c_args)
