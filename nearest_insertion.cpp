#define NDEBUG

#include "Tour.hpp"
#include <cfloat>
#include <assert.h>
#include <cstdlib>

/// Inserts the city i into the tour starting at the city specified,
/// such that the quantity d[a][i] + d[i][b] - d[a][b] is minimised.
/// @param i The city to insert
/// @param start The first city in the tour
/// @param tour The partial tour
/// @param d The distance matrix
void insert(int i, int start, int *tour, double **d) {
	double min = DBL_MAX;
	int a = -1, b = -1, c0 = start, c1 = tour[start];
	do {
		// Calculate the cost of inserting i between c0 and c1
		double cost = d[c0][i] + d[i][c1] - d[c0][c1];
		if (cost < min) {
			min = cost;
			a = c0;
			b = c1;
		}
		// Update c0 and c1
		c0 = c1;
		c1 = tour[c1];
	} while(c0 != start);
	// Insert i between a and b
	tour[a] = i;
	tour[i] = b;
}

/// Constructs a TSP tour using nearest insertion.
/// @param d The distance matrix
/// @param size The number of cities
/// @complexity O(n^2)
Tour* nearest_insertion(double **d, int size) {
	if (size < 4) {
		int *tour = new int[size];
		for (int i = 0; i < size; ++i)
			tour[i] = i;
		Tour *t = new Tour(tour, size);
		return t;
	}
	
	// tour[i] should contain the next city to visit after i
	int *tour = new int[size];
	
	// Randomly pick a triangle of three (unique) cities as the inital tour
	int r0, r1, r2;
	r0 = rand() % size;
	r1 = rand() % size;
	if (r1 == r0)
		r1 = (r1 + 1) % size;
	r2 = rand() % size;
	while (r2 == r1 || r2 == r0)
		r2 = (r2 + 1) % size;
	tour[r0] = r1;
	tour[r1] = r2;
	tour[r2] = r0;
	
	// Insert the rest of the cities into the tour
	for (int i = 0; i < size; ++i) {
		if (i != r0 && i != r1 && i != r2) {
			insert(i, r0, tour, d);
		}
	}
	
	// Encapsulate the tour array into a tour object
	Tour *t = new Tour(tour, size);
	t->transform();
	return t;
}
