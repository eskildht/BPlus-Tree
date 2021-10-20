INC_DIR = inc

SRC = src/main.cpp \
      src/tree.cpp \
      src/node.cpp \
      src/internalnode.cpp \
      src/leafnode.cpp

SRC_SINGLE = src/single-main.cpp \
	     src/tree.cpp \
	     src/node.cpp \
	     src/internalnode.cpp \
	     src/leafnode.cpp


SRC_PARALLEL = src/paralleltree.cpp \
	       src/tree.cpp \
	       src/node.cpp \
	       src/internalnode.cpp \
	       src/leafnode.cpp

original: $(SRC)
	@echo "Compiling original B+ Tree source... "
	@g++ -std=c++11 $(SRC) -I$(INC_DIR) -o main-original.out -w
	@echo "Done!"

single:	$(SRC_SINGLE)
	@echo "Compiling single B+ Tree source... "
	@g++ -std=c++11 $(SRC_SINGLE) -I$(INC_DIR) -o main-single.out -w
	@echo "Done!"

parallel: $(SRC_PARALLEL)
	@echo "Compiling parallel B+ Tree source... "
	@g++ -std=c++11 $(SRC_PARALLEL) -I$(INC_DIR) -o main-parallel.out -w
	@echo "Done!"

test-parallel-build: $(SRC_PARALLEL)
	@echo "Compiling test-parallel-build... "
	@g++ -std=c++11 $(SRC_PARALLEL) tests/parallel-build.cpp -I$(INC_DIR) -o test-parallel-build.out -w
	@echo "Done!"

clean:
	@echo "Removing output_file.txt... "
	@rm output_file.txt
	@echo "Done!"
