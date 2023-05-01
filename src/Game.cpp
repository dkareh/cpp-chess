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
		std::cout << "Your move? ____\b\b\b\b";
		std::string input;
		if (!std::getline(std::cin >> std::ws, input)) {
			std::cin.clear();
			continue;
		}

		auto move{ parse_move(input, active_color) };
		if (move)
			return move.value();

		std::cout << "Please try again.\n";
	}
}

static std::optional<int> convert_string_to_int(std::string string) {
	// Remove trailing whitespace.
	const std::size_t last_non_space{ string.find_last_not_of(" \t") };
	string = string.substr(0, last_non_space + 1);

	try {
		std::size_t chars_processed{ 0 };
		int integer{ std::stoi(string, &chars_processed) };
		if (chars_processed == string.size())
			return integer;

		std::cout << "Your input is invalid.\n";
	} catch (const std::invalid_argument&) {
		std::cout << "Your input is invalid.\n";
	} catch (const std::out_of_range&) {
		std::cout << "The number you entered is too big.\n";
	}

	return std::nullopt;
}

static int read_move_index(int max_move) {
	std::cout << "Choose a move by entering its number: _\b";

	for (;;) {
		std::string input;
		if (!std::getline(std::cin, input)) {
			std::cin.clear();
			std::cout << "Please try again: _\b";
			continue;
		}

		auto maybe_index{ convert_string_to_int(input) };
		if (maybe_index.has_value()) {
			int index{ *maybe_index };
			if (0 < index && index <= max_move)
				return index - 1;

			std::cout << "The number must be between 1 and " << max_move << ".\n";
		}

		std::cout << "Please try again: _\b";
	}
}

void Game::run() {
	for (;;) {
		display->show(board);

		auto active_name{ active_color == color::black ? "Black" : "White" };
		std::cout << '\n' << active_name << ": ";

		if (board.is_piece_under_attack(board.find_king(active_color)))
			std::cout << "Your king is in check.\n";

		auto choose_move{ std::bind(&Game::choose_move, this, std::placeholders::_1) };
		auto move{ read_move(active_color) };
		auto details{ board.move(move, choose_move) };

		if (!details) {
			std::cout << "Illegal move. Press enter to continue.\n";
			std::cin.get();
			continue;
		}

		active_color = get_opposing_color(active_color);
	}
}

int Game::choose_move(const std::vector<MoveDetails>& choices) {
	if (choices.size() == 0)
		// No legal choice exists.
		return -1;

	if (choices.size() == 1)
		// Don't prompt the user when there's only one legal choice.
		return 0;

	std::cout << "Here are all of your legal choices:\n";
	for (std::size_t index{ 0 }; index < choices.size(); index++) {
		const auto& choice{ choices[index] };
		std::cout << '\t' << (index + 1) << ". ";

		if (choice.captured_square) {
			std::cout << "Capture " << choice.captured_square->print() << ". ";
		}

		if (choice.promote_to) {
			std::cout << "Promote to " << get_piece_name(*choice.promote_to) << ". ";
		}

		if (choice.castling) {
			auto side{ choice.castling->side == side::a_side ? "a" : "h" };
			auto notation{ choice.castling->side == side::a_side ? "0-0-0" : "0-0" };
			std::cout << "Castle " << side << "-side (" << notation << "). ";
		}

		if (!choice.captured_square && !choice.promote_to && !choice.castling)
			std::cout << "Standard. ";

		std::cout << '\n';
	}

	// Make sure to convert the one-based index into a zero-based index.
	return read_move_index(choices.size());
}
