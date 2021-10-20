#include "main.h"
#include <sstream>
#include <cmath>

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

	// Instantiate rng and uniform int distribution
	std::random_device rd;
	this->rng = std::mt19937(rd());
	this->uni = std::uniform_int_distribution<int>(0, this->num_trees-1);
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
std::chrono::duration<double, std::milli> ParallelBPlusTree :: build(string input_file) {
#ifdef DEBUG
	std::cout << "Building ParallelBPlustree...\n";
#endif
	std::ifstream file(input_file);
	int pos = ERROR;
	float key = ERROR;
	int cnt = 0;
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
			cnt++;
		}
	}
	// Start build timer
	auto t1 = std::chrono::high_resolution_clock::now();
	// Split inserts into number of trees/threads vectors
	size_t const size = inserts.size() / num_trees;
	vector<vector<tuple<float, string>>> insert_parts(num_trees);
	for (int i = 0; i < (num_trees-1); i++) {
		vector<tuple<float, string>> insert_part(inserts.begin() + i*size, inserts.begin() + (i+1)*size);
		insert_parts.push_back(insert_part);
	}
	vector<tuple<float, string>> insert_part_last(inserts.begin() + (num_trees-1)*size, inserts.end());
	insert_parts.push_back(insert_part_last);

	// Start threads
	for (int i = 0; i < num_trees; i++) {
		std::thread th (&ParallelBPlusTree::thread_insert, this, &insert_parts[i], trees[i], filters[i]);
		threads.push_back(std::move(th));
	}
	// Synchronize all threads
	sync_threads();

	// End build timer
	auto t2 = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double, std::milli> ms_double = t2 - t1;
#ifdef DEBUG
	std::cout << "Build on " << cnt << " key-value pairs finished\n";
#endif
	return ms_double;
}

void ParallelBPlusTree :: search(float key) {
#ifdef DEBUG
	std::cout << "Searching for key=" << key << "\n";
#endif
	for (int i = 0; i < num_trees; i++) {
		if (filters[i]->contains(std::to_string(key))) {
#ifdef DEBUG
			std::stringstream msg;
			msg << "Filter " << i << " reported that it contains " << key << "\n";
			std::cout << msg.str();
#endif
			std::thread th (&ParallelBPlusTree::thread_search, this, trees[i], key);
			threads.push_back(std::move(th));
		}
	}
	sync_threads();
#ifdef DEBUG
	// Flush cout
	std::cout << std::endl;
#endif
}

void ParallelBPlusTree :: thread_search(BPlusTree* tree, float key) {
	vector<string>* result = tree->Search(key);
#ifdef DEBUG
	for (auto val : *result) {
		std::stringstream msg;
		msg << val << ", ";
		std::cout << msg.str();
	}
#endif
	delete(result);
}

void ParallelBPlusTree :: insert(float key, string value, bool preserve_locality) {
	// Insert a single key, value pair into the index structure
	// Optionally preserve_locality
#ifdef DEBUG
	std::cout << "Inserting (" << key << ", " << value << ")\n";
#endif
	if (preserve_locality) {
		for (int i = 0; i < num_trees; i++) {
			if (filters[i]->contains(std::to_string(key))) {
				trees[i]->Insert(key,value);
				return;
			}
		}
	}
	int rand_int = uni(rng);
	filters[rand_int]->insert(std::to_string(key));
	trees[rand_int]->Insert(key, value);
}

void ParallelBPlusTree :: search(float key1, float key2) {
#ifdef DEBUG
	std::cout << "Searching for keys in range [" << key1 << ", " << key2 << "]\n";
	bool trees_searched = false;
#endif
	for (int i = 0; i < num_trees; i++) {
		float max = trees[i]->get_max_insert();
		float min = trees[i]->get_min_insert();
		float k1 = std::isnan(key1) ? min : key1;
		float k2 = std::isnan(key2) ? max : key2;
		if (k1 <= max && min <= k2) {
#ifdef DEBUG
			if (!trees_searched) {
				trees_searched = true;
			}
			std::stringstream msg;
			msg << "Tree " << i << " will get searched\n";
			std::cout << msg.str();
#endif
			std::thread th (&ParallelBPlusTree::thread_range_search, this, trees[i], k1, k2);
			threads.push_back(std::move(th));
		}
	}
	sync_threads();
#ifdef DEBUG
	if (!trees_searched) {
		std::cout << "No values found for range";
	}
#endif
}

void ParallelBPlusTree :: thread_range_search(BPlusTree* tree, float key1, float key2) {
	vector<tuple<float, string>>* result = tree->Search(key1, key2);
#ifdef DEBUG
	for (auto tup : *result) {
		std::stringstream msg;
		msg << "(" << get<0>(tup) << ", " << get<1>(tup) << ")\n";
		std::cout << msg.str();
	}
#endif
	delete(result);
}

void ParallelBPlusTree :: sync_threads() {
	// Synchronize all threads
	for (int i = 0; i < threads.size(); i++) {
		threads[i].join();
	}
	// Clear the threads vector
	threads.clear();
}


