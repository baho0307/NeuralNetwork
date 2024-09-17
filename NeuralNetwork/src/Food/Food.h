#pragma once

#include "../Network/Network.h"
#include "../Random/Random.h"

class Food
{
public:
	Food();
	std::vector<Eigen::Vector2i> foods;
	Random rnd;
	void create();
};

