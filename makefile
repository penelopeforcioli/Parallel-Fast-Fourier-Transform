# Compiler and flags
CXX = g++
CXXFLAGS = -Wall -std=c++17

# ROOT flags
ROOTCFLAGS = `root-config --cflags`
ROOTLIBS = `root-config --glibs`

# Source directories
SRC_DIRS = . sequential_dft parallel_dft data

# Source files
SOURCES = main.cpp compression.cpp multiplication.cpp full_dft.cpp plot.cpp $(wildcard sequential_dft/*.cpp) $(wildcard parallel_dft/*.cpp) $(wildcard data/*.cpp)

# Object files
OBJECTS = $(SOURCES:.cpp=.o)

# Executable
EXECUTABLE = compute

# Default target
all: $(EXECUTABLE)

# Build the executable
$(EXECUTABLE): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(ROOTLIBS)

# Rule for compiling .cpp files to .o files with ROOT include flags
%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(ROOTCFLAGS) -c $< -o $@

# Clean target to remove object files and executable
clean:
	rm -f $(OBJECTS) $(EXECUTABLE)
	rm -rf plots/*
	rm -f selected_dataset.txt
