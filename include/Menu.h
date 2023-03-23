#ifndef CHESS_MENU_H
#define CHESS_MENU_H

#include <string>
#include <vector>

struct MenuItem {
	std::string description;
};

class Menu {
public:
	std::size_t run() const;

	std::string title;
	std::vector<MenuItem> items;
};

#endif
