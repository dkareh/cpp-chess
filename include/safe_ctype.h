// Author: Daniel Kareh
// Summary: Wrappers around functions from <cctype> that don't trigger
//          undefined behavior on negative inputs.

#ifndef CHESS_SAFE_CTYPE_H
#define CHESS_SAFE_CTYPE_H

char safe_to_upper(char);
char safe_to_lower(char);

#endif
