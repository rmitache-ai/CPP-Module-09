#ifndef PMERGEME_HPP
#define PMERGEME_HPP

#include <cstddef>
#include <deque>
#include <iostream>
#include <vector>

#define BASE 10

class PmergeMe {

private:
	std::vector< size_t >                _vector;
	std::vector< std::pair< int, int > > _vectorPaired;
	std::vector< int >                   _vecHighEnd;
	std::vector< int >                   _vecLowEnd;

	std::deque< size_t >                _deque;
	std::deque< std::pair< int, int > > _dequePaired;
	std::deque< int >                   _deqHighEnd;
	std::deque< int >                   _deqLowEnd;

public:
	PmergeMe();
	PmergeMe(int argc, char** argv);
	PmergeMe(PmergeMe const& src);
	~PmergeMe();

	PmergeMe& operator=(PmergeMe const& rhs);

	void displayBefore();
	void displayAfter();
	void mergeInsertSortVector();
	void splitVectorIntoPairs();
	void sortVectorFromPairs();
	void sortByHighEndAndLowEnd();
	void insertUsingJacobSequence();

	void mergeInsertSortDeque();
	void splitDequeIntoPairs();
	void sortDequeFromPairs();
	void sortByHighEndAndLowEndDeque();
	void insertUsingJacobSequenceDeque();

	void mergeSortVector(std::vector< int >& vec, int left,
						 int right);
	void mergeSortDeque(std::deque< int >& deq, int left,
						int right);
	class atLeastTwoNumbersNeeded : public std::exception {
	public:
		const char* what() const throw();
	};

	class positiveNumbersOnly : public std::exception {
	public:
		const char* what() const throw();
	};

	class nonNumeric : public std::exception {
	public:
		const char* what() const throw();
	};

	class duplicate : public std::exception {
	public:
		const char* what() const throw();
	};
};

std::ostream& operator<<(std::ostream& o, PmergeMe const& i);

#endif