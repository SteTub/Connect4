CXX=g++
CXXFLAGS=-std=c++11 -Wall -W

objects = Connect.o Player.o mainConnect.o

all: $(objects)
	$(CXX) $(objects) $(CXXFLAGS) -o connect4

Connect.o : Connect.h  
Player.o : Connect.h Player.h
mainConnect.o : Connect.h Player.h

.PHONY : clean
clean:
	rm -f $(objects) connect4
