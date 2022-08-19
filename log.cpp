#ifndef CHESSBOARD_LOG_CPP
#define CHESSBOARD_LOG_CPP

#include <ncurses.h>

namespace chessboard {

	static int logY = 0;
	static bool CAN_LOG = false;

	template<typename... Args>
	void log(const char* str, Args... args) {
		if(CAN_LOG) {
			move(logY >= LINES ? logY = 0 : logY++, 0);
			printw(str, args...);
		}
	}
}

#endif
