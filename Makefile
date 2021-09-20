INC_DIR = inc
SRC = src/main.cpp \
	src/tree.cpp \
	src/node.cpp \
	src/internalnode.cpp \
	src/leafnode.cpp

all: $(SRC)
	@echo "Compiling B+ Tree source... "
	@g++ -std=c++11 $(SRC) -I$(INC_DIR) -o treesearch
	@echo "Done!" 

clean: 
	@echo "Removing output_file.txt... "
	@rm output_file.txt
	@echo "Done!"
