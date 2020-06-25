#include <iostream>
#include <vector>
#include <thread>
#include <chrono>
#include <conio.h>
using namespace std;

class MyGame
{
public:
	// Темп игры
	int tempo = 50;
	// Включение бота true - включен
	bool autoBot = false;
	// Очки
	int scoreLeft = 0;
	int scoreRight = 0;

	void fillDesk();
	void showDesk();
	void resetDesk();
	void getBallPosition();
	void getPaddleLeftPosition();
	void getPaddleRightPosition();
	void moveBot(int tempPos);
	void padMove();
	void ballMove();
	void botPlay();
private:
	vector<vector<char>> desk;
	int leftPos;
	int rightPos;
	// Символы
	const char border = 177;	// символ рамки
	const char paddle = 219;	// символ ракетки
	const char ball = 249;	// символ шарика
	// Траектории движения мяча
	bool LTR = false; // налево
	bool DIAG = false; // по диагонали
	bool UP = false; // вверх
	int X;
	int Y;
	// Положение бота
	enum posBot {
		posUpper,
		posMiddle,
		posBottom
	} posBot;
};

// Функция забивания массива игрового поля
void MyGame::fillDesk() {
	// Верхние пустые ряды
	vector<char> rowsUp;
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 35; j++) {
			rowsUp.emplace_back(' ');
		}
		desk.push_back(rowsUp);
	}
	// Середина с ракетками
	vector<char> rowsPaddle;
	for (int i = 0; i < 5; i++) {
		rowsPaddle.emplace_back(paddle);
		for (int j = 0; j < 33; j++) {
			rowsPaddle.emplace_back(' ');
		}
		rowsPaddle.emplace_back(paddle);
		desk.push_back(rowsPaddle);
	}
	// Нижние пустые ряды
	vector<char> rowsDown;
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 35; j++) {
			rowsDown.emplace_back(' ');
		}
		desk.push_back(rowsDown);
	}
	// Начальное положение шарика
	srand(static_cast<unsigned int>(time(0)));
	desk[rand() % 4 + 5][1] = 'o';
}

// Функция отрисовки игрового поля
void MyGame::showDesk() {
	for (int m = 0; m < 39; m++) { cout << border; } // Верхний ряд
	// Середина
	cout << endl;
	for (int i = 0; i < 15; i++) {
		cout << border << border;
		for (int j = 0; j < 35; j++) { cout << desk[i][j]; }
		cout << border << border << endl;
	}
	for (int n = 0; n < 39; n++) { cout << border; } // Нижний ряд
	cout << endl;
	cout << "SCORES:\n" << "LEFT:\t" << scoreLeft << "\nRIGHT:\t" << scoreRight << endl;
}

// Функция управления ракеткой
void MyGame::padMove() {
	if (_kbhit()) {
		char but = _getch();
		// Движение вверх
		int row;
		if (but == 'a') {
			if (but == 'a') row = 0;
			for (int i = 0; i < 15; i++) {
				if (desk[0][row] != paddle) {
					if (desk[i][row] == paddle) {
						desk[i - 1][row] = paddle;
						desk[i + 4][row] = ' ';
						break;
					}
				}
			}
		}
		// Движение вниз
		else if (but == 'z') {
			if (but == 'z') row = 0;
			for (int i = 14; i >= 0; i--) {
				if (desk[14][row] != paddle) {
					if (desk[i][row] == paddle) {
						desk[i + 1][row] = paddle;
						desk[i - 4][row] = ' ';
						break;
					}
				}
			}
		}
		if (!autoBot) {
			// Движение вверх
			int row;
			if (but == 'k') {
				if (but == 'k') row = 34;
				for (int i = 0; i < 15; i++) {
					if (desk[0][row] != paddle) {
						if (desk[i][row] == paddle) {
							desk[i - 1][row] = paddle;
							desk[i + 4][row] = ' ';
							break;
						}
					}
				}
			}
			// Движение вниз
			else if (but == 'm') {
				if (but == 'm') row = 34;
				for (int i = 14; i >= 0; i--) {
					if (desk[14][row] != paddle) {
						if (desk[i][row] == paddle) {
							desk[i + 1][row] = paddle;
							desk[i - 4][row] = ' ';
							break;
						}
					}
				}
			}
		}
	}
}

// Функция сброса положения ракеток и шарика и темпа на начальное положение
void MyGame::resetDesk() {
	for (int i = 0; i < 15; i++) {
		desk[i][0] = ' ';
		desk[i][34] = ' ';
		if (i == 5 || i == 6 || i == 7 || i == 8 || i == 9) {
			desk[i][0] = paddle;
			desk[i][34] = paddle;
		}
	}
	for (int i = 0; i < 15; i++) {
		for (int j = 0; j < 35; j++) {
			if (desk[i][j] == 'o') {
				desk[i][j] = ' ';
			}
		}
	}
	srand(static_cast<unsigned int>(time(0)));
	desk[rand()%4 + 5][1] = 'o';
	tempo = 70;
}

// Функция взятия положения шарика
void MyGame::getBallPosition() {
	for (int i = 0; i < 15; i++) {
		for (int j = 0; j < 35; j++) {
			if (desk[i][j] == 'o') {
				X = i;
				Y = j;
			}
		}
	}
}

