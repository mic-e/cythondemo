#ifndef TEST_H_
#define TEST_H_

#include <vector>
#include <string>

#include "Python.h"

namespace test {

void add_callback(PyObject *fun);

class Args {
public:
	int exit;
	int exitcode;

	int thatnumber;
};

/**
 * squares the given number.
 */
unsigned square(unsigned number);

extern std::vector<std::string> inputstrings;

} // namespace test

#endif
