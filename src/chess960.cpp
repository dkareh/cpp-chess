// Author: Daniel Kareh
// Summary: Some utility functions for the chess variant, Chess960.
//          See https://en.wikipedia.org/wiki/Fischer_random_chess_numbering_scheme
//          for more details.

#include "chess960.h"
#include <random> // For std::random_device, std::mt19937.
#include "throw_if_empty.h"

template <typename T>
static T div_rem(T& dividend, T divisor);
static std::optional<Piece>& nth_free_square(Board::Rank& rank, int n);

struct KnightPositions {
	int one;
	int two;
};

static std::array<KnightPositions, 10> knights_positioning{ {
	{ 0, 1 }, // NN___
	{ 0, 2 }, // N_N__
	{ 0, 3 }, // N__N_
	{ 0, 4 }, // N___N
	{ 1, 2 }, // _NN__
	{ 1, 3 }, // _N_N_
	{ 1, 4 }, // _N__N
	{ 2, 3 }, // __NN_
	{ 2, 4 }, // __N_N
	{ 3, 4 }, // ___NN
} };

Board generate_chess960_board() {
	// Seed a Mersenne Twister PRNG using the OS random device.
	// This is not the best way, but it's good enough for me.
	static std::mt19937 prng{ std::random_device{}() };

	// Select one of the 960 valid arrangements.
	std::uniform_int_distribution fischer_scheme{ 0, 960 - 1 };
	int index{ fischer_scheme(prng) };

	// Determine where the bishops and queen will go.
	const int bishop1_position{ div_rem(index, 4) };
	const int bishop2_position{ div_rem(index, 4) };
	const int queen_position{ div_rem(index, 6) };

	// `index` is now between 0 and 9 (inclusive).
	// Start filling out the home rank by placing the bishops and queen.
	Board::Rank white_home_rank{};
	white_home_rank[bishop1_position * 2 + 1] = { piece_type::bishop, color::white };
	white_home_rank[bishop2_position * 2 + 0] = { piece_type::bishop, color::white };
	nth_free_square(white_home_rank, queen_position) = { piece_type::queen, color::white };

	// Place the knights according to the positioning table.
	auto [knight1_position, knight2_position]{ knights_positioning.at(index) };

	// Get both references simultaneously,
	auto& knight1{ nth_free_square(white_home_rank, knight1_position) };
	auto& knight2{ nth_free_square(white_home_rank, knight2_position) };

	// then insert the knights.
	knight1 = { piece_type::knight, color::white };
	knight2 = { piece_type::knight, color::white };

	// Place two rooks in the outer squares and a king in the middle square.
	// Note that we're mutating the home rank and calling `nth_free_square`
	// each time, so we're always getting a new free square, not the same one.
	nth_free_square(white_home_rank, 0) = { piece_type::castleable_rook, color::white };
	nth_free_square(white_home_rank, 0) = { piece_type::castleable_king, color::white };
	nth_free_square(white_home_rank, 0) = { piece_type::castleable_rook, color::white };

	// The black home rank is the same except ... the pieces are black!
	Board::Rank black_home_rank{ white_home_rank };
	for (auto& piece : black_home_rank)
		throw_if_empty(piece).color = color::black;

	// Finally fill out the entire board. The pawns start in the same squares
	// that they normally do.
	std::array<Board::Rank, 8> ranks{};
	ranks[0] = white_home_rank;
	ranks[1].fill(Piece{ piece_type::pawn, color::white });
	ranks[6].fill(Piece{ piece_type::pawn, color::black });
	ranks[7] = black_home_rank;
	return Board{ ranks };
}

template <typename T>
static T div_rem(T& dividend, T divisor) {
	T remainder{ dividend % divisor };
	dividend /= divisor;
	return remainder;
}

static std::optional<Piece>& nth_free_square(Board::Rank& rank, int n) {
	std::size_t file{ 0 };
	for (; n > 0 || rank.at(file).has_value(); file++) {
		if (!rank.at(file).has_value())
			n--;
	}
	return rank.at(file);
}
