#include "Board.h"

//---CONSTRUCTOR---//
Board::Board(){
	rows = 6;
	columns=7;
	new_tile_needed = true;
	tile_1 = '1';
	tile_2 = '2';

	for(int i=0;i<rows;++i)
		for(int j=0;j<columns;++j)
			positions.push_back(' ');
	//init base
	base.setSize(sf::Vector2f(WINDOW_WIDTH*0.8,WINDOW_HEIGHT/20.0));
	base.setPosition(sf::Vector2f(WINDOW_WIDTH*0.1,WINDOW_HEIGHT*0.8));
	base.setFillColor(sf::Color::Blue);
	base.setOutlineThickness(-3);
	base.setOutlineColor(sf::Color(100,100,255));
}

//---OPERATORS---//
//return by reference
char& Board::operator()(const int row_in, const int col_in){

	assert(col_in <= columns);
	assert(col_in > 0);
	assert(row_in > 0);
	assert(row_in <=rows);

	return positions[(row_in-1)*columns+(col_in-1)];
}
//return by value
char Board::operator()(const int row_in, const int col_in) const{
	
	assert(col_in <= columns);
	assert(col_in > 0);
	assert(row_in > 0);
	assert(row_in <=rows);

	return positions[(row_in-1)*columns+(col_in-1)];
}

//---GETTER---//
char Board::getFirstTile() const{
	return tile_1;
}
char Board::getSecondTile() const{
	return tile_2;
}
int Board::getNumberOfRows() const{
	return rows;
}
int Board::getNumberOfCols() const{
	return columns;
}

//---TILE INTERACTION---//
void Board::createTile(){
	Tile temp_tile;
	int player_num = (tiles.size())%2;
	temp_tile.initTile(player_num);	
	tiles.push_back(temp_tile);
}
bool Board::needNewTile(){	
	if(new_tile_needed){
		new_tile_needed=false;
		return true;
	}
	return false;
}
void Board::moveTileLeft(){
	tiles.back().moveLeft();
}
void Board::moveTileRight(){
	tiles.back().moveRight();
}
void Board::startDroppingTile(){
	tiles.back().startedMoving();
}
void Board::finishDroppingTile(){
	tiles.back().finishedMoving();
	new_tile_needed = true;
//	printBoard();			//uncomment to compare graphics with memory
}

bool Board::isTileMoving() const{
	if(tiles.back().isMoving())
		return true;

	return false;	
}		

int Board::getTilePosition_Column() const{
	return tiles.back().getColumnPosition(); 
}

void Board::checkForCollisions(Tile &dropping_tile){

	if(dropping_tile.checkCollisionWithBase(base)){ 	
		while(dropping_tile.checkCollisionWithBase(base)){		
			dropping_tile.move(0,-0.1);		//move tile up pixel by pixel until no longer colliding
		}
		//if collision, if speed is low, then stop, else rebound 
		if(dropping_tile.getMySpeed() < 0.5)
			finishDroppingTile();
		else dropping_tile.setMySpeed(dropping_tile.getMySpeed()*(-0.35));
	}
	else{
		for(int i=tiles.size()-2;i>-1;--i){		//iterate through tiles from most to least recent
			if(dropping_tile.checkCollisionWithTile(tiles[i])){		
				while(dropping_tile.checkCollisionWithTile(tiles[i])){		
					dropping_tile.move(0,-0.1);
				}	
			//if collision, if speed is low, then stop, else rebound 
			if(dropping_tile.getMySpeed() < 0.5)
				finishDroppingTile();
			else dropping_tile.setMySpeed(dropping_tile.getMySpeed()*(-0.35));
			
			break;
			}
		}
	}
}

void Board::tileDropping(sf::Int32 update_time){
	//speed is in units/ms, where 1 unit = 60 pixels
//	tiles.back().setMySpeed(tiles.back().getMySpeed()+9.8*update_time/(1000));		//actual speed too fast
	tiles.back().setMySpeed(tiles.back().getMySpeed()+9.8*update_time*240.0/MILLION);	//240 is constant to modify acceleration
	tiles.back().move(0,+tiles.back().getMySpeed()*update_time);

	checkForCollisions(tiles.back());
}

