
//Diagnostic sanity checks,  because this thing uses pointers
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#ifdef _DEBUG
	#define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
	#define new DEBUG_NEW
#endif

#include "Word.h"
#include "Noun.h"
#include "Item.h"
#include "Equip.h"
#include "Map.h"
#include "NPC.h"
#include "EngageNPC.h"
#include "ShopNPC.h"
#include "Skill.h"
#include "Actor.h"
#include "Player.h"
#include "Enemy.h"
#include "Combat.h"
#include <iostream>
#include <fstream>
#include <memory>
using namespace std;

vector<Word> verbs;
vector<shared_ptr<Noun>> allNouns;
vector<shared_ptr<NPC>> allNPCs;
vector<Skill> allSkills;
Player player;
vector<pair<string, int>> inventory;
enum NounTypes {OBJECT, ITEM, EQUIPMENT};
enum NPCTypes {NORMAL, ENGAGE, SHOPKEEP};

struct dir {
	string text;
	int code;
};
struct enemy_data {
	string name;
	string trigger;
	int maxhp;
	int maxmp;
	int str;
	int dur;
	int force;
	int resist;
	int spd;
	int baseXP;
	int baseGold;
	int g_code = 0;
};
struct game_data {
	string name;

	int ccode;
	int gender;
	int max_hp;
	int max_mp;
	int str;
	int dur;
	int force;
	int resist;
	int speed;

	int level;
	long xp;
	long toNextLevel;
	int base_xp;

	string equipped[4] { "NOTHING", "NOTHING", "NOTHING", "NOTHING" };
};

enum en_DIRS { NORTH, EAST, SOUTH, WEST };
const int NONE = -1;
const int DIRS = 4;
dir directions[DIRS];
vector<enemy_data> enemyDatas;
Map mainMap;
int location = 0;
default_random_engine main_gen;
uniform_real_distribution<double> encDis(0.0,1.0);

bool game_over = false;
bool bossDefeated = false;

void textWrap(string str, size_t width = 80) {
	string tmpWord;
	string tmpLine;
	int strLength = str.size();

	for (int i = 0; i < strLength; i++) {
		if (str[i] != ' ') {
			tmpWord += str[i];
		}
		else {
			if (tmpWord.size() + tmpLine.size() >= width) {
				cout << tmpLine;
				cout << endl;
				tmpLine.clear();
			}
			tmpLine += tmpWord + ' ';
			tmpWord.clear();
		}
	}
	cout << tmpLine << tmpWord << endl;
}

std::vector<std::string> split(const std::string &text, char sep) {
	std::vector<std::string> tokens;
	std::size_t start = 0, end = 0;
	while ((end = text.find(sep, start)) != std::string::npos) {
		if (end != start) {
			tokens.push_back(text.substr(start, end - start));
		}
		start = end + 1;
	}
	if (end != start) {
		tokens.push_back(text.substr(start));
	}
	return tokens;
}

void initDirs() {
	directions[NORTH].code = NORTH;
	directions[NORTH].text = "NORTH";
	directions[EAST].code = EAST;
	directions[EAST].text = "EAST";
	directions[SOUTH].code = SOUTH;
	directions[SOUTH].text = "SOUTH";
	directions[WEST].code = WEST;
	directions[WEST].text = "WEST";
}

bool initVerbs(const string &verbData = "verbs.txt") {
	ifstream verbFile;
	verbFile.open(verbData);
	string line;
	if (verbFile.is_open()) {
		while (verbFile.good()) {
			getline(verbFile, line);
			vector<string> toks = split(line, ',');
			Word w(toks);
			verbs.push_back(w);
		}
		verbFile.close();
		return true;
	}
	else {
		cout << "Failed to open verbs.txt" << endl;
		return false;
	}
}

bool initMap(const string &mapData = "rooms.txt") {
	ifstream roomfile;
	vector<string> lines;
	vector<Room> rooms;
	string tmp;
	roomfile.open(mapData);
	if (roomfile.is_open()) {
		while (roomfile.good()) {
			tmp.clear();
			getline(roomfile, tmp);

			if (tmp.empty())
			{
				Room room(lines);
				lines.clear();
				rooms.push_back(room);
			}
			else {
				lines.push_back(tmp);
			}
		}
		mainMap.buildMap(rooms);
		roomfile.close();
		return true;
	}
	else {
		cout << "Failed to open rooms.txt" << endl;
		return false;
	}
}

