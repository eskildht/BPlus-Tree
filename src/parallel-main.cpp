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
}

