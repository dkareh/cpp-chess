#ifndef CHESS_GAME_H
#define CHESS_GAME_H

#include <memory>
#include <Board.h>
#include <Display.h>

class Game {
public:
	Game(Board, std::unique_ptr<Display>, color = color::white);

	void run();

private:
	Board board;
	std::unique_ptr<Display> display;
	color active_color;
};

#endif
