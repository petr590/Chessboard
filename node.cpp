#ifndef CHESSBOARD_NODE_CPP
#define CHESSBOARD_NODE_CPP

#include <list>
#include <string>
#include "pos.cpp"

namespace chessboard {

	using std::list;
	using std::string;

	struct Node {
		list<Node*> childs;
		Pos targetPos, eatPos;

		Node(const Pos& targetPos, const Pos& eatPos = NPOS) noexcept:
			targetPos(targetPos), eatPos(eatPos) {}

		~Node() {
			for(const Node* node : childs)
				delete node;
		}

		inline void addNode(Node* node) {
			childs.push_back(node);
		}

		string toString() const {
			string str = "{ target: " + targetPos.toString() + ", eat: " + eatPos.toString();

			for(const Node* node : childs)
				str += ", " + node->toString();

			return str + " }";
		}
	};
}

#endif
