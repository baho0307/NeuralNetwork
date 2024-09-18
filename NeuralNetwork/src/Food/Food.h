#pragma once

#include "../Network/Network.h"
#include "../Random/Random.h"

class Food
{
public:
	Food();
	Food(int size_x, int size_y);
	std::vector<Eigen::Vector2i> foods;
	Random rnd;
	void create();
	void generate();
private:
	int x;
	int y;
};

