#ifndef RPN_HPP
#define RPN_HPP

#include <stack>

class RPN {
private:
	std::stack< float > _Stack;

public:
	RPN();
	RPN(RPN const& src);
	~RPN();

	RPN& operator=(RPN const& rhs);

	void init(char* str);
};

#endif /* ************************************************************* RPN_H */