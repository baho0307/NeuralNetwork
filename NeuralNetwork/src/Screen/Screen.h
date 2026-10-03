#pragma once

#include <string>
#include <sstream>
#include <iostream>
#include <conio.h>
#include <thread>
#include <mutex>
#include <Windows.h>
#include <deque>

class Screen
{
public:
	Screen(int hX, int hY, std::mutex& mutex);
	std::deque<std::string>*	generate();
	void	UpdateLoopBfr(std::deque<std::string> data);
	void	Show();
	int		get_x();
	int		get_y();

private:
	int				x;
	int				y;
	std::string		scrBuffer;
	std::string		prevBuffer;
	std::deque<std::string>*	source;
	std::deque<std::string> loopSource;
	std::deque<std::string>	loopBuffer;
	std::mutex& dataMutex;
	int							loopCounter;
	void	setCursorPosition(int x, int y);
	void			swap();
	void	LoadBuffer();
};

