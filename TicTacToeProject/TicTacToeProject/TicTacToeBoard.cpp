#include "TicTacToeBoard.h"



TicTacToeBoard::TicTacToeBoard(int size)
{
	assert(size >= 3); //the game would be boring if size was 2
					   //first player would win automatically at size 1
	boardSize = size;
	board = new char[boardSize*boardSize];
	for (int i = 0; i < boardSize*boardSize; i++)
		board[i] = BLANK;
	//accessing this single block of information
	//(r * boardSize) + c
}


TicTacToeBoard::~TicTacToeBoard()
{
	delete [] board;
}

int TicTacToeBoard::getSize() {
	return boardSize;
}

bool TicTacToeBoard::isGameOver() {
	//checks if an end state is reached
	//an end state is any win state, or if the board is completely filled
	//A win state is when a row, a column, or one of the board's diagonals 
	//is filled with the same symbol.

	int count = 0;
	for (int i = 0; i < boardSize; i++) {
		for (int j = 0; j < boardSize; j++) {
			if (board[(i * boardSize) + j] != BLANK)
				count++;
		}
	}
	
	if (theWinner == BLANK && count == (boardSize *boardSize)) {
		theWinner = CAT;
	}
	return theWinner != BLANK;
}

char TicTacToeBoard::whoWon() {
	return theWinner; //Returns the winner, if there is one.
}

bool TicTacToeBoard::changeBoard(char symbol, int r, int c) {
	if (r < 0 || r >= boardSize)
		return false;
	if (c < 0 || c >= boardSize)
		return false; 

	if (board[(r * boardSize) + c] == BLANK) {
		board[(r * boardSize) + c] = symbol;
		checkWin(symbol, r, c);
		return true;
	}
	return false;
}

void TicTacToeBoard::clearBoard() {
	for (int i = 0; i < boardSize; i++) {
		for (int j = 0; j < boardSize; j++) {
			board[(i * boardSize) + j] = BLANK;
		}
	}
	theWinner = BLANK;
}

void TicTacToeBoard::printBoard() {
	printBorder();
	std::cout << std::endl;
	for (int i = 0; i < boardSize; i++) {
		for (int j = 0; j < boardSize; j++) {
			std::cout << "| " << board[(i * boardSize) + j] << " ";
		}
		std::cout << "|\n";
	}
	printBorder();

	std::cout << std::endl;
}

void TicTacToeBoard::printBorder() {
	for (int i = 0; i < 4 * boardSize + 1; i++) {
		std::cout << "-";
	}
}

void TicTacToeBoard::checkWin(char symbol, int r, int c) {
	//check to see if the (r,c) coordinates are on the diagonals
	//(r,c) is on a diagonal if r == c 
	//or if the sum of r and c add up to boardSize - 1 (never both)
	//Count the number of symbol row r, col c, and the diagonal if applicable

	bool leftDiagonal = (r == c);
	bool rightDiagonal = ((r + c) == (boardSize - 1));

	int symbolCountRow = 0;
	int symbolCountCol = 0;
	int symbolCountLeftDiag = 0;
	int symbolCountRightDiag = 0;

	//row r
	for (int j = 0; j < boardSize; j++) {
		if (board[(r * boardSize) + j] == symbol) {
			symbolCountRow++;
		}
	}

	//col c
	for (int i = 0; i < boardSize; i++) {
			if (board[(i * boardSize) + c] == symbol) 
				symbolCountCol++;
		
	}

	if (leftDiagonal) {
		for (int d = 0; d < boardSize; d++) {
			if (board[(d * boardSize) + d] == symbol) {
				symbolCountLeftDiag++;
			}
		}
	}

	if (rightDiagonal) {
		int z = boardSize - 1;
		for (int d = 0; d < boardSize; d++) {
			if (board[(d * boardSize) + z] == symbol) {
				symbolCountRightDiag++;
			}
			z--;
		}
	}

	if (symbolCountRow == boardSize ||
		symbolCountCol == boardSize ||
		symbolCountLeftDiag == boardSize ||
		symbolCountRightDiag == boardSize) {
		theWinner = symbol;
	}

}