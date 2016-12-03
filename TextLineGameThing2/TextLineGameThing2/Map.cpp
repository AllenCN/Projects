#include "Map.h"



Map::Map()
{
}


Map::~Map()
{
}

void Map::buildMap(vector<Room> &tmp) {
	for (unsigned int i = 0; i < tmp.size(); i++) {
		rooms[tmp[i].id] = tmp[i];
	}
	numOfRooms = tmp.size();
	tmp.clear();
	find_doors();
}

int Map::do_move(int playerLoc, int dir) {

	if (rooms[playerLoc].exits_to_room[dir] != NOEXIT) {
		if (!passageBlocked(playerLoc, rooms[playerLoc].exits_to_room[dir]))
			playerLoc = rooms[playerLoc].exits_to_room[dir];
	}

	return playerLoc;
}

vector<string> Map::do_look(int playerLoc) {
	vector<string> text;
	string dirs[] = { "north", "east", "south", "west" };
	text.push_back(rooms[playerLoc].name);
	text.push_back(rooms[playerLoc].desc);
	
	for (int i = 0; i < 4; i++) {
		if (rooms[playerLoc].exits_to_room[i] != NOEXIT) {
			text.push_back("There is an exit " + dirs[i] + " to " + rooms[rooms[playerLoc].exits_to_room[i]].name + ".");
		}
	}

	if (!rooms[playerLoc].roomItems.empty()) {
		for (unsigned int i = 0; i < rooms[playerLoc].roomItems.size(); i++) {
			text.push_back("There is a(n) " + rooms[playerLoc].roomItems[i] + " nearby.");
		}
	}

	if (!rooms[playerLoc].npcNames.empty()) {
		for (unsigned int i = 0; i < rooms[playerLoc].npcNames.size(); i++) {
			text.push_back(rooms[playerLoc].npcNames[i] + " is close by.");
		}
	}

	return text;
	
}

bool Map::do_get(int playerLoc, string itemName, vector<pair<string, int>> &inv) {
	vector<string>::iterator it;
	it = find(rooms[playerLoc].roomItems.begin(), rooms[playerLoc].roomItems.end(), itemName);
	if (it == rooms[playerLoc].roomItems.end()) {
		return false;
	}
	else {
		unsigned int loc;
		for (loc = 0; loc < inv.size(); loc++) {
			if (inv[loc].first == itemName)
			{
				break;
			}
		}
		if (loc < inv.size()) {
			inv[loc].second += 1;
		}
		else {
			inv.push_back(make_pair(itemName, 1));
		}
		rooms[playerLoc].roomItems.erase(it);
		return true;
	}
}

bool Map::do_drop(int playerLoc, string itemName, vector<pair<string, int>> &inv) {
	unsigned int loc;
	for (loc = 0; loc < inv.size(); loc++) {
		if (inv[loc].first == itemName)
		{
			break;
		}
	}
	if (loc == inv.size())
		return false;
	else {
		if (inv[loc].second > 1) {
			inv[loc].second -= 1;
		}
		else {
			inv.erase(inv.begin() + loc);
		}
		rooms[playerLoc].roomItems.push_back(itemName);
		return true;
	}

}

bool Map::check_noun(int playerLoc, string itemName) {
	vector<string>::iterator it;
	it = find(rooms[playerLoc].roomItems.begin(), rooms[playerLoc].roomItems.end(), itemName);
	return (it != rooms[playerLoc].roomItems.end());
}

bool Map::check_NPC(int playerLoc, string NPCname) {
	vector<string>::iterator it;
	it = find(rooms[playerLoc].npcNames.begin(), rooms[playerLoc].npcNames.end(), NPCname);
	return (it != rooms[playerLoc].npcNames.end());
}

bool Map::do_open(int playerLoc, string trig) {
	for (auto i = 0; i < doors.size(); i++) {
		if ((doors[i].trigger == trig) && ((doors[i].between.first == playerLoc) 
			|| (doors[i].between.second == playerLoc))) {
			if (doors[i].open)
			{
				return false;
			}
			doors[i].open = true;
			return true;
		}
	}
	return false;
}

bool Map::do_close(int playerLoc, string trig) {
	for (auto i = 0; i < doors.size(); i++) {
		if ((doors[i].trigger == trig) && ((doors[i].between.first == playerLoc)
			|| (doors[i].between.second == playerLoc))) {
			if (!doors[i].open)
			{
				return false;
			}
			doors[i].open = false;
			return true;
		}
	}
	return false;
}

void Map::find_doors() {
	vector<string>::iterator it;
	vector<vector<string>::iterator> itList;
	vector<string>::iterator it2;

	for (int i = 0; i < numOfRooms; i++) {
		it = find(rooms[i].roomItems.begin(), rooms[i].roomItems.end(), "GATE");
		if (it != rooms[i].roomItems.end())
		{
			itList.push_back(it);
			int ret = find_door_adj(i);
			if (ret != -1)
			{
				doorInfo d;
				d.trigger = "GATE";
				d.between = make_pair(i, ret);
				d.open = false;
				doors.push_back(d);
				return; //only good for one right now
			}
		}
	}
}

int Map::find_door_adj(int room) {
	for (int i = 0; i < 4; i++) {
		vector<string>::iterator it;
		it = find(rooms[i].roomItems.begin(), rooms[i].roomItems.end(), "GATE");
		if (it != rooms[i].roomItems.end()) {
			return rooms[room].exits_to_room[i];
		}
	}
	return -1; //somehow did not work
}

bool Map::passageBlocked(int room, int exit) {
	for (auto i = 0; i < doors.size(); i++) {
		if ((doors[i].between.first == room && doors[i].between.second == exit) ||
			(doors[i].between.second == room && doors[i].between.first == exit)) {
			return !doors[i].open;
		}
	}
	return false;
}

bool Map::check_encounter(int playerLoc, double rng) {
	return rng <= rooms[playerLoc].encRate;
}