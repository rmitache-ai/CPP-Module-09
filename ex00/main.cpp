#include "BitcoinExchange.hpp"
#include <fstream>
#include <iostream>

/* 
	Check input.
	Check if file is correctly written.
	Add values to a map.
	Check with csv file.
 */

int main(int argc, const char* argv[]) {
	std::ifstream input(argv[1]);
	if (argc != 2 || !input.is_open()) {
		std::cerr << "Error: could not open file." << std::endl;
		return 1;
	}

	BitcoinExchange myExchange;

	myExchange.isInputFileCorrect(input);

	return 0;
}
