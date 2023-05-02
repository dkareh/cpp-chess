// Author: Daniel Kareh
// Summary: A display that prints one letter per chess piece. The case of
//          each letter denotes the color of the piece.

#ifndef CHESS_DISPLAYS_LETTER_DISPLAY_H
#define CHESS_DISPLAYS_LETTER_DISPLAY_H

#include <Display.h>

class LetterDisplay : public Display {
public:
	virtual void show(const Board&) override;
};

#endif
