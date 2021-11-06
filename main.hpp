#ifndef __TSP
#define __TSP

#include "Tour.hpp"
#include <vector>
#include <cmath>

/// A city in the travelling salesman problem. Two or more 
/// cities form a TSP instance. A city is represented by a 
/// coordinate pair (x, y).
struct City {
	double x, y;
	int name;
	
	/// Computes the square euclidean distance between this city
	/// and another city.
	double dist2(const City &city) const {
		double dx = (x - city.x);
		double dy = (y - city.y);
		return dx*dx + dy*dy;
	}
	
	/// Computes the euclidean distance between this city and
	/// another city.
	double dist(const City &city) const {
		return std::sqrt(dist2(city));
	}
};

void read_input(std::vector<City>&);
Tour* best_solution(std::vector<Tour>&, double**);

#endif
