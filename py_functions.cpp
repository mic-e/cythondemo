#include "py_functions.h"

#include <iostream>
#include <string>

using namespace std;

namespace test {
namespace py_functions {

void (*print_square)(int) = [](int){
	cout << "py_print_square dummy function" << endl;
};

void (*interact)() = [](){
	cout << "py_interact dummy function" << endl;
};

int (*exctest)(int) = [](int) {
	cout << "py_exctest dummy function" << endl;
	return 0;
};

std::string (*invoke_callbacks)(std::string) = [](std::string) -> std::string {
	cout << "py_invoke_callbacks dummy function" << endl;
	return "";
};

}} // namespace test::py_functions
