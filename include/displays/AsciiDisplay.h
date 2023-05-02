// Author: Daniel Kareh
// Summary: A display that prints ASCII pictures for each piece.

#ifndef CHESS_DISPLAYS_ASCII_DISPLAY_H
#define CHESS_DISPLAYS_ASCII_DISPLAY_H

#include <Display.h>

class AsciiDisplay : public Display {
public:
	virtual void show(const Board&) override;
};

#endif
