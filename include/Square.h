#ifndef CHESS_SQUARE_H
#define CHESS_SQUARE_H

inline char convert_rank_to_digit(int rank) { return rank + '1'; }

inline char convert_file_to_letter(int file) { return file + 'a'; }

struct Square {
	char get_rank_digit() const { return convert_rank_to_digit(rank); }
	char get_file_letter() const { return convert_file_to_letter(file); }

	bool operator==(const Square& other) const { return rank == other.rank && file == other.file; }

	bool operator!=(const Square& other) const { return rank != other.rank || file != other.file; }

	int rank{ -1 };
	int file{ -1 };
};

#endif
