#include "Network.h"

Network::Network()
{

}

Network::Network(std::vector<MatrixXd> weights, std::vector<VectorXd> biases, VectorXd out)
{
    this->weights = weights;
    this->biases = biases;
    this->out = out;
}

Network::Network(std::vector<int> layers)
{
    for (int i = 1; i < layers.size(); i++)
    {
        weights.push_back(MatrixXd(layers[i], layers[i - 1]));
        biases.push_back(VectorXd(layers[i]));
        for (int j = 0; j < layers[i]; j++)
        {
            (biases[i - 1])(j) = rand.getRand(-1, 1);
            for (int k = 0; k < layers[i - 1]; k++)
                (weights[i - 1])(j, k) = rand.getRand(-1, 1);
        }
    }
}

Network Network::operator=(const Network& net)
{
   	weights = net.weights;
	biases = net.biases;
    out = net.out;

    return Network(weights, biases, out);
}

static std::string toString(const Eigen::MatrixXd& mat) {
    std::stringstream ss;
    ss << mat;
    return ss.str();
}

static Eigen::VectorXd sigmoid(const Eigen::VectorXd& input) {
    return 1.0 / (1.0 + (-input.array()).exp());
}

Network Network::crossover(const Network& net)
{
    std::vector<MatrixXd> newW;
    std::vector<VectorXd> newB;
    for (int i = 0; i < weights.size(); i++)
    {
        Eigen::MatrixXd identity = Eigen::MatrixXd::Identity(weights[i].cols(), weights[i].cols());
        identity.bottomRightCorner(identity.rows() / 2, identity.cols() / 2) = MatrixXd::Zero(identity.rows() / 2, identity.cols() / 2);
        newW.push_back(identity * weights[i] + identity.transpose() * net.weights[i]);
        newB.push_back((biases[i] + net.biases[i]) / 2);
    }
    return Network(newW, newB, out);
}

void Network::calc(VectorXd in)
{
    VectorXd r = in;
    for (int i = 0; i < weights.size(); i++)
    {
        r = sigmoid((weights[i] * r) + biases[i]);
    }
    out = r;
}

VectorXd Network::getOut()
{
    //std::cout << out << std::endl;
    return out;
}

#if DEBUG

Network::Network(std::vector<int> layers, std::string* scr) : Network(layers)
{
    _scr = scr;
}

void Network::debug(int x)
{
    if (!_scr)
        std::cout << "PLEASE CREATE THE OBJECT WITH DEBUG CONSTRUCTOR" << std::endl;
    else
    {
        _scr->clear();
        *_scr += toString(weights[x % weights.size()]); //+ ((i + x) % weights.size() < weights.size() - 1 ? " : hl" + (i + x) % weights.size() : " : ol") + " | ";
    }
}

#endif
