#ifndef __TSPTOOLS
#define __TSPTOOLS

#include "main.hpp"
#include "Tour.hpp"
#include <vector>

void opt2(Tour&, double**, int);
void opt2k(std::vector<City>&, Tour&, double**, int, int);

void opt3(Tour &t, double **d, int max_iter);
bool opt3search(Tour &tour, double **d);

double** pre_dist(std::vector<City>&);
bool compare_cities(City, City);
void proximity_list(std::vector<City>&);

#endif
