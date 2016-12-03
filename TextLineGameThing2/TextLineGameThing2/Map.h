#pragma once
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#ifdef _DEBUG
#define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define new DEBUG_NEW
#endif
#include "Room.h"
#include "Enemy.h"
struct doorInfo {
	string trigger;
	pair<int, int> between;
	bool open;
};
class Map
{
public:
	Map();
	~Map();

	void buildMap(vector<Room> &tmp);
	int do_move(int playerLoc, int dir);
	vector<string> do_look(int playerLoc);
	bool do_get(int playerLoc, string itemName, vector<pair<string, int>> &inv);
	bool do_drop(int playerLoc, string itemName, vector<pair<string, int>> &inv);
	bool check_noun(int playerLoc, string itemName);
	bool check_NPC(int playerLoc, string NPCName);
	bool passageBlocked(int roomid, int exit);

	bool do_open(int playerLoc, string trig);
	bool do_close(int playerLoc, string trig);

	bool check_encounter(int playerLoc, double rng);

	Room rooms[100];
	vector<doorInfo> doors;
	int numOfRooms;

	const int NOEXIT = -1;

private:
	void find_doors();
	int find_door_adj(int roomid);
};

