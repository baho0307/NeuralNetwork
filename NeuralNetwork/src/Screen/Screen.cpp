#include "Screen.h"

Screen::Screen(int hX, int hY, std::mutex& mutex) : dataMutex(mutex)
{
	x = hX;
	y = hY;
	source = new std::deque<std::string>();
	scrBuffer = std::string(x * y, ' ');
	prevBuffer = std::string(x * y, ' ');
	std::cout << scrBuffer;
	loopCounter = 0;
}

void Screen::setCursorPosition(int x, int y)
{
	static const HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	std::cout.flush();
	COORD coord = { (SHORT)x, (SHORT)y };
	SetConsoleCursorPosition(hOut, coord);
}

std::deque<std::string>* Screen::generate()
{
	return source;
}

void Screen::UpdateLoopBfr(std::deque<std::string> data)
{
	loopBuffer = data;
}

void Screen::swap()
{
	std::string tmp;

	tmp = prevBuffer;
	prevBuffer = scrBuffer;
	scrBuffer = tmp + std::string(x * y - tmp.length(), ' ');
}

void Screen::LoadBuffer()
{
	loopSource = loopBuffer;
	loopBuffer = std::deque<std::string>();
}

void Screen::Show()// it gives some errors (to be fixed)
{
	while (true)
	{
		if ((loopSource.empty() || loopCounter == 0) && !loopBuffer.empty())
			LoadBuffer();
		if ((source == nullptr || source->empty()) && loopSource.empty())
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
			continue;
		}
		else if (source == nullptr || source->empty())
		{
			prevBuffer = loopSource.at(loopCounter % loopSource.size());
			swap();
			for (int i = 0; i < prevBuffer.size(); i++)
			{
				if (scrBuffer[i] != prevBuffer[i])
				{
					setCursorPosition((i % x), (i / x));
					std::cout << scrBuffer[i];
				}
			}
			setCursorPosition(0, 0);
			loopCounter = (loopCounter + 1) % loopSource.size();
		}
		else
		{
			dataMutex.lock();
			if (_kbhit()) //on any input, the screen source will jump to the current frame
			{
				_getch();
				if (source->size() > 1)
				{
					source->erase(source->begin(), source->end() - 1);
				}
			}
			prevBuffer = source->front();
			dataMutex.unlock();
			swap();
			for (int i = 0; i < prevBuffer.size(); i++)
			{
				if (scrBuffer[i] != prevBuffer[i])
				{
					setCursorPosition((i % x), (i / x));
					std::cout << scrBuffer[i];
				}
			}
			setCursorPosition(0, 0);
			dataMutex.lock();
			source->pop_front();
			dataMutex.unlock();
		}
	}
}

int Screen::get_x()
{
	return x;
}

int Screen::get_y()
{
	return y;
}
