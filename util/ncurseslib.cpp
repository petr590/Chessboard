#ifndef CHESSBOARD_NCURSESLIB_CPP
#define CHESSBOARD_NCURSESLIB_CPP

#include "ncurseslib.h"

void ncurses_start() {
	initscr();
	noecho();
	curs_set(false);
	keypad(stdscr, true);
}

void ncurses_end() {
	if(!isendwin()) {
		curs_set(true);
		echo();
		endwin();
	}
}

#endif
