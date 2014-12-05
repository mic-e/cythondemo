#ifndef MAIN_H_
#define MAIN_H_

#include <vector>
#include <string>

/* pxd:
 *
 * from libcpp.vector cimport vector
 * from libcpp.string cimport string
 */

namespace test {

/* pxd:
 *
 * struct Args:
 *     int thatnumber
 */
class Args {
public:
	int thatnumber;
};

/**
 * squares the given number.
 *
 * pxd: unsigned square(unsigned number)
 */
unsigned square(unsigned number);

// pxd: vector[string] inputstrings
extern std::vector<std::string> inputstrings;

// pxd: int main(Args args) except+
int main(Args args);

} // namespace test
#endif
