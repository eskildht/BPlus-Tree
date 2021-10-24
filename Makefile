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

test-all: test-build test-search test-parallel-build test-parallel-search

test-build: $(SRC)
	@echo "Compiling test-build... "
	@g++ -std=c++11 $(SRC) tests/build.cpp -I$(INC_DIR) -o out/test-build.out -w
	@echo "Done!"

test-search: $(SRC)
	@echo "Compiling test-search... "
	@g++ -std=c++11 $(SRC) tests/search.cpp -I$(INC_DIR) -o out/test-search.out -w
	@echo "Done!"

test-parallel-build: $(SRC_PARALLEL)
	@echo "Compiling test-parallel-build... "
	@g++ -std=c++11 $(SRC_PARALLEL) tests/parallel-build.cpp -I$(INC_DIR) -o out/test-parallel-build.out -w
	@echo "Done!"

test-parallel-search: $(SRC_PARALLEL)
	@echo "Compiling test-parallel-search... "
	@g++ -std=c++11 $(SRC_PARALLEL) tests/parallel-search.cpp -I$(INC_DIR) -o out/test-parallel-search.out -w
	@echo "Done!"

debug: $(SRC)
	@echo "Compiling debug... "
	@g++ -std=c++11 $(SRC_PARALLEL) src/single-main.cpp -I$(INC_DIR) -o out/debug.out -w
	@echo "Done!"
