#include "BitcoinExchange.hpp"

#include <algorithm>
#include <cstddef>
#include <cstring>
#include <fstream>
#include <iostream>
#include <string>

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

BitcoinExchange::BitcoinExchange() {
	std::cout << "Constructor called." << std::endl;
}

BitcoinExchange::~BitcoinExchange() {
	std::cout << "Destructor called." << std::endl;
}

BitcoinExchange::BitcoinExchange(const BitcoinExchange& src) {
	*this = src;
}

/*
** --------------------------------- OVERLOAD ---------------------------------
*/

BitcoinExchange&
BitcoinExchange::operator=(BitcoinExchange const& rhs) {
	if (this != &rhs) {
		this->_myMap = rhs.getMyMap();
	}
	return *this;
}

/*
** --------------------------------- METHODS ----------------------------------
*/

void checkDate(std::string beforePipe) {
	// split this into 3 values, year month day and check if less than or greater than
	// unsigned int year         = 0;
	// unsigned int month        = 0;
	// unsigned int day          = 0;
	size_t count = 0;

	std::cout << beforePipe << std::endl;
	for (size_t i = 0; i < beforePipe.size(); i++) {
		if (beforePipe[i] == '-') {
			count++;
		}
	}
	std::cout << count << std::endl;
}

void BitcoinExchange::isInputFileCorrect(std::ifstream& input) {
	std::string line;
	std::string beforePipe;
	// float       afterPipe = NAN;
	while (std::getline(input, line) != 0) {
		size_t delimiterPos = line.find("|");
		if (delimiterPos != std::string::npos) {
			beforePipe = line.substr(0, delimiterPos);
			checkDate(beforePipe);
			// std::cout << beforePipe << std::endl;
		}
		std::cout << _myMap[beforePipe] << std::endl;
	}
}

/*
** --------------------------------- GETTER/SETTER ---------------------------------
*/
std::map<std::string, float> BitcoinExchange::getMyMap() const {
	return _myMap;
}

/* ************************************************************************** */