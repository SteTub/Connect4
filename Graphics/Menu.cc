#include "Menu.h"

Menu::Menu()
{
	current_selection = 0;
	at_wasd_choice = false;
}

bool Menu::atHighestChoice() const{
	if(current_selection == 0)
		return true;
	
	return false;
}
bool Menu::atLowestChoice() const{
	if(current_selection==1)
		return true;
	
	return false;
}

bool Menu::isMenuTileMoving() const{

	return menu_tile.isMoving();
}

void Menu::initMenu(){

	initFont();
	initTitleScreen();
	initPlayerSelectionScreen();
	initMenuTile(0);
	initControlChoice();
	initWinningScreen();
}
void Menu::initFont(){

	if(!font.loadFromFile("FreeSansBold.ttf")){
		std::cerr << "Font file not loaded\n";
		exit(1);
	}
}

void Menu::initTitleScreen(){

	//MAIN TITLE
	title.setString("  C    NNECT 4");
	title.setFont(font);
	title.setCharacterSize(70);
	title.setPosition(WINDOW_WIDTH/2-title.getGlobalBounds().width/2,100);
	title.setColor(sf::Color::Blue);

	//PRESS KEY TO START
	start.setString("Press any key to start");
	start.setFont(font);
	start.setCharacterSize(30);
	start.setPosition(WINDOW_WIDTH/2-start.getGlobalBounds().width/2,400);
	start.setColor(sf::Color::White);

}
void Menu::initPlayerSelectionScreen(){

	//PLAYER 1 SELECT
	select_p1.setString("Would you like\nplayer 1 to be:");
	select_p1.setFont(font);
	select_p1.setCharacterSize(40);
	select_p1.setPosition(WINDOW_WIDTH/2-select_p1.getGlobalBounds().width/2,100);
	select_p1.setColor(sf::Color::White);
	//PLAYER 2 SELECT 
	select_p2.setString("Would you like\nplayer 2 to be:");
	select_p2.setFont(font);
	select_p2.setCharacterSize(40);
	select_p2.setPosition(WINDOW_WIDTH/2-select_p2.getGlobalBounds().width/2,100);
	select_p2.setColor(sf::Color::White);

	//HUMAN CHOICE - BACKGROUND BOX
	human_box.setSize(sf::Vector2f(3*WINDOW_WIDTH/10.0,1*WINDOW_HEIGHT/10.0));
	human_box.setOrigin(sf::Vector2f(3*WINDOW_WIDTH/20.0,1*WINDOW_HEIGHT/20.0));
	human_box.setPosition(6*WINDOW_WIDTH/10.0,5*WINDOW_HEIGHT/10.0);
	human_box.setFillColor(sf::Color::Yellow);
	human_box.setOutlineThickness(-WINDOW_WIDTH/50.0);
	human_box.setOutlineColor(sf::Color(255,105,0));
	//HUMAN CHOICE - TEXT
	human.setString("Human");
	human.setFont(font);
	human.setCharacterSize(30);
	double human_width = human.getGlobalBounds().width;
	double human_height = human.getGlobalBounds().height;
	human.setOrigin(sf::Vector2f(human_width/2.0,human_height/2.0));
	human.setPosition(6*WINDOW_WIDTH/10.0,5*WINDOW_HEIGHT/10.0-10);
	human.setColor(sf::Color::Black);
	
	//AI CHOICE - BACKGROUND BOX
	AI_box.setSize(sf::Vector2f(3*WINDOW_WIDTH/10.0,1*WINDOW_HEIGHT/10.0));
	AI_box.setOrigin(sf::Vector2f(3*WINDOW_WIDTH/20.0,1*WINDOW_HEIGHT/20.0));
	AI_box.setPosition(6*WINDOW_WIDTH/10.0,7*WINDOW_HEIGHT/10.0);
	AI_box.setFillColor(sf::Color::Yellow);
	AI_box.setOutlineThickness(-WINDOW_WIDTH/50.0);
	AI_box.setOutlineColor(sf::Color::Yellow);
	//AI CHOICE - TEXT
	AI.setString("A.I.");
	AI.setFont(font);
	AI.setCharacterSize(30);
	double AI_width = AI.getGlobalBounds().width;
	double AI_height = AI.getGlobalBounds().height;
	AI.setOrigin(sf::Vector2f(AI_width/2.0,AI_height/2.0));
	AI.setPosition(6*WINDOW_WIDTH/10.0,7*WINDOW_HEIGHT/10.0-10);
	AI.setColor(sf::Color::Black);
}