bool initNouns(const string &nounData = "allnouns.txt") {
	
	ifstream nounFile;
	nounFile.open(nounData);
	string line;
	vector<string> toks;
	vector<string> rest;
	if (nounFile.is_open()) {
		while (nounFile.good()) {
			line.clear();
			getline(nounFile, line);
			if (toks.empty()) {
				toks = split(line, ',');
			}
			else {
				if (line.empty()) {
					if (rest.size() == 3)
					{
						shared_ptr<Noun> n(new Noun(toks, rest));
						allNouns.push_back(n);
						toks.clear();
						rest.clear();
					}
					else if (rest.size() == 6 || rest.size() == 7) {
						shared_ptr<Noun> i(new Item(toks, rest));
						allNouns.push_back(i);
						toks.clear();
						rest.clear();
					}
					else if (rest.size() >= 9) {
						shared_ptr<Noun> e(new Equip(toks, rest));
						allNouns.push_back(e);
						toks.clear();
						rest.clear();
					}
					else {
						cout << "An error in parsing" << endl;
						nounFile.close();
						return false;
					}
				}
				else {
					rest.push_back(line);
				}
			}
			
		}
		nounFile.close();
		return true;
	}
	else {
		cout << "Failed to open allnouns.txt" << endl;
		return false;
	}

}

bool initNPCs(const string &npcData = "npcs.txt") {
	ifstream npcfile;
	npcfile.open(npcData);
	string line;
	vector<string> dat;

	if (npcfile.is_open()) {
		while (npcfile.good()) {
			line.clear();
			getline(npcfile, line);
			if (line.empty()) {
				if (dat.size() == 7) {
					shared_ptr<NPC> npc(new NPC(dat));
					allNPCs.push_back(npc);
					dat.clear();
				}
				else if (dat.size() == 8) {
					shared_ptr<EngageNPC> eNPC(new EngageNPC(dat));
					allNPCs.push_back(eNPC);
					dat.clear();
				}
				else if (dat.size() >= 10) {
					shared_ptr<ShopNPC> sNPC(new ShopNPC(dat));
					allNPCs.push_back(sNPC);
					dat.clear();
				}
				else {
					cout << "NPC file is invalid!" << endl;
					npcfile.close();
					return false;
				}
			}
			else {
				dat.push_back(line);
			}
		}
		npcfile.close();
		return true;
	}
	else {
		cout << "Failed to open npcs.txt" << endl;
		return false;
	}
}

bool initSkills(const string &skillData = "allskills.txt") {
	ifstream skillfile;
	skillfile.open(skillData);
	string line;
	vector<string> dat;

	if (skillfile.is_open()) {
		while (skillfile.good()) {
			line.clear();
			getline(skillfile, line);
			if (line.empty()) {
				if (dat.size() == 5 || dat.size() == 6 || dat.size() >= 11) {
					Skill t(dat);
					allSkills.push_back(t);
					dat.clear();
				}
				else {
					cout << "Skill file is invalid!" << endl;
					skillfile.close();
					return false;
				}
			}
			else {
				dat.push_back(line);
			}
		}
		skillfile.close();
		return true;
	}
	else {
		cout << "Failed to open allskills.txt" << endl;
		return false;
	}
}

bool initEnemyData(const string &enemyData = "enemydata.txt") {
	ifstream datfile;
	datfile.open(enemyData);
	string line;
	vector<string> dat;
	if (datfile.is_open()) {
		while (datfile.good()) {
			line.clear();
			getline(datfile, line);
			if (line.empty()) {
				enemy_data e;
				e.name = dat[0];
				e.trigger = dat[1];
				e.maxhp = stoi(dat[2]);
				e.maxmp = stoi(dat[3]);
				e.str = stoi(dat[4]);
				e.dur = stoi(dat[5]);
				e.force = stoi(dat[6]);
				e.resist = stoi(dat[7]);
				e.spd = stoi(dat[8]);
				e.baseXP = stoi(dat[9]);
				e.baseGold = stoi(dat[10]);
				if (dat.size() == 12)
					e.g_code = stoi(dat[11]);
				
				enemyDatas.push_back(e);
				dat.clear();
			}
			else {
				dat.push_back(line);
			}
		}
		datfile.close();
		return true;
	}
	else {
		cout << "Failed to open enemydata.txt";
		return false;
	}
	return true;
}

