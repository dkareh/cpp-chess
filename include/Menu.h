// Author: Daniel Kareh
// Summary: A simple class that anyone can use to make a command line menu.

#ifndef CHESS_MENU_H
#define CHESS_MENU_H

#include <string>
#include <vector>

struct MenuItem {
	std::string description;
};

class Menu {
public:
	// Display the menu and return whatever the user inputs.
	// If the user chooses to exit, this function will immediately call `std::exit()`.
	std::size_t run() const;

	std::string title;
	std::vector<MenuItem> items;
};

#endif
