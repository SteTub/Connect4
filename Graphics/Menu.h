#include"Tile.h"

class Menu{

	public:
	Menu();
	
	//---INITIALIZE---//
	void initMenu();
	void initFont();

	//---TITLE SCREEN---//
	void initTitleScreen();
	void drawTitleScreen(sf::RenderWindow& window) const;

	//---PLAYER SELECT SCREEN---//
	void initPlayerSelectionScreen();
	void drawPlayerSelectionScreen(sf::RenderWindow& window, int player) const;
	void movePlayerChoiceDown();
	void movePlayerChoiceUp();
	bool atHumanChoice() const;
	bool atAIChoice() const;
	bool atHighestChoice() const;
	bool atLowestChoice() const;
	void resetForPlayerTwoChoice();

	//menu tile 
	void initMenuTile(int player);
	void animateMenuTileDropping();
	bool isMenuTileMoving() const;

	//---CONTROLS CHOICE SCREEN---//
	void initControlChoice();
	void drawControlChoice(sf::RenderWindow& window, bool player_1_human, bool player_2_human);
	void moveControlChoiceLeft();
	void moveControlChoiceRight();
	bool playerChoseWASD() const;
	void resetTextForControlChoice(bool player_1_human, bool player_2_human);

	//---WINNING SCREEN---//
	void initWinningScreen();
	void initRematch();
	void drawEndingScreen(sf::RenderWindow& window, int winning_player) const;
	
	private:
//		int current_selection;

		bool at_human_choice;
		sf::RectangleShape human_box, AI_box;
		Tile menu_tile;
		sf::Text title, start, select_p1, select_p2, human, AI, winner, winner2, stale, rematch;
		sf::Font font;
		sf::Text controls;
		sf::Texture wasd_texture, arrows_texture;
		sf::Sprite wasd, arrows;
		sf::RectangleShape wasd_box, arrows_box;
		bool at_wasd_choice;
		sf::Text p1_controls_text,p2_controls_text;
};
