// Define this to *disable* printf debugging and
// assertions. Comment out during debugging.
#define NDEBUG

#include "Tour.hpp"
#include <vector>
#include <algorithm>
#include <iostream>
#include <assert.h>
#include <cstring>
//#include <set>

// Compiles without errors using g++ 4.9.2
// g++ clarke-wright.cpp -c -std=c++11 -Wall
// Reference: http://www.seas.gwu.edu/~simhaweb/champalg/tsp/tsp.html

struct vertex_pair {
	int i;
	int j;
	double weight;
	bool used;
};

bool cmp_vertex_pair(vertex_pair a, vertex_pair b) {
	return a.weight > b.weight;
}

inline double savings(int h, double **d, int i, int j) {
	return d[h][i] + d[h][j] - d[i][j];
}

/// Checks if the tour contains a cycle after the cities
/// i and j have been connected, i.e if there exists a
/// path i to j.
inline double contains_cycle(int i, int j, int *tour) {
	int next = i;
	do {
		if (next == j)
			return true; // path i -> j detected
		next = tour[next];
	} while (next != -1);
	return false;
}

/// Find the endpoints of a non-cyclic partial tour.
/// The endpoints will be stored in end[0] and end[1].
void find_endpoints(int *end, short *degree, int size) {
	int i = 0;
	for (int j = 0; j < size; ++j) {
		if (degree[j] == 1) {
			end[i++] = j;
			if (i == 2)
				return;
		}
	}
	// Could not find two endpoints, error
	assert(false);
}

void __debug_vertex_pairs(std::vector<vertex_pair> pairs) {
#ifndef NDEBUG
	std::cerr << "{ ";
	for (size_t i = 0; i < pairs.size(); ++i) {
		std::cerr << pairs.at(i).weight;
		if (i < pairs.size() - 1)
			std::cerr << ", ";
	}
	std::cerr << " }" << std::endl;
#endif
}

/// Clarke-Wright heuristics for the TSP problem.
/// @param d The distance matrix
/// @param size The number of cities
/// @complexity O(n^3)
Tour* clarke_wright(double **d, int size) {
	// Clark-Wright cannot operate on problem instances of size < 4.
	assert(size > 0);
	if (size < 4) {
		int *tour = new int[size];
		for (int i = 0; i < size; ++i)
			tour[i] = i;
		Tour* t = new Tour(tour, size);
		return t;
	}
	
	// Select a random hub vertex
	int h = rand() % size;
	// Create data structure of unique vertex pairs from [0, size - 1] \ h
	std::vector<vertex_pair> pairs; // possibly switch for a tree?
	pairs.reserve(size);
	for (int i = 0; i < size; ++i) {
		for (int j = i + 1; j < size; ++j) {
			if (i != h && j != h) {
				// Add the pair i, j
				vertex_pair pair;
				pair.i = i;
				pair.j = j;
				pair.weight = savings(h, d, i, j);
				pair.used = false;
				pairs.push_back(pair);
			}
		}
	}
	// Sort vertex pairs in descending order of savings
	std::sort(pairs.begin(), pairs.end(), cmp_vertex_pair);
	__debug_vertex_pairs(pairs);
	assert(pairs.at(0).weight >= pairs.at(pairs.size() - 1).weight);
	
	// degree[i] will be zero if i not included in tour yet,
	// 1 if tour ends in i, or 2 if i is a transit city.
	short *degree = new short[size];
	for (int i=0; i<size; i++) degree[i] = 0;

	degree[h] = 2; // mark hub as transit city
	// Contains number of unvisited cities, h is considered
	// visited from start
	int unvisited = size - 1;
	// The actual tour such that tour[i] contains the next
	// city to visit after i
	int *tour = new int[size];
	// IMPORTANT! Wipe this sector to be able to detect cycles
	memset(tour, -1, size);
	// has_parent[i] is true if there exists a city j such that 
	// tour[j] = i, i.e there exists an edge j -> i
	bool *has_parent = new bool[size];
	for (int i=0; i<size; i++) has_parent[i] = false;
	
	while (unvisited > 2) {
		// Try vertex pair (i, j) in sortlist order
		for (size_t i = 0; i < pairs.size(); ++i) {
			// Make sure the tour is still valid if we insert the
			// segment (i, j). No cycle should be created and
			// all cities should have a degree of at most 2.
			vertex_pair vp = pairs.at(i);
			if (vp.used)
				continue;
			if (degree[vp.i] == 2 || degree[vp.i] == 2)
				continue;
			if (degree[vp.i] == 1 && degree[vp.i] == 1)
				if (contains_cycle(vp.i, vp.j, tour))
					continue;
			
			if (has_parent[vp.i]) {
				// Insert segment (i, j)
				tour[vp.i] = vp.j;
				has_parent[vp.j] = true;
			} else {
				// Insert reverse segment (j, i)
				tour[vp.j] = vp.i;
				has_parent[vp.i] = true;
			}
			vp.used = true;
			degree[vp.i]++;
			degree[vp.j]++;
			if (degree[vp.i] == 2) --unvisited;
			if (degree[vp.j] == 2) --unvisited;
			break;
		}
		// Infinite loop!
		assert(false);
	}
	
	// Stitch together remaining two cities with hub vertex
	int end[2]; // will contain endpoints for partial tour
	find_endpoints(end, degree, size);
	assert(!(has_parent[end[0]] && has_parent[end[1]]));
	if (has_parent[end[0]]) {
		// Complete the tour by adding segments end[0] -> hub -> end[1]
		tour[end[0]] = h;
		tour[h] = end[1];
	} else {
		tour[end[1]] = h;
		tour[h] = end[0];
	}
	
	// Wrap the final tour into a Tour object and return
	Tour *t = new Tour(tour, size);
	t->transform();
	return t;
}
