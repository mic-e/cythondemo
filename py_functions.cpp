#include "py_functions.h"

#include <iostream>
#include <string>

using namespace std;

namespace test {
namespace py_functions {

void (*print_square)(int) = [](int){
	cout << "py_print_square fallback function" << endl;
};

void (*interact)() = [](){
	cout << "py_interact fallback function" << endl;
};

std::string (*invoke_callbacks)(std::string) = [](std::string) -> std::string {
	cout << "py_invoke_callbacks fallback function" << endl;
	return "";
};

}} // namespace test::py_functions