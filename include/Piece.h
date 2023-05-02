#ifndef CHESS_PIECE_H
#define CHESS_PIECE_H

#include <Square.h>
#include <optional>
#include <vector>

// NOTE: Directly insert a forward reference to `Board` instead of including
// <Board.h> to avoid a cycle of "#include"s.
class Board;

struct Move;
struct MoveDetails;

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
	castleable_rook,
	castleable_king,
};

enum class side {
	a_side, // Queenside castling in classical chess
	h_side, // Kingside castling in classical chess
};

struct CastlingDetails {
	Square secondary_from;
	Square secondary_to;
	side side;
};

char convert_piece_type_to_letter(piece_type);
std::optional<piece_type> convert_letter_to_piece_type(char);
std::string get_piece_name(piece_type);
color get_opposing_color(color color);

class Piece {
public:
	bool is_black() const { return color == color::black; }
	bool is_white() const { return color == color::white; }
	static std::vector<MoveDetails> generate_move_details(Move, const Board&);

	bool is_rook() const { return type == piece_type::rook || type == piece_type::castleable_rook; }
	bool is_king() const { return type == piece_type::king || type == piece_type::castleable_king; }

	piece_type type;
	color color;

private:
	static std::vector<MoveDetails> generate_hopping(Move, const Board&);
	static std::vector<MoveDetails> generate_sliding(Move, const Board&);

	static std::vector<MoveDetails> generate_pawn_move_details(Move, const Board&);
	static std::vector<MoveDetails> generate_knight_move_details(Move, const Board&);
	static std::vector<MoveDetails> generate_bishop_move_details(Move, const Board&);
	static std::vector<MoveDetails> generate_rook_move_details(Move, const Board&);
	static std::vector<MoveDetails> generate_queen_move_details(Move, const Board&);
	static std::vector<MoveDetails> generate_king_move_details(Move, const Board&);
};

struct Move {
	color active_color;
	Square from, to;
};

struct MoveDetails {
	std::optional<Square> captured_square{};
	std::optional<Square> en_passant_target{};
	std::optional<piece_type> promote_to{};
	std::optional<CastlingDetails> castling{};
};

#endif
