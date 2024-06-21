#ifndef BITCOINEXCHANGE_HPP
#define BITCOINEXCHANGE_HPP

#include <fstream>
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
	std::map< std::string, float > btc_table;

public:
	BitcoinExchange();
	BitcoinExchange(BitcoinExchange const& src);
	~BitcoinExchange();

	BitcoinExchange& operator=(BitcoinExchange const& rhs);
	void             isInputFileCorrect(std::ifstream& input);
	void
	outputAndCompareInputWithDb(std::string date, float btc,
								std::ifstream& btc_database);

	void makeCalculation(std::string& date, float value);
	void runCalculation(std::string& date, float value,
						std::ifstream& btc_database);

	std::map< std::string, float > getBtcMap() const;
};

#endif /* ************************************************* BITCOINEXCHANGE_H */