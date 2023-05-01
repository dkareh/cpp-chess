#include <Board.h>
#include <Piece.h>
#include <algorithm> // For std::clamp.
#include <cmath> // For std::abs.
#include <safe_ctype.h>
#include <stdexcept> // For std::invalid_argument.

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

MoveDetails Piece::get_move_details(Move move, const Board& board) {
	if (!board.is_in_bounds(move.from) || !board.is_in_bounds(move.to))
		return {};

	// Players can only move the pieces they own.
	auto piece{ board.get_piece(move.from) };
	if (!piece || piece->color != move.active_color)
		return {};

	// Only pawns can be promoted.
	if (piece->type != piece_type::pawn && move.promote_to)
		return {};

	switch (piece->type) {
	case piece_type::pawn:
		return get_pawn_move_details(move, board);
	case piece_type::knight:
		return get_knight_move_details(move, board);
	case piece_type::bishop:
		return get_bishop_move_details(move, board);
	case piece_type::rook:
	case piece_type::castleable_rook:
	case piece_type::queen:
		return get_queen_move_details(move, board);
	case piece_type::king:
	case piece_type::castleable_king:
	default:
		return {};
	}
}

MoveDetails Piece::check_hopping(Move move, const Board& board) {
	auto piece{ board.get_piece(move.to) };
	bool is_legal{ !piece || piece->color != move.active_color };
	return { is_legal, piece ? move.to : Square{} };
}

MoveDetails Piece::check_sliding(Move move, const Board& board) {
	int rank_step{ std::clamp(move.to.rank - move.from.rank, -1, 1) };
	int file_step{ std::clamp(move.to.file - move.from.file, -1, 1) };
	Square current{ move.from };
	for (;;) {
		current.rank += rank_step;
		current.file += file_step;
		if (board.is_out_of_bounds(current))
			return {};

		if (current == move.to)
			return check_hopping(move, board);

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

MoveDetails Piece::get_pawn_move_details(Move move, const Board& board) {
	const int direction{ move.active_color == color::black ? -1 : 1 };
	const int initial_rank{ move.active_color == color::black ? 6 : 1 };
	const bool on_initial_rank{ move.from.rank == initial_rank };
	const int promotion_rank{ move.active_color == color::black ? 0 : 7 };
	const bool is_promotion{ move.to.rank == promotion_rank };

	if (!is_promotion && move.promote_to.has_value())
		return {};

	if (move.promote_to) {
		// Check that the pawn is promoting to one of the allowed types.
		auto it{ std::find(can_promote_to.begin(), can_promote_to.end(), move.promote_to.value()) };
		if (it == can_promote_to.end())
			return {}; // If not, the move is illegal.
	}

	auto rank_change{ move.to.rank - move.from.rank };
	auto file_change{ move.to.file - move.from.file };

	// Handle two step advances.
	if (on_initial_rank && rank_change == direction * 2 && file_change == 0) {
		const Square passing_over{ move.from.rank + direction, move.from.file };
		MoveDetails details;
		details.is_legal = !board.is_occupied(passing_over) && !board.is_occupied(move.to);
		details.en_passant_target = passing_over;
		return details;
	}

	if (rank_change != direction)
		return {};

	// Handle standard advances.
	if (file_change == 0) {
		MoveDetails details{ !board.is_occupied(move.to) };
		details.is_promotion = details.is_legal && is_promotion;
		return details;
	}

	if (file_change != -1 && file_change != 1)
		return {};

	// Handle diagonal captures.
	MoveDetails details{};
	if (auto piece{ board.get_piece(move.to) }; piece && piece->color != move.active_color) {
		details.is_legal = true;
		details.captured_square = move.to;
		details.is_promotion = details.is_legal && is_promotion;
	} else if (move.to == board.get_en_passant_target()) {
		// En passant is a special capture.
		details.is_legal = true;
		details.captured_square = { move.from.rank, move.to.file };
	}
	return details;
}

MoveDetails Piece::get_knight_move_details(Move move, const Board& board) {
	auto abs_rank_change{ std::abs(move.to.rank - move.from.rank) };
	auto abs_file_change{ std::abs(move.to.file - move.from.file) };
	int distance{ abs_rank_change + abs_file_change };
	if (distance == 3 && abs_rank_change != 0 && abs_file_change != 0)
		return check_hopping(move, board);
	return {};
}

MoveDetails Piece::get_bishop_move_details(Move move, const Board& board) {
	auto rank_change{ move.to.rank - move.from.rank };
	auto file_change{ move.to.file - move.from.file };
	if (rank_change == file_change || rank_change == -file_change)
		return check_sliding(move, board);
	return {};
}

MoveDetails Piece::get_rook_move_details(Move move, const Board& board) {
	auto rank_change{ move.to.rank - move.from.rank };
	auto file_change{ move.to.file - move.from.file };
	if (rank_change == 0 || file_change == 0)
		return check_sliding(move, board);
	return {};
}

MoveDetails Piece::get_queen_move_details(Move move, const Board& board) {
	auto rank_change{ move.to.rank - move.from.rank };
	auto file_change{ move.to.file - move.from.file };
	if (rank_change == file_change || rank_change == -file_change)
		return check_sliding(move, board);
	if (rank_change == 0 || file_change == 0)
		return check_sliding(move, board);
	return {};
}

MoveDetails Piece::get_king_move_details(Move move, const Board& board) {
	auto abs_rank_change{ std::abs(move.to.rank - move.from.rank) };
	auto abs_file_change{ std::abs(move.to.file - move.from.file) };
	if (abs_rank_change <= 1 && abs_file_change <= 1)
		return check_hopping(move, board);
	return {};
}