void print_inventory(vector<pair<string, int>> &inv) {
	cout << "ITEM \t QUANTITY" << endl;
	cout << "-------------------" << endl;
	cout << inv[0].first << " \t " << inv[0].second << endl;
	for (unsigned int i = 1; i < inv.size(); i++) {
		if (inv[i].second > 1)
			cout << inv[i].first << "\t" << inv[i].second << endl;
		else
			cout << inv[i].first << endl;
	}
	cout << "-------------------" << endl;
}

bool is_in_inventory(vector<pair<string, int>> &inv, string itemName) {
	unsigned int loc;
	for (loc = 0; loc < inv.size(); loc++) {
		if (inv[loc].first == itemName)
		{
			break;
		}
	}
	return loc != inv.size();
}

int in_inventory(string itemName, vector<pair<string, int>> &inv) {
	for (unsigned int i = 0; i < inv.size(); i++) {
		if (inv[i].first == itemName)
			return i;
	}
	return -1;
}

enemy_data find_enemy(string code) {
	for (unsigned int i = 0; i < enemyDatas.size(); i++) {
		if (code == enemyDatas[i].trigger) {
			return enemyDatas[i];
		}
	}
	assert(false); //fails automatically, should not be possible
}

vector<Enemy> set_encounter(vector<string> enemyNames) {
	vector<Enemy> ret;
	uniform_int_distribution<int> rng(1, 3);
	uniform_int_distribution<int> gend(1, 2);
	uniform_int_distribution<int> gains(50, 200);
	int hordeSize = rng(main_gen);

	string code = "";
	enemy_data dat;
	
	if (enemyNames.size() == 1) {
		code = enemyNames[0];
		dat = find_enemy(code);
		if (dat.g_code == 0)
			dat.g_code = gend(main_gen);
		for (int i = 0; i < hordeSize; i++) {
			Enemy tmp(dat.name, dat.trigger, dat.g_code, dat.maxhp, dat.maxmp, dat.str, dat.dur, dat.force, dat.resist,
				dat.spd, dat.baseXP + gains(main_gen), dat.baseGold + gains(main_gen));
			ret.push_back(tmp);
		}
	}
	else {
		for (int i = 0; i < hordeSize; i++) {
			code = enemyNames[rng(main_gen) % enemyNames.size()];
			dat = find_enemy(code);
			if (dat.g_code == 0)
				dat.g_code = gend(main_gen);
			Enemy tmp(dat.name, dat.trigger, dat.g_code, dat.maxhp, dat.maxmp, dat.str, dat.dur, dat.force, dat.resist,
				dat.spd, dat.baseXP + gains(main_gen), dat.baseGold + gains(main_gen));
			ret.push_back(tmp);
		}
	}

	return ret;
}

vector<Enemy> set_engage(string trigger) {
	vector<Enemy> ret;
	enemy_data dat = find_enemy(trigger);
	Enemy tmp(dat.name, dat.trigger, dat.g_code, dat.maxhp, dat.maxmp, dat.str, dat.dur, dat.force, dat.resist,
		dat.spd, dat.baseXP, dat.baseGold);
	ret.push_back(tmp);
	return ret;
}

