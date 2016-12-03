#include "Player.h"



Player::Player()
{
}

Player::Player(string nm, int sex, int h, int m, int s, int d, int f, int r, int sd, int l, long exp, long next,
	int base, string* equip) {
	name = nm;
	gender = sex;
	hp = max_hp = h;
	mp = max_mp = m;
	str = s;
	dur = d;
	force = f;
	resist = r;
	speed = sd;
	level = l;
	xp = exp;
	lvl_Up_xp = next;
	base_xp = base;
	equipped[0] = equip[0];
	equipped[1] = equip[1];
	equipped[2] = equip[2];
	equipped[3] = equip[3];
}

void Player::load_player(string nm, int c, int sex, int h, int m, int s, int d, int f, int r, int sd, int l, long exp, long next,
	int base, string* equip) {
	name = nm;
	classcode = c;
	gender = sex;
	hp = max_hp = h;
	mp = max_mp = m;
	str = s;
	dur = d;
	force = f;
	resist = r;
	speed = sd;
	level = l;
	xp = exp;
	lvl_Up_xp = next;
	base_xp = base;
	equipped[0] = equip[0];
	equipped[1] = equip[1];
	equipped[2] = equip[2];
	equipped[3] = equip[3];
}

Player::Player(const Player &other) {
	name = other.name;
	gender = other.gender;
	classcode = other.classcode;
	level = other.level;
	xp = other.xp;
	lvl_Up_xp = other.lvl_Up_xp;
	base_xp = other.base_xp;
	hp = other.hp;
	max_hp = other.max_hp;
	mp = other.mp;
	max_mp = other.max_mp;
	str = other.str;
	dur = other.dur;
	force = other.force;
	resist = other.resist;
	speed = other.speed;
	skillList.resize(0);
	learnset.resize(0);
	MAX_LEVEL = other.MAX_LEVEL;
	equipped[0] = other.equipped[0];
	equipped[1] = other.equipped[1];
	equipped[2] = other.equipped[2];
	equipped[3] = other.equipped[3];
}


Player::~Player()
{
}

void Player::create_class(vector<Skill> &all) {
	int gend = 0;
	int classType = 1;
	uniform_int_distribution<int> range(50, 150);
	
	string line;

	cout << "CHARACTER GENERATION" << endl;
	cout << "--------------------" << endl;

	cout << "Enter character's name:\n>";
	getline(cin, name);

	seed_seq seed(name.begin(), name.end());
	gen.seed(seed);

	cout << "Enter character's gender:\n1 = male, 2 = female\n>";
	getline(cin, line);
	gend = stoi(line);
	gender = gend;

	cout << "Enter your character's class:\n1 = brawler, 2 = shooter, 3= speedster\nEnter 0 for balanced\n>";
	getline(cin, line);
	classType = stoi(line);
	vector<int> lv;
	vector<string> codes;

	lv.push_back(2);
	lv.push_back(4);
	lv.push_back(6);
	lv.push_back(8);
	level = 1;
	switch (classType) {
		case 1: //brawler
			classcode = classType;
			hp = max_hp = 50;
			mp = max_mp = 30;
			str = 10;
			dur = 10;
			force = 7;
			resist = 7;
			speed = 7;
			base_xp = 760 + range(gen);
			codes.push_back("PUNCH");
			codes.push_back("STRIKE");
			codes.push_back("STRENGTH");
			codes.push_back("KAIOKEN");
			break;
		case 2: //shooter
			classcode = classType;
			hp = max_hp = 30;
			mp = max_mp = 50;
			str = 7;
			dur = 7;
			force = 10;
			resist = 10;
			speed = 10;
			base_xp = 875 + range(gen);
			codes.push_back("BALL");
			codes.push_back("SHOOTER");
			codes.push_back("POWER");
			codes.push_back("KAIOKEN");
			break;
		case 3:
			classcode = classType;
			hp = max_hp = 40;
			mp = max_mp = 40;
			str = 8;
			dur = 6;
			force = 6;
			resist = 8;
			speed = 12;
			base_xp = 725 + range(gen);
			codes.push_back("KICK");
			codes.push_back("BLITZ");
			codes.push_back("SPEED");
			codes.push_back("KAIOKEN");
			break;
		default:
			classcode = 0;
			hp = max_hp = 40;
			mp = max_mp = 40;
			str = 8;
			dur = 8;
			force = 8;
			resist = 8;
			speed = 8;
			base_xp = 925 + range(gen);
			codes.push_back("PUNCH");
			codes.push_back("SHOOTER");
			codes.push_back("SPEED");
			codes.push_back("KAIOKEN");
			break;
	}
	construct_learnset(all, codes, lv);
	MAX_LEVEL = maxes[0];
	lvl_Up_xp = base_xp;
}

void Player::create_player(vector<string> data, vector<Skill> &all) {

}

void Player::construct_learnset(vector<Skill> &all, vector<string> codes, vector<int> lvls) {
	unsigned int size = lvls.size();
	unsigned int count = 0;

	while (count < size) {
		for (unsigned int i = 0; i < all.size(); i++)
		{
			if (codes[count] == all[i].trigger) {
				learnset.push_back(make_pair(all[i], lvls[count]));
				count++;
				if (count == size)
					break;
			}
		}
	}

	assert(learnset.size() == size);

}

