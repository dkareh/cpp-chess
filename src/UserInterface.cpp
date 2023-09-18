// Author: Daniel Kareh
// Summary: A base class for various user interfaces.

#include <UserInterface.h>
#include <iostream>

void UserInterface::clear_screen() const {
	for (int i{ 0 }; i < 200; i++)
		std::cout << '\n';
}
