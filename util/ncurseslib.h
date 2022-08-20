#ifndef CHESSBOARD_NCURSESLIB_H
#define CHESSBOARD_NCURSESLIB_H

#include <ncurses.h>

extern void ncurses_start();

extern void ncurses_end();

struct Window {
	WINDOW* win;
	int height, width;

	explicit Window(int height, int width, int y, int x);

	~Window();

	inline void keypadOn() {
		keypad(win, true);
	}

	inline void keypadOff() {
		keypad(win, false);
	}

	void drawPlainBorder(attr_t, int thickness = 1);
	void fill(int startY, int startX, int height, int width, chtype);
	void drawHorisontalLine(int startY, int startX, int length, chtype = ' ', int thickness = 1);

	inline void attrOn(attr_t attr) {
		wattron(win, attr);
	}

	inline void attrOff(attr_t attr) {
		wattroff(win, attr);
	}

	inline int getChar() {
		return wgetch(win);
	}

	inline void moveCursor(int y, int x) {
		wmove(win, y, x);
	}

	inline void addStr(const char* str) {
		waddstr(win, str);
	}

	inline void addStrTo(int y, int x, const char* str) {
		moveCursor(y, x);
		addStr(str);
	}

	/*int clearok(WINDOW*, bool);
	int copywin(const WINDOW*, WINDOW*, int, int, int, int, int, int, int);
	WINDOW* derwin(WINDOW*, int, int, int, int);
	WINDOW* dupwin(WINDOW*);
	chtype getbkgd(WINDOW*);
	void idcok(WINDOW*, bool);
	int idlok(WINDOW*, bool);
	void immedok(WINDOW*, bool);
	int intrflush(WINDOW*, bool);
	bool is_linetouched(WINDOW*, int);
	bool is_wintouched(WINDOW*);
	int keypad(WINDOW*, bool);
	int leaveok(WINDOW*, bool);
	int meta(WINDOW*, bool);
	int mvderwin(WINDOW*, int, int);
	int mvaddch(WINDOW*, int, int, const chtype);
	int mvaddchnstr(WINDOW*, int, int, const chtype*, int);
	int mvaddchstr(WINDOW*, int, int, const chtype*);
	int mvaddnstr(WINDOW*, int, int, const char*, int);
	int mvaddstr(WINDOW*, int, int, const char*);
	int mvchgat(WINDOW*, int, int, int, attr_t, NCURSES_PAIRS_T, const void*);
	int mvdelch(WINDOW*, int, int);
	int mvgetch(WINDOW*, int, int);
	int mvgetnstr(WINDOW*, int, int, char*, int);
	int mvgetstr(WINDOW*, int, int, char*);
	int mvhline(WINDOW*, int, int, chtype, int);
	int mvin(WINDOW*, int, int);
	chtype mvinch(WINDOW*, int, int);
	int mvinchnstr(WINDOW*, int, int, chtype*, int);
	int mvinchstr(WINDOW*, int, int, chtype*);
	int mvinnstr(WINDOW*, int, int, char*, int);
	int mvinsch(WINDOW*, int, int, chtype);
	int mvinsnstr(WINDOW*, int, int, const char*, int);
	int mvinsstr(WINDOW*, int, int, const char*);
	int mvinstr(WINDOW*, int, int, char*);
	int mvprintw(WINDOW*, int, int, const char*, ...)
	int mvscanw(WINDOW*, int, int, NCURSES_CONST char*, ...)
	int mvvline(WINDOW*, int, int, chtype, int);
	WINDOW* newpad(int, int);
	WINDOW* newwin(int, int, int, int);
	int nodelay(WINDOW*, bool);
	int notimeout(WINDOW*, bool);
	int overlay(const WINDOW*, WINDOW*);
	int overwrite(const WINDOW*, WINDOW*);
	int pechochar(WINDOW*, const chtype);
	int pnoutrefresh(WINDOW*, int, int, int, int, int, int);
	int prefresh(WINDOW*, int, int, int, int, int, int);
	int putwin(WINDOW*, FILE*);
	int redrawwin(WINDOW*);
	int ripoffline(int, int(*)(WINDOW*, int));
	int scroll(WINDOW*);
	int scrollok(WINDOW*, bool);
	WINDOW* subpad(WINDOW*, int, int, int, int);
	WINDOW* subwin(WINDOW*, int, int, int, int);
	int syncok(WINDOW*, bool);
	int touchline(WINDOW*, int, int);
	int touchwin(WINDOW*);
	int untouchwin(WINDOW*);
	int vwprintw(WINDOW*, const char*, va_list);
	int vw_printw(WINDOW*, const char*, va_list);
	int vwscanw(WINDOW*, NCURSES_CONST char*, va_list);
	int vw_scanw(WINDOW*, NCURSES_CONST char*, va_list);
	int addch(WINDOW*, const chtype);
	int addchnstr(WINDOW*, const chtype*, int);
	int addchstr(WINDOW*, const chtype*);
	int addnstr(WINDOW*, const char*, int);
	int addstr(WINDOW*, const char*);
	int attron(WINDOW*, int);
	int attroff(WINDOW*, int);
	int attrset(WINDOW*, int);
	int attr_get(WINDOW*, attr_t*, NCURSES_PAIRS_T*, void*);
	int attr_on(WINDOW*, attr_t, void*);
	int attr_off(WINDOW*, attr_t, void*);
	int attr_set(WINDOW*, attr_t, NCURSES_PAIRS_T, void*);
	int bkgd(WINDOW*, chtype);
	void bkgdset(WINDOW*, chtype);
	int border(WINDOW*, chtype, chtype, chtype, chtype, chtype, chtype, chtype, chtype);
	int chgat(WINDOW*, int, attr_t, NCURSES_PAIRS_T, const void*);
	int clear(WINDOW*);
	int clrtobot(WINDOW*);
	int clrtoeol(WINDOW*);
	int color_set(WINDOW*,NCURSES_PAIRS_T, void*);
	void cursyncup(WINDOW*);
	int delch(WINDOW*);
	int deleteln(WINDOW*);
	int echochar(WINDOW*, const chtype);
	int erase(WINDOW*);
	int getch(WINDOW*);
	int getnstr(WINDOW*, char*, int);
	int getstr(WINDOW*, char*);
	int hline(WINDOW*, chtype, int);
	chtype inch(WINDOW*);
	int inchnstr(WINDOW*, chtype*, int);
	int inchstr(WINDOW*, chtype*);
	int innstr(WINDOW*, char*, int);
	int insch(WINDOW*, chtype);
	int insdelln(WINDOW*, int);
	int insertln(WINDOW*);
	int insnstr(WINDOW*, const char*, int);
	int insstr(WINDOW*, const char*);
	int instr(WINDOW*, char*);
	int move(WINDOW*, int, int);
	int noutrefresh(WINDOW*);
	int redrawln(WINDOW*, int, int);
	int refresh(WINDOW*);
	int scrl(WINDOW*, int);
	int setscrreg(WINDOW*, int, int);
	int standout(WINDOW*);
	int standend(WINDOW*);
	void syncdown(WINDOW*);
	void syncup(WINDOW*);
	void timeout(WINDOW*, int);
	int touchln(WINDOW*, int, int, int);
	int vline(WINDOW*, chtype, int);
	int getattrs(const WINDOW*);
	int getcurx(const WINDOW*);
	int getcury(const WINDOW*);
	int getbegx(const WINDOW*);
	int getbegy(const WINDOW*);
	int getmaxx(const WINDOW*);
	int getmaxy(const WINDOW*);
	int getparx(const WINDOW*);
	int getpary(const WINDOW*);
	int use_window(WINDOW*, NCURSES_WINDOW_CB, void*);
	int resize(WINDOW*, int, int);
	WINDOW* getparent(const WINDOW*);
	bool is_cleared(const WINDOW*);
	bool is_idcok(const WINDOW*);
	bool is_idlok(const WINDOW*);
	bool is_immedok(const WINDOW*);
	bool is_keypad(const WINDOW*);
	bool is_leaveok(const WINDOW*);
	bool is_nodelay(const WINDOW*);
	bool is_notimeout(const WINDOW*);
	bool is_pad(const WINDOW*);
	bool is_scrollok(const WINDOW*);
	bool is_subwin(const WINDOW*);
	bool is_syncok(const WINDOW*);
	int getdelay(const WINDOW*);
	int getscrreg(const WINDOW*, int*, int*);*/
};

#endif
