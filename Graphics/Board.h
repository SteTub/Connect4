#ifndef _BOARD_H_INCLUDED_
#define _BOARD_H_INCLUDED_

#include "Tile.h"

#define RED "\033[31m"
#define BLUE "\033[34m"
#define YEL "\033[33m"
#define RESET "\x1B[0m"

struct coordinates{
	int row;
	int col;
};

class Board{

	public:
	Board();
	~Board(){};

	char& operator()(const int row_in, const int col_in);
	char operator()(const int row_in, const int col_in) const;
	
	//getters
	char getFirstTile() const;
	char getSecondTile() const;
	int getNumberOfRows() const;
	int getNumberOfCols() const;

	//tile interaction
	void createTile();	
	bool needNewTile();
	void moveTileLeft();
	void moveTileRight();
	void startDroppingTile();
	void finishDroppingTile();
	bool isTileMoving() const;
	int getTilePosition_Column() const;
	void checkForCollisions(Tile &dropping_tile);
	void tileDropping(sf::Int32 update_time);

	//board in memory
	int columnFull(int col) const;
	void insertTileInColumn(int col, char tile);
	int detectWin(int order) const;
	bool detectStalemate() const;
	void printBoard() const;
	void resetBoard();
		
	//draw
	void draw(sf::RenderWindow& window) const;
	void drawTiles(sf::RenderWindow& window) const;
	void drawUnit(sf::RenderWindow& window, int row, int column) const;
	void drawGrid(sf::RenderWindow& window) const; 
	void drawBase(sf::RenderWindow& window) const;
	void drawOutline(sf::RenderWindow& window) const;

	//winning animation
	void findWinningTiles(std::vector<coordinates>&winning_tiles);
	void flashWinningTiles();

	private:	
		int rows;
		int columns;
		bool new_tile_needed;
		sf::RectangleShape base;
		std::vector<Tile> tiles;
		std::vector<int> tiles_vector_position;
		std::vector<char> positions;
		char tile_1, tile_2;
};

#endif
