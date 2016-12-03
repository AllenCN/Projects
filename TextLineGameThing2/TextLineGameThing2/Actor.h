#pragma once
#include "Skill.h"
#include "Equip.h"
#include <string>
#include <vector>
#include <random>
#include <iostream>

using namespace std;
struct finalStats {
	int fStr = 0;
	int fDur = 0;
	int fFor = 0;
	int fRes = 0;
	int fSpd = 0;
};
class Actor
{
public:
	Actor();
	virtual ~Actor();

	enum StatusCodes { NORMAL, POISON, KAIOKEN, KAIOKEN2, KAIOKEN3 };

	string name;

	int status = NORMAL;

	int gender;
	int hp;
	int max_hp;
	int mp;
	int max_mp;
	int str;
	int dur;
	int force;
	int resist;
	int speed;

	vector<Skill> skillList;
	string equipped[4] = { "NOTHING", "NOTHING", "NOTHING", "NOTHING" };
	vector<buff> activeBuffs;

	void attackTarget(Actor &target);
	void attackTarget(Actor &target, Skill &skill);
	void take_damage(int dmg);
	void heal(int hprecov = 0, int mprecov = 0);
	
	void add_skill(Skill s);

	void use_skill(Actor &target, const string &skillName);
	int has_skill(const string &skillName);
	
	void apply_buff(buff &b);
	void check_buff_end();
	void end_buff(const string &buffName);

	bool is_dead();

	void view_stats();
	void print_skills();

	void apply_status();
	default_random_engine gen;

	

private:
	finalStats check_all_buffs();

};

