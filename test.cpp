#include <iostream>
#include <vector>
#include <string>

#include <Python.h>

#include "test.h"

#ifndef DL_IMPORT
// for some reason DL_IMPORT isn't defined on my system...
#define DL_IMPORT(x) x
#endif
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

int main(Args args) {
	//asm volatile ("int3;");

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

} // namespace test

