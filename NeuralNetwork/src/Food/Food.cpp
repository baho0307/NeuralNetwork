#include "Food.h"

Food::Food()
{
	foods.push_back(Eigen::Vector2i((int)rnd.getRand(x/4, 3*x/4), (int)rnd.getRand(y / 4, 3 * y / 4)));
}

Food::Food(int size_x, int size_y)
{
	x = size_x;
	y = size_y;
	foods.push_back(Eigen::Vector2i((int)rnd.getRand(x / 4, 3 * x / 4), (int)rnd.getRand(y / 4, 3 * y / 4)));
}

void Food::create()
{
	foods.push_back(Eigen::Vector2i((int)rnd.getRand(x / 4, 3 * x / 4), (int)rnd.getRand(y / 4, 3 * y / 4)));
}

void Food::generate()
{
	foods.clear();
	foods.push_back(Eigen::Vector2i((int)rnd.getRand(x / 4, 3 * x / 4), (int)rnd.getRand(y / 4, 3 * y / 4)));
}