void Menu::resetForPlayerTwoChoice(){

	initMenuTile(1);
	human_box.setOutlineColor(sf::Color(255,105,0));
	AI_box.setOutlineColor(sf::Color::Yellow);
	current_selection = 0;
}
void Menu::moveChoiceDown(){

	menu_tile.move(0,+2*WINDOW_HEIGHT/10.0);
	AI_box.setOutlineColor(sf::Color(255,105,0));			
	human_box.setOutlineColor(sf::Color::Yellow);
	++current_selection;
}
void Menu::moveChoiceUp(){
	menu_tile.move(0,-2*WINDOW_HEIGHT/10.0);
	human_box.setOutlineColor(sf::Color(255,105,0));
	AI_box.setOutlineColor(sf::Color::Yellow);
	--current_selection;
}
	
void Menu::drawTitleScreen(sf::RenderWindow& window) const{

	window.draw(start);
	window.draw(title);
	window.draw(menu_tile);

}
void Menu::drawPlayerSelectionScreen(sf::RenderWindow& window, int player) const{
	switch(player){
	case 1:	
		window.draw(select_p1);
		break;
	case 2:
		window.draw(select_p2);
		break;
	}
	window.draw(human_box);
	window.draw(human);
	window.draw(AI_box);
	window.draw(AI);
	window.draw(menu_tile);
}

void Menu::initWinningScreen(){

	//PLAYER 1 SELECT
	winner.setString("Player 1 wins!");
	winner.setFont(font);
	winner.setCharacterSize(40);
	//winner.setPosition(WINDOW_WIDTH/2-winner.getGlobalBounds().width/2,WINDOW_HEIGHT/2-winner.getGlobalBounds().height/2);
	winner.setPosition(WINDOW_WIDTH/2-winner.getGlobalBounds().width/2,WINDOW_HEIGHT*0.875);
	winner.setColor(sf::Color::Red);
	//PLAYER 2 SELECT 
	winner2.setString("Player 2 wins!");
	winner2.setFont(font);
	winner2.setCharacterSize(40);
	//winner2.setPosition(WINDOW_WIDTH/2-winner2.getGlobalBounds().width/2,WINDOW_HEIGHT/2-winner2.getGlobalBounds().height/2);
	winner2.setPosition(WINDOW_WIDTH/2-winner2.getGlobalBounds().width/2,WINDOW_HEIGHT*0.875);
	winner2.setColor(sf::Color::Yellow);

	//PLAYER 2 SELECT 
	stale.setString("STALEMATE");
	stale.setFont(font);
	stale.setCharacterSize(40);
	stale.setPosition(WINDOW_WIDTH/2-winner2.getGlobalBounds().width/2,WINDOW_HEIGHT*0.875);
	//stale.setPosition(WINDOW_WIDTH/2-winner2.getGlobalBounds().width/2,WINDOW_HEIGHT/2-winner2.getGlobalBounds().height/2);
	stale.setColor(sf::Color::White);	
}

void Menu::drawEndingScreen(sf::RenderWindow& window, int winning_player) const{

	switch(winning_player){
	case 0: 
		window.draw(stale);
		break;
	case 1: 
		window.draw(winner);
		break;
	case 2: 
		window.draw(winner2);
		break;
	}
}

