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


Window::Window(int height, int width, int y, int x):
	win(newwin(height, width, y, x)), height(height), width(width) {}

Window::~Window() {
	delwin(win);
}

void Window::drawPlainBorder(attr_t attr, int thickness) {
	chtype ch = ' ' | attr;

	for(int i = 0; i < thickness; ++i) {
		wmove(win, i, 0);
		for(int x = 0; x < width; ++x)
			waddch(win, ch);

		wmove(win, height - 1 - i, 0);
		for(int x = 0; x < width; ++x)
			waddch(win, ch);
	}

	for(int y = 1; y < height - 1; ++y) {
		wmove(win, y, 0);
		for(int i = 0; i < thickness; ++i) {
			waddch(win, ch);
			waddch(win, ch);
		}
		wmove(win, y, width - thickness * 2);
		for(int i = 0; i < thickness; ++i) {
			waddch(win, ch);
			waddch(win, ch);
		}
	}
}

void Window::fill(int startY, int startX, int height, int width, chtype ch) {
	for(int y = startY, endY = startY + height; y < endY; ++y) {
		wmove(win, y, startX);

		for(int i = 0; i < width; ++i)
			waddch(win, ch);
	}
}

void Window::drawHorisontalLine(int startY, int startX, int length, chtype ch, int thickness) {

	for(int y = startY, endY = startY + thickness; y < endY; ++y) {
		wmove(win, y, startX);

		for(int i = 0; i < length; ++i)
			waddch(win, ch);
	}
}

#endif
