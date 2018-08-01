#include "Player.h"
#include "Menu.h"

#define MAX_FRAMESKIP 10
#define FRAMES_PER_SECOND 60

class Game{

	public:
		Game();
		//---MAIN FUNCTIONS---//
		void init();
		void run();
		//---DISPLAY---//
		void display();

		//---PROCESS EVENTS---//
		void processEvents();
		
		void processKeyPress_PlayerChoice(int key_press);
		void processKeyPress_ControlsChoice(int key_press);
		void processKeyPress_PlayerTiles(int key_press, int order);

		//---MAKE UPDATES---//
		void update();
		
		void updateDroppingTile();
		void makeAISelection();
		void checkForGameOver();
		void moveToNextGameState();

		//---GET PLAYER TYPES---//
		bool isPlayerOneHuman() const;
		bool isPlayerTwoHuman() const;

		//---SET PLAYER TYPES AND CONTROLS---//			
		void setPlayersAndControls();
		void setPlayerAsHuman();	
		void setPlayerOneHuman(); 
		void setPlayerTwoHuman(); 
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
		Player* one;
		Player* two;
		char p1_choice, p2_choice;
		char tile_1, tile_2;
		sf::Clock ai_clock,update_clock;
		sf::Int32 update_time;
		Menu menu;
		sf::Event event;
		sf::Texture background_texture;
		sf::Sprite background;
		bool p1_human, p2_human;
		int result;
		
		enum state{INTRO,P1_SELECT,P2_SELECT,CONTROLS,PLAYING,ENDING};

		struct controls{
			int left;
			int right;
			int down;
		};
		controls p1_controls,p2_controls;
};
