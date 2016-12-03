#include "Room.h"



Room::Room()
{
	id = -1;
	name = "NOROOM";
	desc = "NOTHING";
	for (int i = 0; i < 4; i++) {
		exits_to_room[i] = -1;
	}
}

Room::Room(vector<string> &line) {
	assert(line.size() >= 7);

	id = stoi(line[0]);
	name = line[1];
	desc = line[2];
	for (int i = 0; i < 4; i++) {
		exits_to_room[i] = stoi(line[i + 3]);
	}

	if (line[7] != "N/A") {
		encRate = stod(line[7]);
	}

	if (line[8] != "N/A") {
		//there are nouns in the room
		roomItems = room_split(line[8]);
	}

	if (line[9] != "N/A") {
		//there are npcs in the room
		npcNames = room_split(line[9]);
	}

	if (line[10] != "N/A") {
		//enemies can spawn here.
		enemynames = room_split(line[10]);
	}
}

Room::~Room()
{
	
}

string Room::to_string() {
	string ret;
	ret += "ID = " + std::to_string(id);
	ret += "\nNAME = " + name;
	ret += "\nDESC = " + desc;
	ret += "\nNORTH EXIT ID = " + std::to_string(exits_to_room[0]);
	ret += "\nEAST EXIT ID = " + std::to_string(exits_to_room[1]);
	ret += "\nSOUTH EXIT ID = " + std::to_string(exits_to_room[2]);
	ret += "\nWEST EXIT ID = " + std::to_string(exits_to_room[3]);
	ret += "\nENCOUNTER RATE = " + std::to_string(encRate);
	ret += "\n";
	for (unsigned int i = 0; i < enemynames.size(); i++) {
		ret += enemynames[i] + ", ";
	}
	ret += "\n";
	return ret;
}

vector<string> Room::room_split(string text, char sep) {
	std::vector<std::string> tokens;
	std::size_t start = 0, end = 0;
	while ((end = text.find(sep, start)) != std::string::npos) {
		if (end != start) {
			tokens.push_back(text.substr(start, end - start));
		}
		start = end + 1;
	}
	if (end != start) {
		tokens.push_back(text.substr(start));
	}
	return tokens;
}