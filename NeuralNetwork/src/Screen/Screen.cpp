#include "Screen.h"

Screen::Screen(int hX, int hY)
{
	x = hX;
	y = hY;
	source = new std::string();
	scrBuffer = std::string(x * y, ' ');
	prevBuffer = std::string(x * y, ' ');
	std::cout << scrBuffer;
}

void Screen::setCursorPosition(int x, int y)
{
	static const HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	std::cout.flush();
	COORD coord = { (SHORT)x, (SHORT)y };
	SetConsoleCursorPosition(hOut, coord);
}

std::string* Screen::generate()
{
	return source;
}

void Screen::swap()
{
	std::string tmp;

	tmp = prevBuffer;
	prevBuffer = scrBuffer;
	scrBuffer = tmp + std::string(x * y - tmp.length(), ' ');
}

void Screen::Show()
{
	prevBuffer = *source;
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
}

int Screen::get_x()
{
	return x;
}

int Screen::get_y()
{
	return y;
}
