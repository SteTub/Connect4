#include<iostream>
#include<unistd.h>
#include<cmath>
#include<random>

class Player {
	public:
		virtual ~Player()=0;
		virtual void makeMove(const Board &Game)=0;
		static Player* createPlayer(char choice,char tile,int order);
};

class AI_player : public Player{

	public:
		//Constructor
		AI_player(char tile, int player_no);
		//virtual destructor
		~AI_player() {};
		
		int chooseRandomColumnGaussian(const Board &Game, char tile, char enemy_tile);
		int choiceWouldCauseLoss(const Board &Game, int col, char tile, char enemy_tile);
		int findHorizontalPair(const Board &Game, char tile);	//finds pair on same row to prevent a 3 in a row win on either side
		int findWinningMove(const Board &Game, char tile);
		int chooseColumnUsingStrategy(const Board &Game);	
		void makeMove(const Board &Game);
		char returnMyTile() const;					//determines own tile

	private:
		char my_tile;		//stores own tile
		int player_no;		//player 1 or player 2
};

class Human : public Player {

	public:
		Human(char tile, int number);
		~Human() {};
		void makeMove(const Board &Game);	

	private:
		char my_tile;	
		int player_no;
};
