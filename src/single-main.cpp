#include "main.h"
#include <tuple>
#include <chrono>

// insert function for vector of key-value pairs
void insert(vector<tuple<float, string>>* inserts, BPlusTree* tree) {
	for(auto insert : *inserts) {
		tree->Insert(get<0>(insert), get<1>(insert));
	}
}

// main function
int main(int argc, char* argv[])
{
	BPlusTree tree(3);
	auto t1 = chrono::high_resolution_clock::now();
	tree.build("input_files/input_file_1000000.txt");
	auto t2 = chrono::high_resolution_clock::now();
	chrono::duration<double, std::milli> ms_double = t2 - t1;
	std::cout << "Build took: " << ms_double.count() << "ms" << std::endl;
	return 0;
}

