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

int Human::makeMove(const Board &board){return 0;} //this does nothing on purpose

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
int AI_player::makeMove(const Board &board)
{	
	//choose column based on strategy
	int selection = chooseColumnUsingStrategy(board);

	//if best column choice is full, select random column
	while(board.columnFull(selection)){
		selection = 1+(rand() %  board.getNumberOfCols());
	}
	return selection;
}

//---DETERMINE STRATEGY---//
int AI_player::chooseColumnUsingStrategy(const Board &board)
{
	int column_choice = 0;
	char enemy_tile = board.getFirstTile();
	if(player_no == 1)
		enemy_tile = board.getSecondTile();

	//TRY TO WIN
	if(column_choice==0)
		column_choice = findWinningMove(board, my_tile);
	//BLOCK AN OPPONENTS WIN
	if(column_choice==0)
		column_choice = findWinningMove(board, enemy_tile);
	//ATTACK - PLAY A POSITION WHICH GUARANTEES A WIN
//	if(column_choice==0)
//		column_choice = findGuaranteedWin(board,my_tile);
	//BLOCK GUARANTEED WINNING POSITION - two win options
	if(column_choice==0)
		column_choice = findGuaranteedWin(board, enemy_tile);
	//Prevent a 3 in a horizontal row
	//has been replaced by findGuaranteedWin()
/*
	if(column_choice==0){
		column_choice = findHorizontalPair(board, enemy_tile);

		if(choiceWouldCauseLoss(board, column_choice+1)){	//avoid giving opponent the win
			column_choice = 0;
		}
	}
*/
	//If the above three fail then picks random	
	if(column_choice == 0){
		column_choice = chooseRandomCol_Weighted(board);
	} 

	return column_choice;
}

int AI_player::findGuaranteedWin(const Board &board, char tile){

	char enemy_tile=(tile=='1') ? '2' : '1';
	//check each column prioritising the middle
	int columns[7] = {4,3,5,2,6,1,7};
	
	for(auto column : columns){

		Board test_board = board;
		test_board.insertTileInColumn(column, tile);
	
		//if playing current tile provides a winning move
		if(findWinningMove(test_board, tile) != 0){
		
			//anticipate opponent blocking the win
			test_board.insertTileInColumn(findWinningMove(test_board,tile),enemy_tile);

			//if after the opponent has blocked a win, there is still a win, then return initial column choice
			if(findWinningMove(test_board,tile)!=0){
				if(!choiceWouldCauseLoss(board, column)){
					return column;
				}
			}

		}
	}

	return 0;
}

int AI_player::findGuaranteedWin_Opponent(const Board &board){

	char enemy_tile=(my_tile=='1') ? '2' : '1';

	for(int column=1;column<=7;++column){

		Board test_board = board;
		test_board.insertTileInColumn(column,my_tile);

		if(findGuaranteedWin(test_board,enemy_tile))
			return column;
	}

	return 0;
}

//---IF NO OTHER OPTION, PRIORITIZES MIDDLE---//
int AI_player::chooseRandomCol_Weighted(const Board &board)
{
	//set up column weights
	//values come from binomial distribution with n=6 and p=0.5
	int ncols = board.getNumberOfCols();
	std::array<int, 7> column_weights = {5,30,75,120,75,30,5};
	
	//if column is full or would result in loss then set weight to zero 
	for(int column=1;column<=ncols;++column){

		if(board.columnFull(column))
			column_weights[column-1] = 0;

		else if(choiceWouldCauseLoss(board,column))
			column_weights[column-1] = 0;	
	}
	//if there are no possible outcomes, it will play a losing tile
	if(std::accumulate(column_weights.begin(),column_weights.end(),0)==0){
	
		for(int column=1;column<=ncols;++column){
			if(choiceWouldCauseLoss(board,column))
				column_weights[column-1] = 100;
		}		
	}

//	std::cout << "{";
//	for(int i=0;i<7;++i)
//		std::cout << column_weights[i] << ", ";
//	std::cout << "}\n";

	std::discrete_distribution<int> column_weights_dist(column_weights.begin(), column_weights.end());
	std::random_device rd;
	std::mt19937 generator(rd());

	int col_choice = column_weights_dist(generator);

	return col_choice+1;		//change column to human readability
}

int AI_player::chooseRandomCol_Gaussian(const Board &board)
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
	while(choiceWouldCauseLoss(board, col+1))
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
int AI_player::choiceWouldCauseLoss(const Board &board, int col)
{
	//get opponents tile
	char enemy_tile;
	if(player_no == 1)
		enemy_tile = board.getSecondTile();
	else if(player_no == 2)
		enemy_tile = board.getFirstTile();

	//enter current selection in test board
	Board test_board = board;
	test_board.insertTileInColumn(col, my_tile);

//	std::cout << "******************\n";
//	test_board.printBoard();
//	std::cout << "******************\n";

	//CHECK FOR WIN
	if(findWinningMove(test_board, enemy_tile) != 0){		//if found 3 and can win, will win
	//	std::cout << "found winning move at " << findWinningMove(test_board,enemy_tile) << std::endl;
		return 1;
	}

	return 0;
}

//Find two tiles in a row to set up or stop a trap
int AI_player::findHorizontalPair(const Board &board, char tile)
{
	int row = board.getNumberOfRows();
	int col = board.getNumberOfCols();

	for(int i=1;i<=row;i++){
		for(int j=2;j<=(col-2);j++){
			//if find a horizontal pair with space on either side
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
	return 0;
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
	return 0;
}	
