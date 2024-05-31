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
	const int FIR_DATE_DELIMITER = 4;
	const int SEC_DATE_DELIMITER = 7;
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

	short year  = -1;
	short month = -1;
	short day   = -1;

	std::stringstream convertToNumber;

	const unsigned short YEAR_START_POS = 0;
	const unsigned short YEAR_END_POS   = 4;
	convertToNumber << beforePipe.substr(YEAR_START_POS,
										 YEAR_END_POS);
	convertToNumber >> year;
	convertToNumber.clear();
	convertToNumber.str("");

	const unsigned short MONTH_START_POS = 6;
	const unsigned short MONTH_END_POS   = 7;
	convertToNumber << beforePipe.substr(MONTH_START_POS,
										 MONTH_END_POS);
	convertToNumber >> month;
	convertToNumber.clear();
	convertToNumber.str("");

	const unsigned short DAY_START_POS = 8;
	const unsigned short DAY_END_POS   = 9;
	convertToNumber << beforePipe.substr(DAY_START_POS,
										 DAY_END_POS);
	convertToNumber >> day;
	convertToNumber.clear();
	convertToNumber.str("");
}

void trimStartEnd(std::string& beforePipe) {
	beforePipe.erase(std::remove_if(beforePipe.begin(),
									beforePipe.end(), ::isspace),
					 beforePipe.end());
}

int countPipes(std::ifstream& input) {
	size_t      count = 0;
	std::string s;
	std::getline(input, s);

	for (size_t i = 0; i < s.size(); i++) {
		if (s[i] == '|' && (isdigit(s[i + 1]) == 0)) {
			count++;
		}
	}
	return count;
}

void BitcoinExchange::isInputFileCorrect(std::ifstream& input) {
	std::string line;
	std::string beforePipe;
	std::string afterPipe;

	if (countPipes(input) != 1) {
		std::cout << "Error: bad input => " << input
				  << std::endl;
	}
	while (std::getline(input, line) != 0) {
		size_t delimiterPos = line.find("|");
		if (delimiterPos != std::string::npos) {
			beforePipe = line.substr(0, delimiterPos);
			trimStartEnd(beforePipe);
			checkDate(beforePipe);
		}
		afterPipe = line.substr(beforePipe.size() + 1);
		std::cout << "After:  " << afterPipe << std::endl;
	}
}

/*
** --------------------------------- GETTER/SETTER ---------------------------------
*/
std::map<std::string, float> BitcoinExchange::getMyMap() const {
	return _myMap;
}

/* ************************************************************************** */