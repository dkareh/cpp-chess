#ifndef CHESS_PIECE_H
#define CHESS_PIECE_H

#include <Square.h>
#include <optional>

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
};

char convert_piece_type_to_letter(piece_type);
std::optional<piece_type> convert_letter_to_piece_type(char);
color get_opposing_color(color color);

class Piece {
public:
	bool is_black() const { return color == color::black; }
	bool is_white() const { return color == color::white; }
	static MoveDetails get_move_details(Move, const Board&);

	piece_type type;
	color color;

private:
	static MoveDetails check_hopping(Move, const Board&);
	static MoveDetails check_sliding(Move, const Board&);

	static MoveDetails get_pawn_move_details(Move, const Board&);
	static MoveDetails get_knight_move_details(Move, const Board&);
	static MoveDetails get_bishop_move_details(Move, const Board&);
	static MoveDetails get_rook_move_details(Move, const Board&);
	static MoveDetails get_queen_move_details(Move, const Board&);
	static MoveDetails get_king_move_details(Move, const Board&);
};

struct Move {
	color active_color;
	Square from, to;
	std::optional<piece_type> promote_to{};
};

struct MoveDetails {
	bool is_legal{ false };
	std::optional<Square> captured_square{};
	std::optional<Square> en_passant_target{};
	bool is_promotion{ false };
};

#endif
