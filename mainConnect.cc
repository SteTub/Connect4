#include "Board.h"
#include "Player.h"

int main()
{
	int rows = 6, cols = 7;
	char player_1,player_2,win_tile = 'Z';

	//RNG seed
	struct timeval ts;	
	gettimeofday(&ts,NULL);
	int seed  = (int)(ts.tv_usec);
	srand((unsigned) seed);

	//print game choice
	printTitle();
	selectPlayers(player_1, player_2);
	//exit if X chosen
	if(player_1 == 'X' || player_2 == 'X'){
		std::cout << "Goodbye!\n";
		return 0;
	}
	
	//----INITIALIZATION---//
	//initialize board and players
	Board board(rows, cols);
	Player* one = Player::createPlayer(player_1,'0',1);
	Player* two = Player::createPlayer(player_2,'O',2);

	//test board
/*
	board(6,1)=' ';board(6,2)=' ';board(6,3)=' ';board(6,4)=' ';board(6,5)=' ';board(6,6)=' ';board(6,7)=' ';
	board(5,1)=' ';board(5,2)=' ';board(5,3)=' ';board(5,4)=' ';board(5,5)=' ';board(5,6)=' ';board(5,7)=' ';
	board(4,1)=' ';board(4,2)=' ';board(4,3)=' ';board(4,4)=' ';board(4,5)=' ';board(4,6)=' ';board(4,7)=' ';
	board(3,1)=' ';board(3,2)='0';board(3,3)='0';board(3,4)='0';board(3,5)=' ';board(3,6)=' ';board(3,7)=' ';
	board(2,1)=' ';board(2,2)='O';board(2,3)='0';board(2,4)='O';board(2,5)=' ';board(2,6)=' ';board(2,7)=' ';
	board(1,1)='0';board(1,2)='O';board(1,3)='O';board(1,4)='O';board(1,5)='0';board(1,6)=' ';board(1,7)=' ';
*/

	//---BEGIN GAME---//
	printTitle();
	board.printBoard();
				
	for(int ply=0;ply<42;++ply){	
		
		if(ply%2==0){
			one->makeMove(board);
	//		board.makeMoveForPlayer(selection,player_1,'0');
			if(board.detectWin(1)){		
				win_tile='0';
				break;
			}
		}
		else{
		 	two->makeMove(board);
	//		board.makeMoveForPlayer(selection,player_2,'O');
			if(board.detectWin(2)){
				win_tile='O';
				break;
			}
		}
	}

	if(win_tile == '0'){
		board.printWinFlash(1);
	}
	else if(win_tile == 'O'){ 
		board.printWinFlash(2);
	}
	else{
		std::cout << "Stalemate, Better Luck Next Time\n";
		std::cout << "\n---------------------------------------------\n";
	}

	delete one;
	delete two;

	return 0;  
}
