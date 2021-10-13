#include "main.h"
#include <chrono>
#include <limits>

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
	t1 = chrono::high_resolution_clock::now();
	ptree.insert(73.13, "Value10201");
	t2 = chrono::high_resolution_clock::now();
	ms_double = t2 - t1;
	std::cout << "Insert took: " << ms_double.count() << "ms" << std::endl;
	t1 = chrono::high_resolution_clock::now();
	ptree.search(72.01, 75.12);
	t2 = chrono::high_resolution_clock::now();
	ms_double = t2 - t1;
	std::cout << "Range search took: " << ms_double.count() << "ms" << std::endl;
	t1 = chrono::high_resolution_clock::now();
	ptree.search(std::numeric_limits<float>::quiet_NaN(), 75.12);
	t2 = chrono::high_resolution_clock::now();
	ms_double = t2 - t1;
	std::cout << "Range search took: " << ms_double.count() << "ms" << std::endl;
}

