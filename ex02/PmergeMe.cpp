#include "PmergeMe.hpp"

#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <unistd.h>
#include <utility>
#include <vector>

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

PmergeMe::PmergeMe()
	: _vector(0)
	, _vectorPaired(0)
	, _vecHighEnd(0)
	, _vecLowEnd(0) {}

PmergeMe::PmergeMe(int argc, char** argv) {
	const double MICROSECONDS_PER_SECOND = 1000000.0;
	if (argc < 3) {
		throw atLeastTwoNumbersNeeded();
	}
	for (int i = 1; i < argc; i++) {
		char* endPtr = NULL;
		long  number = std::strtol(argv[i], &endPtr, BASE);
		if (*endPtr != '\0') {
			throw nonNumeric();
		}
		if (number < 0) {
			throw positiveNumbersOnly();
		}
		_vector.push_back(number);
		// _deque.push_back(number);
	}
	displayBefore();
	clock_t timeVec = 0;
	timeVec         = clock();
	mergeInsertSortVector();
	displayAfter();
	timeVec = clock() - timeVec;
	std::cout << "Time to process a range of " << argc - 1
			  << " elements with std::vector : "
			  << (double)timeVec * MICROSECONDS_PER_SECOND
					 / CLOCKS_PER_SEC
			  << " us" << std::endl;
}

PmergeMe::PmergeMe(PmergeMe const& src) { (void)src; }

/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

PmergeMe::~PmergeMe() {}

/*
** --------------------------------- OVERLOAD ---------------------------------
*/

PmergeMe& PmergeMe::operator=(PmergeMe const& rhs) {
	(void)rhs;
	//if ( this != &rhs )
	//{
	//this->_value = rhs.getValue();
	//}
	return *this;
}

std::ostream& operator<<(std::ostream& o, PmergeMe const& i) {
	//o << "Value = " << i.getValue();
	(void)i;
	(void)o;
	return o;
}

/*
** --------------------------------- METHODS ----------------------------------
*/

void PmergeMe::displayBefore() {
	std::cout << "Before: ";
	;

	for (std::vector< size_t >::iterator it = _vector.begin();
		 it != _vector.end(); it++) {
		std::cout << *it << " ";
	}
	std::cout << std::endl;
}

void PmergeMe::displayAfter() {
	std::cout << "After:  ";
	;

	for (std::vector< int >::iterator it = _vecHighEnd.begin();
		 it != _vecHighEnd.end(); it++) {
		std::cout << *it << " ";
	}
	std::cout << std::endl;
}

void PmergeMe::splitVectorIntoPairs() {
	std::vector< int >::size_type size      = _vector.size();
	std::vector< int >::size_type pairCount = size / 2;

	for (std::vector< int >::size_type i = 0; i < pairCount;
		 ++i) {
		_vectorPaired.push_back(
			std::make_pair(_vector[i * 2], _vector[i * 2 + 1]));
	}
	if (size % 2 != 0) {
		_vectorPaired.push_back(
			std::make_pair(_vector[size - 1], -1));
	}
}

void PmergeMe::sortVectorFromPairs() {
	std::vector< std::pair< int, int > >::iterator it;
	for (it = _vectorPaired.begin(); it != _vectorPaired.end();
		 it++) {
		if (it->first > it->second) {
			std::swap(it->first, it->second); // USE OWN?
		}
	}
}

static void merge(std::vector< int >& vec, int left, int mid,
				  int right) {
	int i;
	int j;
	int k;
	int n1 = mid - left + 1;
	int n2 = right - mid;

	// Create temporary arrays
	int L[n1];
	int R[n2];

	for (i = 0; i < n1; i++)
		L[i] = vec[left + i];
	for (j = 0; j < n2; j++)
		R[j] = vec[mid + 1 + j];

	i = 0;
	j = 0;
	k = left;
	while (i < n1 && j < n2) {
		if (L[i] <= R[j]) {
			vec[k] = L[i];
			i++;
		} else {
			vec[k] = R[j];
			j++;
		}
		k++;
	}

	while (i < n1) {
		vec[k] = L[i];
		i++;
		k++;
	}

	while (j < n2) {
		vec[k] = R[j];
		j++;
		k++;
	}
}

void PmergeMe::mergeSortVector(std::vector< int >& vec, int left,
							   int right) {
	if (left < right) {
		int mid = left + (right - left) / 2;

		mergeSortVector(vec, left, mid);
		mergeSortVector(vec, mid + 1, right);
		merge(vec, left, mid, right);
	}
}

std::vector< int > generateJSeq(int n) {
	std::vector< int > jacobsthal;
	jacobsthal.push_back(0);
	if (n >= 1) {
		jacobsthal.push_back(1);
	}
	if (n >= 2) {
		jacobsthal.push_back(3);
	}
	for (int i = 3; i < n; ++i) {
		jacobsthal.push_back(jacobsthal[i - 1]
							 + 2 * jacobsthal[i - 2]);
	}
	return jacobsthal;
}

void PmergeMe::sortByHighEndAndLowEnd() {
	std::vector< std::pair< int, int > >::iterator it;
	for (it = _vectorPaired.begin(); it != _vectorPaired.end();
		 it++) {
		_vecHighEnd.push_back(it->second);
		_vecLowEnd.push_back(it->first);
	}

	mergeSortVector(_vecHighEnd, 0, _vecHighEnd.size() - 1);
	mergeSortVector(_vecLowEnd, 0, _vecLowEnd.size() - 1);

	if (!_vecLowEnd.empty() && _vecLowEnd[0] == -1) {
		_vecLowEnd.erase(_vecLowEnd.begin());
	} else if (!_vecHighEnd.empty() && _vecHighEnd[0] == -1) {
		_vecHighEnd.erase(_vecHighEnd.begin());
	}
}

void PmergeMe::insertUsingJacobSequence() {
	std::vector< int > jacobsthal
		= generateJSeq(_vecLowEnd.size());

	for (size_t i = 0;
		 i < jacobsthal.size() && i < _vecLowEnd.size(); ++i) {
		size_t idx = jacobsthal[i];
		if (idx >= _vecLowEnd.size()) {
			break;
		}
		std::vector< int >::iterator insertPos
			= std::upper_bound(_vecHighEnd.begin(),
							   _vecHighEnd.end(),
							   _vecLowEnd[idx]);
		_vecHighEnd.insert(insertPos, _vecLowEnd[idx]);
	}
}
void PmergeMe::mergeInsertSortVector() {
	splitVectorIntoPairs();
	sortVectorFromPairs();
	sortByHighEndAndLowEnd();
	insertUsingJacobSequence();
}

/*
** --------------------------------- ACCESSOR ---------------------------------
*/

const char* PmergeMe::atLeastTwoNumbersNeeded::what() const
	throw() {
	return "Error: At least 2 numbers needed for sort.";
}

const char* PmergeMe::positiveNumbersOnly::what() const throw() {
	return "Error: Please, as an Input use only positive "
		   "numbers";
}

const char* PmergeMe::nonNumeric::what() const throw() {
	return "Error: Please, use numbers only";
}

/* ************************************************************************** */