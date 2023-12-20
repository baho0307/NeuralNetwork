#pragma once

#include <vector>
#include <math.h>

class Neuron
{

private:
	std::vector<double>		w;
	double					o;
	double					sigmoid(double x);

public:
	Neuron(std::vector<double> w);
	Neuron(unsigned int w);

	Neuron	operator=(Neuron& other);
	double	get_output();
	double	calculate(std::vector<Neuron> outs);
	double	calculate(std::vector<double> outs);
};

