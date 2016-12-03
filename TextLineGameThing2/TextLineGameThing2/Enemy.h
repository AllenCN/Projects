#pragma once
#include "Actor.h"
class Enemy :
	public Actor
{
public:
	Enemy();
	virtual ~Enemy();
	Enemy(const string &nm, const string &w, int health, int mana, int strength,
		int durability, int f, int res, int spd, int xreward, int greward);
	Enemy(const string &nm, const string &w, int sex, int health, int mana, int strength,
		int durability, int f, int res, int spd, int xreward, int greward);

	string code;
	int xpyield;
	int goldyield;
};

