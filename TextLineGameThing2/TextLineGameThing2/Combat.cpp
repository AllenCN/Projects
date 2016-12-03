#include "Combat.h"



Combat::Combat()
{
}

Combat::Combat(vector<shared_ptr<Noun>> noun, string help) {
	nouns = noun;
	helpFile = help;
	init_words();
}

Combat::~Combat()
{
}

void Combat::init_words() {
	vector<string> attacks;
	attacks.push_back("assault");
	attacks.push_back("assail");
	Word attack("attack", attacks);

	vector<string> uses;
	uses.push_back("utilize");
	Word use("use", uses);

	vector<string> flees;
	flees.push_back("retreat");
	flees.push_back("run");
	Word flee("flee", flees);

	Word rest("rest");

	vector<string> status;
	status.push_back("status");
	Word stats("stats", status);

	vector<string> bag;
	bag.push_back("bag");
	Word inventory("inventory", bag);

	Word help("help");

	vector<string> cancels;
	cancels.push_back("end");
	cancels.push_back("deactivate");
	Word cancel("cancel", cancels);

	combat_words.push_back(attack);
	combat_words.push_back(use);
	combat_words.push_back(flee);
	combat_words.push_back(rest);
	combat_words.push_back(stats);
	combat_words.push_back(inventory);
	combat_words.push_back(help);
	combat_words.push_back(cancel);

}

void Combat::do_use_combat(int item, vector<pair<string, int>> &inv, Player &p) {
	if ((*nouns[item]).type == 1) {
		Item *tmp = dynamic_cast<Item*>(nouns[item].get());
		if (tmp->isKeyItem)
		{
			cout << "That can't be used right now..." << endl;
		}
		cout << "You use the " << (*nouns[item]).code << endl;
		p.heal(tmp->hpUp, tmp->mpUp);
		cout << tmp->use_sentence << endl;
		int index = in_inventory((*nouns[item]).code, inv);
		inv[index].second -= 1;
		if (inv[index].second == 0) {
			inv.erase(inv.begin() + index);
		}
	}
	else {
		cout << "That is not a usable item." << endl;
	}
}

int Combat::in_inventory(string itemName, vector<pair<string, int>> &inv) {
	for (unsigned int i = 0; i < inv.size(); i++) {
		if (inv[i].first == itemName)
			return i;
	}
	return -1;
}

int Combat::parse_combat(vector<string> &words, vector<Enemy> &es, Player &p, vector<pair<string, int>> &inv) {
	int VERB_MATCH = -1;
	int TARGET_MATCH = -1;
	bool is_noun = false;
	int SKILL_MATCH = -1;

	if (words.size() > 3) {
		cout << "Use one through three words when in combat!" << endl;
		cout << "[verb] [target/item] [skill]" << endl;
		return NOT_DONE;
	}

	for (unsigned int i = 0; i < combat_words.size(); i++) {
		if (combat_words[i] == words[0])
		{
			VERB_MATCH = i;
			break;
		}
	}

	if (words.size() >= 2) //can be a target, noun, or skill.
	{
		for (unsigned int i = 0; i < es.size(); i++) {
			if (es[i].code == words[1]) {
				TARGET_MATCH = i;
				break;
			}
		}
		for (unsigned int i = 0; i < nouns.size() && TARGET_MATCH == -1; i++) {
			if ((*nouns[i]).code == words[1]) {
				is_noun = true;
				TARGET_MATCH = i;
				break;
			}
		}
		for (unsigned int i = 0; i < p.skillList.size() && !is_noun && TARGET_MATCH == -1; i++) {
			if (p.skillList[i].trigger == words[1]) {
				SKILL_MATCH = i;
				break;
			}
		}
	}

	if (words.size() == 3) { //can be an enemy only
		for (unsigned int i = 0; i < es.size(); i++) {
			if (es[i].code == words[2]) {
				TARGET_MATCH = i;
				break;
			}
		}
	}

	if (VERB_MATCH == -1) {
		cout << "Invalid command entered." << endl;
		return NOT_DONE;
	}

	if (combat_words[VERB_MATCH] == "ATTACK") {
		if (TARGET_MATCH != -1) {
			p.attackTarget(es[TARGET_MATCH]);
		}
		else {
			p.attackTarget(es[0]);
		}
		return DONE;
	}
	if (combat_words[VERB_MATCH] == "USE") {
		if (TARGET_MATCH != -1 && is_noun) {
			do_use_combat(TARGET_MATCH, inv, p);
			return DONE;
		}
		else if (SKILL_MATCH != -1 && TARGET_MATCH == -1) {
			//using a buff
			if (p.skillList[SKILL_MATCH].mp_cost <= p.mp) {
				if (p.skillList[SKILL_MATCH].type == Skill::BUFF) {
					
					p.use_skill(es[0], p.skillList[SKILL_MATCH].trigger);
					return DONE;
				}
				else {
					p.use_skill(es[0], p.skillList[SKILL_MATCH].trigger);
					return DONE;
				}
			}
			else {
				cout << p.name << " doesn't have enough mana left for that skill!" << endl;
				return NOT_DONE;
			}
		}
		else if (TARGET_MATCH != -1 && !is_noun) {
			//using a skill
			if (SKILL_MATCH == -1) {
				cout << p.name << " doesn't know that skill!" << endl;
				return NOT_DONE;
			}
			else {
				if (p.skillList[SKILL_MATCH].mp_cost <= p.mp) {
					if (p.skillList[SKILL_MATCH].type == Skill::BUFF) {
						
						p.use_skill(es[TARGET_MATCH], p.skillList[SKILL_MATCH].trigger);
						return DONE;
					}
					else {
						p.use_skill(es[TARGET_MATCH], p.skillList[SKILL_MATCH].trigger);
						return DONE;
					}
				}
				else {
					cout << p.name << " doesn't have enough mana left for that skill!" << endl;
					return NOT_DONE;
				}
			}
		}
	}
	if (combat_words[VERB_MATCH] == "FLEE") {
		cout << p.name << " attempts to flee!" << endl;
		return FLEE_ATTEMPT;
	}
	if (combat_words[VERB_MATCH] == "REST") {
		p.do_rest();
		return DONE;
	}
	if (combat_words[VERB_MATCH] == "STATS") {
		view_combat_stats(p, es);
		return NOT_DONE;
	}
	if (combat_words[VERB_MATCH] == "INVENTORY") {
		print_inventory(inv);
		return NOT_DONE;
	}
	if (combat_words[VERB_MATCH] == "HELP") {
		print_help();
		return NOT_DONE;
	}
	if (combat_words[VERB_MATCH] == "CANCEL") {
		if (SKILL_MATCH != -1)
		{
			if (p.skillList[SKILL_MATCH].type == Skill::BUFF) {
				p.end_buff(p.skillList[SKILL_MATCH].trigger);
				return DONE;
			}
			else {
				cout << "You can't end a non-buff!" << endl;
				return NOT_DONE;
			}
			
		}
		cout << "You don't know that buff." << endl;
		return NOT_DONE;
	}
}

