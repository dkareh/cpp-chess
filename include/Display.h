// Author: Daniel Kareh
// Summary: An interface for various types of displays. A display is something
//          that can show a chess board to the players.

#ifndef CHESS_DISPLAY_H
#define CHESS_DISPLAY_H

#include <Board.h>

class Display {
public:
	virtual ~Display() = default;

	virtual void show(const Board&) = 0;

protected:
	void clear_screen() const;
};

#endif
