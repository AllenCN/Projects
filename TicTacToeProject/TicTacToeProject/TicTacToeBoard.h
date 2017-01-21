#pragma once
#include <iostream>
#include <assert.h>
class TicTacToeBoard
{
public:
	TicTacToeBoard(int size = 3);
	~TicTacToeBoard();
	
	const char BLANK = ' ';
	const char XSYMBOL = 'X';
	const char OSYMBOL = 'O';

	const char CAT = 'C';

	int getSize();
	bool isGameOver();
	char whoWon();

	bool changeBoard(char symbol, int r, int c);
	void clearBoard();
	void printBoard();

private:
	int boardSize;
	char* board;
	char theWinner = BLANK;

	void printBorder();

	void checkWin(char symbol, int r, int c);
};

