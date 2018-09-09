#include"Game.h"

Game::Game(){}

//---MAIN FUNCTIONS---//
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
	background.scale(sf::Vector2f(1.0f,1.5f)); 

	//MENU
	menu.initMenu();

	//INIT VALUES
	game_state = INTRO;
	first_time=true;
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

	restart_game = false;	

	//set control scheme
	p1_controls.left = sf::Keyboard::Left;
	p1_controls.right = sf::Keyboard::Right;
	p1_controls.down = sf::Keyboard::Down;
	p2_controls.left = sf::Keyboard::A;
	p2_controls.right = sf::Keyboard::D;
	p2_controls.down = sf::Keyboard::S;

	//test board
/*
	board(6,1)=' ';board(6,2)=' ';board(6,3)=' ';board(6,4)=' ';board(6,5)=' ';board(6,6)=' ';board(6,7)=' ';
	board(5,1)=' ';board(5,2)=' ';board(5,3)=' ';board(5,4)=' ';board(5,5)=' ';board(5,6)=' ';board(5,7)=' ';
	board(4,1)=' ';board(4,2)=' ';board(4,3)=' ';board(4,4)=' ';board(4,5)=' ';board(4,6)=' ';board(4,7)=' ';
	board(3,1)=' ';board(3,2)=' ';board(3,3)='1';board(3,4)='1';board(3,5)=' ';board(3,6)=' ';board(3,7)=' ';
	board(2,1)=' ';board(2,2)=' ';board(2,3)='1';board(2,4)='2';board(2,5)='2';board(2,6)='1';board(2,7)=' ';
	board(1,1)='L';board(1,2)=' ';board(1,3)='2';board(1,4)='1';board(1,5)='1';board(1,6)='2';board(1,7)=' ';
*/
}

void Game::run(){

	while(window.isOpen())
	{
		processEvents();
		update();
		display();
	}
}

//---DISPLAY---//
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

//---PROCESS EVENTS---//
void Game::processEvents(){
		
	while(window.pollEvent(event))
	{
		if(event.type == sf::Event::Closed){
			window.close();
			cleanPlayerMemory();
		}
		else if(event.type == sf::Event::KeyPressed && game_state==INTRO){
			moveToNextGameState();
		}
		else if(event.type == sf::Event::KeyPressed && game_state <= P2_SELECT && !menu.isMenuTileMoving()){
			processKeyPress_PlayerChoice(event.key.code);
		}
		else if(event.type == sf::Event::KeyPressed && game_state==CONTROLS){
			processKeyPress_ControlsChoice(event.key.code);
		}
		else if(event.type == sf::Event::KeyPressed && game_state==PLAYING && isPlayerHuman(tiles%2+1)){
			processKeyPress_PlayerTiles(event.key.code, tiles%2+1);
		}
		else if(event.type == sf::Event::KeyPressed && game_state==ENDING){
			if(event.key.code==sf::Keyboard::R)
				rematch();
		}
	}
}

void Game::processKeyPress_PlayerChoice(int key_press){

	if(key_press==sf::Keyboard::Down){
		menu.movePlayerChoiceDown();
	}
	if(key_press==sf::Keyboard::Up){
		menu.movePlayerChoiceUp();
	}
	if(key_press==sf::Keyboard::Return || event.key.code==sf::Keyboard::Space){
		if(menu.atHumanChoice()){
			setPlayerAsHuman();
		}
		moveToNextGameState();
	}
}

void Game::processKeyPress_ControlsChoice(int key_press){

	if(key_press==sf::Keyboard::Left){
		menu.moveControlChoiceLeft();
	}
	else if(key_press==sf::Keyboard::Right){
		menu.moveControlChoiceRight();
	}
	else if(key_press==sf::Keyboard::Return || event.key.code==sf::Keyboard::Space){
		setPlayersAndControls();			
		moveToNextGameState();
	}
}

