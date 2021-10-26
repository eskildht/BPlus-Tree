#include "main.h"
#include <tuple>
#include <chrono>

// main function
int main(int argc, char* argv[])
{
	BPlusTree tree(3);
	auto build_time = tree.build("input_files/input_file_1000000.txt");
	std::cout << "Build took: " << build_time.count() << "ms" << std::endl;
	auto res = tree.Search(-1418.57);
	for (auto val : *res) {
		std::cout << val << "\n";
	}
	auto result = tree.Search(-0.07, 0.18);
	for (auto keyval : *result) {
		std::cout << "(" << get<0>(keyval) << ", " << get<1>(keyval) << ")";
	}

	return 0;
}

