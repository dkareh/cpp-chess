// Author: Daniel Kareh
// Summary: A class that represents the entire state of a chess board *except*
//          who moves next (that is stored by the game object).

#include "Board.h"
#include "throw_if_empty.h"

static Board::Rank get_home_rank(color color) {
	Board::Rank rank;
	rank[0] = { piece_type::castleable_rook, color };
	rank[1] = { piece_type::knight, color };
	rank[2] = { piece_type::bishop, color };
	rank[3] = { piece_type::queen, color };
	rank[4] = { piece_type::castleable_king, color };
	rank[5] = { piece_type::bishop, color };
	rank[6] = { piece_type::knight, color };
	rank[7] = { piece_type::castleable_rook, color };
	return rank;
}

static std::array<Board::Rank, 8> get_default_board() {
	std::array<Board::Rank, 8> ranks;
	ranks[0] = get_home_rank(color::white);
	ranks[1].fill(Piece{ piece_type::pawn, color::white });
	ranks[6].fill(Piece{ piece_type::pawn, color::black });
	ranks[7] = get_home_rank(color::black);
	return ranks;
}

Board::Board()
	: Board{ get_default_board() } {}

Board::Board(std::array<Rank, 8> ranks)
	: ranks{ ranks } {}

bool Board::is_in_bounds(Square square) const {
	return 0 <= square.rank && square.rank < 8 && 0 <= square.file && square.file < 8;
}

std::optional<Piece> Board::get_piece(Square square) const { return at(square); }

std::optional<MoveDetails> Board::move(Move move, const ChooseMoveCallback& choose_move) {
	auto details{ get_legal_moves(move) };

	// Which move should we actually apply?
	const int choice{ choose_move(details) };
	if (choice < 0 || details.size() <= static_cast<std::size_t>(choice))
		return std::nullopt;

	force_move(move, details[choice]);
	return details[choice];
}

std::vector<MoveDetails> Board::get_legal_moves(Move move) const {
	auto details{ generate_move_details(move, *this) };

	// Ignore pseudo-legal moves that would put the king in check.
	for (auto it{ details.begin() }; it != details.end();) {
		Board copy{ *this };
		copy.force_move(move, *it);
		if (copy.piece_is_under_attack(copy.find_king(move.active_color)))
			it = details.erase(it);
		else
			++it;
	}

	return details;
}

bool Board::piece_is_under_attack(Square square) const {
	const color color{ throw_if_empty(get_piece(square)).color };
	// Loop over every square.
	for (auto from : *this) {
		const Move move{ get_opposing_color(color), from, square };
		const auto details{ generate_move_details(move, *this) };

		// Check if the piece was legally captured.
		for (const auto& legal_move : details) {
			if (legal_move.captured_square && legal_move.captured_square == square)
				return true;
		}
	}
	return false;
}

bool Board::piece_would_be_attacked(Square from, Square to) const {
	Board copy{ *this };
	copy.move_one_piece(from, to);
	return copy.piece_is_under_attack(to);
}

Square Board::find_king(color color) const {
	for (auto square : *this) {
		auto maybe_king{ get_piece(square) };
		if (!maybe_king)
			continue;

		if (maybe_king->is_king() && maybe_king->color == color)
			return square;
	}

	return {};
}

void Board::force_move(Move move, MoveDetails details) {
	// Save the secondary piece before we start moving pieces around
	// (that is, if there is a secondary piece).
	const auto castling{ details.castling };
	const Piece secondary_piece{ castling ? pick_up(castling->secondary_from) : Piece{} };

	// Remove any captured pieces.
	if (details.captured_square) {
		auto captured{ details.captured_square.value() };
		at(captured) = std::nullopt;
	}

	// Move the primary piece.
	auto& primary{ move_one_piece(move.from, move.to) };

	// Promote it if necessary.
	if (details.promote_to)
		primary.type = details.promote_to.value();

	// Update the en passant target.
	// Default to `Square{ -1, -1 }` if there is none.
	en_passant_target = details.en_passant_target.value_or(Square{});

	// If castling occurred, move the secondary piece.
	if (castling) {
		put_down(castling->secondary_to, secondary_piece);
	}
}

Piece& Board::move_one_piece(Square from, Square to) {
	auto piece{ pick_up(from) };
	return put_down(to, piece);
}

Piece Board::pick_up(Square square) {
	const Piece piece{ throw_if_empty(get_piece(square)) };
	at(square) = std::nullopt;
	return piece;
}

Piece& Board::put_down(Square square, Piece piece) {
	// Castleable pieces are no longer castleable once they move.
	piece.make_uncastleable();
	return at(square).emplace(piece);
}

std::optional<Piece>& Board::operator[](Square square) { return at(square); }
const std::optional<Piece>& Board::operator[](Square square) const { return at(square); }
std::optional<Piece>& Board::at(Square square) { return ranks.at(square.rank).at(square.file); }

const std::optional<Piece>& Board::at(Square square) const {
	return ranks.at(square.rank).at(square.file);
}

Board::Iterator& Board::Iterator::operator++() {
	current.file++;
	if (current.file == 8) {
		current.file = 0;
		current.rank--;
	}
	return *this;
}

// NOTE: The post-increment operator must have a dummy "int" parameter to
// differentiate it from the pre-increment operator.
Board::Iterator Board::Iterator::operator++(int) {
	Iterator copy{ *this };
	++(*this);
	return copy;
}

bool Board::Iterator::operator==(const Board::Iterator& other) const {
	return current == other.current;
}

bool Board::Iterator::operator!=(const Board::Iterator& other) const {
	return current != other.current;
}
