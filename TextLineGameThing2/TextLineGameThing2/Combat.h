#pragma once

#include <algorithm>
#include <iostream>
#include <fstream>
#include <memory>
#include <string>
#include <vector>

#include "Actor.h"
#include "Enemy.h"
#include "Player.h"

#include "Word.h"
#include "Noun.h"
#include "Item.h"

using namespace std;

class Combat
{
public:
	Combat();
	Combat(vector<shared_ptr<Noun>> noun, string help);
	~Combat();

	enum turn_over { NOT_DONE, DONE, FLEE_ATTEMPT };

	//combat loop, returns true if the player survives and false if not.
	bool combat_loop(Player &p, vector<pair<string, int>> &inv, vector<Enemy> &enemies);

	//Party vs enemies, returns true if at least one of its members survives
	//bool combat_loop(vector<Player> &p, vector<pair<string, int>> &inv, vector<Enemy> &enemies);

	vector<shared_ptr<Noun>> nouns;
	vector<Word> combat_words;

private:

	void init_words();
	void do_use_combat(int item, vector<pair<string, int>> &inv, Player &p);
	void print_inventory(vector<pair<string, int>> &inv);
	void view_combat_stats(Player &p, vector<Enemy> &enemies);
	int parse_combat(vector<string> &words, vector<Enemy> &es, Player &p, vector<pair<string, int>> &inv);
	int in_inventory(string itemName, vector<pair<string, int>> &inv);
	void init_combat(Player &p, vector<Enemy> &es);
	vector<string> combat_split(string line, char sep = ' ');
	void print_help();
	
	//vector<shared_ptr<Actor>> initiativeOrder;
	//vector<shared_ptr<Actor>> determineOrder(vector<Player> p, vector<Enemy> es);

	//same as above parser
	//int parse_combat(vector<string> &words, vector<Enemy> &es, vector<Player> &p, vector<pair<string, int>> &inv);
	default_random_engine gen;
	string helpFile;
};