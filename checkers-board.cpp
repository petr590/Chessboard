#ifndef CHESSBOARD_CHECKERS_BOARD_CPP
#define CHESSBOARD_CHECKERS_BOARD_CPP

#include "game-board.cpp"
#include "keys.cpp"
#include "direction.cpp"
#include "node.cpp"
#include "textures.cpp"
#include "log.cpp"

#include <vector>
#include <algorithm>

namespace chessboard {

	using std::vector;
	using std::find;
	using std::begin;
	using std::end;


	struct CheckersBoard: GameBoard {
		public:
			static constexpr figure_t
				_  = N_FIGURE, // empty field
				W  = 0, // white checker
				B  = 1, // black checker
				WK = 2, // white king checker
				BK = 3; // black king checker

			static void Direction_step(Direction direction, const CheckersBoard* board, Pos& pos, int distance = 1) {
				board->step(pos, Direction_isForward(direction) ? distance : -distance);

				Direction_isLeft(direction) ?
					pos.x -= distance :
					pos.x += distance;
			}


			static Side getCheckerSide(figure_t checker) {
				return checker & 0x1 ? Side::BLACK : Side::WHITE;
			}

			static bool isCheckerKing(figure_t checker) {
				return checker & 0x2;
			}

			static const vector<Direction>& getCheckerDirections(figure_t checker) {
				static const vector<Direction> PLAIN_DIRECTIONS { Direction::FORWARD_LEFT, Direction::FORWARD_RIGHT };
				static const vector<Direction>& KING_DIRECTIONS = ALL_DIRECTIONS;

				return isCheckerKing(checker) ? KING_DIRECTIONS : PLAIN_DIRECTIONS;
			}

			//int whiteCount = 12, blackCount = 12;
			int whiteCount = 1, blackCount = 1;

			int& countBySide(Side side) {
				return side == Side::WHITE ? whiteCount : blackCount;
			}


			explicit CheckersBoard(Side side) noexcept:
				CheckersBoard({
					/*B,_,B,_,B,_,B,_,
					_,B,_,B,_,B,_,B,
					B,_,B,_,B,_,B,_,
					_,_,_,_,_,_,_,_,
					_,_,_,_,_,_,_,_,
					_,W,_,W,_,W,_,W,
					W,_,W,_,W,_,W,_,
					_,W,_,W,_,W,_,W,*/
					_,_,_,_,_,_,_,_,
					_,_,_,_,_,_,_,_,
					_,_,_,_,_,_,_,_,
					_,_,_,B,_,_,_,_,
					_,_,_,_,_,_,_,_,
					_,WK,_,_,_,_,_,_,
					_,_,_,_,_,_,_,_,
					_,_,_,_,_,_,_,_,
				}, {5, 1}, side) {

				if(side == Side::BLACK)
					flip();
			}


			explicit CheckersBoard(const Board& board, const Pos& pos, Side side) noexcept:
				GameBoard(board, pos, side) {}

		protected:
			virtual pair<pair_t, texture_t> getColorPairAndFigureTexture(int h, int w) const override {
				bool isBlackField = ((h + w) & 0x1) == 0;

				figure_t checker = board[h * FIELDS + w];
				if(checker >= 0)
					return { CHECKER_COLOR_PAIRS[(checker & 0x1) + (isBlackField << 1)], isCheckerKing(checker) ? CHECKER_KING : CHECKER };
				else
					return { isBlackField ? BLACK_FIELD_COLOR_PAIR : WHITE_FIELD_COLOR_PAIR, nullptr };
			}

		public:
			virtual void step() override {
				currentSide = Side_opposite(currentSide);
			}

			virtual bool stepAvailable() const {
				return whiteCount > 0 && blackCount > 0;
			}

			inline void step(Pos& pos, int distance = 1) const {
				currentSide == defaultSide ?
					pos.y -= distance :
					pos.y += distance;
			}

			const Node* availableSteps(const Pos& pos) {
				Node* const root = new Node(pos);

				list<Pos> eatedPoses;

				const figure_t checker = board[pos];

				for(Direction direction : getCheckerDirections(checker)) {
					Pos targetPos = pos;

					Direction_step(direction, this, targetPos);

					if(isValidPos(targetPos)) {
						if(board[targetPos] == N_FIGURE)
							root->addNode(new Node(targetPos));

						if(isCheckerKing(checker)) {
							Direction_step(direction, this, targetPos);

							Node* currentNode = root;

							while(isValidPos(targetPos)) {
								const figure_t targerChecker = board[targetPos];

								if(targerChecker == N_FIGURE) {
									currentNode->addNode(new Node(targetPos));

								} else if(getCheckerSide(targerChecker) != currentSide) {
									Pos eatPos = targetPos;

									Direction_step(direction, this, targetPos);

									if(isValidPos(targetPos) && board[targetPos] == N_FIGURE) {
										Node* newNode = new Node(targetPos, eatPos);
										currentNode->addNode(newNode);
										currentNode = newNode;
										eatedPoses.push_back(eatPos);
									} else {
										break;
									}

								} else {
									break;
								}

								Direction_step(direction, this, targetPos);
							}
						}
					}
				}

				getEatPath(pos, root, Direction::NONE, eatedPoses);

				return root;
			}


