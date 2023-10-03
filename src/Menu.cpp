// Author: Daniel Kareh
// Summary: A simple class that anyone can use to make a command line menu.

#include "Menu.h"
#include <cstdlib> // For std::exit.
#include <iostream>
#include <limits>

using std::cout, std::cin;

std::size_t Menu::run() const {
	cout << '\n' << title << '\n';

	// Underline the menu's title.
	cout << std::string(title.size(), '-') << '\n';

	// Display all of the possible choices.
	for (std::size_t i{ 0 }; i < items.size(); i++)
		cout << '\t' << (1 + i) << ". " << items[i].description << '\n';
	cout << "\t0. Exit\n";

	for (;;) {
		cout << "Choose an item by entering the corresponding number: _\b";
		std::size_t choice{};
		if (cin >> std::ws >> choice) {
			if (choice == 0)
				std::exit(0);

			if (1 <= choice && choice <= items.size())
				return choice - 1;

			cout << "'" << choice << "' is not a valid choice.\n";
			continue;
		}

		cout << "Your input is invalid.\n";
		cin.clear();

		// Discard any bad input.
		cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	}
}
