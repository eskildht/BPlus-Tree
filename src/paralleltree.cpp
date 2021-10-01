#include "main.h"
#include <thread>
#include <iostream>
#include <string>

// Constructor
ParallelBPlusTree :: ParallelBPlusTree(int trees_order, int num_trees) : trees_order(trees_order) {
	// Set the number of trees/threads to use
	int conc = std::thread::hardware_concurrency();
	if (num_trees > 0 && num_trees != conc) {
		std::cout << "Chosen num_trees=" << num_trees << " differs from reported hardware_concurrency=" << conc << "\n";
		this->num_trees = num_trees;
	}
	else {
		this->num_trees = conc;
	}
	// Instantiate the trees
	for (int i = 0; i < this->num_trees; i++) {
		BPlusTree* bptreepointer = new BPlusTree;
		bptreepointer->Initialize(trees_order);
		trees.push_back(bptreepointer);
	}
}

// Destructor
ParallelBPlusTree :: ~ParallelBPlusTree() {
	for (int i; i < num_trees; i++)	{
		delete(trees[i]);
	}
}

// Helper insert function for vector of key-value pairs into a specific tree
void ParallelBPlusTree :: insert(vector<tuple<float, string>>* inserts, BPlusTree* tree) {
	for (auto insert : *inserts) {
		tree->Insert(get<0>(insert), get<1>(insert));
	}
}

// Build the BPlusTrees in parallel after all data is loaded into main memory
void ParallelBPlusTree :: build(string input_file) {
	std::ifstream file(input_file);
	int pos = ERROR;
	float key = ERROR;
	string line, value;
	vector<tuple<float, string>> inserts;

	// Get all inserts from file
	while (getline(file, line)) {
		if(0 == line.compare(0, 6, "Insert"))
		{
			pos = line.find(",");
			key = atof(line.substr(7, pos - 7).c_str());
			value = line.substr(pos + 1, line.size() - pos - 2);

			// add (key, value) to in-memory vector
			inserts.push_back(make_tuple(key, value));
		}
	}

	// Split inserts into number of trees/threads vectors
	size_t const size = inserts.size() / num_trees;
	vector<vector<tuple<float, string>>> insert_parts;
	for (int i = 0; i < (num_trees-1); i++) {
		vector<tuple<float, string>> insert_part(inserts.begin() + i*size, inserts.begin() + (i+1)*size);
		insert_parts.push_back(insert_part);
	}
	vector<tuple<float, string>> insert_part_last(inserts.begin() + (num_trees-1)*size, inserts.end());
	insert_parts.push_back(insert_part_last);
	// Print all vectors about to insert
	//	for (auto part : insert_parts) {
	//		for (auto tup : part) {
	//			std::cout << "(" << get<0>(tup) << ", " << get<1>(tup) << ") ";
	//		}
	//		std::cout << "\n\n";
	//	}

	auto t1 = chrono::high_resolution_clock::now();
	// Start threads
	for (int i = 0; i < num_trees; i++) {
		std::thread th (&ParallelBPlusTree::insert, this, &insert_parts[i], trees[i]);
		threads.push_back(std::move(th));
	}
	// Synchronize all threads
	for (int i = 0; i < num_trees; i++) {
		threads[i].join();
	}
	auto t2 = chrono::high_resolution_clock::now();
	chrono::duration<double, std::milli> ms_double = t2 - t1;
	std::cout << "Build took: " << ms_double.count() << "ms" << std::endl;

	// Print all trees
	//	for (int i = 0; i < num_trees; i++) {
	//		std::cout << "Tree " << i << std::endl;
	//		trees[i]->Print_Tree();
	//	}
}