void Menu::initMenuTile(int which_player){

	menu_tile.setRadius(WINDOW_WIDTH/20.0);
	menu_tile.setOrigin(menu_tile.getRadius()/2.0,menu_tile.getRadius()/2.0);
	menu_tile.setOutlineThickness(-WINDOW_WIDTH/60.0);

	int player = which_player%2;	

	switch(player){
	
	case 0:
		menu_tile.setFillColor(sf::Color::Red);	
		menu_tile.setOutlineColor(sf::Color(220,0,0));
		menu_tile.setPosition(3*WINDOW_WIDTH/10.0-1,128);
		menu_tile.startedMoving();
		break;
	case 1:
		menu_tile.setFillColor(sf::Color::Yellow);	
		menu_tile.setOutlineColor(sf::Color(255,180,0));
		menu_tile.setPosition(3*WINDOW_WIDTH/10.0,5*WINDOW_HEIGHT/10.0-15);
		break;
	}
}
void Menu::animateMenuTileDropping(){

	//CREATE A BOX FOR COLLISION CHECKS
	sf::RectangleShape stopper;
	stopper.setSize(sf::Vector2f(WINDOW_WIDTH,WINDOW_HEIGHT/10.0));
	stopper.setOrigin(WINDOW_WIDTH/2.0,WINDOW_HEIGHT/20.0);
	stopper.setPosition(0,(5*WINDOW_HEIGHT/10.0)+WINDOW_HEIGHT/10.0);

	//update tile speed and position
	menu_tile.setMySpeed(menu_tile.getMySpeed()+9.8*240.0*(1000.0/60.0)/MILLION);
	menu_tile.move(0,+menu_tile.getMySpeed()*(1000.0/60.0));
				
	//stop at menu button
	if(menu_tile.checkCollisionWithBase(stopper)){ 	
		while(menu_tile.checkCollisionWithBase(stopper)){		
			menu_tile.move(0,-0.1);
		}
		menu_tile.finishedMoving();
	}
}
void Menu::initControlChoice(){

	//CONTROL CHOICE TITLE
	controls.setString("Which controls\nwould you like:");
	controls.setFont(font);
	controls.setCharacterSize(40);
	controls.setPosition(WINDOW_WIDTH/2-controls.getGlobalBounds().width/2,100);
	controls.setColor(sf::Color::White);
	
	//WASD 
	if(!wasd_texture.loadFromFile("wasd2.jpg", sf::IntRect(0,0,220,151))){
		std::cerr << "Error loading from file\n";
		exit(1);
	}
	wasd.setTexture(wasd_texture);
	wasd.setPosition(6*WINDOW_WIDTH/10.0,5*WINDOW_HEIGHT/10.0);

	wasd_box.setSize(sf::Vector2f(210.0,151.0));
	wasd_box.setPosition(6*WINDOW_WIDTH/10.0,5*WINDOW_HEIGHT/10.0);
	wasd_box.setFillColor(sf::Color::Transparent);
	wasd_box.setOutlineThickness(10.0);
	wasd_box.setOutlineColor(sf::Color::Transparent);

	//ARROWS 
	if(!arrows_texture.loadFromFile("arrows.jpg", sf::IntRect(0,0,220,151))){
		std::cerr << "Error loading from file\n";
		exit(1);
	}
	arrows.setTexture(arrows_texture);
	arrows.setPosition(1*WINDOW_WIDTH/10.0,5*WINDOW_HEIGHT/10.0);

	arrows_box.setSize(sf::Vector2f(210.0,151.0));
	arrows_box.setPosition(1*WINDOW_WIDTH/10.0,5*WINDOW_HEIGHT/10.0);
	arrows_box.setFillColor(sf::Color(255,105,0));
	arrows_box.setOutlineThickness(10.0);
	arrows_box.setOutlineColor(sf::Color(255,105,0));

	//Player 1 text
	p1_controls_text.setString("P1");
	p1_controls_text.setFont(font);
	p1_controls_text.setCharacterSize(20);
	p1_controls_text.setPosition((1*WINDOW_WIDTH/10.0)+105,(5*WINDOW_HEIGHT/10.0)+180);
	p1_controls_text.setColor(sf::Color::Red);	
	//Player 2 text
	p2_controls_text.setString("P2");
	p2_controls_text.setFont(font);
	p2_controls_text.setCharacterSize(20);
	p2_controls_text.setPosition((6*WINDOW_WIDTH/10.0)+105,(5*WINDOW_HEIGHT/10.0)+180);
	p2_controls_text.setColor(sf::Color::Yellow);	
}

void Menu::resetTextForControlChoice(bool player_1_human,bool player_2_human){

	if(!player_1_human && player_2_human){
		p1_controls_text.setColor(sf::Color::Yellow);
	}
}

void Menu::drawControlChoice(sf::RenderWindow& window, bool player_1_human,bool player_2_human){
	window.draw(controls);
	window.draw(wasd_box);
	window.draw(wasd);
	window.draw(arrows_box);
	window.draw(arrows);
	window.draw(p1_controls_text);
	if(player_1_human && player_2_human)
		window.draw(p2_controls_text);
}
void Menu::moveControlChoiceLeft(){
	if(at_wasd_choice){
		arrows_box.setFillColor(sf::Color(255,105,0));
		arrows_box.setOutlineColor(sf::Color(255,105,0));
		wasd_box.setFillColor(sf::Color::Transparent);
		wasd_box.setOutlineColor(sf::Color::Transparent);
	
		p1_controls_text.move(-5*WINDOW_WIDTH/10.0,0);
		p2_controls_text.move(5*WINDOW_WIDTH/10.0,0);

		at_wasd_choice = false;
	}
}
void Menu::moveControlChoiceRight(){

	if(!at_wasd_choice){

		wasd_box.setFillColor(sf::Color(255,105,0));
		wasd_box.setOutlineColor(sf::Color(255,105,0));
		arrows_box.setFillColor(sf::Color::Transparent);
		arrows_box.setOutlineColor(sf::Color::Transparent);

		p1_controls_text.move(5*WINDOW_WIDTH/10.0,0);
		p2_controls_text.move(-5*WINDOW_WIDTH/10.0,0);
		at_wasd_choice = true;
	}
}
bool Menu::playerChoseWASD() const{
	return at_wasd_choice;
}
