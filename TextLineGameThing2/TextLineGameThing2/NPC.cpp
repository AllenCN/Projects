#include "NPC.h"



NPC::NPC()
{
}

NPC::NPC(vector<string> data) {
	assert(data.size() == 7);

	type = stoi(data[0]);
	trigger = data[1];
	name = data[2];
	bio = data[3];
	greeting = data[4];
	on_engage = data[5];
	shop_init = data[6];
}


NPC::~NPC()
{
}
