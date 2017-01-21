#include <iostream>
#include <string>
#include <random>

#include "TicTacToeBoard.h"

int main() {
	//Greet user
	std::cout << "Welcome to Tic-Tac-Toe Infinity, a tic-tac-toe game\n ";
	std::cout << "which lets you control the size of the board!\n";
	std::cout << "Once the settings have been set, you cannot change them.\n";
	std::cout << std::endl;
	//prompt user for 1 or 2 players
	std::cout << "First, one or two players? \n>";
	int numPlayers;
	std::cin >> numPlayers;
	assert(numPlayers > 0 && numPlayers <= 2);
	//prompt user for size of board
	std::cout << "Next, the size of the board? (Must be at least 3) \n>";
	int size;
	std::cin >> size;
	std::cin.clear();
	std::cin.ignore(INT_MAX, '\n');
	TicTacToeBoard gameBoard(size);
	bool isPlayerOnesTurn;
	//Player 1 is always X and will always be human, Player 2 is always O and is the AI if 1 player selected

	std::default_random_engine rng;
	std::uniform_int_distribution<int> die(1, 6);
	std::uniform_int_distribution<int> rnd_ai(1, 100);
	std::pair<int, int> rolls; //rolls.first will always be p1, rolls.second will be 2.

	std::cout << "Player 1 is X; Player 2 is O." << std::endl;
	std::cout << "A die is rolled to determine who goes first. If there is a tie,\n"
		<< "then Player 1 will go first." << std::endl;

	std::cout << "Player 1, please press enter!" << std::endl;
	std::cin.get();
	rolls.first = die(rng);

	if (numPlayers == 2) {
		std::cout << "Player 2, please press enter!" << std::endl;
		std::cin.get();
		rolls.second = die(rng);
	}
	else {
		rolls.second = die(rng);
	}

	std::cout << "Player 1 rolled " << rolls.first << "!" << std::endl;
	std::cout << "Player 2 rolled " << rolls.second << "!" << std::endl;

	if (rolls.first >= rolls.second) {
		std::cout << "Player 1 goes first!" << std::endl;
		isPlayerOnesTurn = true;
	}
	else {
		std::cout << "Player 2 goes first!" << std::endl;
		isPlayerOnesTurn = false;
	}

	bool quit = false;
	gameBoard.printBoard();
	while (!quit) {
		int row, col;
		if (isPlayerOnesTurn || numPlayers == 2) {
			char symbol = (isPlayerOnesTurn) ? 'X' : 'O';
			do {
				std::cout << "Please enter a move, player! (0 starts a row or column)" << std::endl;
				std::cin >> row;
				std::cin >> col;

			} while (!gameBoard.changeBoard(symbol, row, col));
			gameBoard.printBoard();
			isPlayerOnesTurn = !isPlayerOnesTurn;
			if (gameBoard.isGameOver()) {
				std::cin.clear();
				std::cin.ignore(INT_MAX, '\n');
				std::cout << "The game is over! ";
				std::cout << gameBoard.whoWon() << " wins!\n";
				std::cout << "Do you wish to reset the game board and try again? (y/n)" << std::endl;
				char answer;
				answer = std::cin.get();
				if (answer == 'y') {
					std::cout << "Clearing everything.\n";
					gameBoard.clearBoard();
				}
				else {
					quit = true;
					break;
				}
			}
		}

		if (!isPlayerOnesTurn && numPlayers < 2) {
			//AI makes a move
			do {
				row = rnd_ai(rng) % size;
				col = rnd_ai(rng) % size;
			} while (!gameBoard.changeBoard('O', row, col));
			isPlayerOnesTurn = !isPlayerOnesTurn;
			gameBoard.printBoard();
			//Once a game is over, ask the user y/n do they want to reset
			if (gameBoard.isGameOver()) {
				std::cin.clear();
				std::cin.ignore(INT_MAX, '\n');
				std::cout << "The game is over! ";
				std::cout << gameBoard.whoWon() << " wins!\n";
				std::cout << "Do you wish to reset the game board and try again? (y/n)" << std::endl;
				char answer;
				answer = std::cin.get();
				if (answer == 'y') {
					std::cout << "Clearing everything.\n";
					gameBoard.clearBoard();
				}
				else {
					quit = true;
					break;
				}
			}
		}
	}
	std::cout << "Thank you for playing, goodbye!" << std::endl;
	return 0;
}