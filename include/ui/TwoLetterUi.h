// Author: Daniel Kareh
// Summary: An interface that prints two letters per chess piece. One letter (b/w)
//          denotes the color and another denotes the piece type.

#ifndef CHESS_UI_TWO_LETTER_UI_H
#define CHESS_UI_TWO_LETTER_UI_H

#include <UserInterface.h>

class TwoLetterUi : public UserInterface {
public:
	virtual void show(const Board&) override;
};

#endif
