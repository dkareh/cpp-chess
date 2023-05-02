// Author: Daniel Kareh
// Summary: A class that represents the state of a game of chess. This
//          includes the board as well as who moves next. The Game class is
//          where everything comes together.

#ifndef CHESS_GAME_H
#define CHESS_GAME_H

#include <Board.h>
#include <Display.h>
#include <memory>

class Game {
public:
	Game(Board, std::unique_ptr<Display>, color = color::white);

	void run();

private:
	int choose_move(const std::vector<MoveDetails>&);

	Board board;
	std::unique_ptr<Display> display;
	color active_color;
};

#endif
