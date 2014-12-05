#include "main.h"

#include <iostream>
#include <vector>
#include <string>
#include <stdexcept>

#include "py_functions.h"

using namespace std;

namespace test {

vector<string> inputstrings;

unsigned square(unsigned number) {
	return number * number;
}

void process_input(string s) {
	if (s == "p") {
		py_functions::interact();
	} else {
		s = py_functions::invoke_callbacks(s);
		inputstrings.push_back(s);
		cout << "processed input: " << s << endl;
	}
}

int main(Args args) {
	try {
		cout << "exctest returned " << py_functions::exctest(1337) << endl;
	} catch (runtime_error &e) {
		cout << "exception during exctest: " << e.what() << endl;
	}

	py_functions::print_square(args.thatnumber);

	cout << "type p to enter interactive python interp, anything else to add strings" << endl;

	while (true) {
		string l;

		cout << "> " << flush;
		getline(cin, l);

		if (l.size() == 0) {
			cout << "kthxbai" << endl;
			break;
		}

		try {
			test::process_input(l);
		} catch (runtime_error &e) {
			cout << "error processing line: " << e.what() << endl;
		}
	}

	return 0;
}

} // namespace test
