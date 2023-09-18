// Author: Daniel Kareh
// Summary: A base class for various user interfaces.

#ifndef CHESS_USER_INTERFACE_H
#define CHESS_USER_INTERFACE_H

#include <Board.h>

class UserInterface {
public:
	virtual ~UserInterface() = default;

	virtual void show(const Board&) = 0;
};

#endif
