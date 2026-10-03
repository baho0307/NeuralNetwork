#pragma once

#include "../Network/Network.h"
#include "../Random/Random.h"
#include <deque>

class Food
{
public:
	Food();
	Food(int size_x, int size_y);
	Random rnd;
	void create();
	void update_list(Eigen::Vector2i food);
	void createFromList();
	Eigen::Vector2i food;
private:
	int max_x;
	int max_y;
	std::deque<Eigen::Vector2i> food_list;
};

