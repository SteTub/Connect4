#include "Player.h"

Player* Player::createPlayer(char choice, char tile, int order)
{
	switch(choice) {
		case 'H': return new Human(tile, order);
		case 'A': return new AI_player(tile, order);
		default:  return 0;
	}
}

Player::~Player() {}

Human::Human(char tile, int player_num)
{
	 my_tile=tile;
	 player_no=player_num;
}

int Human::makeMove(Board &board){return 0;} //this does nothing on purpose

//=======AI PLAYER=======//

//---CONSTRUCTOR---//
AI_player::AI_player(char tile, int order)
{
	my_tile=tile;
	player_no=order;
}

//---ACCESS PRIVATE MEMBERS---//
char AI_player::returnMyTile() const{
	return my_tile;
}

//---MAKES THE MOVE USING STRATEGY---//
int AI_player::makeMove(Board &board)
{	
	//choose column based on strategy
	int selection = chooseColumnUsingStrategy(board);

	//if best column choice is full, select random column
	while(board.columnFull(selection)){
		selection = 1+(rand() %  board.getNumberOfCols());
	}
	
	//insert into board
//	usleep(400000);
//	board.insertInColumn(selection,player_no);
//	board.fancyPrint(selection,my_tile);
	return selection;
}

//---DETERMINE STRATEGY---//
int AI_player::chooseColumnUsingStrategy(const Board &board)
{
	int ncols = board.getNumberOfCols();
	int col = 0;
	char enemy_tile = board.getFirstTile();
	if(player_no == 1)
		enemy_tile = board.getSecondTile();

	//TRY TO WIN
	if(findWinningMove(board, my_tile) < ncols+1){		//if found 3 and can win, will win
		col = findWinningMove(board, my_tile);
	}	

	//BLOCK AN OPPONENTS WIN
	else if(findWinningMove(board, enemy_tile) < ncols+1){	//if opponent has 3 in row, block opponents win
		col = findWinningMove(board, enemy_tile);
	}	
	//Prevent a 3 in a horizontal row
	else if(findHorizontalPair(board, enemy_tile) < ncols+1){	//cut off opponent trap, if opponent has 2 in a row, will play next to one
		col = findHorizontalPair(board, enemy_tile);
	
		if(choiceWouldCauseLoss(board, col+1, my_tile, enemy_tile)){	//avoid giving opponent the win
			col = 0;
		}
	}
	//If the above three fail then picks random	
	if(col == 0){
		col = chooseRandomColumnGaussian(board, my_tile, enemy_tile);
	} 

	return col;
}

//---IF NO OTHER OPTION, PRIORITIZES MIDDLE---//
int AI_player::chooseRandomColumnGaussian(const Board &board, char tile, char enemy_tile)
{
	int ncols = board.getNumberOfCols();
	int mid = ncols/2;
	int col = ncols;

	std::random_device rd;
	std::mt19937 generator(rd());
	std::normal_distribution<> distrib(mid,0.7);

	//If number of columns is even, requires a different rng
	//for now ignoring and locking number of columns at 7
	if(ncols % 2 == 0)
		 return rand() % ncols;
		
	//odd number of columns
	else {			
		col = std::round(distrib(generator));	
		
		while(board.columnFull(col) && !(col >= 0) && !(col <= ncols-1))
			col = std::round(distrib(generator));	
	}

	//check if chosen column would create winning scenario for opponent
	while(choiceWouldCauseLoss(board, col+1, tile, enemy_tile))
	{
		int count=0;
		col = std::round(distrib(generator));
		count++;

		if(count > 1000)
			break;	
	}

	return col+1;		//change column to human readability

}

//---CHECK IF CHOSEN COLUMN CREATES A WINNING SCENARIO FOR OPPONENT---//
int AI_player::choiceWouldCauseLoss(const Board &board, int col, char tile, char enemy_tile)
{
	Board test_game = board;
	test_game.insertInColumn(col, tile);

	//CHECK FOR WIN
	if(findWinningMove(test_game, enemy_tile) < test_game.getNumberOfCols())		//if found 3 and can win, will win
		return 1;
	
	return 0;
}

