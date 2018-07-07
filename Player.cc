#include "Connect.h"
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

Human::Human(char tile, int number)
{
	 my_tile=tile;
	 player_no=number;
}

void Human::makeMove(const Board &Game)
{
	int selection;
	std::cout << "Player " << player_no << "\nChoose a column: ";
	std::cin>>selection;
 	
	//error check selection
	while(selection < 1 || selection > Game.col_size() || Game.columnFull(selection)==1){
		std::cout << "Error: Please select a valid column\n";
		std::cin>>selection;
	}
	
	Game.insertInColumn(selection,my_tile);
	Game.fancyPrint(selection, my_tile);
}

//=======AI PLAYER=======//

//---CONSTRUCTOR---//
AI_player::AI_player(char tile, int order)
{
	my_tile=tile;
	player_no=order;
}

//---ACCESS PRIVATE MEMBERS---//
char AI_player::WhatTile() const{
	return my_tile;
}

//---MAKES THE MOVE USING STRATEGY---//
void AI_player::makeMove(const Board &Game)
{	
	//choose column based on strategy
	int selection = Strategy(Game);

	//if best column choice is full, select random column
	while(Game.columnFull(selection)){
		selection = 1+(rand() %  Game.col_size());
	}
	
	//insert into board
	usleep(400000);
	Game.insertInColumn(selection,my_tile);
	Game.fancyPrint(selection, my_tile);
}

//---DETERMINE STRATEGY---//
int AI_player::Strategy(const Board &Game)
{
	int ncols = Game.col_size();
	int col = 0;
	char enemy_tile = Game.firstTile();
	if(player_no == 1)
		enemy_tile = Game.secondTile();

	//TRY TO WIN
	if(FindThree(Game, my_tile) < ncols+1){		//if found 3 and can win, will win
		col = FindThree(Game, my_tile);
	}	

	//BLOCK AN OPPONENTS WIN
	else if(FindThree(Game, enemy_tile) < ncols+1){	//if opponent has 3 in row, block opponents win
		col = FindThree(Game, enemy_tile);
	}	
	//Prevent a 3 in a horizontal row
	else if(FindHorizontalPair(Game, enemy_tile) < ncols+1){	//cut off opponent trap, if opponent has 2 in a row, will play next to one
		col = FindHorizontalPair(Game, enemy_tile);
	
		if(choiceWouldCauseLoss(Game, col+1, my_tile, enemy_tile)){	//avoid giving opponent the win
			col = 0;
		}
	}
	//If the above three fail then picks random	
	if(col == 0){
		col = WeightedChoice(Game, my_tile, enemy_tile);
	} 

	return col;
}

