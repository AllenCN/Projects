#pragma once
#include "Actor.h"
class Player :
	public Actor
{
public:
	Player();
	Player(const Player &p);
	Player(string nm, int sex, int h, int m, int s, int d, int f, int r, int sd, int l, long xp, long next,
		int base, string* equip);
	virtual ~Player();
	long xp;
	long lvl_Up_xp;
	int level;
	
	void level_up();

	void view_stats();

	void create_player(vector<string> data, vector<Skill> &all);
	void create_class(vector<Skill> &all);

	bool do_equip(Equip e);
	string do_unequip(Equip e);

	void do_rest();
	void victory(long reward);

	void load_player(string nm, int c, int sex, int h, int m, int s, int d, int f, int r, int sd, int l, long xp, long next,
		int base, string* equip);
	void load_data(vector<Skill> &load1, vector<pair<Skill, int>> &learnset);

	Player save_player();

	vector<Skill> get_Skills();
	vector<pair<Skill, int>> get_learnset();
	int get_base_xp();
	int get_class();

	vector<pair<Skill, int>> learnset;

private:
	
	int base_xp;
	int MAX_LEVEL;
	int classcode;
	const int maxes[1] = { 10 };
	const string parts[4] = { "HEAD", "TORSO", "ARMS", "LEGS" };
	const string classnames[4] = { "Balanced", "Bruiser", "Shooter", "Speedster" };
	bool is_equipped_with(string e);
	void construct_learnset(vector<Skill> &all, vector<string> skillTrigs, vector<int> lvls);
	void print_equip();
};

