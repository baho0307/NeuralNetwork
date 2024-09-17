#include "Food.h"

Food::Food()
{
	foods.push_back(Eigen::Vector2i((int)rnd.getRand(30, 90), (int)rnd.getRand(10, 20)));
}

void Food::create()
{
	foods.push_back(Eigen::Vector2i((int)rnd.getRand(30, 90), (int)rnd.getRand(10, 20)));
}
