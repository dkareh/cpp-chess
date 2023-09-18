#ifndef CHESS_TERMINAL_USER_INTERFACE_H
#define CHESS_TERMINAL_USER_INTERFACE_H

#include <UserInterface.h>

class TerminalUserInterface : public UserInterface {
public:
	virtual Move read_move(color active_color) override;
	virtual void notify(std::string_view message, notify_pause) override;
	virtual int choose_move(const std::vector<MoveDetails>&) override;

protected:
	void clear_screen() const;
};

#endif
