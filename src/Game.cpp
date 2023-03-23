#include <Game.h>
#include <iostream>
#include <safe_ctype.h>

Game::Game(Board board, std::unique_ptr<Display> display, color active_color)
	: board{ board }
	, display{ std::move(display) }
	, active_color{ active_color } {}

static std::optional<Move> parse_move(std::string input, color active_color) {
	if (input.size() != 4)
		return {};

	char from_file{ safe_to_lower(input[0]) };
	if (from_file < 'a' || from_file > 'h')
		return {};

	char from_rank{ input[1] };
	if (from_rank < '1' || from_rank > '8')
		return {};

	char to_file{ safe_to_lower(input[2]) };
	if (to_file < 'a' || to_file > 'h')
		return {};

	char to_rank{ input[3] };
	if (to_rank < '1' || to_rank > '8')
		return {};

	Square from{ from_rank - '1', from_file - 'a' };
	Square to{ to_rank - '1', to_file - 'a' };
	return Move{ active_color, from, to };
}

static Move read_move(color active_color) {
	for (;;) {
		std::cout << "Your move? ";
		std::string input;
		if (!std::getline(std::cin >> std::ws, input)) {
			std::cin.clear();
			continue;
		}

		auto move{ parse_move(input, active_color) };
		if (move)
			return move.value();
		else
			std::cout << "Please try again.\n";
	}
}

static piece_type read_promotion_type() {
	std::cout << "Enter the letter that corresponds to the piece type, "
	             "such as 'Q' for queen or 'N' for knight.\n";

	for (;;) {
		std::cout << "Promote to? ";
		std::string input;
		if (!std::getline(std::cin >> std::ws, input)) {
			std::cin.clear();
			continue;
		}

		if (input.size() != 1)
			continue;

		auto type{ convert_letter_to_piece_type(input[0]) };
		if (type)
			return type.value();
		else
			std::cout << "Please try again.\n";
	}
}

void Game::run() {
	for (;;) {
		display->show(board);

		auto active_name{ active_color == color::black ? "Black" : "White" };
		std::cout << '\n' << active_name << ": ";

		if (board.is_piece_under_attack(board.find_king(active_color)))
			std::cout << "Your king is in check.\n";

		auto move{ read_move(active_color) };
		auto details{ board.move(move) };
		if (!details.is_legal && details.is_promotion) {
			// Ask the user what they want to promote their piece to
			// only if the move appears to be a promotion.
			move.promote_to = read_promotion_type();
			details = board.move(move);
		}

		if (!details.is_legal) {
			std::cout << "Illegal move. Press enter to continue.\n";
			std::cin.get();
			continue;
		}

		active_color = get_opposing_color(active_color);
	}
}
