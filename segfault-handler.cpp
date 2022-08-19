#ifndef CHESSBOARD_SEGFAULT_HANDLER_CPP
#define CHESSBOARD_SEGFAULT_HANDLER_CPP

#include <csignal>
#include "util/ncurseslib.cpp"

namespace chessboard {

	using std::signal;

	static void segfaultHandler(int signum) {
		ncurses_end();
		signal(signum, SIG_DFL);
	}
}

#endif
