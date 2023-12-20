#include "Neuron.h"

double Neuron::sigmoid(double x)
{
	return (2/(1 + exp(-x))) - 1;
}

Neuron::Neuron(std::vector<double> w)
{
	this->w = w;
	o = 0;
}

Neuron::Neuron(unsigned int w)
{
	//+1 is for bias
	this->w = std::vector<double>(w + 1, 1);
	this->w[w] = 0;
	o = 0;
}

Neuron Neuron::operator=(Neuron& other)
{
	this->w = other.w;
	o = 0;
	return *this;
}

double Neuron::get_output()
{
	return o;
}

double Neuron::calculate(std::vector<Neuron> outs)
{
	for (int k = 0; k < outs.size(); k++)
		o += outs[k].get_output() * w[k];
	//adding bias
	o += w[w.size() - 1];
	o = sigmoid(o);
	return get_output();
}

double Neuron::calculate(std::vector<double> outs)
{
	for (int k = 0; k < outs.size(); k++)
		o += outs[k] * w[k];
	o += w[w.size() - 1];
	o = sigmoid(o);
	return get_output();
}
