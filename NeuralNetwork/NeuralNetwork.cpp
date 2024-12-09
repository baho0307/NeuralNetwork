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
        if ((GetAsyncKeyState('A') & 0x8001) && !aKeyPressed) // 'A' tu�una bas�ld���nda (ama daha �nce bas�lmam��sa)
        {
            i -= debug_sp;
            aKeyPressed = true;
            dKeyPressed = false; // 'D' tu�unun bas�lmad���ndan emin olun
        }
        if ((GetAsyncKeyState('D') & 0x8001) && !dKeyPressed) // 'D' tu�una bas�ld���nda (ama daha �nce bas�lmam��sa)
        {
            i += debug_sp;
            dKeyPressed = true;
            aKeyPressed = false; // 'A' tu�unun bas�lmad���ndan emin olun
        }
        if ((GetAsyncKeyState('A') & 0x8001) == 0) // 'A' tu�u serbest b�rak�ld���nda
        {
            aKeyPressed = false;
        }
        if ((GetAsyncKeyState('D') & 0x8001) == 0) // 'D' tu�u serbest b�rak�ld���nda
        {
            dKeyPressed = false;
        }
        net.debug(i);
        scr.Show();
    }
}

int main()
{
    Screen scr(50, 30);
    Population pop(2000, 2000, { 24, 64, 32, 32, 16, 3}, &scr);

    bool dKeyPressed = false; // 'D' tu�unun mevcut durumu
    bool wasDKeyReleased = true; // 'D' tu�unun �nceki durumu (ba�lang��ta serbest)

    while (true)
    {
        // 'D' tu�unun durumunu kontrol et
        bool isDKeyPressed = (GetAsyncKeyState('D') & 0x8001) != 0; // 'D' tu�unun bas�l� olup olmad���

        // Tu�un serbest b�rak�ld��� durum
        if (!isDKeyPressed && wasDKeyReleased)
        {
            // Tu� serbest b�rak�ld���nda flip-flop i�lemi
            dKeyPressed = !dKeyPressed; // Flip-Flop: mevcut durumu tersine �evir
            wasDKeyReleased = false; // Art�k tu� serbest b�rak�lmad�
        }
        else if (isDKeyPressed)
        {
            wasDKeyReleased = true; // Tu� bas�l� oldu�u i�in serbest b�rak�ld� olarak i�aretle
        }

        // pop.Run() fonksiyonu, dKeyPressed de�i�kenine ba�l� olarak cizim yapacak
        pop.Run(dKeyPressed);
    }
}