//---BOARD IN MEMORY---//
int Board::columnFull(int col) const
{
	if((*this)(rows,col) != ' ')	//topmost element of each column
		return 1;

	return 0;
}

void Board::insertTileInColumn(int col, char tile)
{
	int row=1;

	if(columnFull(col))
		return;

	while((*this)(row,col)!=' '){
		++row;	

		if(row==rows)		//stop at the top row
			break;
	}

	(*this)(row,col)=tile;	
	startDroppingTile();
}

int Board::detectWin(int order) const
{
	char tile='1';
	if(order==2)
		tile='2';

	//Horizontal
	for(int i=1;i<=rows;++i){
		for(int j=1;j<=(columns-3);++j){
		
			if((*this)(i,j) == tile && (*this)(i,j+1) == tile && (*this)(i,j+2) == tile && (*this)(i, j+3)==tile){
				return 1;	
			}
		}
	}

	//VERTICAL
	for(int j=1;j<=columns;++j){
		for(int i=1;i<=(rows-3);++i){
		
			if((*this)(i,j) == tile && (*this)(i+1,j) == tile && (*this)(i+2,j) == tile && (*this)(i+3, j)==tile){
				return 1;	
			}
		}
	}

	//DIAGONAL DOWN
	for(int i=4;i<=rows;++i){
		for(int j=1;j<=(columns-3);++j){
		
			if((*this)(i,j)==tile && (*this)(i-1,j+1) == tile && (*this)(i-2,j+2) == tile && (*this)(i-3, j+3)==tile){
				return 1;	
			}
		}
	}
	
	//DIAGONAL UP
	for(int i=1;i<=(rows-3);++i){
		for(int j=1;j<=(columns-3);++j){
		
			if((*this)(i,j)==tile && (*this)(i+1,j+1) == tile && (*this)(i+2,j+2) == tile && (*this)(i+3, j+3)==tile){
				return 1;
			}
		}
	}

	return 0;
}

void Board::printBoard() const{
	
	std::cout << std::endl;

	std::cout << "\t      ";
	for(int j=0;j<columns;++j)
		std::cout << " " <<j+1;
	std::cout << std::endl;
	
	std::cout << "\t      ";
	for(int j=0;j<columns;++j)
		std::cout << BLUE << " _" << RESET;
	std::cout << std::endl;

	for (int i=rows-1; i>-1; i--){
		std::cout << BLUE << "\t     ||" << RESET;		

		for(int j=0;j<columns;++j){
			if(positions[i*columns + j] == '1')
   				std::cout << RED << positions[i*columns + j] << RESET << BLUE << "|" << RESET;
					
			else if(positions[i*columns + j] == '2')
   				std::cout << YEL  << positions[i*columns + j] <<  RESET << BLUE << "|" << RESET;

			else std::cout << positions[i*columns + j] << BLUE << "|" << RESET;
  		}
	std::cout << BLUE << "|" << RESET << std::endl;
	}

	std::cout << BLUE << "\t     ==" <<RESET;
	for(int j=1;j<columns;j++)
		std::cout << BLUE << "==" <<RESET; 
	std::cout << BLUE << "===" << RESET << std::endl;
		
	std::cout << "\n---------------------------------------------\n";
}

//---DRAW TO SCREEN---//
void Board::draw(sf::RenderWindow& window) const{

	drawTiles(window);
	drawGrid(window);
	window.draw(base);
	drawOutline(window);	
}

void Board::drawTiles(sf::RenderWindow& window) const{
	for(auto tile: tiles)
		window.draw(tile);
}

void Board::drawGrid(sf::RenderWindow& window) const{
	for(int row=0;row<6;++row)
		for(int column=0;column<7;++column)
			drawUnit(window, row, column);
}

