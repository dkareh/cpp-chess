// Author: Daniel Kareh
// Summary: A base class for various user interfaces.

#ifndef CHESS_USER_INTERFACE_H
#define CHESS_USER_INTERFACE_H

#include <string_view>
#include "Board.h"

enum class notify_pause { no, yes };

class UserInterface {
public:
	virtual ~UserInterface() = default;

	virtual void show(const Board&) = 0;
	virtual Move read_move(color active_color) = 0;
	virtual void notify(std::string_view message, notify_pause = notify_pause::no) = 0;
	virtual int choose_move(const std::vector<MoveDetails>&) = 0;
};

#endif
