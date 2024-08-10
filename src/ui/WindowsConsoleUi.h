// Author: Daniel Kareh
// Summary: An interface that prints a Unicode symbol for each chess piece.
//          The squares are also colored correctly.

#ifndef CHESS_UI_WINDOWS_CONSOLE_UI_H
#define CHESS_UI_WINDOWS_CONSOLE_UI_H

#include "../TerminalUserInterface.h"

// A `glyph_size` represents a possible width for chess glyphs in a terminal.
// Depending on the terminal and the chosen font, chess glyphs may take up one
// cell or two cells, i.e., they may be single width or double width.
enum class glyph_size {
	single_width,
	double_width,
};

class WindowsConsoleUi : public TerminalUserInterface {
public:
	// NOTE: We can't determine the size of glyphs automatically, so it must be
	// specified explicitly. The glyph size cannot be changed later on.
	explicit WindowsConsoleUi(enum glyph_size);

	virtual void show(const Board&) override;

private:
	void clear_screen();

	void* console_output{ nullptr };
	enum glyph_size glyph_size;
};

#endif
