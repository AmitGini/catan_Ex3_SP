CXX = g++
CXXFLAGS = -g -std=c++11 -Wall

# Object files
OBJ = Board.o Edge.o KnightCard.o LargestArmyCard.o MonopolyCard.o Player.o RoadCard.o Tile.o Vertex.o VictoryPointCard.o YearOfPlentyCard.o

all: catan catan_tests

# Main application
catan: $(OBJ) catan.o
	$(CXX) $(CXXFLAGS) -o catan $(OBJ) catan.o

# Tests
catan_tests: $(OBJ) catan_tests.o
	$(CXX) $(CXXFLAGS) -o catan_tests $(OBJ) catan_tests.o

# Compile object files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean
clean:
	rm -f catan catan_tests $(OBJ) catan.o catan_tests.o

.PHONY: all clean catan catan_tests