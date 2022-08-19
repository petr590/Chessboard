#ifndef CHESSBOARD_TYPEDEFS_CPP
#define CHESSBOARD_TYPEDEFS_CPP

#include <ncurses.h>

namespace chessboard {

	typedef NCURSES_PAIRS_T pair_t;
	typedef NCURSES_PAIRS_T id_t;
	typedef NCURSES_COLOR_T color_t;
	typedef unsigned int colordef_t;
	typedef const char* const* texture_t;
}

#endif
