#include"Board.h"

class Player {
	public:
		virtual ~Player()=0;
		virtual int makeMove(Board &board)=0;
		static Player* createPlayer(char choice,char tile,int order);
};

class AI_player : public Player{

	public:
		//Constructor
		AI_player(char tile, int player_no);
		//virtual destructor
		~AI_player() {};
		
		int chooseRandomColumnGaussian(const Board &board, char tile, char enemy_tile);
		int choiceWouldCauseLoss(const Board &board, int col, char tile, char enemy_tile);
		int findHorizontalPair(const Board &board, char tile);	//finds pair on same row to prevent a 3 in a row win on either side
		int findWinningMove(const Board &board, char tile);
		int chooseColumnUsingStrategy(const Board &board);	
		int makeMove(Board &board);
		char returnMyTile() const;					//determines own tile

	private:
		char my_tile;		//stores own tile
		int player_no;		//player 1 or player 2
};

class Human : public Player {

	public:
		Human(char tile, int player_no);
		~Human() {};
		int makeMove(Board &board);

	private:
		char my_tile;	
		int player_no;
};
