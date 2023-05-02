// Author: Daniel Kareh
// Summary: A class and several helper functions to describe and manipulate
//          squares. A square is one location on a chess board.

#ifndef CHESS_SQUARE_H
#define CHESS_SQUARE_H

#include <safe_ctype.h>
#include <string>

inline char convert_rank_to_digit(int rank) { return rank + '1'; }
inline char convert_file_to_letter(int file) { return file + 'a'; }

struct Square {
	char get_rank_digit() const { return convert_rank_to_digit(rank); }
	char get_file_letter() const { return convert_file_to_letter(file); }
	std::string print() const { return std::string{ get_file_letter(), get_rank_digit() }; }

	bool operator==(const Square& other) const { return rank == other.rank && file == other.file; }
	bool operator!=(const Square& other) const { return rank != other.rank || file != other.file; }

	static Square from_chars(char file, char rank) {
		return Square{ rank - '1', safe_to_lower(file) - 'a' };
	}

	int rank{ -1 };
	int file{ -1 };
};

#endif
