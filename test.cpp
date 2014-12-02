#include <iostream>
#include <vector>
#include <string>

#include <Python.h>

#include "test.h"

#define DL_IMPORT(x) x
#include "interface.h"

using namespace std;

namespace test {

vector<string> inputstrings;

unsigned square(unsigned number) {
	return number * number;
}

void process_input(string s) {
	if (s == "p") {
		py_interact();
	} else {
		s = py_invoke_callbacks(s);
		inputstrings.push_back(s);
		cout << "processed input: " << s << endl;
	}
}

class PyInterp {
public:
	PyInterp() {
		Py_Initialize();

		// init routines for all pyx modules
		// TODO this returns the module object... do something with it?
		PyInit_interface();

		// setup module import path
		py_init("");
	}

	~PyInterp() {
		Py_Finalize();
	}
};

} // namespace test

int main(int argc, char **argv) {
	test::PyInterp i;

	cout << "interp initialized" << endl;

	test::Args args = py_handle_args(argc, argv);

	if (args.exit) {
		return args.exitcode;
	}

	py_print_square(args.thatnumber);

	cout << "type p to enter interactive python interp, anything else to add strings" << endl;

	while (true) {
		string l;

		cout << "> " << flush;
		getline(cin, l);

		if (l.size() == 0) {
			cout << "kthxbai" << endl;
			break;
		}

		test::process_input(l);
	}

	return 0;
}
