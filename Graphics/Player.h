#include"Board.h"

class Player {
	public:
		virtual ~Player()=0;
		virtual int makeMove(const Board &board)=0;
		static Player* createPlayer(char choice,char tile,int order);
};

class AI_player : public Player{

	public:
		//Constructor
		AI_player(char tile, int player_num);
		//virtual destructor
		~AI_player() {};
		
		char returnMyTile() const;					//determines own tile
		
		int makeMove(const Board &board);
		int chooseColumnUsingStrategy(const Board &board);	
		int findWinningMove(const Board &board, char tile);
		int findHorizontalPair(const Board &board, char tile);	//finds pair on same row to prevent a 3 in a row win on either side
		int chooseRandomCol_Weighted(const Board &board);
		int chooseRandomCol_Gaussian(const Board &board);
		int choiceWouldCauseLoss(const Board &board, int column);
		int findGuaranteedWin(const Board &board, char tile);
		int findGuaranteedWin_Opponent(const Board &board);
		
	private:
		char my_tile;		//stores own tile
		int player_no;		//player 1 or player 2
};

class Human : public Player {

	public:
		Human(char tile, int player_num);
		~Human() {};
		int makeMove(const Board &board);

	private:
		char my_tile;	
		int player_no;
};
