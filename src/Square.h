// Author: Daniel Kareh
// Summary: A class and several helper functions to describe and manipulate
//          squares. A square is one location on a chess board.

#ifndef CHESS_SQUARE_H
#define CHESS_SQUARE_H

#include <cassert>
#include <optional>
#include <string>
#include <string_view>
#include "safe_ctype.h"

/// `rank` must be between 0 and 7.
inline char convert_rank_to_digit(int rank) {
	assert(0 <= rank && rank < 8);
	return static_cast<char>(rank + '1');
}

/// `file` must be between 0 and 7.
inline char convert_file_to_letter(int file) {
	assert(0 <= file && file < 8);
	return static_cast<char>(file + 'a');
}

struct Square {
	char get_rank_digit() const { return convert_rank_to_digit(rank); }
	char get_file_letter() const { return convert_file_to_letter(file); }
	std::string print() const { return std::string{ get_file_letter(), get_rank_digit() }; }

	bool operator==(const Square& other) const { return rank == other.rank && file == other.file; }
	bool operator!=(const Square& other) const { return rank != other.rank || file != other.file; }

	static Square from_chars(char file, char rank) {
		return Square{ rank - '1', safe_to_lower(file) - 'a' };
	}

	/// Parse a square string in the form 'xN', such as 'd2'.
	/// The string must be exactly two characters long.
	static std::optional<Square> parse(std::string_view string) {
		if (string.length() != 2)
			return {};

		// The file must be a letter from 'A' to 'H'.
		const char file{ safe_to_lower(string[0]) };
		if (file < 'a' || 'h' < file)
			return {};

		// The rank must be a number from 1 to 8.
		const char rank{ string[1] };
		if (rank < '1' || '8' < rank)
			return {};

		return from_chars(file, rank);
	}

	int rank{ -1 };
	int file{ -1 };
};

#endif
