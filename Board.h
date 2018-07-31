#include<iostream>
#include<cstring>
#include<unistd.h>
#include<sys/time.h>

#define RED "\033[31m"
#define BLUE "\033[34m"
#define YEL "\033[33m"
#define RESET "\x1B[0m"

class Board {

public:
	//Constructor
	Board(int nrows, int ncols);
	
	Board(const Board& board_in);

	//Destructor
	~Board();

	char& operator()(const int row_in, const int col_in) const;
	
	int row_size() const;	
	int col_size() const;
	char firstTile() const;
	char secondTile() const;
	
	int columnFull(int col) const;		
	void insertInColumn(int col, char tile) const;	
	void removeFromColumn(int col) const;
	//print methods
	void printBoard() const;		
	void fancyPrint(int col, char tile_holder) const;
	void printWinFlash(int order) const;
	//end conditions
	int staleMate() const;
	int detectWin(int order) const;
	
private:
	int nrows, ncols;	//row and column sizes
	char tile_1, tile_2;	//tiles for player 1 and 2
	char *elem;		//elements of grid, store grid elements as 1D array
		
};

//---NON CLASS FUNCTIONS---//

char selectMode();
void selectPlayers(char& player_1, char& player_2);
void printPlayerSelect(int player_no);
void printTitle();
void printWinner(int order);
