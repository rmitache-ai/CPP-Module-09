#include "BitcoinExchange.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

BitcoinExchange::BitcoinExchange(){
	std::cout << "Constructor called." << std::endl;
}

BitcoinExchange::~BitcoinExchange(){
	std::cout << "Destructor called." << std::endl;
}


BitcoinExchange::BitcoinExchange( const BitcoinExchange & src )
{
	(void)src;
}


/*
** --------------------------------- OVERLOAD ---------------------------------
*/

BitcoinExchange &				BitcoinExchange::operator=( BitcoinExchange const & rhs )
{
	// if ( this != &rhs )
	// {
	// 	this->_file = rhs._file;
	// }
	(void)rhs;
	return *this;
}


/*
** --------------------------------- METHODS ----------------------------------
*/

#include <string>
#include <fstream>

bool	BitcoinExchange::isInputFileCorrect(std::ifstream& input){
	std::string myFile;
	
	std::getline(input, myFile);

	std::cout << myFile << std::endl;
	return true;
}


/*
** --------------------------------- ACCESSOR ---------------------------------
*/


/* ************************************************************************** */