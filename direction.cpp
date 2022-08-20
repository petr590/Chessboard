#ifndef CHESSBOARD_DIRECTION_CPP
#define CHESSBOARD_DIRECTION_CPP

#include <vector>

namespace chessboard {

	using std::vector;


	enum class Direction {
		NONE = -1, FORWARD_LEFT, FORWARD_RIGHT, BACKWARD_LEFT, BACKWARD_RIGHT
	};

	static const vector<Direction> ALL_DIRECTIONS {
		Direction::FORWARD_LEFT,
		Direction::FORWARD_RIGHT,
		Direction::BACKWARD_LEFT,
		Direction::BACKWARD_RIGHT
	};

	static Direction Direction_opposite(Direction direction) {
		switch(direction) {
			case Direction::FORWARD_LEFT:   return Direction::BACKWARD_RIGHT;
			case Direction::FORWARD_RIGHT:  return Direction::BACKWARD_LEFT;
			case Direction::BACKWARD_LEFT:  return Direction::FORWARD_RIGHT;
			case Direction::BACKWARD_RIGHT: return Direction::FORWARD_LEFT;
			default: throw direction;
		}
	}

	static bool Direction_isForward(Direction direction) {
		return direction == Direction::FORWARD_LEFT || direction == Direction::FORWARD_RIGHT;
	}

	static bool Direction_isLeft(Direction direction) {
		return direction == Direction::FORWARD_LEFT || direction == Direction::BACKWARD_LEFT;
	}
}

#endif
