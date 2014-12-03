#ifndef PY_FUNCTIONS_H_
#define PY_FUNCTIONS_H_

#include <string>

namespace test {
namespace py_functions {

extern void (*print_square)(int);
extern void (*interact)();
extern std::string (*invoke_callbacks)(std::string);

}} // namespace test::py_functions

#endif
