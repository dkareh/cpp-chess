#ifndef CHESS_DISPLAY_H
#define CHESS_DISPLAY_H

// TODO: A simple forward declaration of `Board` would be enough.
//       Should I just do that?
#include <Board.h>

class Display {
public:
	virtual ~Display() = default;

	virtual void show(const Board&) = 0;

protected:
	void clear_screen() const;
};

#endif
