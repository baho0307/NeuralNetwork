#pragma once

#include "../Network/Network.h"
#include <math.h>
#include "../Food/Food.h"

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
	bool dead;
	int life;
	int lifeTime;
	int lastTime;
	int score;
	double fitness;
	int f_i;
	Food *food;
	Eigen::Vector2i size;
	Eigen::Vector2i hLoc;
	std::vector<Eigen::Vector2i> body;
	Random	rnd;
	Network	net;

	Eigen::VectorXd		decision;

	VectorXd input;

	DIR					dir;

	bool bodyCollide(Eigen::Vector2i pos);
	bool wallCollide(Eigen::Vector2i pos);
	bool foodCollide(Eigen::Vector2i pos);
	Eigen::Vector3d lookInDirection(Eigen::Vector2i dir);
	void Look();
	void Think();
	void shiftBody();
	void moveUp();
	void moveLeft();
	void moveRight();

	void createBody();
	void eat();

	Eigen::Vector2i relative(Eigen::Vector2i vec, DIR dir);
public:
	Snake();
	Snake(const Snake& other);
	Snake(Network brain, Food* f, int life, int x, int y);
	Snake(std::vector<int> brain, Food* f, int life, int  x, int y);

	bool getDead();
	double getFitness();

	void Move();
	void Draw(std::string *str);
	Snake crossover(Snake& other);
};

