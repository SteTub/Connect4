#include<iostream>
#include<unistd.h>
#include<cmath>
#include<random>
#include<array>

class Player {
	public:
		virtual ~Player()=0;
		virtual void makeMove(const Board &board)=0;
		static Player* createPlayer(char choice,char tile,int order);
};

class AI_player : public Player{

	public:
		//Constructor
		AI_player(char tile, int player_num);
		//virtual destructor
		~AI_player() {};
		
		int chooseRandomCol_Weighted(const Board &board);
		int chooseRandomCol_Gaussian(const Board &board);
		int choiceWouldCauseLoss(const Board &board, int col);
		int findHorizontalPair(const Board &board, char tile);	//finds pair on same row to prevent a 3 in a row win on either side
		int findWinningMove(const Board &board, char tile);
		int chooseColumnUsingStrategy(const Board &board);	
		void makeMove(const Board &board);
		char returnMyTile() const;					//determines own tile

	private:
		char my_tile;		//stores own tile
		int player_no;		//player 1 or player 2
};

class Human : public Player {

	public:
		Human(char tile, int player_num);
		~Human() {};
		void makeMove(const Board &board);	

	private:
		char my_tile;	
		int player_no;
};