bool parse_command(int &loc, vector<string> &words, vector<pair<string, int>> &inv) {
	int VERB_MATCH = -1;
	int NOUN_MATCH = -1;
	int NPC_MATCH = -1;
	for (int i = 0; i < DIRS; i++) {
		if (words[0] == directions[i].text) {
			int newLoc = mainMap.do_move(loc, directions[i].code);
			if (newLoc == loc)
			{
				cout << "Cannot move that way!" << endl;
			}
			else {
				loc = newLoc;
				textWrap(mainMap.rooms[loc].name);
				textWrap(mainMap.rooms[loc].desc);
				
				bool encounter = mainMap.check_encounter(loc, encDis(main_gen));
				if (encounter) {

					Combat c(allNouns, "helpfile.txt");
					vector<Enemy> encounter = set_encounter(mainMap.rooms[loc].enemynames);
					bool alive = c.combat_loop(player, inv, encounter);
					player.activeBuffs.clear();
					if (!alive) {
						game_over = true;
					}
				}
			}
			return true;
		}
	}
	for (unsigned int i = 0; i < verbs.size(); i++) {
		if (verbs[i] == words[0]) {
			VERB_MATCH = i;
			break;
		}
	}
	if (words.size() == 2) {
		for (unsigned int i = 0; i < allNouns.size(); i++) {
			if (*allNouns[i] == words[1]) {
				NOUN_MATCH = i;
				break;
			}
		}
		for (unsigned int i = 0; i < allNPCs.size(); i++) {
			if ((*allNPCs[i]).trigger == words[1]) {
				NPC_MATCH = i;
				break;
			}
		}
	}
	if (VERB_MATCH == -1) {
		cout << "You can not " << words[0] << " something in this game." << endl;
		return false;
	}

	if (verbs[VERB_MATCH] == "INVENTORY") {
		print_inventory(inv);
		return true;
	}
	if (verbs[VERB_MATCH] == "LOOK") {
		vector<string> lines = mainMap.do_look(loc);
		for (unsigned int i = 0; i < lines.size(); i++) {
			textWrap(lines[i]);
		}
		return true;
	}
	if (verbs[VERB_MATCH] == "GET") {
		if (NOUN_MATCH != -1 && (*allNouns[NOUN_MATCH]).type != OBJECT) {
			bool success = mainMap.do_get(loc, (*allNouns[NOUN_MATCH]).code, inv);
			if (success)
				cout << "You pick up the " << words[1] << "." << endl;
			else
				cout << "That is not in the area." << endl;
		}
		else {
			cout << "You cannot pick that up." << endl;
		}
		return true;
	}
	if (verbs[VERB_MATCH] == "DROP") {
		if (NOUN_MATCH != -1 && (*allNouns[NOUN_MATCH]).type != OBJECT) {
			if ((*allNouns[NOUN_MATCH]).type == ITEM) {
				Item* tmp = (dynamic_cast<Item*>(allNouns[NOUN_MATCH].get()));
				if (tmp->isKeyItem) {
					cout << "You shouldn't drop this item regardless." << endl;
					return true;
				}
			}
			bool success = mainMap.do_drop(loc, (*allNouns[NOUN_MATCH]).code, inv);
			if (success)
				cout << "You drop the " << words[1] << "." << endl;
			else
				cout << "You do not have this." << endl;

		}
		return true;
	}
	if (verbs[VERB_MATCH] == "EXAMINE") {
		if (NOUN_MATCH != -1) {
			if (mainMap.check_noun(loc, (*allNouns[NOUN_MATCH]).code) 
				|| is_in_inventory(inv, (*allNouns[NOUN_MATCH]).code)) {
				textWrap((*allNouns[NOUN_MATCH]).code);
				textWrap((*allNouns[NOUN_MATCH]).shortdesc);
				textWrap((*allNouns[NOUN_MATCH]).longdesc);
				return true;
			}
		}
		else if (NPC_MATCH != -1) {
			if (mainMap.check_NPC(loc, (*allNPCs[NPC_MATCH]).trigger)) {
				textWrap((*allNPCs[NPC_MATCH]).name);
				textWrap((*allNPCs[NPC_MATCH]).bio);
			}
			else {
				cout << "S/he is not here." << endl;
			}
			return true;
		}
		else {
			cout << "That is not in this location or on your person." << endl;
			return true;
		}
	}
	if (verbs[VERB_MATCH] == "USE") {
		if (NOUN_MATCH != -1 && is_in_inventory(inv, (*allNouns[NOUN_MATCH]).code))
		{
			if ((*allNouns[NOUN_MATCH]).type == ITEM) {
				Item *tmp = dynamic_cast<Item*>(allNouns[NOUN_MATCH].get());
				if (tmp->isKeyItem)
				{
					cout << "That can't be used right now..." << endl;
					return true;
				}
				cout << "You use the " << words[1] << endl;
				player.heal(tmp->hpUp, tmp->mpUp);
				textWrap(tmp->use_sentence);
				int index = in_inventory((*allNouns[NOUN_MATCH]).code, inv);
				inv[index].second -= 1;
				if (inv[index].second == 0) {
					inv.erase(inv.begin() + index);
				}
				return true;
			}
			else {
				cout << "That is not a usable item." << endl;
				return true;
			}
		}
		else {
			cout << "You can't use that." << endl;
			return true;
		}
	}
	if (verbs[VERB_MATCH] == "OPEN") {
		if (NOUN_MATCH != -1 && (*allNouns[NOUN_MATCH]).type == OBJECT) {
			bool opened = mainMap.do_open(loc, (*allNouns[NOUN_MATCH]).code);
			if (opened)
				cout << "You open the door." << endl;
			else
				cout << "Opening failed for some reason." << endl;
		}
		else {
			cout << "That is not a gate or door." << endl;
		}
	}
	if (verbs[VERB_MATCH] == "CLOSE") {
		if (NOUN_MATCH != -1 && (*allNouns[NOUN_MATCH]).type == OBJECT) {
			bool closed = mainMap.do_close(loc, (*allNouns[NOUN_MATCH]).code);
			if (closed)
				cout << "You close the door." << endl;
			else
				cout << "Closing failed for some reason." << endl;
		}
		else {
			cout << "That is not a gate or door." << endl;
		}
	}
	if (verbs[VERB_MATCH] == "REST") {
		player.do_rest();
		bool encounter = mainMap.check_encounter(loc, encDis(main_gen));
		if (encounter) {

			Combat c(allNouns, "helpfile.txt");
			vector<Enemy> encounter = set_encounter(mainMap.rooms[loc].enemynames);
			bool alive = c.combat_loop(player, inv, encounter);
			player.activeBuffs.clear();
			if (!alive) {
				game_over = true;
			}
		}
	}
	if (verbs[VERB_MATCH] == "TALK") {
		if (NPC_MATCH != -1) {
			if (mainMap.check_NPC(loc, (*allNPCs[NPC_MATCH]).trigger)) {
				textWrap((*allNPCs[NPC_MATCH]).name + ": \"" + (*allNPCs[NPC_MATCH]).greeting + "\"");
			}
			else {
				cout << "S/he is not here." << endl;
			}
			return true;
		}
		else if (NOUN_MATCH != -1) {
			cout << "You can't talk to that!" << endl;
			return true;
		}
		else {
			cout << "That person does not exist." << endl;
			return true;
		}
	}
	if (verbs[VERB_MATCH] == "ENGAGE") {
		if (NPC_MATCH != -1) {
			if (mainMap.check_NPC(loc, (*allNPCs[NPC_MATCH]).trigger)) {
				textWrap((*allNPCs[NPC_MATCH]).name + ": \"" + (*allNPCs[NPC_MATCH]).on_engage + "\"");
				if ((*allNPCs[NPC_MATCH]).type == ENGAGE) {
					EngageNPC* tmp = dynamic_cast<EngageNPC*>(allNPCs[NPC_MATCH].get());

					vector<Enemy> against = set_engage(tmp->trigger);
					Combat c(allNouns, "helpfile.txt");
					bool alive = c.combat_loop(player, inv, against);
					player.activeBuffs.clear();
					if (!(tmp->is_fatal) && !alive) {
						cout << tmp->name << " heals your wounds..." << endl;
						player.heal(9999, 9999);
						alive = true;
						cout << player.name << " gains some EXP anyway!" << endl;
						player.victory(against[0].xpyield / 2);
						return true;
					}
					if (!alive) {
						game_over = true;
						return true;
					}
					if (tmp->trigger == "BOSS") {
						bossDefeated = true;
						return true;
					}
				}
			}
			else {
				cout << "S/he is not here." << endl;
			}
			return true;
		}
		else if (NOUN_MATCH != -1) {
			cout << "You can't talk to that!" << endl;
			return true;
		}
		else {
			cout << "That person does not exist." << endl;
			return true;
		}
	}
	if (verbs[VERB_MATCH] == "SHOP") {
		if (NPC_MATCH != -1) {
			if (mainMap.check_NPC(loc, (*allNPCs[NPC_MATCH]).trigger)) {
				textWrap((*allNPCs[NPC_MATCH]).name + ": \"" + (*allNPCs[NPC_MATCH]).shop_init + "\"");
				if ((*allNPCs[NPC_MATCH]).type == SHOPKEEP) {
					ShopNPC* shop = dynamic_cast<ShopNPC*>(allNPCs[NPC_MATCH].get());
					shop->do_shop(inv);
				}
			}
			else {
				cout << "S/he is not here." << endl;
			}
			return true;
		}
		else if (NOUN_MATCH != -1) {
			cout << "You can't talk to that!" << endl;
			return true;
		}
		else {
			cout << "That person does not exist." << endl;
			return true;
		}
	}
	if (verbs[VERB_MATCH] == "EQUIP") {
		if (NOUN_MATCH != -1 && is_in_inventory(inv, (*allNouns[NOUN_MATCH]).code)) {
			if ((*allNouns[NOUN_MATCH]).type == EQUIPMENT) {
				Equip *tmp = dynamic_cast<Equip*>(allNouns[NOUN_MATCH].get());
				player.do_equip(*tmp);
				int index = in_inventory((*allNouns[NOUN_MATCH]).code, inv);
				inv[index].second -= 1;
				if (inv[index].second == 0) {
					inv.erase(inv.begin() + index);
				}
				return true;
			}
			else {
				cout << "That is not a equipable item." << endl;
				return true;
			}
		}
		else {
			cout << "You can't equip something that doesn't exist..." << endl;
			return true;
		}
	}
	if (verbs[VERB_MATCH] == "UNEQUIP") {
		if (NOUN_MATCH != -1) {
			if ((*allNouns[NOUN_MATCH]).type == EQUIPMENT) {
				Equip *tmp = dynamic_cast<Equip*>(allNouns[NOUN_MATCH].get());
				string tmp2 = player.do_unequip(*tmp);
				if (tmp2 != "NOTHING") {
					if (is_in_inventory(inv, (*allNouns[NOUN_MATCH]).code)) {
						int index = in_inventory((*allNouns[NOUN_MATCH]).code, inv);
						inv[index].second += 1;
					}
					else {
						inv.push_back(make_pair((*allNouns[NOUN_MATCH]).code, 1));
					}
				}
				else {
					cout << "You are not equipped with that." << endl;
				}
				return true;
			}
			else {
				cout << "That is not a equipable item." << endl;
				return true;
			}
			return true;
		}
		else {
			cout << "You can't unequip something that doesn't exist..." << endl;
			return true;
		}
	}
	if (verbs[VERB_MATCH] == "STATS") {
		player.view_stats();
	}

	return false;
}

