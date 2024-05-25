#include "BitcoinExchange.hpp"
#include <iostream>
#include <fstream>


int main(int argc, char **argv){
	std::ifstream input(argv[1]);
	if (argc != 2 || !input.is_open()){
		std::cerr << "Error: could not open file." << std::endl;
		return 1;
	}
	BitcoinExchange myExchange;

	if (!myExchange.isInputFileCorrect(input)){
		input.close();
		return 1;
	}
	return 0;
}