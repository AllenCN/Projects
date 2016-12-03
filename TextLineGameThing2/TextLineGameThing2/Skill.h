#pragma once

#include <string>
#include <vector>
#include <assert.h>

using namespace std;

struct buff {

	string buffName = ""; //typing this in is required to end certain buffs.
	int turnsRemain = -1;

	int strChange;
	int durChange;
	int forChange;
	int resChange;
	int spdChange;

	int hp_drain = 0;
	int mp_drain = 0;
};

class Skill
{
public:
	Skill();
	Skill(vector<string> info);
	~Skill();

	enum SkillType { PHYSICAL, ENERGY, BUFF };

	string name;
	string trigger;
	int type;
	int base_power;
	int mp_cost;
	buff buff_data;
private:
	bool is_integer(string s);
};