void Combat::print_inventory(vector<pair<string, int>> &inv) {
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

void Combat::print_help() {
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

void Combat::view_combat_stats(Player &p, vector<Enemy> &es) {
	p.view_stats();
	for (unsigned int i = 0; i < es.size(); i++)
		es[i].view_stats();
}

bool Combat::combat_loop(Player &p, vector<pair<string, int>> &inv, vector<Enemy> &enemies) {
	init_combat(p, enemies);
	string command;
	vector<string> words;
	
	cout << "Prepare for battle!" << endl;
	cout << "You have encountered " << endl;

	for (unsigned int i = 0; i < enemies.size(); i++) {
		cout << enemies[i].code << ", ";
	}
	cout << endl;
	int turn = NOT_DONE;
	while (true) {
		while (turn == NOT_DONE) {
			command.clear();
			words.clear();
			cout << ">";
			getline(cin, command);
			transform(command.begin(), command.end(), command.begin(), (int(*)(int))toupper);
			words = combat_split(command);
			if (words.size() > 0)
				turn = parse_combat(words, enemies, p, inv);
		}
		if (turn == FLEE_ATTEMPT) {
			bernoulli_distribution escape(0.7);
			if (escape(gen)) {
				cout << p.name << " got away!" << endl;
				return true;
			}
			else {
				cout << p.name << " failed to escape!" << endl;
			}
		}

		for (unsigned int i = 0; i < enemies.size(); i++) {
			if (enemies[i].is_dead()) {
				cout << enemies[i].name << " has been defeated!" << endl;
				cout << p.name << " gained " << enemies[i].xpyield << " EXP!" << endl;
				p.victory(enemies[i].xpyield);
				inv[0].second += enemies[i].goldyield;
				cout << p.name << " earned " << enemies[i].goldyield << " GOLD!" << endl;
				enemies.erase(enemies.begin() + i);
			}
		}
		if (enemies.size() == 0) {
			cout << "The enemy mob was defeated!\n-----------------------" << endl;
			return true;
		}
		for (int i = 0; i < enemies.size(); i++) {
			enemies[i].attackTarget(p);
		}
		if (p.is_dead()) {
			cout << "You were defeated in battle...\n-----------------------" << endl;
			return false;
		}
		p.check_buff_end();
		p.apply_status();
		
		turn = NOT_DONE;
	}

	return false;
}

void Combat::init_combat(Player &p, vector<Enemy> &es) {
	string seed_str;
	seed_str += p.name;
	for (unsigned int i = 0; i < es.size(); i++)
		seed_str += es[i].name;

	random_shuffle(seed_str.begin(), seed_str.end());
	seed_seq seeds(seed_str.begin(), seed_str.end());

	gen.seed(seeds);
}

vector<string> Combat::combat_split(string line, char sep) {
	std::vector<std::string> tokens;
	std::size_t start = 0, end = 0;
	while ((end = line.find(sep, start)) != std::string::npos) {
		if (end != start) {
			tokens.push_back(line.substr(start, end - start));
		}
		start = end + 1;
	}
	if (end != start) {
		tokens.push_back(line.substr(start));
	}
	return tokens;
}