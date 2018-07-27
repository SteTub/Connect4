#include"Game.h"

Game::Game(){}

void Game::init(){

	//RNG seed
	struct timeval ts;	
	gettimeofday(&ts,NULL);
	int seed  = (int)(ts.tv_usec);
	srand((unsigned) seed);
	
	//WINDOW	
	sf::VideoMode video_mode(WINDOW_WIDTH,WINDOW_HEIGHT);
	window.create(video_mode, "Connect 4");
	window.setFramerateLimit(60);
	window.setVerticalSyncEnabled(true);		
	//BACKGROUND
	if(!background_texture.loadFromFile("background_1.jpg", sf::IntRect(0,0,1000,1000))){
		std::cerr << "Error loading from file\n";
		exit(1);
	}
	background.setTexture(background_texture);

	background.scale(sf::Vector2f(1.0f,1.5f)); //background 4
	//MENU
	menu.initMenu();
	//INIT VALUES
	game_state = INTRO;
	first_time=true;
	set_controls=true;
	reset_control_text=true;
	tiles=-1;	//for now tiles are labelled in C standard, ie 0 is first 
	p1_human = false;
	p2_human = false;
	p1_choice = 'A';
	p2_choice = 'A';
	tile_1 = '1';
	tile_2 = '2';

	reset_clock = true;

	result=0;

	//set control scheme
	p1_controls.left = sf::Keyboard::Left;
	p1_controls.right = sf::Keyboard::Right;
	p1_controls.down = sf::Keyboard::Down;
	p2_controls.left = sf::Keyboard::A;
	p2_controls.right = sf::Keyboard::D;
	p2_controls.down = sf::Keyboard::S;
}

void Game::setPlayerAsHuman(){
	if(game_state==P1_SELECT)
		setPlayerOneHuman();
	if(game_state==P2_SELECT)
		setPlayerTwoHuman();
}
void Game::setPlayerOneHuman(){
	p1_human = true;
	p1_choice = 'H';
}
void Game::setPlayerTwoHuman(){
	p2_human = true;
	p2_choice = 'H';
}
bool Game::isPlayerOneHuman() const{
	return p1_human;
}
bool Game::isPlayerTwoHuman() const{
	return p2_human;
}

void Game::processEvents(){
		
	while(window.pollEvent(event))
	{
		if(event.type == sf::Event::Closed)
			window.close();
		else if(event.type == sf::Event::KeyPressed && game_state==INTRO){
			++game_state;
		}
		else if(event.type == sf::Event::KeyPressed && game_state <= P2_SELECT && !menu.isMenuTileMoving()){
			if(event.key.code==sf::Keyboard::Down){
				if(!menu.atLowestChoice()){
					menu.moveChoiceDown();
				}
			}
			if(event.key.code==sf::Keyboard::Up){
				if(!menu.atHighestChoice()){
					menu.moveChoiceUp();
				}
			}
			if(event.key.code==sf::Keyboard::Return || event.key.code==sf::Keyboard::Space){
				if(menu.atHighestChoice()){	//if at highest option then choice is HUMAN
					setPlayerAsHuman();
				}
				++game_state;
			}
		}
		else if(event.type == sf::Event::KeyPressed && game_state==CONTROLS){
			if(event.key.code==sf::Keyboard::Left){
				menu.moveControlChoiceLeft();
			}
			else if(event.key.code==sf::Keyboard::Right){
				menu.moveControlChoiceRight();
			}
			else if(event.key.code==sf::Keyboard::Return || event.key.code==sf::Keyboard::Space){
				++game_state;
			}
		}
		
		else if(event.type == sf::Event::KeyPressed && game_state==PLAYING){
			if(isPlayerOneHuman() && tiles%2==0){
				if(event.key.code==p1_controls.left){
					board.moveTileLeft();
				}
				else if(event.key.code==p1_controls.right){
					board.moveTileRight();
				}
				else if(event.key.code==p1_controls.down && !board.columnFull(board.getTilePosition_Column())){
					board.insertInColumn(board.getTilePosition_Column(),1);
				//	board.startDroppingTile();
					reset_clock=true;	
				}
			}
			if(isPlayerTwoHuman() && tiles%2==1){	
				if(event.key.code==p2_controls.left){
					board.moveTileLeft();
				}
				else if(event.key.code==p2_controls.right){
					board.moveTileRight();
				}
				else if(event.key.code==p2_controls.down && !board.columnFull(board.getTilePosition_Column())){ 
					board.insertInColumn(board.getTilePosition_Column(),2);
					//board.startDroppingTile();
					reset_clock=true;	
				}
			}
		}
	}
}

