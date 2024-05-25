#ifndef BITCOINEXCHANGE_HPP
# define BITCOINEXCHANGE_HPP

# include <iostream>
# include <string>

class BitcoinExchange
{
	private:
	public:

		BitcoinExchange();
		BitcoinExchange( BitcoinExchange const & src );
		~BitcoinExchange();

		BitcoinExchange &operator=( BitcoinExchange const & rhs );
		bool	isInputFileCorrect(std::ifstream& input);

};

#endif /* ************************************************* BITCOINEXCHANGE_H */