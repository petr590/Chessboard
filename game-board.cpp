#ifndef CHESSBOARD_GAME_BOARD_CPP
#define CHESSBOARD_GAME_BOARD_CPP

#include "pos.cpp"
#include "side.cpp"
#include "colors.cpp"

#include <tuple>
#include <functional>
#include <cassert>

#define SCREEN_WIDTH COLS
#define SCREEN_HEIGHT LINES

namespace chessboard {

	using std::pair;
	using std::function;


	struct GameBoard {
		public:
			static constexpr int
					FIELD_WIDTH = 10,
					FIELD_HEIGHT = FIELD_WIDTH / 2,

					FIELDS = 8,
					SQUARE = FIELDS * FIELDS,

					BOARD_WIDTH = FIELD_WIDTH * FIELDS,
					BOARD_HEIGHT = BOARD_WIDTH / 2,

					PADDING_WIDTH  = 3,
					PADDING_HEIGHT = 1,

					FULL_WIDTH  = BOARD_WIDTH + PADDING_WIDTH * 2,
					FULL_HEIGHT = BOARD_HEIGHT + PADDING_HEIGHT * 2;

			static const char *borderLine, *borderTopLine, *borderBottomLine, *fieldLine;

			static int
					startY, startX,
					boardStartY, boardStartX;

			static void initStatic() {
				char* borderLine = repeat_char(' ', FULL_WIDTH);

				for(int i = 0; i < FIELDS; ++i)
					borderLine[i * FIELD_WIDTH + FIELD_WIDTH / 2 + 2] = static_cast<char>(i + 'A');

				GameBoard::borderLine = borderLine;

				borderTopLine = repeat_str("▀", FULL_WIDTH);
				borderBottomLine = repeat_str("▄", FULL_WIDTH);
				fieldLine = repeat_char(' ', FIELD_WIDTH);

				startY = (SCREEN_HEIGHT - FULL_HEIGHT) / 2;
				startX = (SCREEN_WIDTH - FULL_WIDTH) / 2;

				boardStartY = startY + PADDING_HEIGHT,
				boardStartX = startX + PADDING_WIDTH;
			}


			typedef signed char figure_t;

			static constexpr figure_t N_FIGURE = -1;


			struct Board {
				figure_t board[SQUARE];

				figure_t& operator[](const Pos& pos) {
					if(!(pos.y >= 0 && pos.y < FIELDS && pos.x >= 0 && pos.x < FIELDS))
						throw pos.toString();
					return board[pos.y * FIELDS + pos.x];
				}

				figure_t operator[](const Pos& pos) const {
					assert(pos.y >= 0 && pos.y < FIELDS);
					assert(pos.x >= 0 && pos.x < FIELDS);
					return board[pos.y * FIELDS + pos.x];
				}

				figure_t& operator[](size_t index) {
					assert(index < SQUARE);
					return board[index];
				}

				figure_t operator[](size_t index) const {
					assert(index < SQUARE);
					return board[index];
				}

				operator figure_t*() {
					return board;
				}

				operator const figure_t*() const {
					return board;
				}
			};

		protected:

			Board board;

			Pos selectedPos,
				offPos = NPOS;

			Side currentSide, defaultSide;


			explicit GameBoard(const Board& board, Pos selectedPos, Side defaultSide) noexcept:
				board(board), selectedPos(selectedPos), currentSide(defaultSide), defaultSide(defaultSide) {}

			GameBoard(const GameBoard&) = delete;
			GameBoard& operator=(const GameBoard&) = delete;

			virtual ~GameBoard() {
				delete borderLine;
				delete borderTopLine;
				delete borderBottomLine;
			}

			void flip() {
				for(figure_t *front = board, *back = front + SQUARE - 1, *center = front + SQUARE / 2; front < center; ++front, --back) {
					figure_t figure = *front;
					*front = *back;
					*back = figure;
				}
			}

			virtual pair<pair_t, texture_t> getColorPairAndFigureTexture(int, int) const = 0;

		public:
			virtual void step() = 0;

			virtual bool keypress(int) = 0;

			virtual bool stepAvailable() const = 0;

			void draw() const {

				attron(BORDER_COLOR_PAIR);

				// Draw horisontal borders
				if(startY > 0)
					mvaddstr(startY - 1, startX, borderTopLine);
				mvaddstr(startY, startX, borderLine);
				mvaddstr(startY + FULL_HEIGHT - 1, startX, borderLine);
				if(startY + FULL_HEIGHT < SCREEN_HEIGHT)
					mvaddstr(startY + FULL_HEIGHT, startX, borderBottomLine);

				// Draw vertical borders
				const int
						endY = startY + PADDING_HEIGHT + BOARD_HEIGHT,
						endX = startX + PADDING_WIDTH  + BOARD_WIDTH;

				for(int y = startY + 1; y < endY; ++y) {
					move(y, startX);
					for(int i = 0; i < PADDING_WIDTH; ++i) addch(' ');
					move(y, endX);
					for(int i = 0; i < PADDING_WIDTH; ++i) addch(' ');
				}

				for(int i = 0; i < FIELDS; ++i) {
					int y = startY + i * FIELD_HEIGHT + (FIELD_HEIGHT + 1) / 2;
					chtype ch = static_cast<chtype>(i + '1');

					mvaddch(y, startX + PADDING_WIDTH / 2, ch);
					mvaddch(y, endX   + PADDING_WIDTH / 2, ch);
				}

				attroff(BORDER_COLOR_PAIR);

				update();
			}