void Game::update(){

	//animation for menu tile	
	if(game_state==P1_SELECT && menu.isMenuTileMoving()){
		menu.animateMenuTileDropping();	
	}
	if(game_state == CONTROLS && reset_control_text){
		reset_control_text=false;
		menu.resetTextForControlChoice(p1_human,p2_human);
	}
	//swap control scheme
	if(game_state==PLAYING && set_controls == true){
		set_controls=false;
		if(!p1_human && p2_human)
			switchPlayerControls();
		if(menu.playerChoseWASD()){
			switchPlayerControls();
		}
		one = Player::createPlayer(p1_choice,'1',1);		
		two = Player::createPlayer(p2_choice,'2',2);		
	}		
	if(game_state == PLAYING && !board.isTileMoving()){
	
		//check for win or stalemate
		if(tiles == 42){
			stalemate = true;
			++game_state;
			result = 0;
		}
		//check for win or stalemate
		else if(board.detectWin(tiles%2+1)){
			if(tiles%2==0)
				result = 1;
			else result = 2;
			++game_state;
		}
	}
	//create new tile	
	if(board.needNewTile()){
		board.createTile();
		++tiles;
	}
	if(game_state==PLAYING && !board.isTileMoving()){

		//make AI move
		//if(!isPlayerOneHuman())
	//	if(tiles%2 == 0)
			makeAISelection(0);
		//if(!isPlayerTwoHuman())
//		else makeAISelection(1);
	}
	updateDroppingTile();
}

void Game::makeAISelection(int player){

	if(reset_clock){
		if(tiles%2 == 0)
			ai_selection = one->makeMove(board);
		else if(tiles%2==1) 
			ai_selection = two->makeMove(board);
	
		ai_clock.restart();
		reset_clock = false;
	}

	sf::Time ai_time=ai_clock.getElapsedTime();
	if(ai_time > sf::milliseconds(300)){
		if(board.getTilePosition_Column() == ai_selection && ai_selection > 0){
			board.insertInColumn(board.getTilePosition_Column(),tiles%2+1);
			ai_clock.restart();
			ai_selection = 0;
		}
		else if(board.getTilePosition_Column() > ai_selection && ai_selection > 0){
			board.moveTileLeft();
			ai_clock.restart();
		} 
		else if(board.getTilePosition_Column() < ai_selection && ai_selection > 0){
			ai_clock.restart();
			if(!p1_human && !p2_human)
				reset_clock = true;
			board.moveTileRight();
		}
	}  
}

void Game::updateDroppingTile(){

	const sf::Int64 frame_time_secs = MILLION/FRAMES_PER_SECOND;
	sf::Clock c;
	sf::Time time=c.getElapsedTime();
	sf::Int64 next_frame_time=time.asMicroseconds()+frame_time_secs;
	int loops=0;
	
	while(time.asMicroseconds() < next_frame_time && loops < MAX_FRAMESKIP){
		update_time=update_clock.restart().asMilliseconds();	
		if(board.isTileMoving()){
			board.tileDropping(update_time);	
		}
		time=c.getElapsedTime();
		++loops;
	}
}

void Game::run(){

	while(window.isOpen())
	{
		if(game_state == P2_SELECT && first_time){
			menu.resetForPlayerTwoChoice();
			first_time = false;
		}

		processEvents();
		update();
		display();
	}
}

void Game::display(){

	//----DISPLAY----//
	window.clear(sf::Color::Black);
	window.draw(background);
	switch(game_state){	
	
	case INTRO:	//intro screen
		menu.drawTitleScreen(window);
		break;
	case P1_SELECT:	//player 1 select screen
		menu.drawPlayerSelectionScreen(window,1);
		break;
	case P2_SELECT:	//player 2 select screen
		menu.drawPlayerSelectionScreen(window,2);
		break;
	case CONTROLS:	//controls choice
		menu.drawControlChoice(window,p1_human,p2_human);
		break;
	case PLAYING:	//main game
		board.draw(window);
		break;
	case ENDING:	//winning screen
		board.draw(window);
		menu.drawEndingScreen(window,result);
		break;
	}
	window.display();
}

void Game::switchPlayerControls(){
	
	//set control scheme
	if(p1_controls.left == sf::Keyboard::Left){
		p2_controls.left = sf::Keyboard::Left;
		p2_controls.right = sf::Keyboard::Right;
		p2_controls.down = sf::Keyboard::Down;
		p1_controls.left = sf::Keyboard::A;
		p1_controls.right = sf::Keyboard::D;
		p1_controls.down = sf::Keyboard::S;
	}	
	else if(p1_controls.left==sf::Keyboard::A){
		p1_controls.left = sf::Keyboard::Left;
		p1_controls.right = sf::Keyboard::Right;
		p1_controls.down = sf::Keyboard::Down;
		p2_controls.left = sf::Keyboard::A;
		p2_controls.right = sf::Keyboard::D;
		p2_controls.down = sf::Keyboard::S;
	}

}
