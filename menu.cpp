#ifndef CHESSBOARD_MENU_CPP
#define CHESSBOARD_MENU_CPP

#include "util/ncurseslib.cpp"
#include "colors.cpp"
#include "keys.cpp"
#include <tuple>
#include <functional>
#include <climits>

namespace chessboard {

	using std::pair;
	using std::function;

	struct Menu: Window {

		public:
			const int
					punctHeight, punctWidth,
					menuHeight, menuWidth;

			typedef pair<const char*, const function<void()>> punct_t;

			const char* const name;
			const punct_t* const puncts;
			const size_t menuSize;

			size_t selectedPunct = 0;

			template<size_t menuSize>
			explicit Menu(const char* name, const punct_t(&puncts)[menuSize], int punctHeight, int punctWidth):
					Menu(punctHeight, punctWidth, menuSize * punctHeight + 4, punctWidth + 4, name, puncts, menuSize) {}

		protected:
			explicit Menu(int punctHeight, int punctWidth, int menuHeight, int menuWidth, const char* name, const punct_t* puncts, size_t menuSize):
					punctHeight(punctHeight), punctWidth(punctWidth), menuHeight(menuHeight), menuWidth(menuWidth), name(name), puncts(puncts), menuSize(menuSize),
					Window(menuHeight, menuWidth, (LINES - menuHeight) / 2, (COLS - menuWidth) / 2) {

				keypadOn();
			}

		public:
			inline void drawAndRun() {
				draw();
				run();
			}

			void draw() {
				attrOn(BORDER_COLOR_PAIR);

				drawPlainBorder(0);
				drawHorisontalLine(1, 2, punctWidth, ' ', 2);
				addStrTo(1, (menuWidth - /*mblen(name, MB_CUR_MAX)*/strlen(name)) / 2, name);

				attrOff(BORDER_COLOR_PAIR);

				for(size_t i = 0; i < menuSize; ++i)
					drawPunct(i);
			}

			void drawPunct(size_t punctID) {
				const pair_t colorPair = punctID == selectedPunct ? SELECTED_TEXT_COLOR_PAIR : TEXT_COLOR_PAIR;

				attrOn(colorPair);

				const int startY = punctID * punctHeight + 3,
				          endY = startY + punctHeight;

				fill(startY, 2, punctHeight, punctWidth, ' ');

				const char* punctText = puncts[punctID].first;

				addStrTo(startY + punctHeight / 2, 1 + (punctWidth - /*mblen(punctText, MB_CUR_MAX)*/strlen(punctText) / 2) / 2, punctText);

				attrOff(colorPair);
			}

			void run() {

				while(true) {
					switch(this->getChar()) {
						case KEY_UP:
							if(selectedPunct > 0) {
								selectedPunct -= 1;
								drawPunct(selectedPunct + 1);
								drawPunct(selectedPunct);
							}

							break;

						case KEY_DOWN:
							if(selectedPunct < menuSize - 1) {
								selectedPunct += 1;
								drawPunct(selectedPunct - 1);
								drawPunct(selectedPunct);
							}

							break;

						case KEY_ENTER: case KEY_SPACE:
							puncts[selectedPunct].second();
							return;
					}
				}
			}
	};
}

#endif
