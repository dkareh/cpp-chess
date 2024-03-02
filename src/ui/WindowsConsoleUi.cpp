// Author: Daniel Kareh
// Summary: An interface that prints a Unicode symbol for each chess piece.
//          The squares are also colored correctly.

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include "WindowsConsoleUi.h"
#include <iostream>
#include <stdexcept>
#include <windows.h>

using std::cout;

WindowsConsoleUi::WindowsConsoleUi() {
	HANDLE console_output{ GetStdHandle(STD_OUTPUT_HANDLE) };
	if (console_output == INVALID_HANDLE_VALUE || console_output == NULL) {
		throw std::runtime_error{ "Standard output cannot be opened" };
	}

	// Try to get the current console mode to test if standard output is a
	// console or a pipe, file, device file, etc.
	DWORD console_mode;
	if (!GetConsoleMode(console_output, &console_mode)) {
		throw std::runtime_error{ "Standard output is not connected to a console" };
	}

	// According to https://learn.microsoft.com/en-us/windows/console/getstdhandle,
	// we don't need to close `console_output`, so no custom destructor is needed.
	this->console_output = console_output;
}

// clang-format off
static std::array black_symbols{
	L'\u265F', L'\u265E', L'\u265D', L'\u265C',
	L'\u265B', L'\u265A', L'\u265C', L'\u265A',
};

static std::array white_symbols{
	L'\u2659', L'\u2658', L'\u2657', L'\u2656',
	L'\u2655', L'\u2654', L'\u2656', L'\u2654',
};
// clang-format on

void WindowsConsoleUi::show(const Board& board) {
	// Empty C++ output stream buffers before writing to standard output
	// directly using functions like `WriteConsoleW`.
	cout.flush();

	clear_screen();

	const auto dimensions{ board.get_dimensions() };
	for (int rank{ dimensions.rank - 1 }; rank >= 0; rank--) {
		cout << convert_rank_to_digit(rank) << ' ';
		cout.flush();

		for (int file{ 0 }; file < dimensions.file; file++) {
			auto piece{ board.get_piece({ rank, file }) };

			bool square_is_dark{ (rank + file) % 2 == 0 };
			WORD attributes{ 0 };
			attributes |= square_is_dark ? 0 : BACKGROUND_INTENSITY;
			attributes |= BACKGROUND_RED | BACKGROUND_GREEN; // Yellow
			SetConsoleTextAttribute(console_output, attributes);

			if (piece) {
				// Print the correct Unicode character for the piece's type.
				const auto& symbols{ piece->is_white() ? white_symbols : black_symbols };
				const int index{ static_cast<int>(piece->type) };
				const wchar_t symbol{ symbols.at(index) };
				WriteConsoleW(console_output, &symbol, 1, nullptr, nullptr);
			} else {
				cout << ' ';
				cout.flush();
			}

			// Change colors back to a white foreground and a black background.
			attributes = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;
			SetConsoleTextAttribute(console_output, attributes);
		}

		cout << std::endl;
	}

	// We're done using Windows API functions, so no need to flush here.
	cout << "  ";
	for (int file{ 0 }; file < dimensions.file; file++)
		cout << convert_file_to_letter(file);
	cout << "\n\n";
}

void WindowsConsoleUi::clear_screen() {
	// Get the number of cells in the console screen buffer.
	CONSOLE_SCREEN_BUFFER_INFO buffer_info;
	if (!GetConsoleScreenBufferInfo(console_output, &buffer_info))
		return;

	const DWORD size{ static_cast<DWORD>(buffer_info.dwSize.X * buffer_info.dwSize.Y) };

	// Fill the buffer with spaces (beginning from the top left corner).
	const COORD corner{ 0, 0 };
	DWORD chars_written; // Dummy output argument
	if (!FillConsoleOutputCharacterW(console_output, L' ', size, corner, &chars_written))
		return;

	// Set the attribute of each cell to the current console attribute.
	// Attributes include the foreground and background color.
	const WORD attributes{ buffer_info.wAttributes };
	if (!FillConsoleOutputAttribute(console_output, attributes, size, corner, &chars_written))
		return;

	// Move the cursor to the top left corner.
	SetConsoleCursorPosition(console_output, corner);
}
