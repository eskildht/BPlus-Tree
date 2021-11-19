# B+ Tree
B+ trees are order “m” trees, often with a variable number of children per node. Unlike other such trees, in B+ trees, the internal nodes store only keys, while key-value pairs are stored in the leaf nodes. 

This repo holds a C++ implementation of a memory-resident B+ tree, as well as a B+ tree version using multiple threads during operations.

### Commands:
To build the source code:
- **`mkdir out`**
- **`make`**

To execute one of the benchmarking tests: 
- **`./out/test-<test-name>`**
- Possible options are found in **`./tests/<test-name>.cpp`**
