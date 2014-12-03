#ifndef MAIN_H_
#define MAIN_H_

#include <vector>
#include <string>

#include "Python.h"

namespace test {

void add_callback(PyObject *fun);

class Args {
public:
	int thatnumber;
};

/**
 * squares the given number.
 */
unsigned square(unsigned number);

extern std::vector<std::string> inputstrings;

int main(Args args);

} // namespace test

#endif
