#include "Board.h"

//---CONSTRUCTOR---//
Board::Board(int nrows, int ncols) : nrows{nrows}, ncols{ncols}, elem{new char[nrows * ncols]}
{
	tile_1 = '0';
	tile_2 = 'O';

	for(int i=0;i<nrows;++i) 
		for(int j=0;j<ncols;++j)
   			elem[i*ncols+j] =  ' ';	
}

//---COPY CONSTUCTOR---//
Board::Board(const Board& board_in) : nrows(board_in.nrows), ncols(board_in.ncols), tile_1(board_in.tile_1), tile_2(board_in.tile_2)
{
	elem = new char[nrows * ncols];
	memcpy(elem, board_in.elem, nrows*ncols*sizeof(char));
}

//---DESTRUCTOR---
Board::~Board() { delete[] elem; } //deallocates

//SIZES
int Board::getNumberOfRows() const{
	return nrows;
}
int Board::getNumberOfCols() const{
	return ncols;
}
char Board::getFirstTile() const{
	return tile_1;
}
char Board::getSecondTile() const{
	return tile_2;
}

//---SELECT ELEMENTS OF BOARD BY HUMAN READABILITY---//
char& Board::operator()(const int row_in, const int col_in) const
{
	//this function replaces [0][0]->(1,1), [2][3]->(3,4) etc

	//can't access forbidden memory 
	if(col_in > ncols){
		std::cerr << "Error: Column selection too large" << std::endl;
		exit(1);
	}
	if(col_in < 1){
		std::cerr << "Column must be greater than 0\n" << std::endl;
		exit(1);
	}

	if(row_in > nrows)
		exit(1);	
		//row_in = nrows;

	//To access a specific element, simply give the (row, col)
			
	return elem[(row_in-1)*ncols + (col_in-1)];
}

void Board::makeMoveForPlayer(int selection, char player_type, char player_tile)
{
	if(player_type == 'A')
		usleep(400000);
		
	insertInColumn(selection,player_tile);
	fancyPrint(selection, player_tile);
}

//---DETECT IF COLUMN IS FULL---//
int Board::columnFull(int col) const
{
	if(elem[(nrows-1)*ncols + (col-1)] != ' ')	//topmost element of each column
		return 1;

	return 0;
}

//---INSERT AT COLUMN---//
void Board::insertInColumn(int col, char tile) const
{
	int row=1;

	if(columnFull(col))
		return;

	while((*this)(row,col)!=' '){
		++row;	

		if(row==nrows)		//stop at the top row
			break;
	}

	(*this)(row,col)=tile;	
}

//---REMOVE TOPMOST IN COLUMN---//
void Board::removeFromColumn(int col) const
{
	int row=nrows;

	while((*this)(row,col)==' '){
			--row;
		
			if(row==1)	//stop at bottom row
				break;
	}

	(*this)(row,col)=' ';	
}

//---FANCY PRINT---//
void Board::fancyPrint(int col, char tile_holder) const
{
	int row = nrows;
	removeFromColumn(col);
		
	printTitle();
	while((*this)(row,col) == ' ' && row != 1){ 

		(*this)(row, col) = tile_holder;	//put tile at place in column, starting from top
		printTitle();
		printBoard();				//print board
		usleep(90000);
		
		(*this)(row, col) = ' ';		//put tile at place in column, starting from top
		
		row--;
	}

	insertInColumn(col, tile_holder);

	printTitle();
	printBoard();

}

