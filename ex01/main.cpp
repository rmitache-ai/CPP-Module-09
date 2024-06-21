#include "RPN.hpp"
#include <iostream>

int main(int argc, char** argv) {
	if (argc != 2) {
		std::cerr << "Error" << std::endl;
		return 1;
	}
	RPN rpn;

	if (argv[1] != NULL) {
		rpn.init(argv[1]);
	} else {
		std::cerr << "Error: Argument is NULL" << std::endl;
		return 1;
	}
	return 0;
}
