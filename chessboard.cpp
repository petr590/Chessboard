#include <ncurses.h>
#include <locale.h>
#include <iostream>
#include <tuple>
#include <cmath>
#include <cstring>
#include <functional>
#include <csignal>

#include "util.cpp"

#define SCREEN_WIDTH COLS
#define SCREEN_HEIGHT LINES

namespace chessboard {

	static int logY = 0;
	static bool CAN_LOG = false;
	static bool ncursesStarted = false;

	template<typename... Args>
	void log(const char* str, Args... args) {
		if(CAN_LOG) {
			move(logY >= LINES ? logY = 0 : logY++, 0);
			printw(str, args...);
		}
	}

	using std::cout;
	using std::cin;
	using std::cerr;
	using std::endl;
	using std::max;
	using std::pair;
	using std::function;


	static void ncursesStart() {
		ncursesStarted = true;

		initscr();
		noecho();
		curs_set(false);
		keypad(stdscr, true);
	}

	static void ncursesEnd() {
		if(ncursesStarted) {
			ncursesStarted = false;

			curs_set(true);
			echo();
			endwin();
		}
	}


	static constexpr unsigned int
		TEXT_COLOR        = 1000'200,
		BORDER_COLOR      = 800,
		WHITE_FIELD_COLOR = 1000,
		BLACK_FIELD_COLOR = 000,

		WHITE_CHECKER_COLOR = 750,
		BLACK_CHECKER_COLOR = 150,

		ON_FRAME_COLOR  = hexColorToDemical(0x2CA107),
		OFF_FRAME_COLOR = hexColorToDemical(0xD4AA00);

	static constexpr short
		ON_FRAME_COLOR_ID  = 0x20,
		OFF_FRAME_COLOR_ID = 0x40;

	static constexpr pair_t
		ON_FRAME_COLOR_PAIR  = COLOR_PAIR(ON_FRAME_COLOR_ID),
		OFF_FRAME_COLOR_PAIR = COLOR_PAIR(OFF_FRAME_COLOR_ID);

	static constexpr const short FRAME_COLOR_IDS[]     { ON_FRAME_COLOR_ID, OFF_FRAME_COLOR_ID };
	static constexpr const unsigned int FRAME_COLORS[] { ON_FRAME_COLOR,    OFF_FRAME_COLOR };


	static constexpr pair_t
		TEXT_COLOR_PAIR        = COLOR_PAIR(1),
		BORDER_COLOR_PAIR      = COLOR_PAIR(2),
		WHITE_FIELD_COLOR_PAIR = COLOR_PAIR(3),
		BLACK_FIELD_COLOR_PAIR = COLOR_PAIR(4);

	static constexpr const pair_t CHECKER_COLOR_PAIRS[] {
		COLOR_PAIR(5),
		COLOR_PAIR(6),
		COLOR_PAIR(7),
		COLOR_PAIR(8),
	};



	static constexpr const unsigned int COLOR_VALUES[] {
		TEXT_COLOR,
		BORDER_COLOR,
		WHITE_FIELD_COLOR,
		BLACK_FIELD_COLOR,

		WHITE_CHECKER_COLOR << 16 | WHITE_FIELD_COLOR,
		BLACK_CHECKER_COLOR << 16 | WHITE_FIELD_COLOR,
		WHITE_CHECKER_COLOR << 16 | BLACK_FIELD_COLOR,
		BLACK_CHECKER_COLOR << 16 | BLACK_FIELD_COLOR,
	};

	static void init_color_pairs() {

		for(unsigned int i = 0; i < size(FRAME_COLOR_IDS); ++i) {
			short id = FRAME_COLOR_IDS[i];
			unsigned int color = FRAME_COLORS[i];
			init_color(id, static_cast<short>(color / 1000'000), static_cast<short>(color / 1000 % 1000), static_cast<short>(color % 1000));
			init_pair(id, 0, id);
		}

		for(short id = 0; id < static_cast<short>(size(COLOR_VALUES)); ) {
			const unsigned int color = COLOR_VALUES[id++];
			const short
					background = static_cast<short>(color),
					foreground = static_cast<short>(color >> 16);

			init_color(id, background, background, background);

			short foregroundID;

			if(foreground != 0) {
				foregroundID = static_cast<short>(0x10 + id);
				init_color(foregroundID, foreground, foreground, foreground);
			} else {
				foregroundID = 0;
			}

			init_pair(id, foregroundID, id);
			for(unsigned int i = 0; i < size(FRAME_COLOR_IDS); ++i) {
				short frameColorID = FRAME_COLOR_IDS[i];
				init_pair(static_cast<short>(id + frameColorID), frameColorID, id);
				if(foregroundID != 0)
					init_pair(static_cast<short>(foregroundID + frameColorID), frameColorID, foregroundID);
			}
		}
	}


	static constexpr const char* const CHECKER[] {
		"   ▄▄▄▄   ",
		" ▄██████▄ ",
		" ████████ ",
		" ▀██████▀ ",
		"   ▀▀▀▀   ",
	};

	static constexpr const char* const CHECKER_KING[] {
		"   ▄▄▄▄   ",
		" ▄▀▄▄▄▄▀▄ ",
		" █ ████ █ ",
		" ▀▄▀▀▀▀▄▀ ",
		"   ▀▀▀▀   ",
	};

	/*
		"    ▄▄    ",
		"   ████   ",
		"    ██    ",
		"  ▄████▄  ",
		"  ▀▀▀▀▀▀  ",

		"   ▄  ▄   ",
		"   ████   ",
		"    ██    ",
		"  ▄████▄  ",
		"  ▀▀▀▀▀▀  ",
	*/


	using std::signal;

	static void segfaultHandler(int signum) {
		ncursesEnd();
		signal(signum, SIG_DFL);
	}
}

#include "game-board.cpp"
#include "checkers-board.cpp"

int main() {

	using namespace chessboard;

	atexit(&ncursesEnd);
	signal(SIGSEGV, &segfaultHandler);

	try {
		setlocale(LC_ALL, "");

		ncursesStart();

		if(SCREEN_WIDTH < GameBoard::FULL_WIDTH && SCREEN_HEIGHT < GameBoard::FULL_HEIGHT) {
			ncursesEnd();
			cerr << "Screen too small, minimum size is " << GameBoard::FULL_WIDTH << 'x' << GameBoard::FULL_HEIGHT << " chars" << endl;
			return 1;
		}

		GameBoard::initStatic();

		start_color();

		init_color_pairs();


		CheckersBoard board(Side::WHITE);
		board.draw();

		CAN_LOG = true;

		while(board.stepAvailable()) {
			if(board.keypress(getch()))
				board.step();
		}

		board.printResult();
		getch();

		ncursesEnd();

	} catch(const string& message) {
		ncursesEnd();
		cout << "Error: " << message << endl;
		throw;

	} catch(const char* message) {
		ncursesEnd();
		cout << "Error: " << message << endl;
		throw;

	} catch(...) {
		ncursesEnd();
		throw;
	}
}
