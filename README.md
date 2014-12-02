demo project to demonstrate cython py <-> c++ interfacing

warning: sample is for embedded python version

sample session:

    mic@mic ~/git/cythondemo $ ./main --help
    interp initialized
    usage: main [-h] [--thatnumber THATNUMBER]

    optional arguments:
    -h, --help            show this help message and exit
    --thatnumber THATNUMBER
    mic@mic ~/git/cythondemo $ ./main
    interp initialized
    square of 17: 289
    type p to enter interactive python interp, anything else to add strings
    > p
    >>> interface.callbacks.append(lambda s: ''.join(reversed(s.decode())).encode())
    >>>
    > teststring
    processed input: gnirtstset
    > p
    >>> list(interface.get_strings())
    [b'gnirtstset']
    >>>
    > kthxbai