void Player::view_stats() {
	cout << name << "'s stats:\n---------------" << endl;
	cout << "Class:\t" << classnames[classcode] << endl;
	if (gender == 1) {
		cout << "Gender:  Male";
	}
	else {
		cout << "Gender:  Female";
	}
	cout << "\n---------------------" << endl;
	cout << "Level " << to_string(level) << endl;
	cout << "EXP:\t" << to_string(xp) << endl;
	cout << "To next level:\t" << to_string(lvl_Up_xp - xp) << endl;
	cout << "HP:\t" << to_string(hp) << " / " << to_string(max_hp) << endl;
	cout << "MP:\t" << to_string(mp) << " / " << to_string(max_mp) << endl;
	cout << "STR:\t" << to_string(str) << "\tDUR:\t" << to_string(dur) << endl;
	cout << "FOR:\t" << to_string(force) << "\tRESIST:\t" << to_string(resist) << endl;
	cout << "SPD:\t" << to_string(speed) << endl;
	cout << "---------------" << endl;
	print_equip();
	cout << "---------------" << endl;
	print_skills();
}

void Player::print_equip() {
	for (int i = 0; i < 4; i++) {
		cout << parts[i] << "\t" << equipped[i] << endl;
	}
}

void Player::victory(long exp) {
	xp += exp;
	level_up();
}

void Player::do_rest() {
	cout << name << " rests to recover." << endl;
	heal(max_hp / 4, max_mp / 4);
}

bool Player::do_equip(Equip e) {
	if (equipped[e.equipWhere] == "NOTHING") {
		equipped[e.equipWhere] = e.code;
		str += e.strChange;
		dur += e.durChange;
		force += e.forChange;
		resist += e.resChange;
		speed += e.spdChange;
		cout << "You have equipped the " << e.code << endl;
		return true;
	}
	else {
		cout << "You already have something equipped there." << endl;
		return false;
	}
}

bool Player::is_equipped_with(string e) {
	for (int i = 0; i < 4; i++) {
		if (equipped[i] == e) {
			return true;
		}
	}
	return false;
}

string Player::do_unequip(Equip e) {
	if (!is_equipped_with(e.code))
		return "NOTHING";
	else {
		equipped[e.equipWhere] = "NOTHING";
		str -= e.strChange;
		dur -= e.durChange;
		force -= e.forChange;
		resist -= e.resChange;
		speed -= e.spdChange;
		return e.code;
	}
}

void Player::level_up() {
	uniform_int_distribution<int> bigPBoost(10, 20);
	uniform_int_distribution<int> smlPBoost(5, 10);
	uniform_int_distribution<int> bigBoost(2, 4);
	uniform_int_distribution<int> midBoost(1, 3);
	uniform_int_distribution<int> smlBoost(0, 2);
	if (xp >= lvl_Up_xp) {
		while (xp >= lvl_Up_xp) {
			cout << "Level up! " << name << " is now level " << to_string(++level) << "!" << endl;
			if (level < MAX_LEVEL)
				lvl_Up_xp = level * level * base_xp;
			else
				lvl_Up_xp = (numeric_limits<long>::max)();

			if (learnset.size() > 0 && level >= learnset.front().second) {
				add_skill(learnset.front().first);
				cout << name << " has learned " << learnset.front().first.name <<
					" ( " << learnset.front().first.trigger << " )!" << endl;
				learnset.erase(learnset.begin());
			}
		}
		//check learnset
		
		//increase all stats randomly
		switch (classcode) {
		case 1: //brawler
			max_hp += bigPBoost(gen);
			max_mp += smlPBoost(gen);
			str += bigBoost(gen);
			dur += bigBoost(gen);
			force += smlBoost(gen);
			resist += midBoost(gen);
			speed += smlBoost(gen);
			break;
		case 2: //shooter
			max_hp += smlPBoost(gen);
			max_mp += bigPBoost(gen);
			str += smlBoost(gen);
			dur += smlBoost(gen);
			force += bigBoost(gen);
			resist += bigBoost(gen);
			speed += midBoost(gen);
			break;
		case 3: //speedster
			max_hp += smlPBoost(gen);
			max_mp += smlPBoost(gen);
			str += smlBoost(gen);
			dur += midBoost(gen);
			force += smlBoost(gen);
			resist += midBoost(gen);
			speed += bigBoost(gen);
			break;
		default: //balanced
			max_hp += bigPBoost(gen);
			max_mp += bigPBoost(gen);
			str += midBoost(gen);
			dur += midBoost(gen);
			force += midBoost(gen);
			resist += midBoost(gen);
			speed += midBoost(gen);
			break;
		}

		hp = max_hp;
		mp = max_mp;
	}
}

Player Player::save_player() {
	Player p(*this);
	return p;
}

void Player::load_data(vector<Skill> &data, vector<pair<Skill, int>> &learn) {
	if (skillList.size() > 0)
		skillList.clear();
	if (learnset.size() > 0)
		learnset.clear();

	for (unsigned int i = 0; i < data.size(); i++) {
		skillList.push_back(data[i]);
	}
	for (unsigned int i = 0; i < learn.size(); i++) {
		learnset.push_back(learn[i]);
	}
}

vector<Skill> Player::get_Skills() {
	return skillList;
}

vector<pair<Skill, int>> Player::get_learnset() {
	return learnset;
}

int Player::get_base_xp() {
	return base_xp;
}

int Player::get_class() {
	return classcode;
}