void print_help(const string &helpFile = "helpfile.txt") {
	ifstream help;
	help.open(helpFile);
	if (help.is_open()) {
		string line;
		while (help.good()) {
			line.clear();
			getline(help, line);
			cout << line << endl;
		}
		help.close();
		cout << endl;
	}
	else {
		cout << "help file not found" << endl;
	}
}

void print_story(const string &storyFile = "storyfile.txt") {
	ifstream story;
	story.open(storyFile);
	if (story.is_open()) {
		string line;
		while (story.good()) {
			line.clear();
			getline(story, line);
			textWrap(line);
		}
	}
	else {
		cout << "Story file not found" << endl;
	}
}

game_data save_player() {
	game_data ret;
	ret.name = player.name;
	ret.ccode = player.get_class();
	ret.gender = player.gender;
	ret.max_hp = player.max_hp;
	ret.max_mp = player.max_mp;
	ret.str = player.str;
	ret.dur = player.dur;
	ret.force = player.force;
	ret.resist = player.resist;
	ret.speed = player.speed;

	ret.level = player.level;
	ret.xp = player.xp;
	ret.toNextLevel = player.lvl_Up_xp;
	ret.base_xp = player.get_base_xp();
	
	ret.equipped[0] = player.equipped[0];
	ret.equipped[1] = player.equipped[1];
	ret.equipped[2] = player.equipped[2];
	ret.equipped[3] = player.equipped[3];

	return ret;
}

