// Author: Daniel Kareh
// Summary: The Piece class is used to describe each piece and generate legal
//          moves. Several more data types are defined here to describe moves.

#ifndef CHESS_PIECE_H
#define CHESS_PIECE_H

#include <Square.h>
#include <optional>
#include <vector>

// NOTE: Directly insert a forward reference to `Board` instead of including
// <Board.h> to avoid a cycle of "#include"s.
class Board;

enum class color : unsigned char {
	black,
	white,
};

enum class piece_type : unsigned char {
	pawn,
	knight,
	bishop,
	rook,
	queen,
	king,
	// Castleable pieces are pieces that can participate in castling.
	// For example, a rook that has never been moved is castleable.
	castleable_rook,
	castleable_king,
};

enum class side {
	a_side, // Queenside castling in classical chess
	h_side, // Kingside castling in classical chess
};

// If the move involved castling, this structure tells the board about the
// secondary piece. In classical chess, the secondary piece is always a rook.
struct CastlingDetails {
	Square secondary_from;
	Square secondary_to;
	side side;
};

struct Move {
	// Who is moving a piece?
	color active_color;
	Square from, to;
};

struct MoveDetails {
	std::optional<Square> captured_square{};
	// In classical chess, the en passant target is the square that a pawn
	// skips over when it advances by two ranks.
	std::optional<Square> en_passant_target{};
	std::optional<piece_type> promote_to{};
	std::optional<CastlingDetails> castling{};
};

char convert_piece_type_to_letter(piece_type);
std::optional<piece_type> convert_letter_to_piece_type(char);
std::string get_piece_name(piece_type);
color get_opposing_color(color color);

struct Piece {
	bool is_black() const { return color == color::black; }
	bool is_white() const { return color == color::white; }

	bool is_rook() const { return type == piece_type::rook || type == piece_type::castleable_rook; }
	bool is_king() const { return type == piece_type::king || type == piece_type::castleable_king; }

	void make_uncastleable();

	piece_type type;
	color color;
};

std::vector<MoveDetails> generate_move_details(Move, const Board&);

#endif
