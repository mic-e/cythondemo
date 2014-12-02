demo project to demonstrate cython py <-> c++ interfacing

warning: sample is for embedded python version

sample session:

    mic@mic ~/git/cythondemo $ p3 -m demo --help                                                                                                                                        [extended]
    usage: __main__.py [-h] [--thatnumber THATNUMBER]

    optional arguments:
    -h, --help            show this help message and exit
    --thatnumber THATNUMBER
    mic@mic ~/git/cythondemo $ p3 -m demo --thatnumber=1337                                                                                                                             [extended]
    square of 1337: 1787569
    type p to enter interactive python interp, anything else to add strings
    > p
    >>> interface.callbacks.append(lambda s: s.upper())
    >>> 
    > teststring
    processed input: TESTSTRING
    > kthxbai
