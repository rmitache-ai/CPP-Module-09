#include "BitcoinExchange.hpp"

#include <algorithm>
#include <cctype>
#include <cstddef>
#include <cstring>
#include <fstream>
#include <iostream>
#include <sstream>
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

bool isValidDateFormat(const std::string& date) {
	const int SEC_DATE_DELIMITER = 7;
	const int FIR_DATE_DELIMITER = 4;
	const int DATE_FORMAT_LENGHT = 10;

	if (date.size() != DATE_FORMAT_LENGHT) {
		return false;
	}
	if (date[FIR_DATE_DELIMITER] != '-'
		|| date[SEC_DATE_DELIMITER] != '-') {
		return false;
	}
	for (int i = 0; i < DATE_FORMAT_LENGHT; i++) {
		if (i == FIR_DATE_DELIMITER || i == SEC_DATE_DELIMITER) {
			continue;
		}
		if (isdigit(date[i]) == 0) {
			return false;
		}
	}
	return true;
}

void checkDate(std::string beforePipe) {
	if (!isValidDateFormat(beforePipe)) {
		std::cerr << "Error: bad input: " << beforePipe
				  << std::endl;
		return;
	}

	short year  = 0;
	short month = 0;
	short day   = 0;

	std::stringstream convertToNumber;

	convertToNumber << beforePipe.substr(0, 4);
	convertToNumber >> year;
	convertToNumber.clear();
	convertToNumber.str("");

	convertToNumber << beforePipe.substr(5, 2);
	convertToNumber >> month;
	convertToNumber.clear();
	convertToNumber.str("");

	convertToNumber << beforePipe.substr(8, 2);
	convertToNumber >> day;
}

void trimStartEnd(std::string& beforePipe) {
	beforePipe.erase(std::remove_if(beforePipe.begin(),
									beforePipe.end(), ::isspace),
					 beforePipe.end());
}

void BitcoinExchange::isInputFileCorrect(std::ifstream& input) {
	std::string line;
	std::string beforePipe;
	// float       afterPipe = NAN;
	while (std::getline(input, line) != 0) {
		size_t delimiterPos = line.find("|");
		if (delimiterPos != std::string::npos) {
			beforePipe = line.substr(0, delimiterPos);
			trimStartEnd(beforePipe);
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