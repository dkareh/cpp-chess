// Author: Daniel Kareh
// Summary: An interface that prints a Unicode symbol for each chess piece.
//          The squares are also colored correctly.

#ifndef CHESS_UI_WINDOWS_CONSOLE_UI_H
#define CHESS_UI_WINDOWS_CONSOLE_UI_H

#include <UserInterface.h>

class WindowsConsoleUi : public UserInterface {
public:
	WindowsConsoleUi();

	virtual void show(const Board&) override;

private:
	void clear_screen();

	void* console_output{ nullptr };
};

#endif
