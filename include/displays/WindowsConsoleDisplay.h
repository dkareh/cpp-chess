// Author: Daniel Kareh
// Summary: A display that prints a Unicode symbol for each chess piece.
//          The squares are also colored correctly.

#ifndef CHESS_DISPLAYS_WINDOWS_CONSOLE_DISPLAY_H
#define CHESS_DISPLAYS_WINDOWS_CONSOLE_DISPLAY_H

#include <Display.h>

class WindowsConsoleDisplay : public Display {
public:
	WindowsConsoleDisplay();

	virtual void show(const Board&) override;

private:
	void clear_screen();

	void* console_output{ nullptr };
};

#endif