// Функция взятия положения верхнего края левой ракетки
void MyGame::getPaddleLeftPosition() {
	for (int i = 0; i < 15; i++) {
		if (desk[i][0] == paddle) {
			leftPos = i;
			break;
		}
	}
}

// Функция взятия положения верхнего края правой ракетки
void MyGame::getPaddleRightPosition() {
	for (int i = 0; i < 15; i++) {
		if (desk[i][34] == paddle) {
			rightPos = i;
			break;
		}
	}
}

// Функция смещения бота
void MyGame::moveBot(int tempPos) {
	for (int i = 0; i < 15; i++) { desk[i][34] = ' '; }
	int fillNum;
	if (tempPos == posUpper) { fillNum = 0; }
	else if (tempPos == posMiddle) { fillNum = 5; }
	else if (tempPos == posBottom) { fillNum = 10; }
	for (int i = fillNum; i < fillNum + 5; i++) { desk[i][34] = paddle; }
}

// Интелект бота (c поддачей)
void MyGame::botPlay() {
	if (Y == 29) {
		if ((UP && X < 10 && X != 0) || (!DIAG && X < 5)) { MyGame::moveBot(posUpper); }
		else if (((UP && X <= 15 && X > 10) || X == 0) || (!DIAG && X >= 5 && X < 10)) { MyGame::moveBot(posMiddle); }
		else if ((!UP && X < 15 && X >= 5) || (!DIAG && X >= 10)) { MyGame::moveBot(posBottom); }
	}
}

// Функция движения шарика
void MyGame::ballMove() {
	MyGame::getBallPosition();
	MyGame::getPaddleLeftPosition();
	MyGame::getPaddleRightPosition();
	// ЛОГИКА
	// 1. У левой ракетки
	if (Y == 1) {
		// 1.1. Пропуск левой ракеткой
		if (desk[X][Y - 1] == ' ') {
			++scoreRight;
			MyGame::resetDesk();
			return;
		}
		else {
			LTR = true; // Движение направо
			if (leftPos > 4 && leftPos < 6) {DIAG = false;}	// 1.2. Движение прямо
			else {
				DIAG = true;
				if (leftPos >= 6) {UP = true;}	// 1.3. Движение по диагонали вверх
				else {UP = false;}	// 1.4. Движение по диагонали вниз
			}
		}
	}
	// 2. У правой ракетки
	if (Y == 33) {
		// 2.1. Пропуск правой ракеткой
		if (desk[X][Y + 1] == ' ') {
			++scoreLeft;
			resetDesk();
			return;
		}
		// Движение налево
		else {
			LTR = false;	// 2.2. Движение прямо
			if (rightPos > 4 && rightPos < 6) {DIAG = false;}
			else {
				DIAG = true;
				if (rightPos >= 6) {UP = true;}	// 2.3. Движение по диагонали вверх
				else {UP = false;}	// 2.4. Движение по диагонали вниз
			}
		}
	}
	if (X < 1) {UP = false;}	// Мяч отбился от верхней стенки
	if (X > 13) {UP = true;}	// Мяч отбился от нижней стенки
	if (autoBot) botPlay();

	// САМИ ХОДА
	if (LTR) {
		if (DIAG && UP) {
			desk[X][Y] = ' ';
			desk[X - 1][Y + 1] = 'o';
		}
		else if (DIAG && !UP) {
			desk[X][Y] = ' ';
			desk[X + 1][Y + 1] = 'o';
		}
		else {
			desk[X][Y] = ' ';
			desk[X][Y + 1] = 'o';
		}
	}
	if (!LTR) {
		if (DIAG && UP) {
			desk[X][Y] = ' ';
			desk[X - 1][Y - 1] = 'o';
		}
		else if (DIAG && !UP) {
			desk[X][Y] = ' ';
			desk[X + 1][Y - 1] = 'o';
		}
		else {
			desk[X][Y] = ' ';
			desk[X][Y - 1] = 'o';
		}
	}
}

int main()
{
	MyGame game;
	char but;
	do {
		game.fillDesk();
		_kbhit();
		cout << "Press 'b' to play with BOT\nPress 't' to play with friend\n";
		but = _getch();
		if (but == 'b') game.autoBot = true;
		else if (but == 't') game.autoBot = false;
		do {
			system("cls");
			game.ballMove();
			game.padMove();
			game.showDesk();
			this_thread::sleep_for(chrono::milliseconds(game.tempo));
			game.tempo--;
			if (game.tempo < 5) game.tempo = 5;
		} while (game.scoreLeft != 3 && game.scoreRight != 3);
		const char* winner;
		(game.scoreLeft == 3) ? (winner = "LEFT") : (winner = "RIGHT");
		cout << winner << " wins!\nPress 'q' to quit\nPress 'r' to restart\n";
		but = _getch();
		if (but == 'q') { cout << "\nThank you for game!\n"; break; }
		if (but == 'r') {
			game.resetDesk();
			game.scoreLeft = game.scoreRight = 0;
			continue;
		}
	} while (but != 'q');

}