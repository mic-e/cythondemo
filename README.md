demo project to demonstrate cython py <-> c++ interfacing

sample session:

    mic@mic ~/git/cythondemo $ p3 -m demo --help
    usage: __main__.py [-h] [--thatnumber THATNUMBER]

    optional arguments:
    -h, --help            show this help message and exit
    --thatnumber THATNUMBER
    mic@mic ~/git/cythondemo $ p3 -m demo --thatnumber=17
    square of 17: 289
    type p to enter interactive python interp, anything else to add strings
    > p
    >>> if_strings.callbacks.append(lambda s: s.upper())
    >>>
    > teststring
    processed input: TESTSTRING
    > kthxbai