			void update() const {

				// Draw fields
				for(int h = 0; h < FIELDS; ++h)
					for(int w = 0; w < FIELDS; ++w)
						drawField(h, w);
			}

		protected:
			void drawField(const Pos& pos) const {
				return drawField(pos.y, pos.x);
			}

			void drawField(int h, int w) const {
				const int fieldStartY = boardStartY + h * FIELD_HEIGHT,
				          fieldStartX = boardStartX + w * FIELD_WIDTH;

				move(fieldStartY, fieldStartX);

				pair<pair_t, texture_t> colorPairAndTexture = getColorPairAndFigureTexture(h, w);
				pair_t fieldColorPair = colorPairAndTexture.first;
				texture_t texture = colorPairAndTexture.second;

				attron(fieldColorPair);

				const int fieldEndY = fieldStartY + FIELD_HEIGHT;

				for(int y = fieldStartY; y < fieldEndY; ++y)
					mvaddstr(y, fieldStartX, texture != nullptr ? texture[y - fieldStartY] : fieldLine);

				attroff(fieldColorPair);

				if(h == selectedPos.y && w == selectedPos.x)
					drawFrame(h, w, texture, fieldColorPair, ON_FRAME_COLOR_ID);
				else if(h == offPos.y && w == offPos.x)
					drawFrame(h, w, texture, fieldColorPair, OFF_FRAME_COLOR_ID);
			}

			void drawFrame(int h, int w, texture_t texture, pair_t fieldColorPair, id_t frameColorID) const {
				const int fieldStartY = boardStartY + h * FIELD_HEIGHT,
				          fieldStartX = boardStartX + w * FIELD_WIDTH,

				          fieldEndY = fieldStartY + FIELD_HEIGHT,
				          fieldEndX = fieldStartX + FIELD_WIDTH - 1;

				const pair_t frameColorPair = COLOR_PAIR(frameColorID);

				if(texture == nullptr) {
					attron(fieldColorPair + frameColorPair);

					for(int x = fieldStartX + 1; x < fieldEndX; ++x) {
						mvaddstr(fieldStartY,   x, "▀");
						mvaddstr(fieldEndY - 1, x, "▄");
					}

					attroff(fieldColorPair + frameColorPair);

				} else {

					function<void(int, const char*, bool)> drawHorisontalFrameBorder =
						[=] (int borderStartY, const char* line, bool upper) {
							const char* const pixel = upper ? "▀" : "▄";

							for(int x = fieldStartX; *line != '\0'; ++line, ++x) {
								switch(*line & 0xFF) {
									case 0x20: {
										pair_t colorPair = fieldColorPair + frameColorPair;
										attron(colorPair);
										mvaddstr(borderStartY, x, pixel);
										attroff(colorPair);
										break;
									}

									case 0xE2:
										if((line[1] & 0xFF) == 0x96) {
											id_t colorID, nullID;
											pair_t colorPair;

											switch(line[2] & 0xFF) {
												case 0x80:
												case 0x84:
													upper ^ ((line[2] & 0xFF) == 0x80) ?
														pair_content(fieldColorPair >> NCURSES_ATTR_SHIFT, &colorID, &nullID) :
														pair_content(fieldColorPair >> NCURSES_ATTR_SHIFT, &nullID, &colorID);

													colorPair = COLOR_PAIR(colorID + frameColorID);

													attron(colorPair);
													mvaddstr(borderStartY, x, pixel);
													attroff(colorPair);
											}

											line += 2;
										}

										break;
								}
							}
						};

						drawHorisontalFrameBorder(fieldStartY, texture[0], true);
						drawHorisontalFrameBorder(fieldEndY - 1, texture[FIELD_HEIGHT - 1], false);
					}

					for(int y = fieldStartY; y < fieldEndY; ++y) {
						mvaddch(y, fieldStartX, ' ' | frameColorPair);
						mvaddch(y,   fieldEndX, ' ' | frameColorPair);
					}
				}

				static inline bool isValidPos(const Pos& pos) {
					return pos.y >= 0 && pos.y < FIELDS && pos.x >= 0 && pos.x < FIELDS;
				}

	};

	const char
		*GameBoard::borderLine = nullptr,
		*GameBoard::borderTopLine = nullptr,
		*GameBoard::borderBottomLine = nullptr,
		*GameBoard::fieldLine = nullptr;

	int GameBoard::startY = 0,
	    GameBoard::startX = 0,
	    GameBoard::boardStartY = 0,
	    GameBoard::boardStartX = 0;
}

#endif
