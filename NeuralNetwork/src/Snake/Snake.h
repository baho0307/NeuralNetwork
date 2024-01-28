#pragma once

#include "../Network/Network.h"

enum DIR
{
	LEFT = -1,
	RIGHT = 1,
	UP = 2,
	DOWN = -2
};

class Snake
{
private:
	bool dead = false;
	int life;
	int lifeTime = 0;
	int score = 0;
	Eigen::Vector2i food;
	Eigen::Vector2i size;
	Eigen::Vector2i hLoc;
	std::vector<Eigen::Vector2i> body;
	Random	rnd;
	Network	net;

	Eigen::VectorXd		decision;

	VectorXd input;

	DIR					dir = UP;

	bool bodyCollide(Eigen::Vector2i pos);
	bool wallCollide(Eigen::Vector2i pos);
	bool foodCollide(Eigen::Vector2i pos);
	Eigen::Vector3d lookInDirection(Eigen::Vector2i dir);
	void Look();
	void Think();
	void shiftBody();
	void moveUp();
	void moveDown();
	void moveLeft();
	void moveRight();

	Eigen::Vector2i relative(Eigen::Vector2i vec, DIR dir);
public:
	Snake();
	Snake(Network brain, int life, int x, int y);
	Snake(std::vector<int> brain, int life, int  x, int y);

	bool getDead();

	void Move();
	void Draw(std::string *str);
	Snake crossover(Snake& other);
};

