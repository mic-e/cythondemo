#include "py_functions.h"

#include <iostream>
#include <string>

#include <Python.h>

using namespace std;

namespace test {
namespace py_functions {

std::string repr(PyObject *obj) {
	PyObject *str = PyObject_Repr(obj);
	PyObject *bytes = PyUnicode_AsEncodedString(str, "utf-8", "Error ~");

	std::string result = PyBytes_AS_STRING(bytes);

	Py_XDECREF(bytes);
	Py_XDECREF(str);

	return result;
}

std::string str(PyObject *obj) {
	PyObject *str = PyObject_Str(obj);
	PyObject *bytes = PyUnicode_AsEncodedString(str, "utf-8", "Error ~");

	std::string result = PyBytes_AS_STRING(bytes);

	Py_XDECREF(bytes);
	Py_XDECREF(str);

	return result;
}

void throw_py_err_as_cpp_exc() {
	if (!PyErr_Occurred()) {
		return;
	}

	PyObject *type = NULL, *value = NULL, *traceback = NULL;
	PyErr_Fetch(&type, &value, &traceback);

	std::string typerepr = str(type);
	std::string valrepr = str(value);

	Py_XDECREF(type);
	Py_XDECREF(value);
	Py_XDECREF(traceback);

	PyErr_Clear();

	throw runtime_error("python exception: " + typerepr + ": " + valrepr);
}

PyFunc<void, int> print_square;
PyFunc<void> interact;
PyFunc<int, int> exctest;
PyFunc<std::string, std::string> invoke_callbacks;

}} // namespace test::py_functions
