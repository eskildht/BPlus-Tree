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

test-all: test-insert test-build test-parallel-build

test-insert: $(SRC)
	@echo "Compiling test-insert... "
	@g++ -std=c++11 $(SRC) tests/insert.cpp -I$(INC_DIR) -o out/test-insert.out -w
	@echo "Done!"

test-build: $(SRC)
	@echo "Compiling test-build... "
	@g++ -std=c++11 $(SRC) tests/build.cpp -I$(INC_DIR) -o out/test-build.out -w
	@echo "Done!"

test-parallel-build: $(SRC_PARALLEL)
	@echo "Compiling test-parallel-build... "
	@g++ -std=c++11 $(SRC_PARALLEL) tests/parallel-build.cpp -I$(INC_DIR) -o out/test-parallel-build.out -w
	@echo "Done!"
