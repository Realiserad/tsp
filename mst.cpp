#include "mst.hpp"
#include <limits.h>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <algorithm>

/// Create a random TSP tour from a minimum spanning tree.
/// @param parent A minimum spanning tree
/// @param size The number of cities
/// @complexity O(n)
Tour* mst_heuristic(int *parent, int size) {
	// Create a neighbour list
	std::vector<std::vector<int>> neighbors(size);
	for (int i = 0; i < size; ++i) {
		// Vertex i has parent[i] as parent
		int pos = parent[i]; 
		if (pos == -1) continue;
		neighbors.at(pos).push_back(i);
	}
	
	// Traverse the tree in random order
	for (int i = 0; i < size; ++i) {
		std::random_shuffle(neighbors.at(i).begin(), neighbors.at(i).end());
	}
	
	int *pre_order = new int[size];
	int pos = 0;

	dfs(pre_order, pos, 0, neighbors);

	Tour *t = new Tour(pre_order, size);
	return t;
}

void dfs(int *pre_order, int &pos, int current, std::vector<std::vector<int>> &neighbors) {
	pre_order[pos++] = current;
	std::vector<int> to_visit = neighbors.at(current);
	for (auto i = to_visit.begin(); i != to_visit.end(); ++i) {
		dfs(pre_order, pos, *i, neighbors);
	}
}

/// A utility function to find the vertex with minimum key value, from
/// the set of vertices not yet included in MST.
int min_key(double key[], bool mst_set[], int V) {
	int min = INT_MAX;
	int min_index;

	for (int v = 0; v < V; ++v) {
		if (!mst_set[v] && key[v] < min) {
			min = key[v];
			min_index = v;
		}
	}

	return min_index;
}

/// Find a minimum spanning tree using Prim's algorithm.
/// The tree is represented with an array A, such that A[i]
/// contains the parent of the child node i or -1 if i is
/// the root of the tree.
/// @dist Distance matrix
/// @V The number of vertices
/// @parent The minimum spanning tree (output)
/// @complexity O(n^2)
void mst(double **d, int V, int *parent) {
	// Key values used to pick minimum weight edge in cut
	double *key = new double[V];
	// To represent set of vertices not yet included in MST
	bool *mst_set = new bool[V];

	// Initialize all keys as INF
	for (int i = 0; i < V; ++i) {
		key[i] = INT_MAX, mst_set[i] = false;
	}

	key[0] = 0;     // Make key 0 so that this vertex is picked as first vertex
	parent[0] = -1; // First node is always root of MST 

	for (int count = 0; count < V-1; ++count) {
		// Pick the minimum key vertex from the set of vertices
		// not yet included in MST
		int u = min_key(key, mst_set, V);

		// Add the picked vertex to the MST Set
		mst_set[u] = true;

		// Update key value and parent index of the adjacent vertices of
		// the picked vertex. Consider only those vertices which are not yet
		// included in MST
		for (int v = 0; v < V; ++v) {
			if (!mst_set[v] && d[u][v] <  key[v]) {
				parent[v] = u;
				key[v] = d[u][v];
			}
		}
	}
	
	delete[] key;
	delete[] mst_set;
}
