#include "main.h"

int main(int argc, char *argv[]) {
	int order = 128, searches = 100000, scans = 10;
	std::string file = "input_files/input_file_1000000.txt", search_method = "rand";
	if (argc > 1) {
		// Parse args
		for (int i=1; i<argc; i++) {
			std::string arg = argv[i];
			if (arg == "-o") {
				order = std::stoi(argv[i+1]);
			}
			else if (arg == "-se") {
				searches = std::stoi(argv[i+1]);
			}
			else if (arg == "-sc") {
				scans = std::stoi(argv[i+1]);
			}
			else if (arg == "-f") {
				std::string file_name = argv[i+1];
				file = "input_files/" + file_name;
			}
			else if (arg == "-sm") {
				search_method = argv[i+1];
			}
		}
	}
	std::cout << "----------test-search----------\n";
	std::cout << "BPlusTree: order=" << order <<"\n";
	std::cout << "Doing " << searches << " searches\n";
	std::cout << "Doing " << scans << " scans\n";
	std::cout << "Building tree...\n";
	BPlusTree tree(order);
	auto build_time = tree.build(file);
	std::cout << "Build took: " << build_time.count() << "ms" << "\n";
	std::chrono::duration<double, std::micro> tot_time;
	std::cout << "Searching tree...\n";
	if (search_method == "rand") {
		for (int i=0; i < searches; i++) {
			float key = -1500 + static_cast <float> (rand())/(static_cast <float> (RAND_MAX/3000));
			key = roundf(key * 100) / 100;
			auto t1 = std::chrono::high_resolution_clock::now();
			tree.Search(key);
			auto t2 = std::chrono::high_resolution_clock::now();
			std::chrono::duration<double, std::micro> search_time = t2 - t1;
			tot_time += search_time;
		}
	}
	else if (search_method == "prim") {
		std::random_device rd;
		std::mt19937 rng(rd());
		std::uniform_int_distribution<int> uni(0, 999999);
		for (int i=0; i < searches; i++) {
			auto random_integer = uni(rng);
			float key = static_cast<float>(random_integer);
			auto t1 = std::chrono::high_resolution_clock::now();
			tree.Search(key);
			auto t2 = std::chrono::high_resolution_clock::now();
			std::chrono::duration<double, std::micro> search_time = t2 - t1;
			tot_time += search_time;
		}
	}
	double ops = searches/(tot_time.count()/1000000);
	double k_ops = ops/1000;
	std::cout << "Average search time: " << tot_time.count()/searches << "μs\n";
	std::cout << "Average search performance: " << k_ops << " KOps/sec" << std::endl;

	std::cout << "Scaning tree...\n";
	std::chrono::duration<double, std::milli> tot_scan_time;
	for (int i=0; i < scans; i++) {
		auto t1 = std::chrono::high_resolution_clock::now();
		tree.scan(std::numeric_limits<float>::quiet_NaN(), std::numeric_limits<float>::quiet_NaN());
		auto t2 = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double, std::milli> scan_time = t2 - t1;
		tot_scan_time += scan_time;
		std::cout << "Scan " << i << " took " << scan_time.count() << "ms\n";
	}
	std::cout << "Average scan time: " << tot_scan_time.count()/scans << "ms" << std::endl;
}