//Find two tiles in a row to set up or stop a trap
int AI_player::findHorizontalPair(const Board &board, char tile)
{
	
	int row = board.getNumberOfRows();
	int col = board.getNumberOfCols();

	for(int i=1;i<=row;i++){
		for(int j=2;j<=(col-3);j++){
			if(board(i,j) == tile && board(i,j+1) == tile && board(i,j-1)==' '&& board(i,j+2)==' '){

				//bottom row
				if(i==1){
					//prioritize the space closer to the middle
					if(abs(4-(j-1))>abs(4-(j+2)))
						return j+2;
					else return j-1;
				}
				else if(i!=1){
					//if both sides are playable same as case on bottom row
					if(board(i-1,j-1)!=' ' && board(i-1,j+2)!=' '){
						if(abs(4-(j-1))>abs(4-(j+2)))
							return j+2;
						else return j-1;
					}
					else if(board(i-1, j-1) != ' ')
						return j-1;
					else
						return j+2;
				}
			}
		}
	}
	return board.getNumberOfCols()+1;
}
//---FIND THREE TILES WHERE FOURTH TILE WOULD WIN---//
//This is just a sea of if-else statements
int AI_player::findWinningMove(const Board &board, char tile)
{
	int nrows = board.getNumberOfRows();
	int ncols = board.getNumberOfCols();
	
	//Horizontal, free on right
	for(int i=1;i<=nrows;++i){
		for(int j=1;j<=(ncols-3);++j){
			if(board(i,j)==tile && board(i,j+1)==tile && board(i,j+2)==tile){
				//bottom row
				if(i==1){
					if(board(i,j+3)==' ')
						return j+3;	
				}
				else{
					if(board(i,j+3)==' ' && board(i-1,j+3)!=' ')
						return j+3;
				}
			}
		}
	}
	//Horizontal, free on left
	for(int i=1;i<=nrows;++i){
		for(int j=2;j<(ncols-1);++j){
			if(board(i,j)==tile && board(i,j+1)==tile && board(i,j+2)==tile){		
				//bottom row
				if(i==1){
					if(board(i,j-1)==' ')
						return j-1;
				}
				else{
					if(board(i,j-1)==' ' && board(i-1,j-1)!=' ')
						return j-1;
				}
			}
		}
	}
	//HORIZONTAL WITH GAP
	for(int i=1;i<=nrows;++i){
		for(int j=1;j<=(ncols-3);++j){
			//if OO_O
			if(board(i,j) == tile && board(i,j+1) == tile && board(i,j+3) == tile){
				//bottom row
				if(i==1){
					if(board(i,j+2) == ' ')
						return j+2;	
				}
				//not on bottom row
				else{	
					if(board(i-1,j+2) != ' ' && board(i,j+2) == ' ')
						return j+2;	
				}
			}
			//if O_OO
			if(board(i,j) == tile && board(i,j+2) == tile && board(i,j+3) == tile){
				//bottom row
				if(i==1){
					if(board(i, j+1) == ' ')
						return j+1;	
				}
				//not on bottom row
				else{	
					if(board(i-1, j+1) != ' ' && board(i, j+1) == ' ')
						return j+1;	
				}
			}
		}
	}

	//VERTICAL
	for(int j=1;j<=ncols;++j){
		for(int i=1;i<=(nrows-3);++i){
			if(board(i,j) == tile && board(i+1,j) == tile && board(i+2,j) == tile){
				if(board(i+3,j) == ' ')
					return j;	
			}			
		}
	}
	
	//DIAGONAL UP TOGETHER
	//diagonal up together - top right
	for(int i=1;i<=(nrows-3);++i){
		for(int j=1;j<=(ncols-3);++j){
			if(board(i,j) == tile && board(i+1,j+1) == tile && board(i+2,j+2) == tile){
				if(board(i+3,j+3)==' ' && board(i+2,j+3)!=' ')
					return j+3;		
			}
		}
	}
	for(int i=2;i<=(nrows-2);++i){
		for(int j=2;j<=(ncols-2);++j){
			if(board(i,j) == tile && board(i+1,j+1) == tile && board(i+2,j+2) == tile){
				//bottom row
				if(i==2){
					if(board(i-1,j-1)==' ')
						return j-1; 
				}
				else{
					if(board(i-1,j-1)==' ' && board(i-2,j-1)!=' ')
						return j-1; 
				}
			}
		}
	}

	//DIAGONAL UP SEPARATE
	for(int i=1;i<=(nrows-3);++i){
		for(int j=1;j<(ncols-2);++j){
			//O_OO
			if(board(i,j) == tile && board(i+2,j+2) == tile && board(i+3,j+3) == tile){
				if(board(i+1, j+1)==' ' && board(i, j+1) !=' ' ){
					return j+1;	
				}
			}
			//OO_O
			if(board(i,j) == tile && board(i+1,j+1) == tile && board(i+3,j+3) == tile){
				if(board(i+2, j+2)==' ' && board(i+1,j+2)!=' ' ){
					return j+2;	
				}
			}
			
		}
	}

	//DIAGONAL DOWN TOGETHER
	//diagonal down together open bottom right
	for(int i=4;i<=nrows;++i){
		for(int j=1;j<=(ncols-3);++j){
			if(board(i,j)==tile && board(i-1,j+1)==tile && board(i-2,j+2)==tile){
				//bottom row
				if(i==4){
					if(board(i-3,j+3)==' ')
						return j+3;
				}			
				//not bottom row
				else{
					if(board(i-3,j+3)==' ' && board(i-4,j+3)!=' ')
						return j+3;
				}
			}	

		}
	}
	
	//diagonal down together, top left
	for(int i=3;i<=(nrows-1);++i){
		for(int j=2;j<(ncols-1);++j){	
			if(board(i,j)==tile && board(i-1,j+1)==tile && board(i-2,j+2)==tile){
				if(board(i+1,j-1)==' ' && board(i,j-1)!=' ')
					return j-1;
			}
		}
	}

	//DIAGONAL DOWN SEPARATE
	for(int i=4;i<=nrows;++i){
		for(int j=1;j<=(ncols-3);++j){
			//O_OO
			if(board(i,j) == tile && board(i-2,j+2) == tile && board(i-3,j+3) == tile){
				if(board(i-1, j+1)==' ' && board(i-2, j+1)!=' '){
					return j+1;	
				}
			}
			//OO_O
			if(board(i,j) == tile && board(i-1,j+1) == tile && board(i-3,j+3) == tile){
				if(board(i-2, j+2)==' ' && board(i-3, j+2)!=' ' ){
					return j+2;	
				}
			}
		}
	}
	return ncols+1;
}	