void Game::processKeyPress_PlayerTiles(int key_press, int order){

	controls player_controls;
	char tile;

	if(order == 1){
		player_controls = p1_controls;
		tile = '1';
	}
	else if(order == 2){
		player_controls = p2_controls;
		tile = '2';
	}
	if(key_press==player_controls.left){
		board.moveTileLeft();
	}
	else if(key_press==player_controls.right){
		board.moveTileRight();
	}
	else if(key_press==player_controls.down && !board.columnFull(board.getTilePosition_Column())){
		if(!board.isTileMoving()){
			board.insertTileInColumn(board.getTilePosition_Column(),tile);
			reset_clock=true;	
		}
	}
}

//---MAKE UPDATES---//
void Game::update(){

	//animation for menu tile	
	if(game_state==P1_SELECT && menu.isMenuTileMoving()){
		menu.animateMenuTileDropping();	
	}
	if(game_state == P2_SELECT && first_time){
		menu.resetForPlayerTwoChoice();
		first_time = false;
	}
	if(game_state == CONTROLS && reset_control_text){
		reset_control_text=false;
		menu.resetTextForControlChoice(p1_human,p2_human);
	}
	//check if game over before creating new tile	
	if(game_state == PLAYING && !board.isTileMoving()){
		checkForGameOver();
	}
	//create new tile	
	if(board.needNewTile() && game_state<ENDING){
		board.createTile();
		++tiles;
	}
	if(game_state==PLAYING && !board.isTileMoving()){
		makeAISelection();
	}
	//reset game
	if(game_state==ENDING && restart_game==true)
		rematch();

	updateDroppingTile();
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

void Game::makeAISelection(){

	if(reset_clock){
		if(tiles%2 == 0)
			ai_selection = one->makeMove(board);	//if either player is human, this will return 0
		else if(tiles%2==1) 
			ai_selection = two->makeMove(board);
	
		ai_clock.restart();
		reset_clock = false;
	}

	char tile = '1';
	if(tiles%2==1)
		tile = '2';

	sf::Time ai_time=ai_clock.getElapsedTime();
	if(ai_time > sf::milliseconds(300)){
		if(board.getTilePosition_Column() == ai_selection && ai_selection > 0){		//doesn't make a move if player is human
			board.insertTileInColumn(board.getTilePosition_Column(),tile);
			ai_clock.restart();
			ai_selection = 0;
			if(!p1_human && !p2_human)
				reset_clock = true;
		}
		else if(board.getTilePosition_Column() > ai_selection && ai_selection > 0){
			board.moveTileLeft();
			ai_clock.restart();
		} 
		else if(board.getTilePosition_Column() < ai_selection && ai_selection > 0){
			ai_clock.restart();
			board.moveTileRight();
		}
	}  
}

void Game::checkForGameOver(){
	
	//check for win or stalemate
	if(tiles == 42){
		result = 0;
		moveToNextGameState();
	}
	//check for win or stalemate
	else if(board.detectWin(tiles%2+1)){
		if(tiles%2==0)
			result = 1;
		else result = 2;
		moveToNextGameState();
	}
}

void Game::moveToNextGameState(){
	++game_state;
}

//---GET PLAYER TYPES---//
bool Game::isPlayerHuman(int order) const{
	if(order == 1)
		return isPlayerOneHuman();
	else if(order == 2)
		return isPlayerTwoHuman();
	else return false;
}
bool Game::isPlayerOneHuman() const{
	return p1_human;
}
bool Game::isPlayerTwoHuman() const{
	return p2_human;
}

//---SET PLAYER TYPES AND CONTROLS---//
void Game::setPlayersAndControls(){

	if(!p1_human && p2_human)
		switchPlayerControls();
	if(menu.playerChoseWASD()){
		switchPlayerControls();
	}
	one = Player::createPlayer(p1_choice,'1',1);
	two = Player::createPlayer(p2_choice,'2',2);
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

//----REMATCH----//
void Game::rematch(){
	
	//INIT VALUES
	game_state = PLAYING;
	first_time=true;
	reset_control_text=true;
	tiles=-1;	 

	reset_clock = true;
	restart_game=false;

	board.resetBoard();
}

void Game::cleanPlayerMemory(){
	if(game_state > CONTROLS){
		delete one;
		delete two;	
	}
	return;
}