void save_game() {
	string fileName;
	cout << "Enter a name for the file:\n>";

	getline(cin, fileName);

	ofstream saveGame;
	saveGame.open(fileName);
	
	size_t invSize = inventory.size();
	size_t skillSize = player.skillList.size();
	size_t learnsetSize = player.learnset.size();
	vector<Skill> skills = player.get_Skills();
	vector<pair<Skill, int>> learnsets = player.get_learnset();
	game_data tmp = save_player();
	if (saveGame) {
		saveGame << tmp.name << endl;
		saveGame << tmp.ccode << endl;
		saveGame << tmp.gender << endl;
		saveGame << tmp.max_hp << endl;
		saveGame << tmp.max_mp << endl;
		saveGame << tmp.str << endl;
		saveGame << tmp.dur << endl;
		saveGame << tmp.force << endl;
		saveGame << tmp.resist << endl;
		saveGame << tmp.speed << endl;
		saveGame << tmp.level << endl;
		saveGame << tmp.xp << endl;
		saveGame << tmp.toNextLevel << endl;
		saveGame << tmp.base_xp << endl;
		saveGame << tmp.equipped[0] << endl;
		saveGame << tmp.equipped[1] << endl;
		saveGame << tmp.equipped[2] << endl;
		saveGame << tmp.equipped[3] << endl;
		saveGame << location << endl;
		saveGame << skills.size() << endl;
		for (unsigned int i = 0; i < skills.size(); i++) {
			saveGame << skills[i].trigger << endl;
		}
		saveGame << learnsets.size() << endl;
		for (unsigned int i = 0; i < learnsets.size(); i++) {
			saveGame << learnsets[i].first.trigger << "," << learnsets[i].second << endl;
		}
		saveGame << inventory.size() << endl;
		for (unsigned int i = 0; i < inventory.size(); i++) {
			saveGame << inventory[i].first << "," << inventory[i].second << endl;
		}
		
		cout << "Saved game successfully!" << endl;
		saveGame.close();
	}
}

