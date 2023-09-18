#ifndef CHESS_TERMINAL_USER_INTERFACE_H
#define CHESS_TERMINAL_USER_INTERFACE_H

#include <UserInterface.h>

class TerminalUserInterface : public UserInterface {
protected:
	void clear_screen() const;
};

#endif
