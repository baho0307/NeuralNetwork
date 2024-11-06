#pragma once

#include "../Network/Network.h"
#include "../Random/Random.h"

class Food
{
public:
	Food();
	Food(int size_x, int size_y);
	Random rnd;
	void create();
	Eigen::Vector2i food;
private:
	int max_x;
	int max_y;
};

