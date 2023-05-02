// Name: Daniel Kareh
// Course: COP 3003 - CRN 15050
// Summary: The final deployment of my semester project. The user can play
//          chess and all of the rules are enforced. The user can also choose
//          between several different visual styles.

#include <Game.h>
#include <Menu.h>
#include <chess960.h>
#include <displays/AsciiDisplay.h>
#include <displays/LetterDisplay.h>
#include <displays/TwoLetterDisplay.h>
#include <displays/WindowsConsoleDisplay.h>
#include <iostream>
#include <memory>

static const Menu main_menu{
	"Daniel's Chess Program",
	{ { "Play a game" } },
};

static const Menu visual_style_menu{
	"Visual Style",
	{
		{ "ASCII drawings" },
		{ "Uppercase and lowercase letters (e.g. K, q)" },
		{ "Two letters (e.g. wk, bq)" },
#ifdef CHESS_ON_WINDOWS
		{ "Colorful Unicode (Windows only)" },
#endif
	},
};

static const Menu variant_menu{
	"Chess Variant",
	{
		{ "Classical" },
		{ "Chess960" },
	},
};

enum class variant {
	classical,
	chess960,
};

static Board setup_initial_board(variant);

int main() {
	auto choice{ main_menu.run() };
	if (choice == 0) {
		auto visual_style{ visual_style_menu.run() };
		std::unique_ptr<Display> display{};
		switch (visual_style) {
		case 0:
			display.reset(new AsciiDisplay{});
			break;
		case 1:
			display.reset(new LetterDisplay{});
			break;
		case 2:
			display.reset(new TwoLetterDisplay{});
			break;
#ifdef CHESS_ON_WINDOWS
		case 3:
			display.reset(new WindowsConsoleDisplay{});
			break;
#endif
		}

		auto variant{ static_cast<enum variant>(variant_menu.run()) };
		const Board initial_board{ setup_initial_board(variant) };
		Game game{ initial_board, std::move(display) };
		game.run();
	} else {
		return 1;
	}
}

static Board setup_initial_board(variant variant) {
	switch (variant) {
	case variant::classical:
		return Board{};
	case variant::chess960:
		return generate_chess960_board();
	}
}