void Board::drawOutline(sf::RenderWindow& window) const{
	
	std::vector<sf::RectangleShape> boundary(3);

	boundary[0].setPosition(sf::Vector2f(1.5*WINDOW_WIDTH/10.0,2*WINDOW_HEIGHT/10.0));
	boundary[1].setPosition(sf::Vector2f(1.5*WINDOW_WIDTH/10.0,2*WINDOW_HEIGHT/10.0));
	boundary[2].setPosition(sf::Vector2f(8.5*WINDOW_WIDTH/10.0,2*WINDOW_HEIGHT/10.0));

	boundary[0].setSize(sf::Vector2f(2,6*WINDOW_HEIGHT/10.0));
	boundary[1].setSize(sf::Vector2f(2,7*WINDOW_HEIGHT/10.0+2));
	boundary[2].setSize(sf::Vector2f(2,6*WINDOW_HEIGHT/10.0));

	boundary[1].rotate(-90);

	boundary[0].setFillColor(sf::Color(100,100,255));
	boundary[1].setFillColor(sf::Color(100,100,255));
	boundary[2].setFillColor(sf::Color(100,100,255));

	for(auto bound : boundary)
		window.draw(bound);
}

void Board::drawUnit(sf::RenderWindow& window, int row, int col) const{

//	double thickness_factor = 1.0/6.5;	//makes just circle
	double thickness_factor = 1.0/9.0;	//favourite so far
	double border_width_vert = (WINDOW_WIDTH/10.0)*thickness_factor;
	double border_height_vert = WINDOW_HEIGHT/10.0;
	double border_width_horiz = (WINDOW_WIDTH/10.0);
	double border_height_horiz = (WINDOW_HEIGHT/10.0)*thickness_factor;
	
	sf::CircleShape hole;
	hole.setRadius(WINDOW_HEIGHT/20.0);		//half the height of border
	hole.setOutlineThickness(-border_width_vert);	//outline thickness = width of border
	hole.setOutlineColor(sf::Color::Blue);
	hole.setFillColor(sf::Color::Transparent);
	hole.setPosition(sf::Vector2f((1.5+col)*WINDOW_WIDTH/10.0,((2+row)*WINDOW_HEIGHT/10.0)));

	std::vector<sf::RectangleShape> border(4);
	
	border[0].setSize(sf::Vector2f(border_width_vert,border_height_vert));
	border[1].setSize(sf::Vector2f(border_width_horiz,border_height_horiz));
	border[2].setSize(sf::Vector2f(border_width_vert,border_height_vert));
	border[3].setSize(sf::Vector2f(border_width_horiz,border_height_horiz));

	border[0].setPosition(sf::Vector2f((1.5+col)*WINDOW_WIDTH/10.0,((2+row)*WINDOW_HEIGHT/10.0)));
	border[1].setPosition(sf::Vector2f((1.5+col)*WINDOW_WIDTH/10.0,((2+row)*WINDOW_HEIGHT/10.0)));
	border[2].setPosition(sf::Vector2f((1.5+(1-thickness_factor)+col)*WINDOW_WIDTH/10.0,((2+row)*WINDOW_HEIGHT/10.0)));
	border[3].setPosition(sf::Vector2f((1.5+col)*WINDOW_WIDTH/10.0,((2+(1-thickness_factor)+row)*WINDOW_HEIGHT/10.0)));		
	
	border[0].setFillColor(sf::Color::Blue);	
	border[1].setFillColor(sf::Color::Blue);	
	border[2].setFillColor(sf::Color::Blue);	
	border[3].setFillColor(sf::Color::Blue);	

	window.draw(hole);	
	for(auto& side : border)
		window.draw(side);
}

//---WINNING ANIMATION---//
void Board::findWinningTiles(std::vector<coordinates>&winning_tiles){

	for(auto winning_tile : winning_tiles){
		for(unsigned int i=0;i<tiles.size();++i ){
			if(winning_tile.row == tiles[i].getRowPosition() && winning_tile.col == tiles[i].getColumnPosition()){
				tiles_vector_position.push_back(i);
			}
		}
	}
}
