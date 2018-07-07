CC=g++
CFLAGS=-std=c++11 -Wall -W

objects = Connect.o Player.o mainConnect.o

all: $(objects)
	$(CC) $(objects) $(CFLAGS) -o connect4

Connect.o : Connect.cc Connect.h
	$(CC) $(CFLAGS) -c Connect.cc -o Connect.o
Player.o : Player.cc Connect.h Player.h
	$(CC) $(CFLAGS) -c Player.cc -o Player.o
mainConnect.o : mainConnect.cc Player.h Connect.h
	$(CC) $(CFLAGS) -c mainConnect.cc -o mainConnect.o

.PHONY : clean
clean:
	rm -f $(objects) connect4
