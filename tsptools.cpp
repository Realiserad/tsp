//define NDEBUG

#include "Tour.hpp"
#include "main.hpp"
#include "tsptools.hpp"
#include <vector>
#include <algorithm>
#include <iostream>
#include <cfloat>
#include <unistd.h>

void log(std::string msg) {
#ifndef NDEBUG
	std::cerr << msg << std::endl;
#endif
}

static std::vector<City>* prox_list = nullptr;
static City cmp_city;

bool cmp_cities(City a, City b) {
	return a.dist2(cmp_city) < b.dist2(cmp_city);
}

/// Computes an array of vectors sorted by distance, such
/// that proximity_list[i].at(k) contains the k:th closest
/// city to i.
/// @param cities The cities used to build the proximity list
/// @complexity O(n^2log n)
void proximity_list(std::vector<City> &cities) { 
	prox_list = new std::vector<City>[cities.size()];
	for (size_t i = 0; i < cities.size(); ++i) {
		prox_list[i] = cities;
		cmp_city = cities.at(i);
		std::sort(prox_list[i].begin(), prox_list[i].end(), cmp_cities);
	}
}

/// Reverse the subtour j -> ... -> a.
/// @param t The tour containing the subtour to be reversed
/// @param j The start index
/// @param a The end index
void opt2move(Tour &t, int j, int a) {
	// Assert j <= a
	if (j > a) {
		int tmp = a;
		a = j;
		j = tmp;
	}
	
	int len = a-j+1;
	for (int i = 0, r = j+len-1; i < len/2; ++i, --r) {
		t.swap(j+i, r);
	}
}

/// Look at all unique edge pairs (i, j) and (a, b)
/// and return true if an improvement was made.
bool opt2search(Tour &tour, double **d) {
	for (int j = 1; j < tour.size(); ++j) {
		for (int b = j+2; b <= tour.size(); ++b) {
			int I = tour[j-1];
			int J = tour[j];
			int A = tour[b-1];
			int B = tour[b % tour.size()];
			if (d[I][J] + d[A][B] > d[I][A] + d[J][B]) {
				opt2move(tour, j, b-1);
				return true;
			}
		}
	}
	return false;
}
 
/// Naive 2-Opt for TSP.
/// @param t The tour to improve
/// @param d The distance matrix
/// @param max_iter The maximum number of swaps
/// @complexity ~O(n^3)
void opt2(Tour &t, double **d, int max_iter) {
	int iter = 0; // Number of edge swaps so far
	while (opt2search(t, d) && ++iter < max_iter);
}

/// Search through the k-neighbourhood of j, i.e the k-1 closest cities
/// to j, for each edge (i, j). To consider candidates for b, we need 
/// only start at the beginning of j:s list and proceed down it until a 
/// city x with d(j, x) ≥ d(i, j) is found. Returns true if an improvement 
/// was found. 
bool opt2ksearch(Tour &tour, double **d, int k) {
	for (int j = 1; j < tour.size(); ++j) {
		// Note that prox_list[j].at(0) contains j, skip this
		// city by starting pos at 1.
		for (int pos = 1; pos < k; ++pos) {
			City &c = prox_list[j].at(pos);
			int b = tour.index_of(c.name);
			int I = tour[j-1];
			int J = tour[j];
			int B = tour[b]; // candidate for b
			if (d[J][B] >= d[I][J]) {
				// b is a good candidate. Check if the tour become
				// shorter if the edge (i, j) and (a, b) is swapped.
				int a = b == 0 ? tour.size() - 1 : b - 1;
				int A = tour[a];
				if (d[I][J] + d[A][B] > d[I][A] + d[J][B]) {
					// Important, make sure the right part is swapped
					// Case b < j : swap subarray b to i
					// ---xxxxxxxxx--------------
					//   ^^       ^^
					//   ab       ij
					// Case b >= j : swap subarray j to a 
					// ----xxxxxxxxxxxxx---------
					//    ^^           ^^
					//    ij           ab
					if (b < j)
						opt2move(tour, b, j-1);
					else
						opt2move(tour, j, a);
					return true;
				} 
			}
		}
	}
	return false;
}

/// Fast implementation of 2-Opt using neighbourhood search.
/// @param tour The tour to improve
/// @param proximity_list A list containing the k closest cities for each city
/// @param d The distance matrix
/// @param k The number of neighbouring cities to consider
/// @param max_iter The maximum number of swaps
/// @complexity ~O(kn^2)
void opt2k(std::vector<City> &cities, Tour &t, double **d, int k, int max_iter) {
	if (k == 0) {
		// Neighbourhood search disabled
		return;
	}
	if (prox_list == nullptr) {
		// Compute proximity list
		proximity_list(cities);
	}
	int iter = 0;
	while (opt2ksearch(t, d, k) && ++iter < max_iter);
}

/// Computes a distance matrix D, such that D[i][j]
/// is the distance between city i and j.
double** pre_dist(std::vector<City> &cities) {
	double **m = new double*[cities.size()];
	for (size_t i = 0; i < cities.size(); ++i) {
		m[i] = new double[cities.size()];
		for (size_t j = 0; j < cities.size(); ++j) {
			m[i][j] = cities.at(i).dist(cities.at(j));
		}
	}
	return m;
}

void opt3(Tour &t, double **d, int max_iter) {
	int iter = 0; // Number of edge swaps so far
	while (opt3search(t, d) && ++iter < max_iter);
}

double select_min(double d0, double d1, double d2) {
	double min = d0 < d1 ? d0 : d1;
	return min < d2 ? min : d2;
}

bool opt3search(Tour &tour, double **dist) {
	for (int b = 1; b < tour.size(); b++) {
		for (int d = b + 2; d < tour.size(); d++) {
			for (int f = d + 2; f < tour.size(); f++) {
				// try to rearrange edges (a, b) (c, d) and (e, f)
				int a = b - 1;
				int c = d - 1;
				int e = f - 1;

				int A = tour[a];
				int B = tour[b];
				int C = tour[c];
				int D = tour[d];
				int E = tour[e];
				int F = tour[f];

				// Possible tours						Cost
				// 0 >>>> ab >>>> cd >>>> ef >>>> ~		id
				// 0 >>>> ad >>>> ec <<<< bf >>>> ~		d0
				// 0 >>>> ac <<<< be >>>> df >>>> ~		d1
				// 0 >>>> ae >>>> db >>>> cf >>>> ~		d2

				double id = dist[A][B] + dist[C][D] + dist[E][F];
				double d0 = dist[A][D] + dist[E][C] + dist[B][F];
				double d1 = dist[A][C] + dist[B][E] + dist[D][F];
				double d2 = dist[A][E] + dist[D][B] + dist[C][F];
				
				double min = select_min(d0, d1, d2);
				if (min >= id) {
					continue;
				}
				
				// Backup tour to be modified
				int old[1000];
				for (int i = a + 1; i < f; i++) 
					old[i] = tour[i];
				
				// Write new tour
				int pos = a + 1;
				if (min == d0) {
					for (int i = d; i <= e; i++) tour.set(pos++, old[i]);
					for (int i = c; i >= b; i--) tour.set(pos++, old[i]);
				} else if (min == d1) {
					for (int i = c; i >= b; i--) tour.set(pos++, old[i]);
					for (int i = e; i <= d; i++) tour.set(pos++, old[i]);
				} else if (min == d2) {
					for (int i = e; i <= d; i++) tour.set(pos++, old[i]);
					for (int i = b; i <= c; i++) tour.set(pos++, old[i]);
				}
				return true;
			}
		}
	}
	return false;
}

