#include "main.h"
#include <limits>

int main(int argc, char *argv[]) {
	int trees = 4, order = 128, searches = 100000, scans = 10;
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
			else if (arg == "-se") {
				searches = std::stoi(argv[i+1]);
			}
			else if (arg == "-sc") {
				scans = std::stoi(argv[i+1]);
			}
		}
	}
	std::cout << "----------test-parallel-search----------\n";
	std::cout << "ParallelBPlusTree: trees/threads=" << trees << ", order=" << order << "\n";
	std::cout << "Doing " << searches << " searches\n";
	std::cout << "Doing " << scans << " scans\n";
	std::cout << "Building tree...\n";

	ParallelBPlusTree ptree(order, trees, 1000000);
	auto build_time = ptree.build("input_files/input_file_1000000.txt");
	std::cout << "Build took: " << build_time.count() << "ms" << "\n";

	std::chrono::duration<double, std::micro> tot_search_time;
	std::cout << "Searching tree...\n";
	for (int i=0; i < searches; i++) {
		float key = -1500 + static_cast <float> (rand())/(static_cast <float> (RAND_MAX/3000));
		key = roundf(key * 100) / 100;
		auto t1 = std::chrono::high_resolution_clock::now();
		ptree.search(key);
		auto t2 = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double, std::micro> search_time = t2 - t1;
		tot_search_time += search_time;
	}
	double ops = searches/(tot_search_time.count()/1000000);
	double k_ops = ops/1000;
	std::cout << "Average search time: " << tot_search_time.count()/searches << "μs\n";
	std::cout << "Average search performance: " << k_ops << " KOps/sec\n";

	std::cout << "Scaning tree...\n";
	std::chrono::duration<double, std::milli> tot_scan_time;
	for (int i=0; i < scans; i++) {
		auto t1 = std::chrono::high_resolution_clock::now();
		ptree.search(std::numeric_limits<float>::quiet_NaN(), std::numeric_limits<float>::quiet_NaN());
		auto t2 = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double, std::milli> scan_time = t2 - t1;
		tot_scan_time += scan_time;
		std::cout << "Scan " << i << " took " << scan_time.count() << "ms\n";
	}
	std::cout << "Average scan time: " << tot_scan_time.count()/scans << "ms" << std::endl;
}
