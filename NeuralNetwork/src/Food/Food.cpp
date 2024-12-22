#include "Food.h"

Food::Food()
{
	food = (Eigen::Vector2i((int)rnd.getRand(1, max_x - 1), (int)rnd.getRand(1, max_y - 3)));
}

Food::Food(int size_x, int size_y)
{
	max_x = size_x;
	max_y = size_y;
	food = (Eigen::Vector2i((int)rnd.getRand(1, max_x - 1), (int)rnd.getRand(1, max_y - 1)));
}

void Food::create()
{
	food = (Eigen::Vector2i((int)rnd.getRand(1, max_x - 1), (int)rnd.getRand(1, max_y - 1)));// snake size adaptation should be added

}

//#include "Food.h"
//
//Food::Food()
//{
//	food = Eigen::Vector2i((int)rnd.getRand(max_x / 4, 3 * max_x / 4), (int)rnd.getRand(max_y / 4, 3 * max_y / 4));
//}
//
//Food::Food(int size_x, int size_y)
//{
//	max_x = size_x;
//	max_y = size_y;
//	food = (Eigen::Vector2i((int)rnd.getRand(max_x / 4, 3 * max_x / 4), (int)rnd.getRand(max_y / 4, 3 * max_y / 4)));
//}
//
//void Food::create()
//{
//	food = (Eigen::Vector2i((int)rnd.getRand(max_x / 4, 3 * max_x / 4), (int)rnd.getRand(max_y / 4, 3 * max_y / 4)));
//}