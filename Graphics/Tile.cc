#include "Tile.h"

//---CONSTRUCTOR---//
Tile::Tile(){

	falling_speed=0.0;
	moving=false;
	row_position=0;
	col_position=4;
}

//---INITIALIZE---//	
void Tile::initTile(int which_player){

	setRadius(WINDOW_WIDTH/20.0);
	setPosition(sf::Vector2f((4.5)*WINDOW_WIDTH/10.0,((1)*WINDOW_HEIGHT/10.0)));
	setOutlineThickness(-WINDOW_WIDTH/60.0);
	int player = which_player%2;	

	switch(player){
	
	case 0:
		setFillColor(sf::Color::Red);	
		setOutlineColor(sf::Color(219,15,15));
		break;
	case 1:
		setFillColor(sf::Color::Yellow);	
		setOutlineColor(sf::Color(255,210,0));
		break;
	}
}

//---GETTERS---//
double Tile::getMySpeed() const{
	return falling_speed;
}
int Tile::getRowPosition() const{
	return row_position;
}
int Tile::getColumnPosition() const{
	return col_position;
}

//---SETTERS---//

void Tile::setMySpeed(double new_speed){
	falling_speed = new_speed;
}
void Tile::setRowPosition(int row_in){
	row_position=row_in;
}
void Tile::setColumnPosition(int column_in){
	col_position=column_in;
}

//---MOVEMENT---//

void Tile::moveLeft(){
	if(!moving){
		if(col_position > 1){
			--col_position;
			move(-WINDOW_WIDTH/10.0,0);
		}
	}
}

void Tile::moveRight(){
	if(!moving){
		if(col_position < 7){
			++col_position;
			move(+WINDOW_WIDTH/10.0,0);
		}
	}
}
void Tile::startedMoving(){
	moving=true;
}
void Tile::finishedMoving(){
	moving=false;
}
bool Tile::isMoving() const{
	return moving;
}

//---COLLISIONS---//

bool Tile::intersectWith(const sf::CircleShape &other_tile) const{
	sf::FloatRect my_position=(*this).getGlobalBounds();
	sf::FloatRect other_position=other_tile.getGlobalBounds();

	return my_position.intersects(other_position);
}

bool Tile::checkCollisionWithTile(const sf::CircleShape &other_tile){
	
	if(intersectWith(other_tile)){
		return true;
	}
	return false;
}
bool Tile::checkCollisionWithBase(const sf::RectangleShape &base){

	sf::FloatRect my_position = (*this).getGlobalBounds();
	sf::FloatRect base_position = base.getGlobalBounds();

	if(my_position.intersects(base_position)){
		return true;
	}
	return false;
}
