// Author: Daniel Kareh
// Summary: The Piece class is used to describe each piece and generate legal
//          moves. Several more data types are defined here to describe moves.

#include "Piece.h"
#include <algorithm> // For std::clamp.
#include <cmath> // For std::abs.
#include <stdexcept> // For std::invalid_argument.
#include "Board.h"
#include "safe_ctype.h"
#include "throw_if_empty.h"

char convert_piece_type_to_letter(piece_type type) {
	switch (type) {
	case piece_type::pawn:
		return 'P';
	case piece_type::knight:
		return 'N';
	case piece_type::bishop:
		return 'B';
	case piece_type::rook:
	case piece_type::castleable_rook:
		return 'R';
	case piece_type::queen:
		return 'Q';
	case piece_type::king:
	case piece_type::castleable_king:
		return 'K';
	default:
		throw std::invalid_argument{ "Invalid piece type" };
	}
}

std::optional<piece_type> convert_letter_to_piece_type(char ch) {
	switch (safe_to_upper(ch)) {
	case 'P':
		return piece_type::pawn;
	case 'N':
		return piece_type::knight;
	case 'B':
		return piece_type::bishop;
	case 'R':
		return piece_type::rook;
	case 'Q':
		return piece_type::queen;
	case 'K':
		return piece_type::king;
	default:
		return {};
	}
}

std::string get_piece_name(piece_type type) {
	switch (type) {
	case piece_type::pawn:
		return "pawn";
	case piece_type::knight:
		return "knight";
	case piece_type::bishop:
		return "bishop";
	case piece_type::rook:
	case piece_type::castleable_rook:
		return "rook";
	case piece_type::queen:
		return "queen";
	case piece_type::king:
	case piece_type::castleable_king:
		return "king";
	default:
		throw std::invalid_argument{ "Invalid piece type" };
	}
}

color get_opposing_color(color color) {
	return color == color::white ? color::black : color::white;
}

void Piece::make_uncastleable() {
	if (type == piece_type::castleable_rook)
		type = piece_type::rook;
	else if (type == piece_type::castleable_king)
		type = piece_type::king;
}

static std::vector<MoveDetails> generate_hopping(Move move, const Board& board) {
	auto piece{ board.get_piece(move.to) };
	// "Hopping" pieces cannot move onto another piece of the same color.
	// Note that this includes the moving piece itself. If `move.from` equals
	// `move.to`, this function returns NO legal moves.
	const bool is_legal{ !piece || piece->color != move.active_color };
	if (is_legal) {
		// NOTE: If there is no piece at `move.to`, return `std::nullopt`, not
		// a defaulted square, as in, not `Square{ -1, -1 }`.
		auto captured_square{ piece ? std::optional{ move.to } : std::nullopt };
		return { MoveDetails{ captured_square } };
	}
	return {};
}

static std::vector<MoveDetails> generate_sliding(Move move, const Board& board) {
	const int rank_step{ std::clamp(move.to.rank - move.from.rank, -1, 1) };
	const int file_step{ std::clamp(move.to.file - move.from.file, -1, 1) };
	Square current{ move.from };
	for (;;) {
		current.rank += rank_step;
		current.file += file_step;
		if (board.is_out_of_bounds(current))
			return {};

		if (current == move.to)
			return generate_hopping(move, board);

		// Sliding pieces cannot move through another piece.
		if (board.is_occupied(current))
			return {};
	}
}

static const std::array can_promote_to{
	piece_type::knight,
	piece_type::bishop,
	piece_type::rook,
	piece_type::queen,
};

// Take in a base move and return every possible combination of that
// move and a legal promotion.
static std::vector<MoveDetails> add_promotions(MoveDetails details, bool is_promotion) {
	if (is_promotion) {
		std::vector<MoveDetails> all_promotions;
		for (auto promote_to : can_promote_to) {
			details.promote_to = promote_to;
			all_promotions.push_back(details);
		}
		return all_promotions;
	}
	return { details };
}

