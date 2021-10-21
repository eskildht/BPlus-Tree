#include "main.h"

// Build ParallelBPlusTree using config specified by args or defult
// Report average time and average performance
int main(int argc, char *argv[]) {
	int trees = 4, order = 128, runs = 10;
	if (argc > 1) {
		// Parse args
		for (int i=1; i<argc; i++) {
			std::string arg = argv[i];
			if (arg == "-t") {
				trees = std::stoi(argv[i+1]);
			}
			else if (arg == "-o") {
				order = std::stoi(argv[i+1]);
			}
			else if (arg == "-r") {
				runs = std::stoi(argv[i+1]);
			}
		}
	}
	std::cout << "----------test-parallel-build----------\n";
	std::cout << "ParallelBPlusTree: trees/threads=" << trees << ", order=" << order << "\n";
	std::cout << "Doing " << runs << " run(s)\n";
	std::chrono::duration<double, std::milli> tot_time;
	for (int i=0; i < runs; i++) {
		ParallelBPlusTree ptree(order, trees, 1000000);
		auto build_time = ptree.build("input_files/input_file_1000000.txt");
		tot_time += build_time;
		std::cout << "Build " << i << " took: " << build_time.count() << "ms" << "\n";
	}
	std::chrono::duration<double, std::milli> avg_time = tot_time/runs;
	double m_ops = 1000/avg_time.count();
	double k_ops = m_ops*1000;
	std::cout << "Average build time: " << avg_time.count() << "ms\n";
	std::cout << "Average insert performance: " << k_ops << " KOps/sec" << std::endl;
}
