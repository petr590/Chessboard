#ifndef CHESSBOARD_NODE_CPP
#define CHESSBOARD_NODE_CPP

#include <list>
#include "pos.cpp"

namespace chessboard {

	using std::list;

	struct Node {
		list<Node*> childs;
		Pos targetPos, eatPos;

		Node(const Pos& targetPos, const Pos& eatPos) noexcept:
			targetPos(targetPos), eatPos(eatPos) {}

		~Node() {
			for(const Node* node : childs)
				delete node;
		}

		std::string toString() const {
			std::string str = "{ target: " + targetPos.toString() + ", eat: " + eatPos.toString();

			for(const Node* node : childs)
				str += ", " + node->toString();

			return str + " }";
		}
	};
}

#endif
