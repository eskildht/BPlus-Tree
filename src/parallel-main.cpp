#include "main.h"
#include <chrono>

// main function
int main() {
	ParallelBPlusTree ptree(3, 4);
	auto t1 = chrono::high_resolution_clock::now();
	ptree.build("input_file.txt");
	auto t2 = chrono::high_resolution_clock::now();
	chrono::duration<double, std::milli> ms_double = t2 - t1;
	std::cout << "Build took: " << ms_double.count() << "ms" << std::endl;
	float key = 72.12;
	t1 = chrono::high_resolution_clock::now();
	ptree.search(key);
	t2 = chrono::high_resolution_clock::now();
	ms_double = t2 - t1;
	std::cout << "Search for key=" << key << " took: " << ms_double.count() << "ms" << std::endl;
}

