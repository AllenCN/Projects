#include "Enemy.h"



Enemy::Enemy()
{
}


Enemy::~Enemy()
{
}

Enemy::Enemy(const string &nm, const string &w, int health, int mana, int strength,
	int durability, int f, int res, int spd, int xreward, int greward) {
	name = nm;
	code = w;
	hp = max_hp = health;
	mp = max_mp = mana;
	str = strength;
	dur = durability;
	force = f;
	resist = res;
	speed = spd;
	xpyield = xreward;
	goldyield = greward;
}

Enemy::Enemy(const string &nm, const string &w, int sex, int health, int mana, int strength,
	int durability, int f, int res, int spd, int xreward, int greward) {
	name = nm;
	code = w;
	gender = sex;
	hp = max_hp = health;
	mp = max_mp = mana;
	str = strength;
	dur = durability;
	force = f;
	resist = res;
	speed = spd;
	xpyield = xreward;
	goldyield = greward;
}