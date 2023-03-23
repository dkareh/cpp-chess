#include <displays/LetterDisplay.h>
#include <iostream>
#include <safe_ctype.h>

using std::cout;

void LetterDisplay::show(const Board& board) {
	clear_screen();

	const auto dimensions{ board.get_dimensions() };
	for (int rank{ dimensions.rank - 1 }; rank >= 0; rank--) {
		cout << convert_rank_to_digit(rank) << ' ';

		for (int file{ 0 }; file < dimensions.file; file++) {
			auto piece{ board.get_piece({ rank, file }) };
			if (!piece) {
				cout << ' ';
				continue;
			}

			char letter{ convert_piece_type_to_letter(piece->type) };
			if (piece->is_black())
				letter = safe_to_lower(letter);
			cout << letter;
		}

		cout << '\n';
	}

	cout << "\n ";
	for (int file{ 0 }; file < dimensions.file; file++)
		cout << convert_file_to_letter(file);
	cout << '\n';
}