void Board::printWinFlash(int order) const{
	
	int row,col,win_dir;
	char tile='0';
	if(order==2)
		tile='O';
	//Horizontal
	for(int i=1;i<=nrows;++i){
		for(int j=1;j<=(ncols-3);++j){	
			if((*this)(i,j) == tile && (*this)(i,j+1) == tile && (*this)(i,j+2) == tile && (*this)(i, j+3)==tile){
				row=i;
				col=j;
				win_dir=0;	
			}
		}
	}
	//VERTICAL
	for(int j=1;j<=ncols;++j){
		for(int i=1;i<=(nrows-3);++i){
			if((*this)(i,j) == tile && (*this)(i+1,j) == tile && (*this)(i+2,j) == tile && (*this)(i+3, j)==tile){
				row=i;
				col=j;
				win_dir=1;	
			}
		}
	}
	//DIAGONAL DOWN
	for(int i=4;i<=nrows;++i){
		for(int j=1;j<=(ncols-3);++j){
			if((*this)(i,j)==tile && (*this)(i-1,j+1) == tile && (*this)(i-2,j+2) == tile && (*this)(i-3, j+3)==tile){
				row=i;
				col=j;
				win_dir=2;	
			}
		}
	}
	//DIAGONAL UP
	for(int i=1;i<=(nrows-3);++i){
		for(int j=1;j<=(ncols-3);++j){
			if((*this)(i,j)==tile && (*this)(i+1,j+1) == tile && (*this)(i+2,j+2) == tile && (*this)(i+3, j+3)==tile){
				row=i;
				col=j;
				win_dir=3;	
			}
		}
	}

	//SET THE WINNING TILES TO BLANK SPACE
	for(int iter=0;iter<14;++iter){
		printTitle();	

		if(iter%2==0){
			if(win_dir==0){
				(*this)(row,col)=' ';(*this)(row,col+1)=' ' ;(*this)(row,col+2)=' ';(*this)(row, col+3)=' ';
			}
			else if(win_dir==1){
				(*this)(row,col)=' ';(*this)(row+1,col)=' ';(*this)(row+2,col)=' ';(*this)(row+3,col)=' ';
			}
			else if(win_dir==2){
				(*this)(row,col)=' ';(*this)(row-1,col+1)=' ';(*this)(row-2,col+2)=' ';(*this)(row-3,col+3)=' ';
			}
			else{	
				(*this)(row,col)=' ';(*this)(row+1,col+1)=' ';(*this)(row+2,col+2)=' ';(*this)(row+3,col+3)=' ';
			}
			printBoard();	
			printWinner(order);
			usleep(100000);
		}
		else{
			if(win_dir==0){
				(*this)(row,col)=tile;(*this)(row,col+1)=tile;(*this)(row,col+2)=tile;(*this)(row,col+3)=tile;
			}
			else if(win_dir==1){
				(*this)(row,col)=tile;(*this)(row+1,col)=tile;(*this)(row+2,col)=tile;(*this)(row+3,col)=tile;
			}
			else if(win_dir==2){
				(*this)(row,col)=tile;(*this)(row-1,col+1)=tile;(*this)(row-2,col+2)=tile;(*this)(row-3,col+3)=tile;
			}
			else{	
				(*this)(row,col)=tile;(*this)(row+1,col+1)=tile;(*this)(row+2,col+2)=tile;(*this)(row+3,col+3)=tile;
			}
			printBoard();	
			printWinner(order);
			usleep(100000);
		}
	}
}

//---PRINT BOARD---//
void Board::printBoard() const{
	
	std::cout << std::endl;

	std::cout << "\t      ";
	for(int j=0;j<ncols;++j)
		std::cout << " " <<j+1;
	std::cout << std::endl;
	
	std::cout << "\t      ";
	for(int j=0;j<ncols;++j)
		std::cout << BLUE << " _" << RESET;
	std::cout << std::endl;

	for (int i=nrows-1; i>-1; i--){
		std::cout << BLUE << "\t     ||" << RESET;		

		for(int j=0;j<ncols;++j){
			if(elem[i*ncols + j] == tile_1)
   				std::cout << RED << elem[i*ncols + j] << RESET << BLUE << "|" << RESET;
					
			else if(elem[i*ncols + j] == tile_2)
   				std::cout << YEL  << elem[i*ncols + j] <<  RESET << BLUE << "|" << RESET;

			else std::cout << elem[i*ncols + j] << BLUE << "|" << RESET;
  		}
	std::cout << BLUE << "|" << RESET << std::endl;
	}

	std::cout << BLUE << "\t     ==" <<RESET;
	for(int j=1;j<ncols;j++)
		std::cout << BLUE << "==" <<RESET; 
	std::cout << BLUE << "===" << RESET << std::endl;
		
	std::cout << "\n---------------------------------------------\n";
}

//---DETECT STALEMATE---//
int Board::staleMate() const
{
	for(int i=1;i<=nrows;i++){
		for(int j=1;j<=ncols;j++){
			if((*this)(i,j) == ' ')	//if at any point a free space is found then not stalemate, return 0
				return 0;
		}
	}
	return 1;
}

