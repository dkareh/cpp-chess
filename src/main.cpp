// Name: Daniel Kareh
// Course: COP 3003 - CRN 15050
// Summary: The second deployment of my semester project. The user can play
//          chess and most of the rules are enforced. The user can also choose
//          between three different visual styles.

#include <Game.h>
#include <Menu.h>
#include <displays/AsciiDisplay.h>
#include <displays/LetterDisplay.h>
#include <displays/TwoLetterDisplay.h>
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
	},
};

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
		}

		Game game{ Board{}, std::move(display) };
		game.run();
	} else {
		return 1;
	}
}
