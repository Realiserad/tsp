#ifndef __MST
#define __MST

#include "Tour.hpp"
#include <vector>

void mst(double**, int, int*);
void dfs(int*, int&, int, std::vector<std::vector<int>>&);
Tour* mst_heuristic(int*, int);

#endif
