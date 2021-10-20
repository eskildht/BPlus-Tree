#include "main.h"
#include <chrono>
#include <limits>

// Build parallel tree 10 times using order 128
// Report average time
int main() {
	std::chrono::duration<double, std::milli> tot_time;
	for (int i=0; i < 10; i++) {
		ParallelBPlusTree ptree(128, 4, 1000000);
		auto build_time = ptree.build("input_files/input_file_1000000.txt");
		tot_time += build_time;
		std::cout << "Build " << i << " took: " << build_time.count() << "ms" << "\n";
	}
	std::chrono::duration<double, std::milli> avg_time = tot_time/10;
	std::cout << "Avarage build time of 10 runs: " << avg_time.count() << "ms" << std::endl;
}
