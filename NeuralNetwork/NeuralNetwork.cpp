#include <iostream>
#include "src/Neuron/Neuron.h"
#include "src/Network/Network.h"
#include "src/Snake/Snake.h"
#include "src/Population/Population.h"

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

void NetworkDebug()
{
    float i = 0;
    float debug_sp = 1;
    // weights and neurons needs to be illustrated in terminal (for debug mode)
    VectorXd a(2);
    Screen scr(120, 30);
    std::string* scraddr = scr.generate();
    Network net(std::vector<int> {2, 8, 8, 4}, scraddr);

    a(0) = 0.5;
    a(1) = 1;
    net.calc(a);
    //net.getOut();
    bool aKeyPressed = false;
    bool dKeyPressed = false;

    while (true)
    {
        if ((GetAsyncKeyState('A') & 0x8001) && !aKeyPressed) // 'A' tuþuna basýldýðýnda (ama daha önce basýlmamýþsa)
        {
            i -= debug_sp;
            aKeyPressed = true;
            dKeyPressed = false; // 'D' tuþunun basýlmadýðýndan emin olun
        }
        if ((GetAsyncKeyState('D') & 0x8001) && !dKeyPressed) // 'D' tuþuna basýldýðýnda (ama daha önce basýlmamýþsa)
        {
            i += debug_sp;
            dKeyPressed = true;
            aKeyPressed = false; // 'A' tuþunun basýlmadýðýndan emin olun
        }
        if ((GetAsyncKeyState('A') & 0x8001) == 0) // 'A' tuþu serbest býrakýldýðýnda
        {
            aKeyPressed = false;
        }
        if ((GetAsyncKeyState('D') & 0x8001) == 0) // 'D' tuþu serbest býrakýldýðýnda
        {
            dKeyPressed = false;
        }
        net.debug(i);
        scr.Show();
    }
}

int main()
{
    Screen scr(40, 30);
    Population pop(2000, 200, { 23, 32, 32, 3}, &scr);

    while (true)
    {
        pop.Run();
    }
}
