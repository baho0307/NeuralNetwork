#pragma once

#include <random>

class Random
{
public:
	Random();
	double getRand(double start, double end);
private:
	std::random_device rd;
	std::mt19937 gen;
};

