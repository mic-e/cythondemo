demo project to demonstrate cython py <-> c++ interfacing

warning: may contain unexpected segfaults.

sample session:

	mic@mic ~/git/cythondemo $ ./main
	pymod init done
	square of 17: 289
	> p
	openage console
	>>> import sys
	>>> interface=sys.modules['interface']
	>>> def mycallback(s):
	...   print(''.join(reversed(s.decode())))
	...
	>>> interface.add_callback(mycallback)
	>>>
	> test
	input: test
	tset
	> lolwtf
	input: lolwtf
	ftwlol
	> gschicht
	input: gschicht
	thcihcsg
	> p
	openage console
	>>> list(interface.get_strings())
	[b'test', b'lolwtf', b'gschicht']
	>>>
	> kthxbai
	mic@mic ~/git/cythondemo $ ./main --help
	pymod init done
	usage: main [-h] [--thatnumber THATNUMBER]

	optional arguments:
	-h, --help            show this help message and exit
	--thatnumber THATNUMBER
