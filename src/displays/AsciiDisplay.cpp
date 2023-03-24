#include <displays/AsciiDisplay.h>
#include <iostream>
#include <stdexcept> // For std::invalid_argument.

using std::cout;

static const int columns_per_drawing{ 5 };
static const int rows_per_drawing{ 4 };
using Drawing = std::array<std::string, rows_per_drawing>;

static Drawing pawn_drawing{
	R"(     )",
	R"(  o  )",
	R"( ( ) )",
	R"( /_\ )",
};

static Drawing knight_drawing{
	R"( /-) )",
	R"(/o ) )",
	R"(\| | )",
	R"( |_| )",
};

static Drawing bishop_drawing{
	R"(  o  )",
	R"( (+) )",
	R"( | | )",
	R"(~~^~~)",
};

static Drawing rook_drawing{
	R"(|_|_|)",
	R"( | | )",
	R"( / \ )",
	R"(|___|)",
};

static Drawing queen_drawing{
	R"( o o )",
	R"(oVVVo)",
	R"( \ / )",
	R"( /_\ )",
};

static Drawing king_drawing{
	R"(  +  )",
	R"(()|())",
	R"( | | )",
	R"(_|_|_)",
};

static const Drawing& choose_drawing(piece_type type) {
	switch (type) {
	case piece_type::pawn:
		return pawn_drawing;
	case piece_type::knight:
		return knight_drawing;
	case piece_type::bishop:
		return bishop_drawing;
	case piece_type::rook:
		return rook_drawing;
	case piece_type::queen:
		return queen_drawing;
	case piece_type::king:
		return king_drawing;
	default:
		throw std::invalid_argument{ "Invalid piece type" };
	}
}

void AsciiDisplay::show(const Board& board) {
	clear_screen();

	const auto dimensions{ board.get_dimensions() };
	for (int rank{ dimensions.rank - 1 }; rank >= 0; rank--) {
		for (int row{ 0 }; row < rows_per_drawing; row++) {
			// Display the rank digit in roughly the middle of the rank.
			if (row == rows_per_drawing / 2)
				cout << convert_rank_to_digit(rank) << ' ';
			else
				cout << "  ";

			for (int file{ 0 }; file < dimensions.file; file++) {
				auto piece{ board.get_piece({ rank, file }) };
				if (!piece) {
					cout << ' ' << std::string(columns_per_drawing, ' ') << ' ';
					continue;
				}

				const Drawing& drawing{ choose_drawing(piece->type) };
				auto drawing_row{ drawing[row] };
				if (row == rows_per_drawing / 2) {
					// Make the middle character of a piece 'b' or 'w' to
					// indicate its color.
					drawing_row[columns_per_drawing / 2] = piece->is_black() ? 'b' : 'w';
				}
				cout << ' ' << drawing_row << ' ';
			}
			cout << '\n';
		}
		cout << '\n';
	}

	cout << "  ";
	for (int file{ 0 }; file < dimensions.file; file++) {
		// Add two to account for the extra padding added between pieces.
		const int total_columns{ columns_per_drawing + 2 };
		const int space_count{ total_columns / 2 };
		cout << std::string(space_count, ' ');
		cout << convert_file_to_letter(file);
		cout << std::string(total_columns - space_count - 1, ' ');
	}
	cout << '\n';
}
