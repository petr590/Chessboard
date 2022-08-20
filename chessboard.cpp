#include "menu.cpp"
#include "checkers-board.cpp"
#include "segfault-handler.cpp"
#include "keys.cpp"
#include "log.cpp"

#include <iostream>
#include <locale.h>

int main() {

	using namespace chessboard;
	using std::cout;
	using std::cerr;
	using std::endl;

	atexit(&ncurses_end);
	signal(SIGSEGV, &segfaultHandler);

	setlocale(LC_ALL, "");

	try {
		ncurses_start();

		if(COLS < GameBoard::FULL_WIDTH && LINES < GameBoard::FULL_HEIGHT) {
			ncurses_end();
			cerr << "Screen too small, minimum size is " << GameBoard::FULL_WIDTH << 'x' << GameBoard::FULL_HEIGHT << " chars" << endl;
			return 1;
		}

		GameBoard::initStatic();

		start_color();

		init_color_pairs();

		CAN_LOG = true;

		GameBoard* board = nullptr;

		{ // Menu

			static constexpr int
					MENU_PUNCT_HEIGHT = 3, MENU_PUNCT_WIDTH = 36;

			//const function<void()> selectSide

			Menu mainMenu("Chess Board", {
					{"Новая игра",
						[&board] () {
							Menu gameMenu("", {
									{"Шашки", [&board] () { board = new CheckersBoard(Side::WHITE); }},
									{"Шахматы", [&board] () { /*board = new ChessBoard(Side::WHITE);*/ /* TODO */ }},
								}, MENU_PUNCT_HEIGHT, MENU_PUNCT_WIDTH);

							gameMenu.drawAndRun();
						}
					},

				}, MENU_PUNCT_HEIGHT, MENU_PUNCT_WIDTH);

			mainMenu.drawAndRun();
		}

		if(board != nullptr) {
			board->draw();

			while(board->stepAvailable()) {
				if(board->keypress(getch()))
					board->step();
			}

			board->showResult();

			delete board;
		}

		ncurses_end();

	} catch(const string& message) {
		ncurses_end();
		cout << "Error: " << message << endl;
		throw;

	} catch(const char* message) {
		ncurses_end();
		cout << "Error: " << message << endl;
		throw;

	} catch(...) {
		ncurses_end();
		throw;
	}
}
