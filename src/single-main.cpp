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
	int pos = ERROR;
	float key = ERROR, key1 = ERROR, key2 = ERROR;
	string line, value;
	vector<tuple<float, string>> inserts;
	BPlusTree tree;

	ifstream file("input_file.txt");

	tree.Initialize(3);

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

	auto t1 = chrono::high_resolution_clock::now();
	insert(&inserts, &tree);
	auto t2 = chrono::high_resolution_clock::now();
	chrono::duration<double, std::milli> ms_double = t2 - t1;

#ifdef DEBUG
	// print the current state of the B+ tree
	// tree.Print_Tree();
#endif

	cout << ms_double.count() << "ms\n";
	return 0;
}

