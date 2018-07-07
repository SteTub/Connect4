#include "Connect.h"
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
	Board Game(rows, cols);
	Player* one = Player::createPlayer(player_1,'0',1);
	Player* two = Player::createPlayer(player_2,'O',2);

	//test board
/*
	Game(6,1)=' ';Game(6,2)=' ';Game(6,3)=' ';Game(6,4)=' ';Game(6,5)=' ';Game(6,6)='O';Game(6,7)=' ';
	Game(5,1)=' ';Game(5,2)=' ';Game(5,3)=' ';Game(5,4)='O';Game(5,5)=' ';Game(5,6)='O';Game(5,7)=' ';
	Game(4,1)=' ';Game(4,2)=' ';Game(4,3)=' ';Game(4,4)='O';Game(4,5)=' ';Game(4,6)='O';Game(4,7)=' ';
	Game(3,1)=' ';Game(3,2)=' ';Game(3,3)=' ';Game(3,4)='O';Game(3,5)=' ';Game(3,6)='O';Game(3,7)=' ';
	Game(2,1)=' ';Game(2,2)=' ';Game(2,3)=' ';Game(2,4)='O';Game(2,5)=' ';Game(2,6)='O';Game(2,7)=' ';
	Game(1,1)=' ';Game(1,2)='O';Game(1,3)=' ';Game(1,4)='O';Game(1,5)=' ';Game(1,6)='O';Game(1,7)=' ';
*/

	//---BEGIN GAME---//
	printTitle();
	Game.printBoard();
				
	for(int ply=0;ply<42;++ply){	
		
		if(ply%2==0){
			one->makeMove(Game);
			if(Game.detectWin(1)){		
				win_tile='0';
				break;
			}
		}
		else{
		 	two->makeMove(Game);
			if(Game.detectWin(2)){
				win_tile='O';
				break;
			}
		}
	}

	if(win_tile == '0'){
		Game.printWinFlash(1);
	}
	else if(win_tile == 'O'){ 
		Game.printWinFlash(2);
	}
	else{
		std::cout << "Stalemate, Better Luck Next Time\n";
		std::cout << "\n---------------------------------------------\n";
	}

	delete one;
	delete two;

	return 0;  
}
