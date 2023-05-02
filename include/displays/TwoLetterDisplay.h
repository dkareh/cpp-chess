// Author: Daniel Kareh
// Summary: A display that prints two letters per chess piece. One letter (b/w)
//          denotes the color and another denotes the piece type.

#ifndef CHESS_DISPLAYS_TWO_LETTER_DISPLAY_H
#define CHESS_DISPLAYS_TWO_LETTER_DISPLAY_H

#include <Display.h>

class TwoLetterDisplay : public Display {
public:
	virtual void show(const Board&) override;
};

#endif
