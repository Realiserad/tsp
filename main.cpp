#include "main.hpp"
#include "Tour.hpp"
#include "tsptools.hpp"
#include "mst.hpp"
#include "nearest_insertion.hpp"
#include "nearest_neighbour.hpp"
#include "clarke_wright.hpp"

#include <iostream>
#include <unordered_set>
#include <vector>
#include <cstdio>
#include <cmath>
#include <ctime>
#include <iomanip>
#include <algorithm>
#include <cstddef>
#include <cfloat>
#include <climits>

void read_input(std::vector<City> &cities) {
	int size;
	std::cin >> size;
	cities.reserve(size);
	for (int i = 0; i < size; ++i) {
		City city;
		city.name = i;
		std::cin >> city.x;
		std::cin >> city.y;
		cities.push_back(city);
	}
}

int min(int a, int b) {
	return a < b ? a : b;
}

Tour* best_solution(std::vector<Tour*> &tours, double **d) {
	Tour* best = nullptr;
	double min = DBL_MAX;
	for (size_t i = 0; i < tours.size(); ++i) {
		double next_distance = tours.at(i)->length(d);
		if (next_distance < min) {
			min = next_distance;
			best = tours.at(i);
		}
	}
	return best;
}

int main() {
	// Disable syncing with C printf and scanf
	std::ios_base::sync_with_stdio(false);
	// Initialize the PRNG
	srand(time(NULL));
	
	std::vector<City> cities;
	read_input(cities);
	
	double **dist = pre_dist(cities);			// Distance matrix
	int *tree = new int[cities.size()];			// Minimum spanning tree
	mst(dist, cities.size(), tree);
	
	if (cities.size() < 4) {
		int n = cities.size();
		int *tour = new int[n];
		for (int i = 0; i < n; ++i)
			tour[i] = i;
		Tour t(tour, n);
		t.print();
		return 0;
	}
	
	// Will contain a set of candidate solutions
	std::vector<Tour*> tours;
	
	// Determine parameters
	// nn_count - The number of tours created using nearest neighbour
	// ni_count - The number of tours created using nearest insertion
	// mst_count - The number of tours created using MST heuristic
	// k2l - The size of the neighbourhood used by opt2k
	// k2g - The number of global 2-Opt moves which should be applied
	int nn_count, ni_count, mst_count, k2l, k2g;
	if (cities.size() <= 50) {
		nn_count = 1000;
		ni_count = 2000;
		mst_count = 1000;
		k2l = 0;
		k2g = INT_MAX;
	} else if (cities.size() <= 100) {
		nn_count = 200;
		ni_count = 300;
		mst_count = 100;
		k2l = 0;
		k2g = INT_MAX;
	} else if (cities.size() <= 200) {
		nn_count = 30;
		ni_count = 50;
		mst_count = 40;
		k2l = 0;
		k2g = 70;
	} else if (cities.size() <= 300) {
		nn_count = 1;
		ni_count = 2;
		mst_count = 0;
		k2l = 20;
		k2g = 10;
	} else if (cities.size() <= 500) {
		nn_count = 0;
		ni_count = 1;
		mst_count = 0;
		k2l = 0;
		k2g = 198;
	} else if (cities.size() <= 700) {
		nn_count = 0;
		ni_count = 1;
		mst_count = 0;
		k2l = 0;
		k2g = 192;
	} else {
		nn_count = 0;
		ni_count = 1;
		mst_count = 0;
		k2l = 0;
		k2g = 190;
	}
	
	// Create candidate solutions
	for (int i = 0; i < nn_count; ++i) {
		Tour *t = nearest_neighbour(dist, cities.size());
		tours.push_back(t);
	}
	//for (int i = 0; i < ni_count; ++i) {
		//Tour *t = nearest_insertion(dist, cities.size());
		//tours.push_back(t);
	//}
	//for (int i = 0; i < mst_count; ++i) {
		//Tour *t = mst_heuristic(tree, cities.size());
		//tours.push_back(t);
	//}
	
	// Improve the solutions using local search
	for (auto i = tours.begin(); i != tours.end(); ++i) {
		//opt2k(cities, **i, dist, k2l, INT_MAX);
		//opt2(**i, dist, k2g); 
		opt3(**i, dist, INT_MAX); 
	}
	
	// Select and print the best solution
	Tour *best = best_solution(tours, dist);
	best->print();
}
