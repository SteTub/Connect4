#include"Tile.h"

class Menu{

	public:
	Menu();
	//init 
	void initMenu();
	void initFont();
	void initTitleScreen();
	void initPlayerSelectionScreen();
	void initControlChoice();
	void initWinningScreen();
	
	void resetForPlayerTwoChoice();
	//menu tile 
	void initMenuTile(int player);
	void animateMenuTileDropping();
	bool isMenuTileMoving() const;
	//draw
	void drawTitleScreen(sf::RenderWindow& window) const;
	void drawPlayerSelectionScreen(sf::RenderWindow& window, int player) const;
	void drawControlChoice(sf::RenderWindow& window, bool player_1_human, bool player_2_human);
	void drawEndingScreen(sf::RenderWindow& window, int winning_player) const;
	//movement
	void moveChoiceDown();
	void moveChoiceUp();
	bool atHighestChoice() const;
	bool atLowestChoice() const;
	void moveControlChoiceLeft();
	void moveControlChoiceRight();
	//control scheme
	bool playerChoseWASD() const;
	void resetTextForControlChoice(bool player_1_human, bool player_2_human);

	//----WINNING SCREENS----//

	private:
		int current_selection;
		sf::RectangleShape human_box, AI_box;
		Tile menu_tile;
		sf::Text title, start, select_p1, select_p2, human, AI, winner, winner2, stale;
		sf::Font font;
		sf::Text controls;
		sf::Texture wasd_texture, arrows_texture;
		sf::Sprite wasd, arrows;
		sf::RectangleShape wasd_box, arrows_box;
		bool at_wasd_choice;
		sf::Text p1_controls_text,p2_controls_text;
};
