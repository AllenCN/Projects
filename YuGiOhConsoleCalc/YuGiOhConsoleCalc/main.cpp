#include <iostream>
#include <fstream>
#include <windows.h>
#include <iomanip>
#include <ctime>
#include <string>
#include <vector>

using namespace std;

struct player {
	int id;
	string name;
	int lp;
};

const int BLUE = 0x90;
const int YELLOW = 0xE0;
const int RED = 0xC0;
const int BLACK = 0x0F;

int get_player_with_name(const string &search, vector<player> &p) {
	for (unsigned int i = 0; i < p.size(); i++) {
		if (p[i].name == search) {
			return i;
		}
	}
	
	return -1;
}

int get_player_with_id(const int search, vector<player> &p) {
	for (unsigned int i = 0; i < p.size(); i++) {
		if (p[i].id == search) {
			return i;
		}
	}

	return -1;
}

string increase_decrease_lp(vector<player> &p, int id) {
	int val;
	cout << "Enter lp to lose (-number for down, number for up)\n>";
	cin >> val;
	string ret;
	ret += "(" + p[id].name + " " + to_string(p[id].lp) + " -> ";
	p[id].lp += val;

	if (p[id].lp < 0)
		p[id].lp = 0;

	ret += to_string(p[id].lp) + ")";
	cin.ignore(1);
	return ret;
}

string removePlayers(vector<player> &p) {
	string ret = "";
	for (vector<player>::iterator it = p.begin(); it != p.end(); it++) {
		if ((*it).lp == 0)
		{
			ret += (*it).name + " was defeated.\n";

			p.erase(it);
			
			it = p.begin();
		}
	}
	return ret;
}

void print_players(vector<player> &p, HANDLE &hConsole, const int &startLP) {
	int lpChunk = startLP / 100;
	for (vector<player>::iterator it = p.begin(); it != p.end(); it++) {
		int numChunks = (*it).lp / lpChunk;
		if (numChunks > 100) {
			numChunks = 100;
		}
		int counter = 0;
		cout << "Player " << (*it).id << "\t" << (*it).name << "\t" << setfill('0') << setw(4) 
			<< (*it).lp << endl;
		cout << "[";
		if ((*it).lp > startLP) {
			SetConsoleTextAttribute(hConsole, BLUE);
		}
		else {
			SetConsoleTextAttribute(hConsole, RED);
		}
		while (counter < numChunks) {
			cout << " ";
			counter++;
			if (counter == 33 && (*it).lp <= startLP)
			{
				SetConsoleTextAttribute(hConsole, YELLOW);
			}
			if (counter == 67 && (*it).lp <= startLP) {
				SetConsoleTextAttribute(hConsole, BLUE);
			}
		}
		SetConsoleTextAttribute(hConsole, BLACK);
		if (numChunks < 100) {
			int spaces = 100 - numChunks;
			for (int i = 0; i < spaces; i++)
				cout << " ";
		}
		cout << "]" << endl;
	}
}

int main() {

	HANDLE hConsole;

	int players;
	vector<player> inBattle;
	int turn = 1;
	int STARTLP;
	
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	cout << "Enter number of players:\n>";
	cin >> players;
	cin.ignore(1);
	if (players < 2) {
		cout << "You can't play Yugioh by yourself!" << endl;
		return EXIT_FAILURE;
	}
	
	cout << "Enter starting LP:\n>";
	cin >> STARTLP;
	cin.ignore(1);
	if (STARTLP < 1) {
		cout << "You need at least some LP..." << endl;
		return EXIT_FAILURE;
	}

	int pid = 1;
	cout << "Line by line, please enter the player's name: " << endl;
	
	while (pid <= players) {
		player p;
		p.id = pid;
		p.lp = STARTLP;
		cout << "Player " << pid << "'s name:>";
		getline(cin, p.name);
		inBattle.push_back(p);
		pid++;
	}
	print_players(inBattle, hConsole, STARTLP);
	time_t timestamp;
	time(&timestamp);
	
	string fileName = "duel" + to_string(timestamp) + ".txt";

	ofstream fileOut;
	fileOut.open(fileName);
	if (!fileOut.is_open()) {
		cout << "Error writing to disk" << endl;
		return EXIT_FAILURE;
	}
	string line;
	while (inBattle.size() > 1) {
		line.clear();
		int pIndex = (turn - 1) % inBattle.size();
		cout << "Turn " << turn << ": " << inBattle[pIndex].name << endl;
		fileOut << "Turn " << turn << endl;
		
		cout << "Enter \"end\" to end the turn.\n";
		cout << "Otherwise, enter player's name or their id.\n>";
		getline(cin, line);
		
		if (line == "end") {
			cout << inBattle[pIndex].name << " ends his turn." << endl;
			fileOut << inBattle[pIndex].name << " ends his turn." << endl;
			turn++;
		}
		else {
			bool is_int = (line.find_first_not_of("0123456789") == string::npos);
			int index = -1;
			if (is_int) {
				index = get_player_with_id(stoi(line), inBattle);
			}
			else {
				index = get_player_with_name(line, inBattle);
			}
			if (index != -1) {
				string output = increase_decrease_lp(inBattle, index);
				print_players(inBattle, hConsole, STARTLP);
				fileOut << "\t" << output << endl;
				output = removePlayers(inBattle);
				if (!output.empty()) {
					cout << output << endl;
					fileOut << "\t" << output << endl;
				}
			}
			else {
				cout << "Assumed line entered is a note." << endl;
				cout << line << endl;
				fileOut << "\t" << line << endl;
			}
		}
	}
	if (inBattle.size() == 1) {
		cout << inBattle[0].name << " wins!" << endl;
		fileOut << inBattle[0].name << " wins!" << endl;
	}
	else {
		cout << "Duel ends in a DRAW." << endl;
		fileOut << "Duel ends in a DRAW." << endl;
	}
	fileOut.close();

	return EXIT_SUCCESS;
}