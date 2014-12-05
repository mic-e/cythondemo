#include "py_functions.h"

#include <iostream>
#include <string>

using namespace std;

namespace test {
namespace py_functions {

void throw_from_py_err() {
	PyErr_Clear();
	throw runtime_error("PyErr_Occured");
}

PyFunc<int, int> print_square;
PyFunc<int> interact;
PyFunc<int, int> exctest;
PyFunc<std::string, std::string> invoke_callbacks;

}} // namespace test::py_functions
