# Name of the executable
OUTPUT = c+

# Cpp compiler
CXX = ccache g++

# Cpp flags
CXXFLAGS = \
	-O3 \
	-g \
	-rdynamic \
	-Wall \
	-Wextra \
	-Werror \
	-std=c++26

# Find all source files
SRCFILES := $(shell find -L * -type f | LC_ALL=C sort)
CPPFILES := $(filter %.cpp,$(SRCFILES))
OBJ := $(addprefix obj/,$(CPPFILES:.cpp=.cpp.o))

.PHONY: all run test clean

all: obj/$(OUTPUT)

# Link rules for the final executable.
obj/$(OUTPUT): $(OBJ)
	mkdir -p "$$(dirname $@)"
	$(CXX) $(CXXFLAGS) $(OBJ) -o $@

# Compilation rules for *.cpp files.
obj/%.cpp.o: %.cpp 
	mkdir -p "$$(dirname $@)"
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Build and run the executable
run: all
	@valgrind --log-file="valgrind.log" ./obj/$(OUTPUT) test.cp

# Assembles and runs the generated assembly file
test: run
	@nasm -f elf64 test.asm -o test.o && \
	gcc -no-pie test.o -o test && \
	./test

# Remove object files and the final executable.
.PHONY: clean
clean:
	rm -rf obj test.asm test.o test
