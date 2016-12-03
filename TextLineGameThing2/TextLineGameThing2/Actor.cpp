#include "Actor.h"



Actor::Actor()
{
}


Actor::~Actor()
{
}

bool Actor::is_dead() {
	return hp <= 0;
}

void Actor::attackTarget(Actor &target) {
	bernoulli_distribution hit(0.75);
	uniform_real_distribution<double> damageMod(0.8, 1.0);
	finalStats strengthChanges = check_all_buffs();
	finalStats otherChanges = target.check_all_buffs();
	cout << name << " attacks " << target.name << "!" << endl;
	if (!hit(gen)) {
		cout << name << " missed!" << endl;
		return;
	}
	if (str >= force) {
		int dmg = strengthChanges.fStr * strengthChanges.fStr / otherChanges.fDur * damageMod(gen);
		target.take_damage(dmg);
	}
	else {
		int dmg = force * force / target.resist * damageMod(gen);
		target.take_damage(dmg);
	}
}

void Actor::attackTarget(Actor &target, Skill &skill) {
	bernoulli_distribution hit(0.75);
	uniform_real_distribution<double> damageMod(0.8, 1.0);
	finalStats strengthChanges = check_all_buffs();
	finalStats otherChanges = target.check_all_buffs();
	cout << name << " attacks " << target.name << " using " << skill.name << "!" << endl;
	if (!hit(gen)) {
		cout << name << " missed!" << endl;
		return;
	}
	else {
		if (skill.type == Skill::PHYSICAL) {
			int dmg = strengthChanges.fStr * skill.base_power / otherChanges.fDur * damageMod(gen);
			target.take_damage(dmg);
		}
		else {
			int dmg = strengthChanges.fFor * skill.base_power / otherChanges.fRes * damageMod(gen);
			target.take_damage(dmg);
		}
	}
}

void Actor::take_damage(int dmg) {
	cout << name << " took " << dmg << " damage!" << endl;
	hp -= dmg;
}

void Actor::heal(int hprecov, int mprecov) {
	hp += hprecov;
	mp += mprecov;
	if (hp > max_hp) {
		hp = max_hp;
	}
	if (mp > max_mp) {
		mp = max_mp;
	}
}

void Actor::add_skill(Skill s) {
	skillList.push_back(s);
}

void Actor::use_skill(Actor &target, const string &skillName) {
	int index = has_skill(skillName);
	if (index == -1)
	{
		cout << name << " does not have this skill!" << endl;
	}
	else {
		if (skillList[index].type != 2)
		{
			mp -= skillList[index].mp_cost;
			attackTarget(target, skillList[index]);
		}
		else {
			mp -= skillList[index].mp_cost;
			cout << name << " activates " << skillList[index].name << endl;
			apply_buff(skillList[index].buff_data);
		}
	}
}

int Actor::has_skill(const string &skillName) {
	for (unsigned int i = 0; i < skillList.size(); i++) {
		if (skillList[i].trigger == skillName) {
			return i;
		}
	}
	return -1;
}

void Actor::view_stats() {
	cout << name << "'s stats:\n-----------------" << endl;
	if (gender == 1) {
		cout << "Gender:\tMale";
	}
	else if (gender == 2) {
		cout << "Gender:\tFemale";
	}
	cout << "\n----------------" << endl;
	cout << "HP:\t" << to_string(hp) << " / " << to_string(max_hp) << endl;
	cout << "MP:\t" << to_string(mp) << " / " << to_string(max_mp) << endl;
	cout << "STR:\t" << to_string(str) << "\tDUR:\t" << to_string(dur) << endl;
	cout << "FOR:\t" << to_string(force) << "\tRESIST:\t" << to_string(resist) << endl;
	cout << "SPD:\t" << to_string(speed) << endl;
}

void Actor::print_skills() {
	cout << name << "'s skills:\n----------------" << endl;
	for (int i = 0; i < skillList.size(); i++) {
		cout << skillList[i].name << " ( " << skillList[i].trigger << " )\t" <<
			skillList[i].mp_cost << endl;
	}
}

void Actor::apply_status() {
	switch (status) {
		case POISON:
			hp -= (max_hp / 12);
			break;
		case KAIOKEN:
			hp -= (max_hp / 10);
			mp -= (max_mp / 20);
			//buff information applies elsewhere
			break;
		case KAIOKEN2:
			hp -= (max_hp / 5);
			mp -= (max_mp / 10);
			//buff information applies elsewhere
			break;
		case KAIOKEN3:
			hp -= (max_hp / 4);
			mp -= (max_mp / 5);
			//buff information applies elsewhere
			break;
		default:
			//no change
			break;
	}
}

void Actor::apply_buff(buff &b) {
	activeBuffs.push_back(b);
	if (b.buffName == "KAIOKEN") {
		status = KAIOKEN;
	}
	else if (b.buffName == "KAIOKEN5") {
		status = KAIOKEN2;
	}
	else if (b.buffName == "KAIOKEN10") {
		status = KAIOKEN3;
	}
}

void Actor::check_buff_end() {
	for (unsigned int i = 0; i < activeBuffs.size(); i++) {
		if (activeBuffs[i].turnsRemain > 0) {
			activeBuffs[i].turnsRemain--;
		}
		if (activeBuffs[i].turnsRemain == 0) {
			activeBuffs.erase(activeBuffs.begin() + i);
			i--; //reset i one space
		}

	}
}

void Actor::end_buff(const string &buffName) {
	for (unsigned int i = 0; i < activeBuffs.size(); i++) {
		if (activeBuffs[i].buffName == buffName) {
			cout << "You stop using the " << activeBuffs[i].buffName << endl;
			activeBuffs.erase(activeBuffs.begin() + i);
			if (buffName.find("KAIOKEN"))
				status = NORMAL;
			break;
		}
	}
}

finalStats Actor::check_all_buffs() {
	finalStats ret;
	ret.fStr = str;
	ret.fDur = dur;
	ret.fFor = force;
	ret.fRes = resist;
	ret.fSpd = speed;
	for (unsigned int i = 0; i < activeBuffs.size(); i++) {
		if (activeBuffs[i].buffName == "KAIOKEN")
		{
			ret.fStr += str;
			ret.fDur += dur;
			ret.fFor += force;
			ret.fRes += resist;
			ret.fSpd += speed;
		}
		else if (activeBuffs[i].buffName == "KAIOKEN5") {
			ret.fStr += 4 * str;
			ret.fDur += 4 * dur;
			ret.fFor += 4 * force;
			ret.fRes += 4 * resist;
			ret.fSpd += 4 * speed;
		}
		else if (activeBuffs[i].buffName == "KAIOKEN10") {
			ret.fStr += 9 * str;
			ret.fDur += 9 * dur;
			ret.fFor += 9 * force;
			ret.fRes += 9 * resist;
			ret.fSpd += 9 * speed;
		}
		else {
			ret.fStr += activeBuffs[i].strChange;
			ret.fDur += activeBuffs[i].durChange;
			ret.fFor += activeBuffs[i].forChange;
			ret.fRes += activeBuffs[i].resChange;
			ret.fSpd += activeBuffs[i].spdChange;
		}
	}
	return ret;
}