#include "Tour.hpp"
#include <cstdlib>
#include <unordered_set>

/// An implementation of the nearest neighbour (NN) construction
/// algorithm for the TSP problem. NN is a greedy algorithm which
/// selects a random city as the start of the tour, and city i+1
/// as the unvisited city closest to i.
/// @param d The distance matrix
/// @param size The number of cities
/// @complexity O(n^2)
Tour* nearest_neighbour(double **d, int size) {
	// Will contain the actual tour such that
	// tour[n] contains the n:th city to visit
	int *tour = new int[size];
	int n = 0;
	// Use a set to keep track of visited cities
	std::unordered_set<int> visited;
	visited.reserve(size);
	// Start the tour in a random city
	int current = rand() % size;
	visited.insert(current);
	tour[n++] = current;
	while (static_cast<int>(visited.size()) < size) {
		int next = -1;
		for (int j = 0; j < size; ++j) {
			if (visited.count(j) == 0) {
				// The city is not yet visited
				if (next == -1) {
					// No next city defined yet
					next = j;
					continue;
				}
				if (d[current][j] < d[current][next]) {
					next = j;
				}
			}
		}
		tour[n++] = next;
		visited.insert(next);
		current = next;
	}
	Tour* t = new Tour(tour, size);
	return t;
}