			void getEatPath(const Pos& pos, Node* node, Direction fromWhere, list<Pos>& eatedPoses) {

				for(Direction direction : ALL_DIRECTIONS) { // We can eat at all directions
					if(direction == fromWhere)
						continue;

					Pos targetPos = pos;
					Direction_step(direction, this, targetPos);

					if(find(eatedPoses.begin(), eatedPoses.end(), targetPos) != eatedPoses.end())
						continue;

					eatedPoses.push_back(targetPos);

					if(isValidPos(targetPos)) {
						figure_t checker = board[targetPos];

						if(checker != N_FIGURE && getCheckerSide(checker) != currentSide) {
							Pos eatPos = targetPos;
							Direction_step(direction, this, targetPos);

							if(isValidPos(targetPos) && board[targetPos] == N_FIGURE) {
								Node* targetNode = new Node(targetPos, eatPos);
								node->addNode(targetNode);
								getEatPath(targetPos, targetNode, Direction_opposite(direction), eatedPoses);
							}
						}
					}
				}
			}


			virtual bool keypress(int key) override {
				switch(key) {
					case KEY_ENTER: case KEY_SPACE: {
						figure_t checker = board[selectedPos];
						if(checker != N_FIGURE && getCheckerSide(checker) == currentSide) {

							Pos oldOffPos = offPos;
							offPos = selectedPos;

							if(oldOffPos != NPOS)
								drawField(oldOffPos);

						} else if(offPos != NPOS) {

							const Node* result = nullptr;
							list<Pos> eatPoses;

							function<bool(const Node*)> processNode = [&result, &processNode, &eatPoses, this] (const Node* targetNode) {

								const Node* res = nullptr;

								for(const Node* node : targetNode->childs) {

									if(node->targetPos == selectedPos) {
										if(result == nullptr) {
											res = node;
											eatPoses.push_back(node->eatPos);
										} else {
											log("ambigous step");
											result = nullptr;
											return false;
										}
									}

									if(!processNode(node))
										return false;
								}

								if(res != nullptr)
									result = res;

								if(result != nullptr)
									eatPoses.push_back(targetNode->eatPos);

								return true;
							};

							{
								const Node* rootNode = availableSteps(offPos);
								processNode(rootNode);
								delete rootNode;
							}

							if(result != nullptr) {
								figure_t checker = board[offPos];
								board[selectedPos] = (selectedPos.y == (currentSide == defaultSide ? 0 : FIELDS - 1)) ? checker | 0x2 : checker;
								board[offPos] = N_FIGURE;

								Pos oldOffPos = offPos;
								offPos = NPOS;

								drawField(oldOffPos);
								drawField(selectedPos);

								size_t eatedPosCount = 0;

								for(const Pos& eatPos : eatPoses) {
									if(eatPos != NPOS) {
										++eatedPosCount;

										board[eatPos] = N_FIGURE;
										drawField(eatPos);
									}
								}

								countBySide(Side_opposite(currentSide)) -= eatedPosCount;

								return true;
							}
						}

						break;
					}

					case KEY_UP:
						if(selectedPos.y > 0) {
							--selectedPos.y;
							drawField(selectedPos.y + 1, selectedPos.x);
							drawField(selectedPos);
						}
						break;

					case KEY_DOWN:
						if(selectedPos.y < FIELDS - 1) {
							++selectedPos.y;
							drawField(selectedPos.y - 1, selectedPos.x);
							drawField(selectedPos);
						}
						break;

					case KEY_LEFT:
						if(selectedPos.x > 0) {
							--selectedPos.x;
							drawField(selectedPos.y, selectedPos.x + 1);
							drawField(selectedPos);
						}
						break;

					case KEY_RIGHT:
						if(selectedPos.x < FIELDS - 1) {
							++selectedPos.x;
							drawField(selectedPos.y, selectedPos.x - 1);
							drawField(selectedPos);
						}
				}

				return false;
			}

			virtual void showResult() const override {
				static constexpr int WIDTH = 21, HEIGHT = 5;
				static const char
						*const borderHorizontal = repeat_char(' ', WIDTH + 4),
						*const line = repeat_char(' ', WIDTH);

				const int
						startY = boardStartY + BOARD_HEIGHT / 2 - HEIGHT / 2,
						startX = boardStartX + BOARD_WIDTH / 2 - WIDTH / 2,

						endY = startY + HEIGHT,
						endX = startX + WIDTH;

				attron(BORDER_COLOR_PAIR);

				mvaddstr(startY - 1, startX - 2, borderHorizontal);
				mvaddstr(endY, startX - 2, borderHorizontal);

				for(int y = startY; y < endY; ++y) {
					mvaddstr(y, startX - 2, "  ");
					mvaddstr(y, endX, "  ");
				}

				attroff(BORDER_COLOR_PAIR);

				attron(TEXT_COLOR_PAIR);

				for(int y = startY; y < endY; ++y)
					mvaddstr(y, startX, line);

				const char* message = whiteCount > blackCount ? "Белые выиграли" : whiteCount < blackCount ? "Чёрные выиграли" : "Ничья";
				mvaddstr(startY + (HEIGHT - 1) / 2, startX + WIDTH / 2 - strlen(message) / 2 / 2, message);

				attroff(TEXT_COLOR_PAIR);

				getch();
			}
	};
}

#endif
