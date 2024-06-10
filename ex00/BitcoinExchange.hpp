#ifndef BITCOINEXCHANGE_HPP
#define BITCOINEXCHANGE_HPP

#include <cstddef>
#include <fstream>
#include <iostream>
#include <map>
#include <string>

enum Month {
	January   = 31,
	February  = 28,
	March     = 31,
	April     = 30,
	May       = 31,
	June      = 30,
	July      = 31,
	August    = 31,
	September = 30,
	October   = 31,
	November  = 30,
	December  = 31,
};

class BitcoinExchange {
private:
	std::map<std::string, float> _myMap;
	size_t                       _at;
	bool                         _error;

public:
	BitcoinExchange();
	BitcoinExchange(BitcoinExchange const& src);
	~BitcoinExchange();

	BitcoinExchange& operator=(BitcoinExchange const& rhs);
	BitcoinExchange& operator+=(const BitcoinExchange& rhs);
	void             isInputFileCorrect(std::ifstream& input);
	void             insertWithUniqueKey(const std::string& key,
										 float              value);
	std::map<std::string, float> getMyMap() const;
	size_t                       getAt() const;
	void                         makeCalculation();
	void checkIfDateExistsInDbElseUpdateToNearest(
		std::ifstream& btc_database);
};

#endif /* ************************************************* BITCOINEXCHANGE_H */