// Author: Daniel Kareh
// Summary: An interface that prints ASCII pictures for each piece.

#ifndef CHESS_UI_ASCII_UI_H
#define CHESS_UI_ASCII_UI_H

#include <UserInterface.h>

class AsciiUi : public UserInterface {
public:
	virtual void show(const Board&) override;
};

#endif
