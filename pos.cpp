#ifndef CHESSBOARD_POS_CPP
#define CHESSBOARD_POS_CPP

#include <string>

namespace chessboard {

	using std::string;
	using std::to_string;

	struct Pos {
		int y, x;

		constexpr Pos() noexcept: y(0), x(0) {}
		constexpr Pos(int y, int x) noexcept: y(y), x(x) {}
		constexpr Pos(const Pos& other) noexcept: y(other.y), x(other.x) {}

		friend constexpr bool operator==(const Pos& pos1, const Pos& pos2) {
			return pos1.y == pos2.y && pos1.x == pos2.x;
		}

		friend constexpr bool operator!=(const Pos& pos1, const Pos& pos2) {
			return !(pos1 == pos2);
		}

		friend std::ostream& operator<<(std::ostream& out, const Pos& pos) {
			return out << '{' << pos.y << ", " << pos.x << '}';
		}

		string toString() const {
			return '{' + to_string(y) + ", " + to_string(x) + '}';
		}
	};

	static constexpr Pos NPOS(-1, -1);
}

#endif
