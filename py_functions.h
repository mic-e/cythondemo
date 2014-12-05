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

	ReturnType operator ()(ArgTypes ...args) {
		if (this->ptr == nullptr) {
			throw std::runtime_error("function ptr is nullptr");
		}

		ReturnType result = this->ptr(std::forward<ArgTypes>(args)...);

		if (PyErr_Occurred()) {
			throw_from_py_err();
		}

		return result;
	}
};


template<typename ...ArgTypes>
class PyFunc<void, ArgTypes ...> {
private:
	void (*ptr)(ArgTypes ...) = nullptr;

public:
	void operator =(void (*ptr)(ArgTypes ...)) {
		this->ptr = ptr;
	}

	void operator ()(ArgTypes ...args) {
		if (this->ptr == nullptr) {
			throw std::runtime_error("function ptr is nullptr");
		}

		this->ptr(std::forward<ArgTypes>(args)...);

		if (PyErr_Occurred()) {
			throw_from_py_err();
		}
	}
};


/**
 * prints the square of a given number, using python .format() and test::square()
 *
 * pxd: void (*print_square)(int) except*
 */
extern PyFunc<void, int> print_square;

/**
 * launches the interactive console
 *
 * pxd: void (*interact)() except*
 */
extern PyFunc<void> interact;

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
