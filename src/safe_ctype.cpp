// Author: Daniel Kareh
// Summary: Wrappers around functions from <cctype> that don't trigger
//          undefined behavior on negative inputs.

#include <cctype>
#include <safe_ctype.h>

// https://en.cppreference.com/w/cpp/string/byte/toupper
char safe_to_upper(char ch) {
	return static_cast<char>(std::toupper(static_cast<unsigned char>(ch)));
}

// https://en.cppreference.com/w/cpp/string/byte/tolower
char safe_to_lower(char ch) {
	return static_cast<char>(std::tolower(static_cast<unsigned char>(ch)));
}