//---DETERMINE IF 4 TILES IN A ROW---//
//simple for loop over elements
int Board::detectWin(int order) const
{
	char tile='0';
	if(order==2)
		tile='O';

	//Horizontal
	for(int i=1;i<=nrows;++i){
		for(int j=1;j<=(ncols-3);++j){
		
			if((*this)(i,j) == tile && (*this)(i,j+1) == tile && (*this)(i,j+2) == tile && (*this)(i, j+3)==tile)
				return 1;	
		}
	}

	//VERTICAL
	for(int j=1;j<=ncols;++j){
		for(int i=1;i<=(nrows-3);++i){
		
			if((*this)(i,j) == tile && (*this)(i+1,j) == tile && (*this)(i+2,j) == tile && (*this)(i+3, j)==tile)
				return 1;	
		}
	}

	//DIAGONAL DOWN
	for(int i=4;i<=nrows;++i){
		for(int j=1;j<=(ncols-3);++j){
		
			if((*this)(i,j)==tile && (*this)(i-1,j+1) == tile && (*this)(i-2,j+2) == tile && (*this)(i-3, j+3)==tile)
				return 1;	
		}
	}
	
	//DIAGONAL UP
	for(int i=1;i<=(nrows-3);++i){
		for(int j=1;j<=(ncols-3);++j){
		
			if((*this)(i,j)==tile && (*this)(i+1,j+1) == tile && (*this)(i+2,j+2) == tile && (*this)(i+3, j+3)==tile)
				return 1;
		}
	}

	return 0;
}

void selectPlayers(char& player_1, char& player_2){

	char choice_in;

	//---PLAYER ONE---//
	printTitle();
	printPlayerSelect(1);
	std::cout << "Player 1:\n";
	std::cout << "Player 2:\n";
	std::cin>>choice_in;
	choice_in=toupper(choice_in);	
	//error check
	while(choice_in != 'H' && choice_in !='A' && choice_in !='X'){
		std::cout << "Error, not a valid option, try again\n";
		std::cin >> choice_in;
		choice_in=toupper(choice_in);
	}
	player_1 = choice_in; 
	//exit if X chosen
	if(choice_in=='X') return;

	//---PLAYER TWO---//
	printTitle();
	printPlayerSelect(2);
	//print previous choice
	std::cout << "Player 1: ";
	if(player_1=='H') std::cout << "Human\n";
	else std::cout << "AI\n";
	std::cout << "Player 2: ";

	std::cin>>choice_in;
	choice_in=toupper(choice_in);
	//error check
	while(choice_in != 'H' && choice_in !='A' && choice_in !='X'){
		std::cout << "Error, not a valid option, try again\n";
		std::cin >> choice_in;
		choice_in=toupper(choice_in);
	}
	player_2 = choice_in; 
	//exit if X chosen
	if(choice_in == 'X') return;
}

//---PRINT TITLE---//

void printTitle()
{
	system("clear");
	std::cout << "---------------------------------------------\n";
	std::cout << "|    ___                      _     _ _     |\n";
	std::cout << "|   / __|___ _ _  _ _  ___ __| |_  | | |    |\n";
	std::cout << "|  | (__/ _ \\ ' \\| ' \\/ -_) _|  _| |_  _|   |\n";
	std::cout << "|   \\___\\___/_||_|_||_\\___\\__|\\__|   |_|    |\n";
	std::cout << "|                                           |\n";
	std::cout << "---------------------------------------------\n";
}
void printPlayerSelect(int player_no)
{
	std::cout << "\n\t-----------------------\n";
	std::cout << "\t|   Select Players:   |\n";
	std::cout << "\t-----------------------\n";
	std::cout << "\t| Would you like      | \n";
	std::cout << "\t| Player " << player_no << " to be      |\n";
	std::cout << "\t|                     | \n";
	std::cout << "\t|      [H]  Human     |\n"; 
	std::cout << "\t|            or       | \n";
	std::cout << "\t|      [A]   AI	      | \n";
	std::cout << "\t| 		      | \n";
	std::cout << "\t|      [X]  Exit      | \n";
	std::cout << "\t|                     | \n";
	std::cout << "\t-----------------------\n";
}

void printWinner(int order)
{
	if(order == 1)
		std::cout << "              WINNER IS PLAYER 1\n";
	else std::cout << "              WINNER IS PLAYER 2\n";
	std::cout << "\n---------------------------------------------\n";
}
