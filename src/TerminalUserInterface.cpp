#include <TerminalUserInterface.h>
#include <iostream>

void TerminalUserInterface::clear_screen() const {
	for (int i{ 0 }; i < 200; i++)
		std::cout << '\n';
}
