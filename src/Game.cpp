// Author: Daniel Kareh
// Summary: A class that represents the state of a game of chess. This
//          includes the board as well as who moves next. The Game class is
//          where everything comes together.

#include "Game.h"

Game::Game(Board board, std::unique_ptr<UserInterface> user_interface, color active_color)
	: board{ board }
	, user_interface{ std::move(user_interface) }
	, active_color{ active_color } {}

void Game::run() {
	for (;;) {
		user_interface->show(board);

		const std::string active_name{ active_color == color::black ? "Black" : "White" };
		const mate mated{ detect_mate(active_color) };
		if (mated != mate::no) {
			if (mated == mate::checkmate) {
				user_interface->notify("Checkmate: " + active_name + " loses.");
			} else {
				user_interface->notify("Stalemate: Draw.");
			}
			return;
		}

		if (king_is_in_check(active_color))
			user_interface->notify(active_name + ": " + "Your king is in check.");

		auto choose_move{
			[this](const std::vector<MoveDetails>& choices) {
				return user_interface->choose_move(choices);
			},
		};
		auto move{ user_interface->read_move(active_color) };
		auto details{ board.move(move, choose_move) };

		if (!details) {
			user_interface->notify("Illegal move.", notify_pause::yes);
			continue;
		}

		active_color = get_opposing_color(active_color);
	}
}

mate Game::detect_mate(color color) const {
	// Checkmate/stalemate occurs whenever a player has no legal moves.
	for (const Square from : board) {
		for (const Square to : board) {
			const Move move{ color, from, to };
			if (!board.get_legal_moves(move).empty())
				return mate::no;
		}
	}

	// If the player has no legal moves and their king currently in check,
	// that's checkmate. If they have no legal moves, but their king is not in
	// check, then stalemate has occurred.
	return king_is_in_check(color) ? mate::checkmate : mate::stalemate;
}

bool Game::king_is_in_check(color color) const {
	// The king is in check when it is under attack by an opponent's piece.
	return board.piece_is_under_attack(board.find_king(color));
}
