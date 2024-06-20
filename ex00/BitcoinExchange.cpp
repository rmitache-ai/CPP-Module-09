#include "BitcoinExchange.hpp"

#include <cmath>
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
	: _at(0), _error(false), _errorNoData(false) {}

BitcoinExchange::~BitcoinExchange() {}

BitcoinExchange::BitcoinExchange(const BitcoinExchange& src)
	: _at(), _error(), _errorNoData() {
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

bool checkDate(std::string beforePipe) {

	if (!isValidDateFormat(beforePipe)) {
		std::cerr << "Error: bad input: " << beforePipe
				  << std::endl;

		return false;
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
		return false;
	}
	return true;
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

void BitcoinExchange::outputAndCompareInputWithDb(
	std::string date, float btc, std::ifstream& btc_database) {
	std::map< std::string, float > btc_table;
	bool                           firstIteration = true;
	std::string                    line;

	while (std::getline(btc_database, line) != 0) {
		if (firstIteration) {
			firstIteration = false;
			if (line != "date,exchange_rate") {
				std::cerr << "Error: Cannot find "
							 "'date,exchange_rate' in DB"
						  << std::endl;
				return;
			}
			continue;
		}
		std::istringstream iss(line);
		std::string        token;

		std::getline(iss, token, ',');
		std::string dateFromFile = token;

		std::getline(iss, token, ',');
		float btcValue
			= static_cast< float >(atof(token.c_str()));
		btc_table[dateFromFile] = btcValue;
	}
	std::map< std::string, float >::iterator it
		= btc_table.lower_bound(date);
	if (it != btc_table.end() && it->first == date) {
		float exchangeRate = it->second;
		float result       = btc * exchangeRate;
		std::cout << date << " => " << btc << " = " << result
				  << std::endl;
	} else {
		if (it != btc_table.begin()) {
			--it;
			float exchangeRate = it->second;
			float result       = btc * exchangeRate;
			std::cout << date << " => " << btc << " = " << result
					  << std::endl;
		} else {
			std::cerr << "Error: no bitcoin data => " << date
					  << std::endl;
			_errorNoData = true;
		}
	}
}
void BitcoinExchange::runCalculation(
	std::ifstream& btc_database) {
	if (_myMap.empty()) {
		std::cerr << "Error: _myMap is empty." << std::endl;
		return;
	}

	std::map< std::string, float >::iterator it = _myMap.begin();

	size_t counter                              = 0;
	while (counter < _at && it != _myMap.end()) {
		++it;
		++counter;
	}
	if (it != _myMap.begin()) {
		--it;
	}
	if (_errorNoData || _error) {
		_errorNoData = false;
		_error       = false;
		++it;
	}
	if (it != _myMap.end()) {
		outputAndCompareInputWithDb(it->first, it->second,
									btc_database);
	} else {
		std::cerr << "Error: Iterator out of range."
				  << std::endl;
	}
}

void BitcoinExchange::makeCalculation() {

	std::ifstream btc_database("data.csv");
	if (btc_database.is_open()) {
		runCalculation(btc_database);
		btc_database.close();
	} else {
		std::cerr << "Failed to open data.csv" << std::endl;
	}
}

bool checkForEmpty(std::string& afterPipe) {
	if (afterPipe.empty()) {
		std::cout << "Error: No Number found." << std::endl;
		return true;
	}
	return false;
}

bool checkForPositiveNumber(std::string& afterPipe) {

	if (atol(afterPipe.c_str()) < 0) {
		std::cout << "Error: not a positive number."
				  << std::endl;
		return true;
	}
	return false;
}

bool checkForBigNumber(std::string& afterPipe) {
	const double       MAX_BTC_INPUT = 1000.0;
	std::istringstream iss(afterPipe);
	double             value = NAN;
	iss >> value;
	if (value > MAX_BTC_INPUT) {
		std::cout << "Error: too large of a number."
				  << std::endl;
		return true;
	}
	return false;
}

void BitcoinExchange::isInputFileCorrect(std::ifstream& input) {
	std::string line;
	std::string beforePipe;
	std::string afterPipe;
	bool        firstIteration = true;

	while (std::getline(input, line) != 0) {
		if (firstIteration) {
			firstIteration = false;
			if (line != "date | value") {
				_error = true;
				std::cerr << "Error: No valid header for input"
						  << std::endl;
				return;
			}
			continue;
		}
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
			if (!checkDate(beforePipe)) {
				continue;
			}
			if (delimiterPos + 1 >= line.size()) {
				std::cout << "Error: No Number found after pipe."
						  << std::endl;
				_error = true;
				continue;
			}
			afterPipe = line.substr(delimiterPos + 1);
			trimStartEnd(afterPipe);

			if (checkForEmpty(afterPipe)
				|| checkForPositiveNumber(afterPipe)
				|| checkForBigNumber(afterPipe)) {
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
std::map< std::string, float >
BitcoinExchange::getMyMap() const {
	return _myMap;
}

size_t BitcoinExchange::getAt() const { return _at; }

/* ************************************************************************** */