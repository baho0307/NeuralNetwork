#include <iostream>
#include "Eigen/Core"
#include "src/Neuron/Neuron.h"
#include "src/Network/Network.h"

using namespace Eigen;

void NeuronTest()
{
    //layers
    std::vector<Neuron> in(1, Neuron(std::vector<double>({ 0.5, 0 })));
    std::vector<Neuron> out(1, Neuron(std::vector<double>({ 0.25, 0 })));
    //debug
    std::vector<double> outs;

    //calculate with manual values
    outs.push_back(in[0].calculate(std::vector<double>(1, 1)));
    //calculate with previous neuron layer
    outs.push_back(out[0].calculate(in));
    std::cout << 1 << " -> " << outs[0] << " -> " << outs[1];
}

int main()
{
    VectorXd a = VectorXd::Random(8);
    Network net(std::vector<int> {8, 8, 8, 4});

    std::cout << a << std::endl;
    net.calc(a);
    net.getOut();
}
