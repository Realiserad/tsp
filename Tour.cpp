#include "Tour.hpp"
#include <iostream>
#include <stdexcept>

void Tour::create_index(int size) {
	_index = new int[size];
	for (int i = 0; i < size; ++i) {
		int elem = _tour[i];
		_index[elem] = i;
	}
}

void Tour::copy_construct(const Tour &t) {
	_size = t.size();
	_tour = new int[t.size()];
	
	for (int i = 0; i < t.size(); ++i) {
		_tour[i] = t._tour[i];
	}
	
	create_index(t.size());
}

void Tour::move_construct(Tour &res) {
	_tour = res._tour;
	_size = res._size;
	_index = res._index;
	
	res._tour = new int[0];
	res._index = new int[0];
	res._size = 0;
}

// Destruct
Tour::~Tour() {
	delete[] _tour;
	delete[] _index;
	// Sanity check
	_tour = nullptr;
	_index = nullptr;
}

// Construct
Tour::Tour(int *tour, int size) {
	_size = size;
	_tour = tour;
	create_index(size);
}

// Copy-construct
Tour::Tour(const Tour &t) {
	copy_construct(t);
}

// Copy-assign
Tour& Tour::operator=(Tour &t) {
	delete[] _tour;
	delete[] _index;
	copy_construct(t);
	return *this;
}

// Move-construct
Tour::Tour(Tour &&res) {
	move_construct(res);
}

/// Returns the number of cities in this tour.
/// @complexity O(1)
int Tour::size() const {
	return _size;
}

// Move-assign
Tour& Tour::operator=(Tour &&res) {
	delete[] _tour;
	delete[] _index;
	move_construct(res);
	return *this;
}

/// Swap two cities in the tour.
/// @param a First city to swap
/// @param b Second city to swap
/// @complexity O(1)
void Tour::swap(int a, int b) {
	int city_a = _tour[a];
	int city_b = _tour[b];
	
	int tmp0 = _tour[a];
	_tour[a] = _tour[b];
	_tour[b] = tmp0;
	
	int tmp1 = _index[city_a];
	_index[city_a] = _index[city_b];
	_index[city_b] = tmp1;
}

/// Compute the length of this tour.
/// @param d Distance matrix
/// @complexity O(n)
double Tour::length(double **d) const {
	double distance = 0;
	for (int i = 0; i < _size-1; ++i) {
		int from = _tour[i];
		int to = _tour[i+1];
		distance += d[from][to];
	}
	int from = _tour[_size-1];
	int to = _tour[0];
	distance += d[from][to];
	return distance;
}

/// Transforms the tour from an index-based to an order based 
/// representation. That is, before transformation A[i] contained
/// the the next city to visit after i, and after transformation
/// A[i] contains the i:th city to cisit.
/// @complexity O(n)
void Tour::transform() {
	int *v = new int[_size];
	
	int pos = 0, next = 0;
	do {
		v[pos] = _tour[next];
		next = _tour[next];
		pos++;
	} while (pos < _size);
	
	delete[] _tour;
	_tour = v;
	
	delete[] _index;
	create_index(_size);
}

/// Prints the tour to standard out.
/// @complexity O(n)
void Tour::print() const {
	for (int i = 0; i < _size; ++i) 
		std::cout << _tour[i] << std::endl;
}

/// Returns the city at index i.
int Tour::operator[](int i) const {
	if (i < 0 || i > _size)
		throw std::out_of_range("operator[] out of range.");
	if (i == _size) 
		return _tour[0];
	return _tour[i];
}

// Set a city in the tour directly. You need to
// rebuild the index using fix_index after invoking
// this method.
void Tour::set(int index, int value) {
	_tour[index] = value;
	_index[value] = index;
}

/// Calculates the index of the city specified.
/// @param i The city to search for
/// @complexity O(1)
int Tour::index_of(int city) const {
	if (city < 0 || city >= _size)
		throw std::out_of_range("index_of out of range");
		
	return _index[city]; 
}
