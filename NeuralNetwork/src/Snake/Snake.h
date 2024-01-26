#pragma once

#include "../Network/Network.h"


class Snake
{
private:
	bool dead = true;
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

	Eigen::Vector2i		vel;

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

public:
	Snake();
	Snake(std::vector<int> brain, int life, int  x, int y);

	void Move();
	void Draw();
};

