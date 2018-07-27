CXX=g++
CXXFLAGS=-std=c++11 -Wall -W
LIBSFML=-lsfml-graphics -lsfml-window -lsfml-system

objects = Tile.o Board.o Player.o Menu.o Game.o mainConnect.o

all: $(objects)
	$(CXX) $(objects) $(CXXFLAGS) $(LIBSFML) -o connect4

mainConnect.o : Game.h
Tile.o : Tile.h
Board.o : Board.h Tile.h
Player.o : Board.h
Menu.o : Menu.h
Game.o : Player.h Menu.h

.PHONY : clean
clean:
	rm -f connect4 $(objects)
