#ifndef PY_FUNCTIONS_H_
#define PY_FUNCTIONS_H_

#include <string>

// pxd: from libcpp.string cimport string

namespace test {
namespace py_functions {

extern void (*print_square)(int);
extern void (*interact)();
extern std::string (*invoke_callbacks)(std::string);

/* pxd:
 *
 * void (*print_square)(int)
 * void (*interact)()
 * string (*invoke_callbacks)(string)
 */

}} // namespace test::py_functions

#endif
