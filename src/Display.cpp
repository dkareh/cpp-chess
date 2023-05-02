// Author: Daniel Kareh
// Summary: An interface for various types of displays. A display is something
//          that can show a chess board to the players.

#include <Display.h>
#include <iostream>

void Display::clear_screen() const {
	for (int i{ 0 }; i < 200; i++)
		std::cout << '\n';
}