static std::vector<MoveDetails> generate_pawn_move_details(Move move, const Board& board) {
	const int direction{ move.active_color == color::black ? -1 : 1 };
	const int initial_rank{ move.active_color == color::black ? 6 : 1 };
	const bool on_initial_rank{ move.from.rank == initial_rank };
	const int promotion_rank{ move.active_color == color::black ? 0 : 7 };
	const bool is_promotion{ move.to.rank == promotion_rank };

	const auto rank_change{ move.to.rank - move.from.rank };
	const auto file_change{ move.to.file - move.from.file };

	// Handle two step advances.
	if (on_initial_rank && rank_change == direction * 2 && file_change == 0) {
		const Square passing_over{ move.from.rank + direction, move.from.file };
		// Pawns cannot skip over another piece.
		// They also can't capture pieces when not moving diagonally.
		const bool is_legal{ !board.is_occupied(passing_over) && !board.is_occupied(move.to) };
		if (!is_legal)
			return {};

		auto en_passant_target{ passing_over };
		return { MoveDetails{ std::nullopt, en_passant_target } };
	}

	if (rank_change != direction)
		return {};

	// Handle standard advances.
	if (file_change == 0) {
		// Pawns can't capture pieces when not moving diagonally.
		const bool is_legal{ !board.is_occupied(move.to) };
		if (!is_legal)
			return {};
		return add_promotions(MoveDetails{}, is_promotion);
	}

	if (file_change != -1 && file_change != 1)
		return {};

	// Handle diagonal captures.
	if (auto piece{ board.get_piece(move.to) }; piece && piece->color != move.active_color) {
		return add_promotions(MoveDetails{ move.to }, is_promotion);
	}

	if (move.to == board.get_en_passant_target()) {
		// En passant is a special capture.
		Square captured_square{ move.from.rank, move.to.file };
		return { MoveDetails{ captured_square } };
	}

	return {};
}

static std::vector<MoveDetails> generate_knight_move_details(Move move, const Board& board) {
	const auto abs_rank_change{ std::abs(move.to.rank - move.from.rank) };
	const auto abs_file_change{ std::abs(move.to.file - move.from.file) };
	const int distance{ abs_rank_change + abs_file_change };
	if (distance == 3 && abs_rank_change != 0 && abs_file_change != 0)
		return generate_hopping(move, board);
	return {};
}

static std::vector<MoveDetails> generate_bishop_move_details(Move move, const Board& board) {
	const auto rank_change{ move.to.rank - move.from.rank };
	const auto file_change{ move.to.file - move.from.file };
	// Bishops only move diagonally.
	if (rank_change == file_change || rank_change == -file_change)
		return generate_sliding(move, board);
	return {};
}

static std::vector<MoveDetails> generate_rook_move_details(Move move, const Board& board) {
	const auto rank_change{ move.to.rank - move.from.rank };
	const auto file_change{ move.to.file - move.from.file };
	// Rooks only move horizontally or vertically.
	if (rank_change == 0 || file_change == 0)
		return generate_sliding(move, board);
	return {};
}

static std::vector<MoveDetails> generate_queen_move_details(Move move, const Board& board) {
	const auto rank_change{ move.to.rank - move.from.rank };
	const auto file_change{ move.to.file - move.from.file };
	// Queens can move in all eight directions.
	if (rank_change == file_change || rank_change == -file_change)
		return generate_sliding(move, board);
	if (rank_change == 0 || file_change == 0)
		return generate_sliding(move, board);
	return {};
}

template <typename T>
static void append(std::vector<T>& dest, const std::vector<T>& source) {
	dest.insert(dest.end(), source.begin(), source.end());
}

static Square get_castling_king_final(color color, side side) {
	const char file{ side == side::a_side ? 'c' : 'g' };
	const char rank{ color == color::black ? '8' : '1' };
	return Square::from_chars(file, rank);
}

static Square get_castling_rook_final(color color, side side) {
	const char file{ side == side::a_side ? 'd' : 'f' };
	const char rank{ color == color::black ? '8' : '1' };
	return Square::from_chars(file, rank);
}

