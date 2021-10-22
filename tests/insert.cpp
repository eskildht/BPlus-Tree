
#include "main.h"

// Build PlusTree using config specified by args or default
// Report average time and average performance
int main(int argc, char *argv[]) {
	int order = 128, runs = 10, inserts = 10000;
	if (argc > 1) {
		// Parse args
		for (int i=1; i<argc; i++) {
			std::string arg = argv[i];
			if (arg == "-o") {
				order = std::stoi(argv[i+1]);
			}
			else if (arg == "-r") {
				runs = std::stoi(argv[i+1]);
			}
			else if (arg == "-i") {
				inserts = std::stoi(argv[i+1]);
			}
		}
	}
	std::cout << "----------test-insert----------\n";
	std::cout << "BPlusTree: order=" << order <<"\n";
	std::cout << "Doing " << runs << " runs(s)\n";
	std::chrono::duration<double, std::milli> tot_time;
	for (int i=0; i < runs; i++) {
		BPlusTree tree(128);

		for (int j=0; j < inserts; j++) {
			float key = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
			string value = "Value" + std::to_string(j);
			auto t1 = std::chrono::high_resolution_clock::now();
			tree.Insert(key, value);
			auto t2 = std::chrono::high_resolution_clock::now();
			std::chrono::duration<double, std::milli> insert_time = t2 - t1;
			tot_time += insert_time;
		}
	}
	std::chrono::duration<double, std::milli> avg_time = tot_time/runs;
	double ops = (inserts/avg_time.count())*1000;
	double k_ops = ops/1000;
	double m_ops = k_ops/1000;
	std::cout << "Average run time: " << avg_time.count() << "ms\n";
	std::cout << "Average insert performance: " << k_ops << " KOps/sec" << std::endl;
	std::cout << "Average insert performance: " << m_ops << " MOps/sec" << std::endl;
}
