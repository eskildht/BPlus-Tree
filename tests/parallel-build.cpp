#include "main.h"

// Build ParallelBPlusTree 10 times using trees/threads=4, order=128
// Report average time and average performance
int main() {
	std::cout << "----------test-parallel-build----------\n";
	std::cout << "ParallelBPlusTree: trees/threads=4, order=128\n";
	std::chrono::duration<double, std::milli> tot_time;
	for (int i=0; i < 10; i++) {
		ParallelBPlusTree ptree(128, 4, 1000000);
		auto build_time = ptree.build("input_files/input_file_1000000.txt");
		tot_time += build_time;
		std::cout << "Build " << i << " took: " << build_time.count() << "ms" << "\n";
	}
	std::chrono::duration<double, std::milli> avg_time = tot_time/10;
	double m_ops = 1000/avg_time.count();
	std::cout << "Average build time: " << avg_time.count() << "ms\n";
	std::cout << "Avarage insert performance: " << m_ops << " MOps/sec" << std::endl;
}