static std::optional<Square> find_castling_rook(Move move, int step, const Board& board) {
	for (Square current{ move.from }; board.is_in_bounds(current); current.file += step) {
		const auto piece{ board.get_piece(current) };
		if (!piece.has_value())
			continue;

		if (piece->type == piece_type::castleable_rook && piece->color == move.active_color)
			return current;
	}
	return std::nullopt;
}

// Is any square between `from` and `to` (not including `from` or `ignore`) occupied by a piece?
static bool any_squares_are_occupied(Square from, Square to, Square ignore, const Board& board) {
	const int file_direction{ to.file < from.file ? -1 : 1 };
	for (int step{ 1 }; step <= std::abs(to.file - from.file); step++) {
		const Square current{ from.rank, from.file + file_direction * step };
		if (current == ignore)
			continue;

		if (board.is_occupied(current))
			return true;
	}
	return false;
}

// Is any square between `move.from` and `move.to` under attack by one of
// the opponent's pieces?
static bool any_squares_are_under_attack(Move move, const Board& board) {
	const int file_direction{ move.to.file < move.from.file ? -1 : 1 };
	for (int step{ 0 }; step <= std::abs(move.to.file - move.from.file); step++) {
		const Square current{ move.from.rank, move.from.file + file_direction * step };
		if (board.piece_would_be_attacked(move.from, current))
			return true;
	}
	return false;
}

static std::vector<MoveDetails> generate_castling(Move move, side side, const Board& board) {
	// The king doesn't move between ranks when castling.
	const color color{ move.active_color };
	if (move.from.rank != move.to.rank)
		return {};

	// When castling, the king always goes to the same square.
	// This is true in classical chess and in variants such as Chess960.
	if (move.to != get_castling_king_final(color, side))
		return {};

	// Find the castling rook.
	const int search_direction{ side == side::a_side ? -1 : 1 };
	const auto maybe_rook{ find_castling_rook(move, search_direction, board) };
	if (!maybe_rook.has_value())
		return {};

	const Square rook{ maybe_rook.value() };
	const auto rook_final{ get_castling_rook_final(color, side) };

	// All the squares that the king crosses over must be empty (ignoring the rook).
	if (any_squares_are_occupied(move.from, move.to, rook, board))
		return {};

	// All the squares that the rook crosses over must be empty (ignoring the king).
	if (any_squares_are_occupied(rook, rook_final, move.from, board))
		return {};

	// None of the squares that the king crosses over can be under attack.
	if (any_squares_are_under_attack(move, board))
		return {};

	MoveDetails details;
	details.castling = CastlingDetails{ rook, rook_final, side };
	return { details };
}

static std::vector<MoveDetails> generate_king_move_details(Move move, const Board& board) {
	std::vector<MoveDetails> details;

	// The king can only move to one of the eight adjacent squares.
	const auto abs_rank_change{ std::abs(move.to.rank - move.from.rank) };
	const auto abs_file_change{ std::abs(move.to.file - move.from.file) };
	if (abs_rank_change <= 1 && abs_file_change <= 1)
		append(details, generate_hopping(move, board));

	const Piece king{ throw_if_empty(board.get_piece(move.from)) };
	if (king.type == piece_type::castleable_king) {
		// Make sure to account for both types of castling!
		append(details, generate_castling(move, side::a_side, board));
		append(details, generate_castling(move, side::h_side, board));
	}

	return details;
}

std::vector<MoveDetails> generate_move_details(Move move, const Board& board) {
	if (board.is_out_of_bounds(move.from) || board.is_out_of_bounds(move.to))
		return {};

	// Players can only move the pieces they own.
	const auto piece{ board.get_piece(move.from) };
	if (!piece || piece->color != move.active_color)
		return {};

	switch (piece->type) {
	case piece_type::pawn:
		return generate_pawn_move_details(move, board);
	case piece_type::knight:
		return generate_knight_move_details(move, board);
	case piece_type::bishop:
		return generate_bishop_move_details(move, board);
	case piece_type::rook:
	case piece_type::castleable_rook:
		return generate_rook_move_details(move, board);
	case piece_type::queen:
		return generate_queen_move_details(move, board);
	case piece_type::king:
	case piece_type::castleable_king:
		return generate_king_move_details(move, board);
	default:
		return {};
	}
}
