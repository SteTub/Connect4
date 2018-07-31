CXX=g++
CXXFLAGS=-std=c++11 -Wall -W

objects = Board.o Player.o mainConnect.o

all: $(objects)
	$(CXX) $(objects) $(CXXFLAGS) -o connect4

Board.o : Board.h  
Player.o : Board.h Player.h
mainConnect.o : Board.h Player.h

.PHONY : clean
clean:
	rm -f $(objects) connect4
