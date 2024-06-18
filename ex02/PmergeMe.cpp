#include "PmergeMe.hpp"

#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <deque>
#include <iostream>
#include <limits>
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
	, _vecLowEnd(0)
	, _deque(0)
	, _dequePaired(0)
	, _deqHighEnd(0)
	, _deqLowEnd(0) {}

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
		if (std::find(_vector.begin(), _vector.end(), number)
			!= _vector.end()) {
			throw duplicate();
		}
		if (number > std::numeric_limits< int >::max()) {
			throw tooBigNumber();
		}
		_vector.push_back(number);
		_deque.push_back(number);
	}
	displayBefore();
	clock_t timeVec = 0;
	timeVec         = clock();
	mergeInsertSortVector();
	timeVec = clock() - timeVec;
	displayAfter();
	std::cout << "Time to process a range of " << argc - 1
			  << " elements with std::vector : "
			  << (double)timeVec * MICROSECONDS_PER_SECOND
					 / CLOCKS_PER_SEC
			  << " us" << std::endl;

	clock_t timeVec2 = 0;
	timeVec2         = clock();
	mergeInsertSortDeque();
	timeVec2 = clock() - timeVec2;
	std::cout << "Time to process a range of " << argc - 1
			  << " elements with std::deque : "
			  << (double)timeVec2 * MICROSECONDS_PER_SECOND
					 / CLOCKS_PER_SEC
			  << " us" << std::endl;
}

PmergeMe::PmergeMe(PmergeMe const& src) { *this = src; }

/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

PmergeMe::~PmergeMe() {}

/*
** --------------------------------- OVERLOAD ---------------------------------
*/

PmergeMe& PmergeMe::operator=(PmergeMe const& rhs) {
	(void)rhs;
	if (this != &rhs) {
		this->_deqHighEnd   = rhs._deqHighEnd;
		this->_deqLowEnd    = rhs._deqLowEnd;
		this->_dequePaired  = rhs._dequePaired;
		this->_deque        = rhs._deque;
		this->_vecHighEnd   = rhs._vecHighEnd;
		this->_vecLowEnd    = rhs._vecLowEnd;
		this->_vectorPaired = rhs._vectorPaired;
		this->_vector       = rhs._vector;
	}
	return *this;
}

/*
** --------------------------------- METHODS ----------------------------------
*/

void PmergeMe::displayBefore() {
	std::cout << "Before: ";

	size_t i = 0;
	if (_vector.size() >= 5) {
		while (i++ != 5) {
			std::cout << _vector[i] << " ";
		}
		std::cout << "[...]";
	} else {
		for (std::vector< size_t >::iterator it
			 = _vector.begin();
			 it != _vector.end(); it++) {
			std::cout << *it << " ";
		}
	}
	std::cout << std::endl;
}

