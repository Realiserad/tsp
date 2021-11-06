#include <iostream>
#include <cstdlib>
int main(int argc, char *argv[]) {
	if (argc < 2) {
		std::cerr << "You must specify the number of cities!" << std::endl;
		return 0;
	}
	int n = atoi(argv[1]);
	if (n == -1) {
		std::cerr << "The argument must be an integer!" << std::endl;
		return 0;
	}
	srand(time(NULL));
	std::cout << n << std::endl;
	for (int i = 0; i < n; ++i) {
		std::cout << (std::rand() % 500) << " " << (std::rand() % 500) << std::endl;
	}
}
