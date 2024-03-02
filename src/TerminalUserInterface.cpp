#include "TerminalUserInterface.h"
#include <cassert>
#include <iostream>
#include <limits>

/// Parse a move string in the form 'xNxN', such as 'd2d4'.
/// The string must be exactly four characters long.
static std::optional<Move> parse_move(std::string_view string, color active_color) {
	if (string.size() != 4)
		return {};

	auto maybe_from{ Square::parse(string.substr(0, 2)) };
	if (!maybe_from.has_value())
		return {};

	auto maybe_to{ Square::parse(string.substr(2, 2)) };
	if (!maybe_to.has_value())
		return {};

	return Move{ active_color, maybe_from.value(), maybe_to.value() };
}

// Trailing whitespace is whitespace that comes at the end of a string.
static std::string remove_trailing_whitespace(const std::string& string) {
	const std::size_t last_non_space{ string.find_last_not_of(" \t") };
	return string.substr(0, last_non_space + 1);
}

static std::optional<int> convert_string_to_int(std::string string) {
	string = remove_trailing_whitespace(string);

	try {
		std::size_t chars_processed{ 0 };
		int integer{ std::stoi(string, &chars_processed) };
		// Don't return successfully unless we parsed the entire input string.
		// Input like '42hello' or '42.0' is considered invalid!
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
			int index{ maybe_index.value() };
			if (0 < index && index <= max_move)
				// Convert the one-based index into a zero-based index.
				return index - 1;

			std::cout << "The number must be between 1 and " << max_move << ".\n";
		}

		std::cout << "Please try again: _\b";
	}
}

Move TerminalUserInterface::read_move(color active_color) {
	auto active_name{ active_color == color::black ? "Black" : "White" };
	std::cout << active_name << ": ";
	for (;;) {
		std::cout << "Your move? ____\b\b\b\b";
		std::string input;
		if (!std::getline(std::cin >> std::ws, input)) {
			std::cin.clear();
			continue;
		}

		input = remove_trailing_whitespace(input);
		auto move{ parse_move(input, active_color) };
		if (move.has_value())
			return move.value();

		std::cout << "Please try again.\n";
	}
}

void TerminalUserInterface::notify(std::string_view message, notify_pause pause) {
	std::cout << message << '\n';
	if (pause == notify_pause::yes) {
		std::cout << "Press enter to continue.";
		std::cin.get();
	}
}

int TerminalUserInterface::choose_move(const std::vector<MoveDetails>& choices) {
	if (choices.size() == 0)
		// No legal choice exists.
		return -1;

	if (choices.size() == 1)
		// Don't prompt the user when there's only one legal choice.
		return 0;

	// Print what happens to the board for each legal choice so that the user
	// can choose the move they intended to play.
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

		// If nothing special happens, we'll call it a standard move.
		if (!choice.captured_square && !choice.promote_to && !choice.castling)
			std::cout << "Standard. ";

		std::cout << '\n';
	}

	assert(choices.size() <= std::numeric_limits<int>::max());
	return read_move_index(static_cast<int>(choices.size()));
}

void TerminalUserInterface::clear_screen() const {
	for (int i{ 0 }; i < 200; i++)
		std::cout << '\n';
}
