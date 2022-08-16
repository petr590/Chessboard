#ifndef CHESSBOARD_SIDE_CPP
#define CHESSBOARD_SIDE_CPP

namespace chessboard {

	enum class Side {
		WHITE, BLACK
	};

	static constexpr Side Side_opposite(Side side) {
		return side == Side::WHITE ? Side::BLACK : Side::WHITE;
	}
}

#endif
