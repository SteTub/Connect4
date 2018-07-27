#ifndef _TILE_H_INCLUDED_
#define _TILE_H_INCLUDED_

#include<iostream>
#include<SFML/Graphics.hpp>
#include<cassert>
#include<iostream>
#include<unistd.h>
#include<cmath>
#include<random>
#include<sys/time.h>

#define WINDOW_WIDTH 600
#define WINDOW_HEIGHT 600
#define MILLION 1000000

class Tile : public sf::CircleShape
{
	public:
	Tile();	

	void initTile(int which_player);

	//getters
	double getMySpeed() const;
	int getRowPosition() const;
	int getColumnPosition() const;
	bool isMoving() const;

	//setters
	void setMySpeed(double new_speed);
	void setRowPosition(int row_in);
	void setColumnPosition(int column_in);
	void startedMoving();
	void finishedMoving();
	
	//graphics
	void draw(sf::RenderWindow& window);
	void moveLeft();
	void moveRight();

	bool intersectWith(const sf::CircleShape& other_tile) const;
	bool checkCollisionWithTile(const sf::CircleShape &other_tile);
	bool checkCollisionWithBase(const sf::RectangleShape &base);

	private:
	bool moving;
	int row_position;
	int col_position;
	double falling_speed;
};

#endif
