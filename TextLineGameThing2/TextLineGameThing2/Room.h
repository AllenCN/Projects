#pragma once
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#ifdef _DEBUG
#define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define new DEBUG_NEW
#endif
#include <vector>
#include <string>
#include <assert.h>

using namespace std;

class Room
{
public:
	Room();
	Room(vector<string> &line);
	~Room();

	int id;
	string name;
	string desc;
	int exits_to_room[4];
	double encRate = 0.0;
	vector<string> roomItems;
	vector<string> npcNames;
	vector<string> enemynames;

	string to_string();
	
private:
	vector<string> room_split(string text, char sep = ',');
	
};

