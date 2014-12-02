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
vector<PyObject *> callbacks;

unsigned square(unsigned number) {
	return number * number;
}

void process_input(string s) {
	if (s == "p") {
		py_interact();
	} else {
		cout << "input: " << s << endl;
		inputstrings.push_back(s);

		for(PyObject *callback: callbacks) {
			py_invoke_callback(callback, s);
		}
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
		py_setup_path();
	}

	~PyInterp() {
		Py_Finalize();
	}
};

} // namespace test

int main(int argc, char **argv) {
	test::PyInterp i;

	test::Args args = py_handle_args(argc, argv);

	if (args.exit) {
		return args.exitcode;
	}

	py_print_square(args.thatnumber);

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
