#include "RPN.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

RPN::RPN() {}

RPN::RPN(const RPN& src) {}

/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

RPN::~RPN() {}

/*
** --------------------------------- OVERLOAD ---------------------------------
*/

RPN& RPN::operator=(RPN const& rhs) {
	if (this != &rhs) {
		*this = rhs;
	}
	return *this;
}

std::ostream& operator<<(std::ostream& o, RPN const& i) {
	//o << "Value = " << i.getValue();
	return o;
}

/*
** --------------------------------- METHODS ----------------------------------
*/

void RPN::init(char* str) {
	while (*str++) {
		std::cout << *str << std::endl;
	}
}

/*
** --------------------------------- ACCESSOR ---------------------------------
*/

/* ************************************************************************** */