//---IF NO OTHER OPTION, PRIORITIZES MIDDLE---//
int AI_player::WeightedChoice(const Board &Game, char tile, char enemy_tile)
{
	int ncols = Game.col_size();
	int mid = ncols/2;
	int col = ncols;

	std::random_device rd;
	std::mt19937 generator(rd());
	std::normal_distribution<> distrib(mid,0.7);	//for now only assume 7 columns

	if(ncols % 2 == 0)	//if even number of cols, ignoring for now
		 return rand() % ncols;
		
	else {			//odd number of cols
		//std::normal_distribution<> distrib(mid,1.5);
		col = std::round(distrib(generator));	
		
		while(Game.columnFull(col) && !(col >= 0) && !(col <= ncols-1))
			col = std::round(distrib(generator));	
	}

	//check if chosen column would create winning scenario for opponent
	while(choiceWouldCauseLoss(Game, col+1, tile, enemy_tile))
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
int AI_player::choiceWouldCauseLoss(const Board &Game, int col, char tile, char enemy_tile)
{
	Board test_game = Game;
	test_game.insertInColumn(col, tile);

	//CHECK FOR WIN
	if(FindThree(test_game, enemy_tile) < test_game.col_size())		//if found 3 and can win, will win
		return 1;
	
	return 0;
}

//Find two tiles in a row to set up or stop a trap
int AI_player::FindHorizontalPair(const Board &Game, char tile)
{
	int row = Game.row_size();
	int col = Game.col_size();

	for(int i=1;i<=row;i++){
		for(int j=2;j<(col-3);j++){
			//if find a horizontal pair with space on either side
			if(Game(i,j) == tile && Game(i,j+1) == tile && Game(i,j-1)==' '&& Game(i,j+2)==' '){
				//bottom row
				if(i==1){
					//prioritize the space closer to the middle
					if(abs(4-(j-1))>abs(4-(j+2)))
						return j+2;
					else return j-1;
				}
				else if(i!=1){
					//if both sides are playable same as case on bottom row
					if(Game(i-1,j-1)!=' ' && Game(i-1,j+2)!=' '){
						if(abs(4-(j-1))>abs(4-(j+2)))
							return j+2;
						else return j-1;
					}
					else if(Game(i-1, j-1) != ' ')
						return j-1;
					else
						return j+2;
				}
			}
		}
	}
	return Game.col_size()+1;
}
//---FIND THREE TILES WHERE FOURTH TILE WOULD WIN---//
//This is just a sea of if-else statements
int AI_player::FindThree(const Board &Game, char tile)
{
	int nrows = Game.row_size();
	int ncols = Game.col_size();
	
	//Horizontal, free on right
	for(int i=1;i<=nrows;++i){
		for(int j=1;j<=(ncols-3);++j){
			if(Game(i,j)==tile && Game(i,j+1)==tile && Game(i,j+2)==tile){
				//bottom row
				if(i==1){
					if(Game(i,j+3)==' ')
						return j+3;	
				}
				else{
					if(Game(i,j+3)==' ' && Game(i-1,j+3)!=' ')
						return j+3;
				}
			}
		}
	}
	//Horizontal, free on left
	for(int i=1;i<=nrows;++i){
		for(int j=2;j<(ncols-1);++j){
			if(Game(i,j)==tile && Game(i,j+1)==tile && Game(i,j+2)==tile){		
				//bottom row
				if(i==1){
					if(Game(i,j-1)==' ')
						return j-1;
				}
				else{
					if(Game(i,j-1)==' ' && Game(i-1,j-1)!=' ')
						return j-1;
				}
			}
		}
	}
	//HORIZONTAL WITH GAP
	for(int i=1;i<=nrows;++i){
		for(int j=1;j<=(ncols-3);++j){
			//if OO_O
			if(Game(i,j) == tile && Game(i,j+1) == tile && Game(i,j+3) == tile){
				//bottom row
				if(i==1){
					if(Game(i,j+2) == ' ')
						return j+2;	
				}
				//not on bottom row
				else{	
					if(Game(i-1,j+2) != ' ' && Game(i,j+2) == ' ')
						return j+2;	
				}
			}
			//if O_OO
			if(Game(i,j) == tile && Game(i,j+2) == tile && Game(i,j+3) == tile){
				//bottom row
				if(i==1){
					if(Game(i, j+1) == ' ')
						return j+1;	
				}
				//not on bottom row
				else{	
					if(Game(i-1, j+1) != ' ' && Game(i, j+1) == ' ')
						return j+1;	
				}
			}
		}
	}

	//VERTICAL
	for(int j=1;j<=ncols;++j){
		for(int i=1;i<=(nrows-3);++i){
			if(Game(i,j) == tile && Game(i+1,j) == tile && Game(i+2,j) == tile){
				if(Game(i+3,j) == ' ')
					return j;	
			}			
		}
	}
	
	//DIAGONAL UP TOGETHER
	//diagonal up together - top right
	for(int i=1;i<=(nrows-3);++i){
		for(int j=1;j<=(ncols-3);++j){
			if(Game(i,j) == tile && Game(i+1,j+1) == tile && Game(i+2,j+2) == tile){
				if(Game(i+3,j+3)==' ' && Game(i+2,j+3)!=' ')
					return j+3;		
			}
		}
	}
	for(int i=2;i<=(nrows-2);++i){
		for(int j=2;j<=(ncols-2);++j){
			if(Game(i,j) == tile && Game(i+1,j+1) == tile && Game(i+2,j+2) == tile){
				//bottom row
				if(i==2){
					if(Game(i-1,j-1)==' ')
						return j-1; 
				}
				else{
					if(Game(i-1,j-1)==' ' && Game(i-2,j-1)!=' ')
						return j-1; 
				}
			}
		}
	}

	//DIAGONAL UP SEPARATE
	for(int i=1;i<=(nrows-3);++i){
		for(int j=1;j<(ncols-2);++j){
			//O_OO
			if(Game(i,j) == tile && Game(i+2,j+2) == tile && Game(i+3,j+3) == tile){
				if(Game(i+1, j+1)==' ' && Game(i, j+1) !=' ' ){
					return j+1;	
				}
			}
			//OO_O
			if(Game(i,j) == tile && Game(i+1,j+1) == tile && Game(i+3,j+3) == tile){
				if(Game(i+2, j+2)==' ' && Game(i+1,j+2)!=' ' ){
					return j+2;	
				}
			}
			
		}
	}

	//DIAGONAL DOWN TOGETHER
	//diagonal down together open bottom right
	for(int i=4;i<=nrows;++i){
		for(int j=1;j<=(ncols-3);++j){
			if(Game(i,j)==tile && Game(i-1,j+1)==tile && Game(i-2,j+2)==tile){
				//bottom row
				if(i==4){
					if(Game(i-3,j+3)==' ')
						return j+3;
				}			
				//not bottom row
				else{
					if(Game(i-3,j+3)==' ' && Game(i-4,j+3)!=' ')
						return j+3;
				}
			}	

		}
	}
	
	//diagonal down together, top left
	for(int i=3;i<=(nrows-1);++i){
		for(int j=2;j<(ncols-1);++j){	
			if(Game(i,j)==tile && Game(i-1,j+1)==tile && Game(i-2,j+2)==tile){
				if(Game(i+1,j-1)==' ' && Game(i,j-1)!=' ')
					return j-1;
			}
		}
	}

	//DIAGONAL DOWN SEPARATE
	for(int i=4;i<=nrows;++i){
		for(int j=1;j<=(ncols-3);++j){
			//O_OO
			if(Game(i,j) == tile && Game(i-2,j+2) == tile && Game(i-3,j+3) == tile){
				if(Game(i-1, j+1)==' ' && Game(i-2, j+1)!=' '){
					return j+1;	
				}
			}
			//OO_O
			if(Game(i,j) == tile && Game(i-1,j+1) == tile && Game(i-3,j+3) == tile){
				if(Game(i-2, j+2)==' ' && Game(i-3, j+2)!=' ' ){
					return j+2;	
				}
			}
		}
	}
	return ncols+1;
}	
