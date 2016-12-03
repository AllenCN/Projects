#pragma once

#include <vector>
#include <string>
#include <assert.h>

#include "Enemy.h"

using namespace std;

class NPC
{
public:
	NPC();
	NPC(vector<string> data);
	virtual ~NPC();

	int type; //0 = normal, 1 = engage, 2 = shopkeeper
	string trigger;
	string name;
	string bio;
	string greeting;
	string on_engage;
	string shop_init;
};

