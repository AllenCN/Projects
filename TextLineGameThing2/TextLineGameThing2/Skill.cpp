#include "Skill.h"



Skill::Skill()
{
}

Skill::Skill(vector<string> info) {
	//info[0] is the name of the skill
	//info[1] is the name of the "trigger word"
	//info[2] is the skill's type
	//info[3] is the skill's base power (if nonzero, it's a multiplicative buff)
	//info[4] will be the buff trigger (to end it) or the turn counter
	//info[5-9] will be the stats they increase
	//info[10-11], if they exist, will be the drain associated with the upkeep.

	assert(info.size() == 5 || info.size() == 6 || info.size() >= 11);

	name = info[0];
	trigger = info[1];
	type = stoi(info[2]);
	base_power = stoi(info[3]);
	mp_cost = stoi(info[4]);
	if (info.size() > 5) {
		if (is_integer(info[5])) {
			buff_data.turnsRemain = stoi(info[5]);
		}
		else {
			buff_data.buffName = info[5];
		}

		buff_data.strChange = stoi(info[6]);
		buff_data.durChange = stoi(info[7]);
		buff_data.forChange = stoi(info[8]);
		buff_data.resChange = stoi(info[9]);
		buff_data.spdChange = stoi(info[10]);
		if (info.size() >= 12)
			buff_data.hp_drain = stoi(info[11]);
		if (info.size() == 13)
			buff_data.mp_drain = stoi(info[12]);
	}
}

Skill::~Skill()
{
}

bool Skill::is_integer(string s) {
	return (s.find_first_not_of("0123456789") == string::npos);
}