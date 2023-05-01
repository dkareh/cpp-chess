#include <Board.h>

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
	return square.rank >= 0 && square.rank < 8 && square.file >= 0 && square.file < 8;
}

std::optional<Piece> Board::get_piece(Square square) const {
	return ranks.at(square.rank).at(square.file);
}

MoveDetails Board::move(Move move) {
	auto details{ Piece::get_move_details(move, *this) };
	if (!details.is_legal)
		return details;

	Board copy{ *this };
	copy.force_move(move, details);

	// Don't actually move the piece if it would put the king in check.
	if (copy.is_piece_under_attack(copy.find_king(move.active_color)))
		details.is_legal = false;
	else
		force_move(move, details);

	return details;
}

bool Board::is_piece_under_attack(Square square) const {
	color color{ get_piece(square)->color };
	for (auto from : *this) {
		auto details{ Piece::get_move_details({ get_opposing_color(color), from, square }, *this) };

		// Check if the piece was legally captured.
		if (details.is_legal && details.captured_square && details.captured_square == square)
			return true;
	}
	return false;
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
	if (details.captured_square) {
		auto captured{ details.captured_square.value() };
		at(captured) = std::nullopt;
	}

	move_one_piece(move.from, move.to);

	if (details.promote_to)
		at(move.to)->type = details.promote_to.value();

	en_passant_target = details.en_passant_target.value_or(Square{});

	// Castleable pieces are no longer castleable once they move.
	auto& piece{ at(move.to).value() };
	if (piece.type == piece_type::castleable_rook)
		piece.type = piece_type::rook;
	else if (piece.type == piece_type::castleable_king)
		piece.type = piece_type::king;
}

void Board::move_one_piece(Square from, Square to) {
	if (from != to) {
		at(to) = at(from);
		at(from) = std::nullopt;
	}
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
	Iterator copy = *this;
	++(*this);
	return copy;
}

bool Board::Iterator::operator==(const Board::Iterator& other) const {
	return current == other.current;
}

bool Board::Iterator::operator!=(const Board::Iterator& other) const {
	return current != other.current;
}
