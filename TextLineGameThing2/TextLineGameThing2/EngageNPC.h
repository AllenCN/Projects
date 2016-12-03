#pragma once
#include "NPC.h"
class EngageNPC :
	public NPC
{
public:
	EngageNPC();
	EngageNPC(vector<string> data);
	virtual ~EngageNPC();

	bool is_fatal;
};