void PmergeMe::displayAfter() {
	std::cout << "After:  ";

	size_t i = 0;
	if (_vecHighEnd.size() >= 5) {
		while (i++ != 5) {
			std::cout << _vecHighEnd[i] << " ";
		}
		std::cout << "[...]";
	} else {
		for (std::vector< int >::iterator it
			 = _vecHighEnd.begin();
			 it != _vecHighEnd.end(); it++) {
			std::cout << *it << " ";
		}
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

void PmergeMe::splitDequeIntoPairs() {
	std::deque< int >::size_type size      = _vector.size();
	std::deque< int >::size_type pairCount = size / 2;

	for (std::deque< int >::size_type i = 0; i < pairCount;
		 ++i) {
		_dequePaired.push_back(
			std::make_pair(_deque[i * 2], _deque[i * 2 + 1]));
	}
	if (size % 2 != 0) {
		_dequePaired.push_back(
			std::make_pair(_deque[size - 1], -1));
	}
}

void swapPairInt(std::pair< int, int >& pair) {
	if (pair.first > pair.second) {
		int temp    = pair.first;
		pair.first  = pair.second;
		pair.second = temp;
	}
}

void PmergeMe::sortVectorFromPairs() {
	for (std::vector< std::pair< int, int > >::iterator it
		 = _vectorPaired.begin();
		 it != _vectorPaired.end(); ++it) {
		swapPairInt(*it); // Call the custom swap function
	}
}

void PmergeMe::sortDequeFromPairs() {
	for (std::deque< std::pair< int, int > >::iterator it
		 = _dequePaired.begin();
		 it != _dequePaired.end(); ++it) {
		swapPairInt(*it); // Call the custom swap function
	}
}

static void merge(std::vector< int >& vec, int left, int mid,
				  int right) {
	int i;
	int j;
	int k;
	int n1 = mid - left + 1;
	int n2 = right - mid;

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

std::deque< int > generateJSeqDeq(int n) {
	std::deque< int > jacobsthal;
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

static void mergeDeque(std::deque< int >& deq, int left, int mid,
					   int right) {
	int i;
	int j;
	int k;
	int n1 = mid - left + 1;
	int n2 = right - mid;

	int L[n1];
	int R[n2];

	for (i = 0; i < n1; i++)
		L[i] = deq[left + i];
	for (j = 0; j < n2; j++)
		R[j] = deq[mid + 1 + j];

	i = 0;
	j = 0;
	k = left;
	while (i < n1 && j < n2) {
		if (L[i] <= R[j]) {
			deq[k] = L[i];
			i++;
		} else {
			deq[k] = R[j];
			j++;
		}
		k++;
	}

	while (i < n1) {
		deq[k] = L[i];
		i++;
		k++;
	}

	while (j < n2) {
		deq[k] = R[j];
		j++;
		k++;
	}
}

void PmergeMe::mergeSortDeque(std::deque< int >& deq, int left,
							  int right) {
	if (left < right) {
		int mid = left + (right - left) / 2;

		mergeSortDeque(deq, left, mid);
		mergeSortDeque(deq, mid + 1, right);
		mergeDeque(deq, left, mid, right);
	}
}

void PmergeMe::sortByHighEndAndLowEndDeque() {
	std::deque< std::pair< int, int > >::iterator it;
	for (it = _dequePaired.begin(); it != _dequePaired.end();
		 it++) {
		_deqHighEnd.push_back(it->second);
		_deqLowEnd.push_back(it->first);
	}

	mergeSortDeque(_deqHighEnd, 0, _deqHighEnd.size() - 1);
	mergeSortDeque(_deqLowEnd, 0, _deqLowEnd.size() - 1);

	if (!_deqLowEnd.empty() && _deqLowEnd[0] == -1) {
		_deqLowEnd.erase(_deqLowEnd.begin());
	} else if (!_deqHighEnd.empty() && _deqHighEnd[0] == -1) {
		_deqHighEnd.erase(_deqHighEnd.begin());
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

void PmergeMe::insertUsingJacobSequenceDeque() {
	std::deque< int > jacobsthal
		= generateJSeqDeq(_deqLowEnd.size());

	for (size_t i = 0;
		 i < jacobsthal.size() && i < _deqLowEnd.size(); ++i) {
		size_t idx = jacobsthal[i];
		if (idx >= _deqLowEnd.size()) {
			break;
		}
		std::deque< int >::iterator insertPos = std::upper_bound(
			_deqHighEnd.begin(), _deqHighEnd.end(),
			_deqLowEnd[idx]);
		_deqHighEnd.insert(insertPos, _deqLowEnd[idx]);
	}
}

void PmergeMe::mergeInsertSortVector() {
	splitVectorIntoPairs();
	sortVectorFromPairs();
	sortByHighEndAndLowEnd();
	insertUsingJacobSequence();
}

void PmergeMe::mergeInsertSortDeque() {
	splitDequeIntoPairs();
	sortDequeFromPairs();
	sortByHighEndAndLowEndDeque();
	insertUsingJacobSequenceDeque();
}

/*
** --------------------------------- EXCEPTIONS ---------------------------------
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

const char* PmergeMe::duplicate::what() const throw() {
	return "Error: Please, don't use duplicates";
}

const char* PmergeMe::tooBigNumber::what() const throw() {
	return "Error: Please, use smaller numbers";
}

/* ************************************************************************** */