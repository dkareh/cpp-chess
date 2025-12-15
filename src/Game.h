// Author: Daniel Kareh
// Summary: A class that represents the state of a game of chess. This
//          includes the board as well as who moves next. The Game class is
//          where everything comes together.

#ifndef CHESS_GAME_H
#define CHESS_GAME_H

#include <memory>
#include "Board.h"
#include "UserInterface.h"

enum class mate : unsigned char {
	no,
	checkmate,
	stalemate,
};

class Game {
public:
	Game(Board, std::unique_ptr<UserInterface>, color = color::white);

	void run();

private:
	mate detect_mate(color) const;
	bool king_is_in_check(color) const;

	Board board;
	std::unique_ptr<UserInterface> user_interface;
	color active_color;
};

#endif
