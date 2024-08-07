// Name: Daniel Kareh
// Course: COP 3003 - CRN 15050
// Summary: The final deployment of my semester project. The user can play
//          chess and all of the rules are enforced. The user can also choose
//          between several different visual styles.

#include <memory>
#include <stdexcept>
#include "Game.h"
#include "Menu.h"
#include "chess960.h"
#include "ui/AsciiUi.h"
#include "ui/LetterUi.h"
#include "ui/TwoLetterUi.h"
#include "ui/WindowsConsoleUi.h"

// FIXME(Daniel): NOLINTNEXTLINE(cert-err58-cpp)
static const Menu main_menu{
	"Daniel's Chess Program",
	{ { "Play a game" } },
};

// FIXME(Daniel): NOLINTNEXTLINE(cert-err58-cpp)
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

// FIXME(Daniel): NOLINTNEXTLINE(cert-err58-cpp)
static const Menu glyph_size_menu{
	"Chess Glyph Size",
	{
		{ "Single width" },
		{ "Double width" },
	},
};

// FIXME(Daniel): NOLINTNEXTLINE(cert-err58-cpp)
static const Menu variant_menu{
	"Chess Variant",
	{
		{ "Classical" },
		{ "Chess960" },
	},
};

enum class variant : unsigned char {
	classical,
	chess960,
};

static Board setup_initial_board(variant);

// FIXME(Daniel): NOLINTNEXTLINE(bugprone-exception-escape)
int main() {
	auto choice{ main_menu.run() };
	if (choice == 0) {
		auto visual_style{ visual_style_menu.run() };
		std::unique_ptr<UserInterface> user_interface{};
		switch (visual_style) {
		case 0:
			user_interface = std::make_unique<AsciiUi>();
			break;
		case 1:
			user_interface = std::make_unique<LetterUi>();
			break;
		case 2:
			user_interface = std::make_unique<TwoLetterUi>();
			break;
#ifdef CHESS_ON_WINDOWS
		case 3: {
			auto size{ static_cast<glyph_size>(glyph_size_menu.run()) };
			user_interface = std::make_unique<WindowsConsoleUi>(size);
			break;
		}
#endif
		default:
			assert(0);
		}

		auto variant{ static_cast<enum variant>(variant_menu.run()) };
		const Board initial_board{ setup_initial_board(variant) };
		Game game{ initial_board, std::move(user_interface) };
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
	default:
		throw std::invalid_argument{ "Invalid variant" };
	}
}
