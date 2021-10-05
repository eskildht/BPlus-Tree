#include "main.h"

// Constructor
ParallelBPlusTree :: ParallelBPlusTree(int trees_order, int num_trees, int projected_element_count) : trees_order(trees_order) {
	// Set the number of trees/threads to use
	int conc = std::thread::hardware_concurrency();
	if (num_trees > 0 && num_trees != conc) {
		std::cout << "Chosen num_trees=" << num_trees << " differs from reported hardware_concurrency=" << conc << "\n";
		this->num_trees = num_trees;
	}
	else {
		this->num_trees = conc;
	}
	// Configure bloom filter parameters
	bloom_parameters parameters;
	parameters.projected_element_count = projected_element_count;
	parameters.false_positive_probability = 0.0001; // 1 in 10000
	parameters.random_seed = 0xA5A5A5A5;
	parameters.compute_optimal_parameters();

	// Instantiate the trees and filters
	for (int i = 0; i < this->num_trees; i++) {
		BPlusTree* bptreepointer = new BPlusTree(trees_order);
		trees.push_back(bptreepointer);
		bloom_filter* filterpointer = new bloom_filter(parameters);
		filters.push_back(filterpointer);
	}
}

// Destructor
ParallelBPlusTree :: ~ParallelBPlusTree() {
	for (int i; i < num_trees; i++)	{
		delete(trees[i]);
		delete(filters[i]);
	}
}

// Helper insert function for vector of key-value pairs into a specific tree and filter
void ParallelBPlusTree :: thread_insert(vector<tuple<float, string>>* inserts, BPlusTree* tree, bloom_filter* filter) {
	for (auto insert : *inserts) {
		float key = get<0>(insert);
		string val = get<1>(insert);
		tree->Insert(key, val);
		filter->insert(std::to_string(key));
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

	// Start threads
	for (int i = 0; i < num_trees; i++) {
		std::thread th (&ParallelBPlusTree::thread_insert, this, &insert_parts[i], trees[i], filters[i]);
		threads.push_back(std::move(th));
	}
	// Synchronize all threads
	sync_threads();

	// Print all trees
	//	for (int i = 0; i < num_trees; i++) {
	//		std::cout << "Tree " << i << std::endl;
	//		trees[i]->Print_Tree();
	//	}
}

void ParallelBPlusTree :: search(float key) {
#ifdef DEBUG
	std::cout << "Searching for key=" << key << "\n";
#endif
	for (int i = 0; i < num_trees; i++) {
		if (filters[i]->contains(std::to_string(key))) {
#ifdef DEBUG
			std::cout << "Filter " << i << " reported that it contains " << key << "\n";
#endif
			std::thread th (&ParallelBPlusTree::thread_search, this, trees[i], (float) key);
			threads.push_back(std::move(th));
		}
	}
	sync_threads();
	// Flush cout
	std::cout << std::endl;
}

void ParallelBPlusTree :: thread_search(BPlusTree* tree, float key) {
	vector<string>* result = tree->Search(key);
#ifdef DEBUG
	for (auto val : *result) {
		std::cout << val << ", ";
	}
#endif
}

void ParallelBPlusTree :: sync_threads() {
	// Synchronize all threads
	for (int i = 0; i < threads.size(); i++) {
		threads[i].join();
	}
	// Clear the threads vector
	threads.clear();
}


