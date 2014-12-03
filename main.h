#ifndef MAIN_H_
#define MAIN_H_

#include <vector>
#include <string>

#include "Python.h"

/* pxd:
 *
 * from libcpp.vector cimport vector
 * from libcpp.string cimport string
 *
 */

namespace test {

class Args {
public:
	int thatnumber;
};

/* pxd:
 *
 * struct Args:
 *     int thatnumber
 */

/**
 * squares the given number.
 */
unsigned square(unsigned number);

// pxd: unsigned square(unsigned number)

extern std::vector<std::string> inputstrings;

// pxd: vector[string] inputstrings

int main(Args args);

// pxd: int main(Args args)

} // namespace test
#endif
