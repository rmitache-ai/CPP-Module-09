#include "RPN.hpp"

#include <cctype>
#include <cstdlib>
#include <math.h>
#include <stack>

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

RPN::RPN() {}

RPN::RPN(const RPN& src)
	: _Stack(src._Stack) {}
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
	(void)i;
	return o;
}

/*
** --------------------------------- METHODS ----------------------------------
*/

void RPN::init(char* str) {
	float tmp1 = NAN;
	float tmp2 = NAN;

	while (*str != 0) {
		if (std::isspace(*str) != 0) {
			str++;
			continue;
		}
		if (std::isdigit(*str) != 0) {
			_Stack.push(static_cast<float>(atof(str)));
			str++;
			continue;
		}
		if (_Stack.size() < 2) {
			std::cerr << "Error" << std::endl;
			return;
		}
		tmp2 = _Stack.top();
		_Stack.pop();
		tmp1 = _Stack.top();
		_Stack.pop();
		switch (*str) {
		case '+':
			_Stack.push(tmp1 + tmp2);
			break;
		case '-':
			_Stack.push(tmp1 - tmp2);
			break;
		case '*':
			_Stack.push(tmp1 * tmp2);
			break;
		case '/':
			if (tmp2 == 0) {
				std::cerr << "Error: Cannot devide by zero"
						  << std::endl;
			}
			_Stack.push(tmp1 / tmp2);
			break;
		default:
			break;
		}
		str++;
	}
	if (_Stack.size() != 1) {
		std::cerr << "Error\n";
		return;
	}
	std::cout << _Stack.top() << std::endl;
}

/*
** --------------------------------- ACCESSOR ---------------------------------
*/

/* ************************************************************************** */