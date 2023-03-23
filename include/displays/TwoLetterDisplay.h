#ifndef CHESS_DISPLAYS_TWO_LETTER_DISPLAY_H
#define CHESS_DISPLAYS_TWO_LETTER_DISPLAY_H

#include <Display.h>

class TwoLetterDisplay : public Display {
public:
	virtual void show(const Board&) override;
};

#endif
