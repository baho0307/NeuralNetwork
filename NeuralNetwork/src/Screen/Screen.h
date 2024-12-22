#pragma once

#include <string>
#include <sstream>
#include <iostream>
#include <Windows.h>
#include <vector>

class Screen
{
public:
	Screen(int hX, int hY);
	std::vector<std::string>*	generate();
	void	Show();
	int get_x();
	int get_y();

private:
	int				x;
	int				y;
	std::string		scrBuffer;
	std::string		prevBuffer;
	std::vector<std::string>*	source;
	void	setCursorPosition(int x, int y);
	void			swap();
};

