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
		
		int WeightedChoice(const Board &Game, char tile, char enemy_tile);
		int choiceWouldCauseLoss(const Board &Game, int col, char tile, char enemy_tile);
		int FindHorizontalPair(const Board &Game, char tile);	//finds pair on same row to prevent a 3 in a row win on either side
		int FindThree(const Board &Game, char tile);		//finds three in a row to block opponent or win
		int Strategy(const Board &Game);			//given the board as it is, what is optimum move
		void makeMove(const Board &Game);			//what move to make using Strategy
		char WhatTile() const;					//determines own tile
		char opponentTile() const;				//determines opponents tile

	private:
		char my_tile;
		int player_no;
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
