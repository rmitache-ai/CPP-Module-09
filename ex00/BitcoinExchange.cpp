#include "BitcoinExchange.hpp"

#include <map>
#include <math.h>

#include <algorithm>
#include <cctype>
#include <cstddef>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iostream>
#include <limits>
#include <sstream>
#include <string>

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

BitcoinExchange::BitcoinExchange()
	: _at(0), _error(false) {
	std::cout << "Constructor called." << std::endl;
}

BitcoinExchange::~BitcoinExchange() {
	std::cout << "Destructor called." << std::endl;
}

BitcoinExchange::BitcoinExchange(const BitcoinExchange& src)
	: _at(), _error() {
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

BitcoinExchange&
BitcoinExchange::operator+=(const BitcoinExchange& rhs) {

	this->_at += rhs.getAt();
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

bool isLeapYear(int year) {
	return (year % 4 == 0 && year % 100 != 0)
		|| (year % 400 == 0);
}

int getMaxDaysInMonth(int month, int year) {
	switch (month) {
	case 1:
		return January;
	case 2:
		return isLeapYear(year) ? 29 : February;
	case 3:
		return March;
	case 4:
		return April;
	case 5:
		return May;
	case 6:
		return June;
	case 7:
		return July;
	case 8:
		return August;
	case 9:
		return September;
	case 10:
		return October;
	case 11:
		return November;
	case 12:
		return December;
	default:
		return -1;
	}
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

	const unsigned short MONTH_START_POS = 5;
	const unsigned short MONTH_END_POS   = 6;
	convertToNumber << beforePipe.substr(MONTH_START_POS,
										 MONTH_END_POS);
	convertToNumber >> month;
	convertToNumber.clear();
	convertToNumber.str("");

	const unsigned short DAY_START_POS = 8;
	const unsigned short DAY_END_POS   = 10;
	convertToNumber << beforePipe.substr(DAY_START_POS,
										 DAY_END_POS);
	convertToNumber >> day;
	convertToNumber.clear();
	convertToNumber.str("");

	const int MAX_MONTH      = 12;
	int       maxDaysInMonth = getMaxDaysInMonth(month, year);
	if (day < 1 || day > maxDaysInMonth || month > MAX_MONTH
		|| month < 1) {
		std::cerr << "Error: bad input => " << beforePipe
				  << std::endl;
	}
}

void trimStartEnd(std::string& beforePipe) {
	beforePipe.erase(std::remove_if(beforePipe.begin(),
									beforePipe.end(), ::isspace),
					 beforePipe.end());
}

size_t countPipes(std::string s) {
	return std::count(s.begin(), s.end(), '|');
}

float convertToFloat(const std::string& str) {
	std::istringstream iss(str);
	float              result = NAN;
	if (!(iss >> result)) {
		return 0.0;
	}
	return result;
}

void checkValue(std::string afterPipe) {
	long checkForMaxInt = std::atol(afterPipe.c_str());
	if (checkForMaxInt >= std::numeric_limits<int>::max()) {
		std::cout << "Error: too large a number." << std::endl;
		return;
	}

	for (size_t i = 0; i < afterPipe.size(); i++) {
		if (isdigit(afterPipe[i]) == 0) {
			if (afterPipe[i] == '.') {
				continue;
			}
			std::cerr << "Error: not a positive number"
					  << std::endl;
			return;
		}
	}
}

// void updateToNearestDate(std::ifstream&          btc_database,
// 						 std::basic_string<char> date) {
// 	std::cout << "date not found\n";
// 	// Placeholder logic for updating to the nearest date
// 	// Replace with actual logic to find the nearest date
// 	date = "2024-01-01"; // Example nearest date
// 	(void)btc_database;
// }

void BitcoinExchange::checkIfDateExistsInDbElseUpdateToNearest(
	std::ifstream& btc_database) {
	std::string line;
	(void)btc_database;
	std::map<std::string, float>::iterator it  = _myMap.begin();
	std::map<std::string, float>::iterator ite = _myMap.end();

	size_t counter                             = 0;
	while (counter != _at) {
		it++;
		counter++;
	}
	--it;
	if (_error) {
		++it;
		_error = false;
	}
	while (it != ite) {
		std::cout << it->first << " with value of " << it->second
				  << std::endl;
		++it;
	}
	// updateToNearestDate(btc_database, it->first);
}

void BitcoinExchange::makeCalculation() {

	std::ifstream btc_database("data.csv");
	if (btc_database.is_open()) {
		checkIfDateExistsInDbElseUpdateToNearest(btc_database);
		btc_database.close();
	} else {
		std::cerr << "Failed to open data.csv" << std::endl;
	}
}

void BitcoinExchange::isInputFileCorrect(std::ifstream& input) {
	std::string line;
	std::string beforePipe;
	std::string afterPipe;

	while (std::getline(input, line) != 0) {
		if (countPipes(line) != 1) {
			std::cout << "Error: bad input => " << line
					  << std::endl;
			_error = true;
			continue;
		}
		size_t delimiterPos = line.find("|");
		if (delimiterPos != std::string::npos) {
			beforePipe = line.substr(0, delimiterPos);
			trimStartEnd(beforePipe);
			checkDate(beforePipe);
			if (delimiterPos + 1 >= line.size()) {
				std::cout << "Error: No Number found after pipe."
						  << std::endl;
				_error = true;
				continue;
			}
			afterPipe = line.substr(delimiterPos + 1);
			trimStartEnd(afterPipe);

			if (afterPipe.empty()) {
				std::cout << "Error: No Number found."
						  << std::endl;
				_error = true;
				continue;
			}
			if (atol(afterPipe.c_str())
					>= std::numeric_limits<int>::max()
				|| atol(afterPipe.c_str())
					   <= -std::numeric_limits<int>::max()) {
				std::cout << "Error: Too big of a number."
						  << std::endl;
				_error = true;
				continue;
			}
		}
		float value        = convertToFloat(afterPipe);
		_myMap[beforePipe] = value;
		makeCalculation();
		++_at;
	}
}

/*
** --------------------------------- GETTER/SETTER ---------------------------------
*/
std::map<std::string, float> BitcoinExchange::getMyMap() const {
	return _myMap;
}

size_t BitcoinExchange::getAt() const { return _at; }

/* ************************************************************************** */