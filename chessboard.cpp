#include "checkers-board.cpp"
#include "segfault-handler.cpp"
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

	try {
		setlocale(LC_ALL, "");

		ncurses_start();

		if(COLS < GameBoard::FULL_WIDTH && LINES < GameBoard::FULL_HEIGHT) {
			ncurses_end();
			cerr << "Screen too small, minimum size is " << GameBoard::FULL_WIDTH << 'x' << GameBoard::FULL_HEIGHT << " chars" << endl;
			return 1;
		}

		GameBoard::initStatic();

		start_color();

		init_color_pairs();


		CheckersBoard board(Side::WHITE);
		board.draw();

		CAN_LOG = true;

		while(board.stepAvailable()) {
			if(board.keypress(getch()))
				board.step();
		}

		board.printResult();
		getch();

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
