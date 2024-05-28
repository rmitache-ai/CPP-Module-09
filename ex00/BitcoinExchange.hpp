#ifndef BITCOINEXCHANGE_HPP
#define BITCOINEXCHANGE_HPP

#include <fstream>
#include <map>
#include <string>


class BitcoinExchange {
private:
	std::map<std::string, float> _myMap;

public:
	BitcoinExchange();
	BitcoinExchange(BitcoinExchange const& src);
	~BitcoinExchange();

	BitcoinExchange& operator=(BitcoinExchange const& rhs);
	void             isInputFileCorrect(std::ifstream& input);

	std::map<std::string, float> getMyMap() const;
};

#endif /* ************************************************* BITCOINEXCHANGE_H */