#ifndef MAIN_H
#define MAIN_H


// uncomment for debug mode
//#define DEBUG


#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <tuple>
#include <stack>
#include <thread>
#include <algorithm>
#include <random>
#include <chrono>
#include "bloom_filter.hpp"


#define ERROR -1
#define NUM_ARGS 2
#define MIN_ORDER 3
#define OUTPUT_FILE "output_file.txt"


using namespace std;


// generic node
class Node
{
	protected:
		bool isLeaf;
		vector<float> keys;

	public:
		bool Get_IsLeaf();
		vector<float>* Get_Keys();
		virtual void Insert(float key, string value){};
		virtual void Insert(float key, Node* rightChild){};
		virtual void Insert(float key, Node* leftChild, Node* rightChild){};
		virtual void Search(float key){};
		virtual void Search(float key1, float key2){};
		virtual Node* Split(float* keyToParent){};
		virtual vector<Node*>* Get_Children(){};
		virtual vector< vector <string> >* Get_Values(){};
		virtual Node* Get_Next(){};
};


// internal node
class InternalNode : public Node
{
	private:
		vector<Node*> children;

	public:
		InternalNode();
		void Insert(float key, Node* rightChild);
		void Insert(float key, Node* leftChild, Node* rightChild);
		Node* Split(float* keyToParent);
		vector<Node*>* Get_Children();
};


// leaf node
class LeafNode : public Node
{
	private:
		LeafNode* prev;
		LeafNode* next;
		vector< vector <string> > values;

	public:
		LeafNode();
		void Insert(float key, string value);
		Node* Split(float* keyToParent);
		vector< vector <string> >* Get_Values();
		Node* Get_Next();
};


// B+ tree
class BPlusTree
{
	private:
		int order;
		Node* root;
		float max_insert;
		float min_insert;
		bool first_insert;
		ofstream outputFile;
		void Search_Path(Node* node, float key, stack<Node*>* path);
		void Destroy(Node* node);

#ifdef DEBUG
		void Reveal_Tree(Node* node);
#endif

	public:
		BPlusTree(int order);
		void Initialize(int m);
		void Insert(float key, string value);
		vector<string>* Search(float key);
		vector<tuple<float, string>>* Search(float key1, float key2);
		vector<tuple<float, string>>* scan(float key1, float key2);
		std::chrono::duration<double, std::milli> build(string input_file);
		void Open_Output_File();
		void Close_Output_File();
		float get_max_insert();
		float get_min_insert();
		~BPlusTree();

#ifdef DEBUG
		void Print_Tree();
#endif
};

class ParallelBPlusTree
{
	private:
		int num_trees;
		int trees_order;
		std::mt19937 rng;
		std::uniform_int_distribution<int> uni;
		vector<thread> threads;
		vector<BPlusTree*> trees;
		vector<bloom_filter*> filters;
		void thread_insert(vector<tuple<float, string>>* inserts, BPlusTree* tree, bloom_filter* filter);
		void thread_search(BPlusTree* tree, float key);
		void thread_range_search(BPlusTree* tree, float key1, float key2);
		void sync_threads();
	public:
		ParallelBPlusTree(int trees_order, int num_trees = 0, int projected_element_count = 10000);
		~ParallelBPlusTree();
		std::chrono::duration<double, std::milli> build(string input_file);
		void search(float key);
		void search(float key1, float key2);
		void insert(float key, string value, bool preserve_locality=true);
};

#endif
