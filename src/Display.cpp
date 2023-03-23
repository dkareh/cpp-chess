#include <Display.h>
#include <iostream>

void Display::clear_screen() const {
	for (int i{ 0 }; i < 200; i++)
		std::cout << '\n';
}
