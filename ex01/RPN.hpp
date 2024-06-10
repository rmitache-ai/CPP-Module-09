#ifndef RPN_HPP
#define RPN_HPP

#include <iostream>
#include <stack>

class RPN {

public:
	RPN();
	RPN(RPN const& src);
	~RPN();

	RPN& operator=(RPN const& rhs);

	static void init(char* str);

private:
	std::stack<float> _Stack;
};

std::ostream& operator<<(std::ostream& o, RPN const& i);

#endif /* ************************************************************* RPN_H */