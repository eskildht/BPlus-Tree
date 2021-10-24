#include "main.h"
#include <tuple>
#include <chrono>

// main function
int main(int argc, char* argv[])
{
	BPlusTree tree(3);
	auto t1 = chrono::high_resolution_clock::now();
	tree.build("input_files/input_file_1000.txt");
	auto t2 = chrono::high_resolution_clock::now();
	chrono::duration<double, std::milli> ms_double = t2 - t1;
	std::cout << "Build took: " << ms_double.count() << "ms" << std::endl;
	auto result = tree.Search(-0.07, 0.18);
	for (auto keyval : *result) {
		std::cout << "(" << get<0>(keyval) << ", " << get<1>(keyval) << ")";
	}

	return 0;
}

