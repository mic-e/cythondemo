#ifndef PY_FUNCTIONS_H_
#define PY_FUNCTIONS_H_

#include <string>
#include <stdexcept>

#include <Python.h>

// pxd: from libcpp.string cimport string

namespace test {
namespace py_functions {

void throw_from_py_err();

template<typename ReturnType, typename ...ArgTypes>
class PyFunc {
private:
	ReturnType (*ptr)(ArgTypes ...) = nullptr;

public:
	void operator =(ReturnType (*ptr)(ArgTypes ...)) {
		this->ptr = ptr;
	}

	ReturnType operator ()(ArgTypes ...args);
};

template<typename ReturnType, typename ...ArgTypes>
ReturnType PyFunc<ReturnType, ArgTypes...>::operator ()(ArgTypes ...args) {
	if (this->ptr == nullptr) {
		throw std::runtime_error("function ptr is nullptr");
	}

	ReturnType result = this->ptr(std::forward<ArgTypes>(args)...);

	if (PyErr_Occurred()) {
		throw_from_py_err();
	}

	return result;
}

template<typename ...ArgTypes>
void PyFunc<void, ArgTypes...>::operator ()(ArgTypes ...args) {
	if (this->ptr == nullptr) {
		throw std::runtime_error("function ptr is nullptr");
	}

	this->ptr(std::forward<ArgTypes>(args)...);

	if (PyErr_Occurred()) {
		throw_from_py_err();
	}
}

/**
 * prints the square of a given number, using python .format() and test::square()
 *
 * pxd: int (*print_square)(int) except*
 */
extern PyFunc<int, int> print_square;

/**
 * launches the interactive console
 *
 * pxd: int (*interact)() except*
 */
extern PyFunc<int> interact;

/**
 * test function that raises a Python exception
 *
 * pxd: int (*exctest)(int) except*
 */
extern PyFunc<int, int> exctest;

/**
 * invokes all string callbacks
 *
 * pxd: string (*invoke_callbacks)(string) except*
 */
extern PyFunc<std::string, std::string> invoke_callbacks;

}} // namespace test::py_functions

#endif
