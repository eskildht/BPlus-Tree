INC_DIR = inc

SRC = src/tree.cpp \
      src/node.cpp \
      src/internalnode.cpp \
      src/leafnode.cpp

SRC_PARALLEL = src/paralleltree.cpp \
	       src/tree.cpp \
	       src/node.cpp \
	       src/internalnode.cpp \
	       src/leafnode.cpp

test-build: $(SRC)
	@echo "Compiling test-parallel-build... "
	@g++ -std=c++11 $(SRC) tests/build.cpp -I$(INC_DIR) -o test-build.out -w
	@echo "Done!"

test-parallel-build: $(SRC_PARALLEL)
	@echo "Compiling test-parallel-build... "
	@g++ -std=c++11 $(SRC_PARALLEL) tests/parallel-build.cpp -I$(INC_DIR) -o test-parallel-build.out -w
	@echo "Done!"