vector<Skill> load_player_skills(unsigned int size, vector<string> lines, int start) {
	vector<Skill> ret;

	while (ret.size() < size) {
		for (unsigned int i = 0; i < allSkills.size(); i++) {
			if (lines[start + ret.size()] == allSkills[i].trigger) {
				ret.push_back(allSkills[i]);
				if (ret.size() == size)
					break;
			}
		}
	}

	return ret;
}

vector<pair<Skill, int>> load_learnset(unsigned int size, vector<string> lines, int start) {
	vector<pair<Skill, int>> ret;

	while (ret.size() < size) {
		vector<string> toks = split(lines[start + ret.size()], ',');
		pair<string, int> temp = make_pair(toks[0], stoi(toks[1]));
		for (unsigned int i = 0; i < allSkills.size(); i++) {
			if (temp.first == allSkills[i].trigger) {
				ret.push_back(make_pair(allSkills[i], temp.second));
				break;
			}
		}
	}

	return ret;
}

void load_inventory(unsigned int size, vector<string> lines, int start) {
	inventory.clear();

	while (inventory.size() < size) {
		vector<string> toks = split(lines[start + inventory.size()], ',');
		inventory.push_back(make_pair(toks[0], stoi(toks[1])));
	}

	assert(inventory.size() == size);
}

