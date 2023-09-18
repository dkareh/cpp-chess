// Author: Daniel Kareh
// Summary: An interface that prints one letter per chess piece. The case of
//          each letter denotes the color of the piece.

#ifndef CHESS_UI_LETTER_UI_H
#define CHESS_UI_LETTER_UI_H

#include <TerminalUserInterface.h>

class LetterUi : public TerminalUserInterface {
public:
	virtual void show(const Board&) override;
};

#endif
