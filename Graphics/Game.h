#include "Player.h"
#include "Menu.h"

#define MAX_FRAMESKIP 10
#define FRAMES_PER_SECOND 60

class Game{

	public:
		Game();
		void init();
		void run();
		void display();
		void processEvents();
		void update();
		void makeAISelection(int selection);
		void makeHumanSelection();
		void updateDroppingTile();
		bool isPlayerOneHuman() const;
		bool isPlayerTwoHuman() const;
		void setPlayerOneHuman(); 
		void setPlayerTwoHuman(); 
		void setPlayerAsHuman();	
		void switchPlayerControls();

	private:
		int game_state;
		bool first_time;
		bool set_controls;
		bool reset_control_text;
		int tiles;
		bool reset_clock;
		int ai_selection;
		sf::RenderWindow window;
		
		Board board;
		//----------//
		Player* one;
		Player* two;
		char p1_choice, p2_choice;
		char tile_1, tile_2;
		//----------//
		sf::Clock ai_clock,update_clock;
		sf::Int32 update_time;
		Menu menu;
		sf::Event event;
		sf::Texture background_texture;
		sf::Sprite background;
		bool p1_human, p2_human;
		bool p1_winner, p2_winner, stalemate;		
		int result;
		
		enum state{INTRO,P1_SELECT,P2_SELECT,CONTROLS,PLAYING,ENDING};

		struct controls{
			int left;
			int right;
			int down;
		};
		controls p1_controls,p2_controls;
};
