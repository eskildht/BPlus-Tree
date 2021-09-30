#include "main.h"
#include <tuple>
#include <chrono>
#include <thread>

// insert function for vector of key-value pairs
void insert(vector<tuple<float, string>>* inserts, BPlusTree* tree) {
	for(auto insert : *inserts) {
		tree->Insert(get<0>(insert), get<1>(insert));
	}
}

// main function
int main(int argc, char* argv[])
{
	int pos = ERROR;
	float key = ERROR, key1 = ERROR, key2 = ERROR;
	string line, value;
	vector<tuple<float, string>> inserts;

	ifstream file("input_file_single.txt");

	// read the input file and perform the corresponding operations
	while (getline(file, line))
	{
		pos   = ERROR;
		key   = ERROR;
		value = ERROR;
		key1  = ERROR;
		key2  = ERROR;
		// operation: Insert(key, value)
		if(0 == line.compare(0, 6, "Insert"))
		{
			pos = line.find(",");
			key = atof(line.substr(7, pos - 7).c_str());
			value = line.substr(pos + 1, line.size() - pos - 2);

			// add (key, value) to in-memory vector
			inserts.push_back(make_tuple(key, value));
		}
	}

	// Split inserts into 4 vectors
	size_t const quarter_size = inserts.size() / 4;
	vector<tuple<float, string>> inserts1(inserts.begin(), inserts.begin() + quarter_size);
	vector<tuple<float, string>> inserts2(inserts.begin() + quarter_size, inserts.begin() + 2*quarter_size);
	vector<tuple<float, string>> inserts3(inserts.begin() + 2*quarter_size, inserts.begin() + 3*quarter_size);
	vector<tuple<float, string>> inserts4(inserts.begin() + 3*quarter_size, inserts.end());

	// Create and initialize trees
	BPlusTree tree1;
	BPlusTree tree2;
	BPlusTree tree3;
	BPlusTree tree4;
	tree1.Initialize(3);
	tree2.Initialize(3);
	tree3.Initialize(3);
	tree4.Initialize(3);

	// Build thread-trees
	auto t1 = chrono::high_resolution_clock::now();
	thread thread1 (insert, &inserts1, &tree1);
	thread thread2 (insert, &inserts2, &tree2);
	thread thread3 (insert, &inserts3, &tree3);
	thread thread4 (insert, &inserts4, &tree4);
	thread1.join();
	thread2.join();
	thread3.join();
	thread4.join();
	auto t2 = chrono::high_resolution_clock::now();
	chrono::duration<double, std::milli> ms_double = t2 - t1;

#ifdef DEBUG
	// print the current state of the B+ tree
	tree.Print_Tree();
#endif

	cout << ms_double.count() << "ms\n";
	return 0;
}