void load_game(string file) {

	ifstream loadGame;
	loadGame.open(file);
	unsigned int skillSize, learnsetSize, invSize;
	vector<Skill> tempSkills;
	vector<pair<Skill, int>> learnsetTemp;
	vector<string> lines;
	game_data tmp;
	if (loadGame) {
		string line;
		while (loadGame.good()) {
			getline(loadGame, line);
			if (line.empty()) {
				cout << "Player data loaded, parsing..." << endl;
				assert(lines.size() >= 19);
			}
			else {
				lines.push_back(line);
			}
		}
		tmp.name = lines[0];
		tmp.ccode = stoi(lines[1]);
		tmp.gender = stoi(lines[2]);
		tmp.max_hp = stoi(lines[3]);
		tmp.max_mp = stoi(lines[4]);
		tmp.str = stoi(lines[5]);
		tmp.dur = stoi(lines[6]);
		tmp.force = stoi(lines[7]);
		tmp.resist = stoi(lines[8]);
		tmp.speed = stoi(lines[9]);
		tmp.level = stoi(lines[10]);
		tmp.xp = stol(lines[11]);
		tmp.toNextLevel = stol(lines[12]);
		tmp.base_xp = stoi(lines[13]);
		for (int i = 0; i < 4; i++) {
			tmp.equipped[i] = lines[14 + i];
		}
		location = stoi(lines[18]);

		skillSize = stoi(lines[19]);
		tempSkills = load_player_skills(skillSize, lines, 20);

		learnsetSize = stoi(lines[20 + skillSize]);
		learnsetTemp = load_learnset(learnsetSize, lines, 21 + skillSize);

		player.load_player(tmp.name, tmp.ccode, tmp.gender, tmp.max_hp, tmp.max_mp, tmp.str, tmp.dur, tmp.force,
			tmp.resist, tmp.speed, tmp.level, tmp.xp, tmp.toNextLevel, tmp.base_xp, tmp.equipped);
		player.load_data(tempSkills, learnsetTemp);

		invSize = stoi(lines[21 + skillSize + learnsetSize]);
		load_inventory(invSize, lines, 22 + skillSize + learnsetSize);
		

		cout << "Game loaded successfully!" << endl;
		textWrap(mainMap.rooms[location].name);
		textWrap(mainMap.rooms[location].desc);
		loadGame.close();

		return;
	}
}

void print_bad_ending() {
	cout << "You died in battle, leaving the city in the tyrannical rule of Boss Mad." << endl;
}

void print_good_ending() {
	textWrap("Congrats, Boss Mad has been defeated! You have liberated the city! You are the hero of the day!");
	textWrap("It's all thanks to you, " + player.name + "!");
	textWrap("Final stats: \n");
	player.view_stats();
}

int main(int argc, char** argv) {
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	string command;
	vector<string> words;

	inventory.push_back(make_pair("GOLD", 100));
	inventory.push_back(make_pair("SWORD", 1));
	inventory.push_back(make_pair("SHIELD", 1));
	const string prompt = ">";
	initDirs();
	if (!initVerbs()) {
		return 1;
	}
	if (!initMap()) {
		return 1;
	}
	if (!initNouns()) {
		return 1;
	}
	if (!initNPCs()) {
		return 1;
	}
	if (!initSkills()) {
		return 1;
	}
	if (!initEnemyData()) {
		return 1;
	}

	vector<string> quits;
	quits.push_back("exit");
	quits.push_back("done");
	Word QUIT("QUIT", quits);
	seed_seq seq(player.name.begin(), player.name.end());
	main_gen.seed(seq);
	player.create_class(allSkills);
	textWrap(mainMap.rooms[location].name);
	textWrap(mainMap.rooms[location].desc);
	while (true) {
		command.clear();
		words.clear();
		
		if (game_over) {
			print_bad_ending();
			break;
		}

		if (bossDefeated) {
			print_good_ending();
			break;
		}

		cout << prompt;
		getline(cin, command);

		transform(command.begin(), command.end(), command.begin(), (int(*)(int))toupper);

		words = split(command, ' ');

		//Gate stuff to be figured out later
		if (words.empty()) {
			continue;
		}
		if (QUIT == words[0]) {
			cout << "Thank you for playing, goodbye!" << endl;
			break;
		}
		else if (words[0] == "HELP") {
			print_help();
		}
		else if (words[0] == "STORY") {
			print_story();
		}
		else if (words[0] == "SAVE") {
			save_game();
		}
		else if (words[0] == "LOAD") {
			load_game(words[1]);
		}
		else if (words.size() > 2) {
			cout << "Use one or two words only!" << endl;
		}
		else {
			parse_command(location, words, inventory);
		}

	}
	for (unsigned int i = 0; i < allNouns.size(); i++)
	{
		allNouns[i].reset();
	}
	verbs.clear();
	command.clear();
	words.clear();
	inventory.clear();
	allNouns.clear();

	
	return 0;
}
