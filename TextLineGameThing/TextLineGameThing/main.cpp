/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.cpp
 * Author: Corey Allen
 *
 * Created on September 7, 2016, 9:27 PM
 */

#include <cstdlib>
#include <cstdio>
#include <cstring>

#include <iostream>
#include <string>
#include <vector>
#include <cctype>
#include <algorithm>
#include <fstream>
#include <random>

#include "global.h"
using namespace std;

enum en_DIRS {NORTH, EAST, SOUTH, WEST};
enum en_VERBS {GET, DROP, USE, OPEN, CLOSE, EXAMINE, INVENTORY, LOOK,
            TALK, SHOP_VERB, STATS, EQUIP, UNEQUIP, ENGAGE, REST, VERBS_MAX};
enum en_ENEMYCODES {BOSS_MAD, TRAINER_DAVE, THUG, RAT, BEAST, ENEMY_MAX};
            
const int NONE = -1;
const int DIRS = 4;
const int VERBS = VERBS_MAX;
const int ENEMY = ENEMY_MAX;

enum en_SHOP_VERBS {BUY, SELL, MENU, EXIT, INVENTORY_S, HELP, SHOP_MAX};
const int SHOP_VERBS = SHOP_MAX;

enum en_PARTS {HEAD, TORSO, ARMS, LEGS, PARTS_MAX};
const int PARTS = PARTS_MAX;
struct word {
    string text;
    int code;
};


static bool gate_open = false;

#include "rooms.h"
#include "nouns.h"
noun nouns[NOUNS_MAX];
const int NOUNS = NOUNS_MAX;
#include "Player.h"
#include "npc.h"
struct enemy_data {
    string nm;
    string word;
    int g_code = -1;
    int m_hp;
    int m_mp;
    int s;
    int d;
    int f;
    int r;
    int sd;
    //vector<string> sk;
    int xyield;
    int gyield;
};
#include "Enemy.h"
#include "combat.h"


const string prompt = ">";
const string STORYINFO = "A small town has been taken over by the powerful Boss Mad. He rules over it with his massive ego and iron fist. You have to defeat him, and save the town's inhabitants from the rule, before the city gets any worse. Boss Mad, awaits the challengers in the square, assured of his own strength. Can you knock him off his pedestal?";
char border[80] = {'\0'};
//vector<Skill> allSkills;
word directions[DIRS];
word verbs[VERBS];
word shop_verbs[SHOP_VERBS];
enemy_data enemies[ENEMY_MAX];
string command;
vector<string> words;
vector<string> shopWords;
int loc = SQUARE;
vector< pair<noun, int> > inventory;
Player player;
default_random_engine main_gen;
uniform_real_distribution<double> encDis(0.0, 1.0);
uniform_int_distribution<int> encNum(1,3);
bool flag = false;

void set_verbs(word *vbs) {
    vbs[GET].code = GET;
    vbs[GET].text = "GET";
    vbs[DROP].code = DROP;
    vbs[DROP].text = "DROP";
    vbs[USE].code = USE;
    vbs[USE].text = "USE";
    vbs[OPEN].code = OPEN;
    vbs[OPEN].text = "OPEN";
    vbs[CLOSE].code = CLOSE;
    vbs[CLOSE].text = "CLOSE";
    vbs[EXAMINE].code = EXAMINE;
    vbs[EXAMINE].text = "EXAMINE";
    vbs[INVENTORY].code = INVENTORY;
    vbs[INVENTORY].text = "INVENTORY";
    vbs[LOOK].code = LOOK;
    vbs[LOOK].text = "LOOK";
    vbs[TALK].code = TALK;
    vbs[TALK].text = "TALK";
    vbs[SHOP_VERB].code = SHOP_VERB;
    vbs[SHOP_VERB].text = "SHOP";
    vbs[STATS].code = STATS;
    vbs[STATS].text = "STATS";
    vbs[EQUIP].code = EQUIP;
    vbs[EQUIP].text = "EQUIP";
    vbs[UNEQUIP].code = UNEQUIP;
    vbs[UNEQUIP].text = "UNEQUIP";
    vbs[ENGAGE].code = ENGAGE;
    vbs[ENGAGE].text = "ENGAGE";
    vbs[REST].code = REST;
    vbs[REST].text = "REST";
}

void set_directions(word *dir) {
    dir[NORTH].code = NORTH;
    dir[NORTH].text = "NORTH";
    dir[EAST].code = EAST;
    dir[EAST].text = "EAST";
    dir[SOUTH].code = SOUTH;
    dir[SOUTH].text = "SOUTH";
    dir[WEST].code = WEST;
    dir[WEST].text = "WEST"; 
}

void set_nouns() {
    //enum en_NOUNS { SWORD, SHIELD, SENZU, GOLD, ODD_STATUE, BOX };
    nouns[SWORD].code = SWORD;
    nouns[SWORD].word = "SWORD";
    nouns[SWORD].desc = "sword";
    nouns[SWORD].longdesc = "A sword made of steel. Given to you at start of the game.";
    nouns[SWORD].can_equip = true;
    nouns[SWORD].where = ARMS;
    nouns[SWORD].strUp = 3;
    nouns[SWORD].location = NONE;
    
    nouns[SHIELD].code = SHIELD;
    nouns[SHIELD].word = "SHIELD";
    nouns[SHIELD].desc = "shield";
    nouns[SHIELD].longdesc = "A shield made of steel. Given to you at start of the game.";
    nouns[SHIELD].can_equip = true;
    nouns[SHIELD].where = TORSO;
    nouns[SHIELD].durUp = 3;
    nouns[SHIELD].resUp = 2;
    nouns[SHIELD].location = NONE;
    
    nouns[SENZU].code = SENZU;
    nouns[SENZU].word = "SENZU";
    nouns[SENZU].desc = "senzu bean";
    nouns[SENZU].longdesc = "An odd bean that apparently restores all your health and mana.";
    nouns[SENZU].can_use = true;
    nouns[SENZU].use_sentence = "You feel incredibly full and healthy. HP restored! MP restored!";
    nouns[SENZU].hpUp = 9999;
    nouns[SENZU].mpUp = 9999;
    nouns[SENZU].location = LAB;
    
    nouns[OINTMENT].code = OINTMENT;
    nouns[OINTMENT].word = "OINTMENT";
    nouns[OINTMENT].desc = "wound ointment";
    nouns[OINTMENT].longdesc = "An ointment that goes on wounds. It stings, but will restore health.";
    nouns[OINTMENT].can_use = true;
    nouns[OINTMENT].hpUp = 25;
    nouns[OINTMENT].use_sentence = "You rub the ointment on your wounds. HP restored!";
    nouns[OINTMENT].location = NONE;
    
    nouns[POTION].code = POTION;
    nouns[POTION].word = "POTION";
    nouns[POTION].desc = "magic potion";
    nouns[POTION].longdesc = "A potion that you drink to restore your mana. It is awfully bitter, though...";
    nouns[POTION].can_use = true;
    nouns[POTION].mpUp = 25;
    nouns[POTION].use_sentence = "You drink the magic potion. MP restored!";
    nouns[POTION].location = NONE;
    
    nouns[GOLD].code = GOLD;
    nouns[GOLD].word = "GOLD";
    nouns[GOLD].desc = "gold";
    nouns[GOLD].longdesc = "The currency of the land, which glimmers under the faintest of light. Guard it well.";
    nouns[GOLD].location = NONE;

    nouns[ODD_STATUE].code = ODD_STATUE;
    nouns[ODD_STATUE].word = "STATUE";
    nouns[ODD_STATUE].desc = "statue";
    nouns[ODD_STATUE].longdesc = "A statue of Boss Mad. Boss Mad's gonna be pissed when he sees that turd spraypainted on it...";
    nouns[ODD_STATUE].can_carry = false;
    nouns[ODD_STATUE].location = STATUE;
    
    nouns[BOX].code = BOX;
    nouns[BOX].word = "BOX";
    nouns[BOX].desc = "cardboard box";
    nouns[BOX].longdesc = "A cardboard box. Why is this even here!?";
    nouns[BOX].can_equip = true;
    nouns[BOX].where = HEAD;
    nouns[BOX].location = DOJO;
    
    nouns[GATE].code = GATE;
    nouns[GATE].word = "GATE";
    nouns[GATE].desc = "a rusty gate";
    nouns[GATE].longdesc = "A rusty gate separating the park from a statue of Boss Mad.";
    nouns[GATE].can_carry = false;
    nouns[GATE].location = PARK;
}

void print_good_ending() {
    textWrap("Congrats, Boss Mad has been defeated! You have liberated the city! You are the hero of the day!");
    textWrap("It's all thanks to you, " + player.name + "!");
    textWrap("Final stats: \n");
    player.view_stats();
    printf("%s\n", border);
    flag = true;
}

const string SKILLFILE = "allskills.txt";

void set_shop(word *vbs) {
    vbs[BUY].code = BUY;
    vbs[BUY].text = "BUY";
    vbs[SELL].code = SELL;
    vbs[SELL].text = "SELL";
    vbs[MENU].code = MENU;
    vbs[MENU].text = "MENU";
    vbs[EXIT].code = EXIT;
    vbs[EXIT].text = "EXIT";
    vbs[INVENTORY_S].code = INVENTORY_S;
    vbs[INVENTORY_S].text = "INVENTORY";
    vbs[HELP].code = HELP;
    vbs[HELP].text = "HELP";
}

void set_enemy_data() {
    //enum en_ENEMYCODES {BOSS_MAD, TRAINER_DAVE, THUG, RAT, BEAST, ENEMY_MAX};
    enemies[BOSS_MAD].nm = "Boss Mad";
    enemies[BOSS_MAD].word = "BOSS";
    enemies[BOSS_MAD].g_code = 0;
    enemies[BOSS_MAD].m_hp = 300;
    enemies[BOSS_MAD].m_mp = 200;
    enemies[BOSS_MAD].s = 30;
    enemies[BOSS_MAD].d = 27;
    enemies[BOSS_MAD].f = 25;
    enemies[BOSS_MAD].r = 20;
    enemies[BOSS_MAD].sd = 35;
    enemies[BOSS_MAD].xyield = 1500;
    enemies[BOSS_MAD].gyield = 1000;
    
    enemies[TRAINER_DAVE].nm = "Trainer Dave";
    enemies[TRAINER_DAVE].word = "TRAINER";
    enemies[TRAINER_DAVE].g_code = 0;
    enemies[TRAINER_DAVE].m_hp = 150;
    enemies[TRAINER_DAVE].m_mp = 250;
    enemies[TRAINER_DAVE].s = 20;
    enemies[TRAINER_DAVE].d = 20;
    enemies[TRAINER_DAVE].f = 30;
    enemies[TRAINER_DAVE].r = 30;
    enemies[TRAINER_DAVE].sd = 30;
    enemies[TRAINER_DAVE].xyield = 1000;
    enemies[TRAINER_DAVE].gyield = 500;
    
    enemies[THUG].nm = "Thug";
    enemies[THUG].word = "THUG";
    enemies[THUG].m_hp = 35;
    enemies[THUG].m_mp = 20;
    enemies[THUG].s = 7;
    enemies[THUG].d = 7;
    enemies[THUG].f = 8;
    enemies[THUG].r = 8;
    enemies[THUG].sd = 9;
    enemies[THUG].xyield = 360;
    enemies[THUG].gyield = 210;
    
    enemies[RAT].nm = "Mutated Rat";
    enemies[RAT].word = "RAT";
    enemies[RAT].m_hp = 50;
    enemies[RAT].m_mp = 30;
    enemies[RAT].s = 11;
    enemies[RAT].d = 11;
    enemies[RAT].f = 13;
    enemies[RAT].r = 13;
    enemies[RAT].sd = 10;
    enemies[RAT].xyield = 400;
    enemies[RAT].gyield = 180;
    
    enemies[BEAST].nm = "Mutant Beast";
    enemies[BEAST].word = "BEAST";
    enemies[BEAST].m_hp = 95;
    enemies[BEAST].m_mp = 50;
    enemies[BEAST].s = 14;
    enemies[BEAST].d = 14;
    enemies[BEAST].f = 20;
    enemies[BEAST].r = 20;
    enemies[BEAST].sd = 18;
    enemies[BEAST].xyield = 900;
    enemies[BEAST].gyield = 550;
}
int find_gold(vector< pair<noun, int> > &inv) {
    for (int i = 0; i < inv.size(); i++) {
        if (inv[i].first.word == "GOLD") {
            return i;
        }
    }
}
/*
 * 
 * 
 */
void set_skill_data() {
    ifstream skillFile;
    skillFile.open(SKILLFILE);
    string line;
    vector<string> toks(11);
    while (!skillFile.eof()) {
        Skill s;
        line.clear();
        toks.clear();
        getline(skillFile, line);
        toks = split(line, ',');
        s.name = toks[0];
        s.word = toks[1];
        s.attackType = stoi(toks[2]);
        s.mp_cost = stoi(toks[3]);
        s.hp_cost = stoi(toks[4]);
        s.str_mod = stoi(toks[5]);
        s.dur_mod = stoi(toks[6]);
        s.force_mod = stoi(toks[7]);
        s.resist_mod = stoi(toks[8]);
        s.speed_mod = stoi(toks[9]);
        s.base_damage = stod(toks[10]);
        allSkills.push_back(s);
    }
    skillFile.close();
}

vector<Enemy> check_encounter(room &r) {
    vector<Enemy> retVal;
    uniform_int_distribution<int> rng(1, r.enemycodes.size() * 5);
    uniform_int_distribution<int> gains(50, 200);
    int hordeSize = encNum(main_gen);
    if (encDis(main_gen) < r.encRate) {  
        //for now, a single enemy is created.
        int code;
        enemy_data dat;
        if (r.enemycodes.size() == 1)
        {
            code = r.enemycodes[0];
            dat = enemies[code];
            //(const string &nm, const string &w, int sex, int health, int mana, int strength,
          //int durability, int f, int res, int spd, int xreward, int greward)
            for (int i = 0; i < hordeSize; i++) { 
                Enemy tmp;
                tmp.set_stats(dat.nm, dat.word, dat.g_code, dat.m_hp, dat.m_mp, dat.s,
                        dat.d, dat.f, dat.r, dat.sd, dat.xyield + gains(main_gen), dat.gyield + gains(main_gen));
                retVal.push_back(tmp);
            }
        }
        else {
            for (int i = 0; i < hordeSize; i++) {
                code = r.enemycodes[rng(main_gen) % r.enemycodes.size()];
                dat = enemies[code];
                Enemy tmp;
                tmp.set_stats(dat.nm, dat.word, dat.g_code, dat.m_hp, dat.m_mp, dat.s,
                        dat.d, dat.f, dat.r, dat.sd, dat.xyield + gains(main_gen), dat.gyield+ gains(main_gen));
                retVal.push_back(tmp);
            }
        }
        
    }
    return retVal;
}

vector<Enemy> do_engage(npc &n) {
    vector<Enemy> retVal;
    enemy_data dat;
    for (int i = 0; i < ENEMY_MAX; i++)
    {
        if (n.word == enemies[i].word)
        {
            dat = enemies[i];
            break;
        }
    }

    Enemy tmp;
    tmp.set_stats(dat.nm, dat.word, dat.g_code, dat.m_hp, dat.m_mp, dat.s,
            dat.d, dat.f, dat.r, dat.sd, dat.xyield, dat.gyield);
    retVal.push_back(tmp);
    return retVal;
}

int get_max_speed(vector<Enemy> &es) {
    int m = -1;
    int index = 0;
    for (int i = 0; i < es.size(); i++) {
        if (m <= es[i].speed)
        {
            m = es[i].speed;
            index = i;
        }
    }
    return index;
}

void do_combat(vector< pair<noun, int> > &inv, vector<Enemy> &es) {
    bool isOver = false;
    printf("%s\n", border);
    cout << "Prepare for battle!" << endl;
    //Print out the enemies
    cout << "You have encountered ";
    for (int i = 0; i < es.size(); i++)
    {
        cout << es[i].word << ", ";
    }
    cout << endl;
    string combat_command;
    vector<string> combat_words;
    int result = 0;
    //Loop until break
    while (true) {
        while (result == 0) {
            combat_command.clear();
            combat_words.clear();
            cout << prompt;
            getline(cin, combat_command);

            transform(combat_command.begin(), combat_command.end(), combat_command.begin(), 
                (int(*)(int))toupper);

            combat_words = split(combat_command, ' ');
            result = parse_combat(combat_words, es, player, inv);
        }
        if (result == 2)
        {
            int maxEnemy = get_max_speed(es);
            if (player.speed >= 2 * es[maxEnemy].speed) {
                cout << "You managed to escape!" << endl;
                return;
            }
            else if (es[maxEnemy].speed >= 2 * player.speed) {
                cout << "No escape!" << endl;
            }
            else {
                
                bernoulli_distribution escDis(0.5);
                if (escDis(main_gen))
                {
                    cout << "You managed to get away!" << endl;
                    return;
                }
                else {
                    cout << "You failed to escape!" << endl;
                }
            }
        }

        for (int i = 0; i < es.size(); i++) {
            if (es[i].is_dead())
            {
                int the_gold = find_gold(inv);
                cout << es[i].word << " has been defeated!" << endl;
                cout << "You have gained " << es[i].xpyield << " EXP!" << endl;
                player.victory(es[i].xpyield);
                inv[the_gold].second += es[i].goldyield;
                cout << "You have earned " << es[i].goldyield << " GOLD!" << endl;
                if (es[i].word == "BOSS") 
                    isOver = true;
             

                es.erase(es.begin() + i);
                
            }
        }
        if (es.size() == 0) {
            cout << "The enemy mob has been defeated!" << endl;
            printf("%s\n", border);
            if (isOver)
                print_good_ending();
            return;
        }
        for (int i = 0; i < es.size(); i++)
        {
            es[i].attackTarget(player);
        }
        if (player.is_dead())
        {
            cout << "You were defeated in battle!" << endl;
            if (es[0].word == "TRAINER") {
                player.heal(9999, 9999);
                cout << "Trainer Dave heals your wounds." << endl;
                cout << "You gain some XP anyway!" << endl;
                player.victory(es[0].xpyield / 2);
            }
            
            printf("%s\n", border);
            return;
        }
        result = 0;
        
    }
    
}

void look_around(int &loc, room *rms, word *dir, noun *nns, npc *ns) {
    int i;
    textWrap(rms[loc].desc);
    textWrap(rms[loc].longdesc);
    
    for (i = 0; i < DIRS; i++) {
        if(rms[loc].exits_to_room[i] != NONE) {
            textWrap("There is an exit " + dir[i].text + " to a " + rms[rms[loc].exits_to_room[i]].desc + ".");
        }
    }
    
    for (i = 0; i < NOUNS; i++) {
        if (nns[i].location == loc) {
            textWrap("You see " + nns[i].desc + " somewhere nearby.");
            if (nns[i].code == GATE) {
                if (loc == STATUE || loc == PARK) {
                    if (gate_open) {
                        textWrap("The gate is open.");
                    }
                    else {
                        textWrap("The gate is closed.");
                    }
                }
            }
        }
    }
    for (i = 0; i < NPCS; i++) {
        if (ns[i].location == loc) {
            textWrap("Nearby, " + ns[i].name + " awaits.   " + ns[i].word);
        }
    }
}
void print_inventory(vector< pair<noun, int> > &inv) {
    if (inv.size() == 0) {
        cout << "You have absolutely nothing!" << endl;
    }
    else {
        cout << "----------------\nPlayer's Inventory\nName\tQuantity" << endl;
        for (int i = 0; i < inv.size(); i++) {
            cout << inv[i].first.word << "\t" << inv[i].second << endl;
        }
        cout << "----------------" << endl;
    }
}

void print_shop(vector< pair<noun, int> > &menu) {
    cout << "----------------\nShop's Menu\nName\tCost in Gold" << endl;
    for (int i = 0; i < menu.size(); i++) {
        cout << menu[i].first.word << "\t" << menu[i].second << endl;
    }
    cout << "----------------" << endl;
}

void do_examine(int &loc, vector< pair<noun, int> > &inv, noun *nns, npc *ns, 
        int &match, bool &non){
    for (int i = 0; i < inv.size(); i++) {
        if (inv[i].first.code == match && !non) {
            textWrap(inv[i].first.longdesc);
            if (inv[i].second > 1) {
                textWrap("You have " + to_string(inv[i].second) + " of these.");
            }
            return;
        }
    }
    for (int i = 0; i < NOUNS; i++) {
        if (nns[i].location == loc && !non) {
            textWrap(nns[i].longdesc);
            return;
        }
    }
    for (int i = 0; i < NPCS; i++) {
        if (ns[i].location == loc && non){
            textWrap(ns[i].desc);
            return;
        }
    }
    
    textWrap("That is not here.");
}

void do_get(int &loc, vector< pair<noun, int> > &inv, noun *nns, int &match){
    
    if (loc == nns[match].location && nns[match].can_carry){ 
        nns[match].location = NONE;
    }                                                                                                   
    else if (!nns[match].can_carry) {
        textWrap("You cannot grab this.");
        return;
    }
    else {
        textWrap("That is not here.");
        return;
    }
    for (int i = 0; i < inv.size(); i++) {
        if (inv[i].first.code == match) {
            textWrap("You pick up another " + nns[match].word + " and stash it "
                    + "with the item(s) of the same type.");
            inv[i].second += 1;
            return;
        }
    }
    inv.push_back(pair<noun, int>(nns[match], 1));
    textWrap("You pick up the " + nns[match].word + " and put it in your inventory.");
}

void do_drop(int &loc, vector< pair<noun, int> > &inv, noun *nns, int &match){
    if (inv.size() == 0) {
        textWrap("You have nothing to drop!");
        return;
    }
    for (int i = 0; i < inv.size(); i++) {
        if (inv[i].first.code == match) {
            if (inv[i].first.word == "GOLD") {
                textWrap("What are you doing!? You shouldn't drop gold!");
                return;
            }
            textWrap("You drop the "  + nns[match].word + " where you stand.");
            inv[i].second -= 1;
            if (inv[i].second <= 0) {
                inv.erase(inv.begin()+i);
            }
            nns[match].location = loc;
            return;
        }
    }
    textWrap("You do not have this.");
}

void do_use(vector< pair<noun, int> > &inv, noun *nns, int &match){
    for (int i = 0; i < inv.size(); i++) {
        if (inv[i].first.code == match && inv[i].first.can_use) {
            textWrap("You use one " + nns[match].word + ".");
            textWrap(nns[match].use_sentence);
            player.heal(nns[match].hpUp, nns[match].mpUp);
            inv[i].second -= 1;
            if (inv[i].second <= 0) {
                inv.erase(inv.begin()+i);
            }
            return;
        }
    }
    textWrap("You do not have this.");
}

void do_open(int &loc, int &match, noun *nns, room *rms) {
    if (match == GATE) {
        if (loc == STATUE || loc == PARK) {
            if (gate_open == false) {
                gate_open = true;
                rms[STATUE].exits_to_room[WEST] = PARK;
                rms[PARK].exits_to_room[EAST] = STATUE;
                textWrap("You open the gate.");
            }
            else {
                textWrap("The gate is already open.");
            }
        }
        else {
            textWrap("There is no gate to open here.");
        }
    }
    else {
         textWrap("You can't open that.");
    }
}

void do_close(int &loc, int &match, noun *nns, room *rms) {
    if (match == GATE) {
        if (loc == STATUE || loc == PARK) {
            if (gate_open == true) {
                gate_open = false;
                rms[STATUE].exits_to_room[WEST] = NONE;
                rms[PARK].exits_to_room[EAST] = NONE;
                textWrap("You close the gate.");
            }
            else {
                textWrap("The gate is already closed.");
            }
        }
        else {
            textWrap("There is no gate to close here.");
        }
    }
    else {
        textWrap("You can't close that.");
    }
}

void do_talk(int &loc, int &match, npc *ns) {
    if (match == NONE) {
        textWrap("They do not exist!");
    }
    else if (ns[match].location != loc) {
        textWrap("They are not here.");
    }
    else {
        textWrap(ns[match].name + " says:  \"" + ns[match].greeting + "\"");
    }
    
}

bool is_in_inv(vector< pair<noun, int> > &inv, int &nmatch) {
    for (int i = 0; i < inv.size(); i++) {
        if (inv[i].first.code == nmatch) {
            return true;
        }
    }
    return false;
}

int find_item(vector< pair<noun, int> > &inv, int &nmatch) {
    for (int i = 0; i < inv.size(); i++) {
        if (inv[i].first.code == nmatch) {
            return i;
        }
    }
    return NONE;
}

void do_shop(int &loc, int &match, npc *ns, vector< pair<noun, int> > &inv) {
    
    if (match == NONE) {
        textWrap("They do not exist");
        return;
    }
    if (ns[match].location != loc) {
        textWrap("They can't sell you stuff here!");
        return;
    }
    if (ns[match].location == loc && !ns[match].is_shopkeeper) {
        textWrap("They will not barter with you.");
        return;
    }
    if (ns[match].location == loc && ns[match].is_shopkeeper) {
        int timesInLoop = 0;
        bool flag = false;
        int goldIndex;
        while (true) {
            command.clear();
            shopWords.clear();
            goldIndex = find_gold(inv);
            if (timesInLoop == 0) {
                textWrap(ns[match].name + " says:  \"" + 
                        ns[match].shop_init + "\"");
                print_shop(ns[match].items);
            }
            else {
                if (!flag) {
                    textWrap(ns[match].name + " says:  \"" + 
                        ns[match].shop_continue + "\"");
                }
                else {
                    textWrap(ns[match].name + " says:  \"" + 
                        ns[match].shop_end + "\"");
                    return;
                }
            }

            cout << prompt;
            getline(cin, command);
        
            transform(command.begin(), command.end(), command.begin(), 
                    (int(*)(int))toupper);
            shopWords = split(command, ' ');
            int SHOP_ACTION = NONE;
            int NOUN_MATCH = NONE;
            for (int i = 0; i < SHOP_VERBS; i++) {

                if (shopWords[0] == shop_verbs[i].text) {
                    SHOP_ACTION = shop_verbs[i].code;
                    break;
                }
            }
            int n = 0;
            for (int i = 0; i < ns[match].items.size(); i++) {
                if (shopWords.size() > 1 && shopWords[1] == ns[match].items[i].first.word) {
                    NOUN_MATCH = ns[match].items[i].first.code;
                    n = i;
                    break;
                }
            }
            if (SHOP_ACTION == NONE) {
                textWrap("Invalid command or noun entered. You are in a shop now.");
                timesInLoop++;
                continue;
            }
            switch (SHOP_ACTION) {
                case BUY:
                    if (NOUN_MATCH != NONE) {

                        if (inv[goldIndex].second >= ns[match].items[n].second) {
                            inv[goldIndex].second -= ns[match].items[n].second;
                            if (!is_in_inv(inv, NOUN_MATCH))
                                inv.push_back(pair<noun, int>(nouns[NOUN_MATCH], 1));
                            else {
                                int index = find_item(inv, NOUN_MATCH);
                                inv[index].second += 1;
                            }
                            textWrap("You have bought one " + nouns[NOUN_MATCH].word + ".");
                            break;
                        }
                        else {
                            textWrap("Buying that won't work...");
                            break;
                        }
                        
                    }
                    else {
                        textWrap("That is not sold here. Use \"MENU\" to see the items offered here.");
                    }
                    break;
                case SELL:
                    if (NOUN_MATCH != NONE) {
                        int index = find_item(inv, NOUN_MATCH);
                        if (inv[index].first.word == "GOLD") {
                            textWrap("What are you doing!? You shouldn't sell gold!");
                            break;
                        }
                        if (index == NONE) {
                            textWrap("You don't have this!");
                            break;
                        }
                        textWrap("You sell the "  + nouns[NOUN_MATCH].word + " to " + ns[match].name +  ".");
                        inv[index].second -= 1;
                        inv[goldIndex].second += 10;
                        if (inv[index].second <= 0) {
                            inv.erase(inv.begin()+index);
                        }
                    }
                    else {
                        textWrap("That is not an item you have.");
                    }
                    break;
                case MENU:
                    print_shop(ns[match].items);
                    break;
                case EXIT:
                    flag = true;
                    break;
                case INVENTORY_S:
                    print_inventory(inv);
                    break;
                case HELP:
                    display_help();
                    break;
            }
            timesInLoop++;
        }
    }
    
}

bool parse_command(int &loc, vector<string> &wds, word *dir, word *vbs, 
        room *rms, noun *nns, npc *ns, vector< pair<noun, int> > &inv) {

    for (int i = 0; i < DIRS; i++) {
        if (wds[0] == dir[i].text) {
            if (rms[loc].exits_to_room[dir[i].code] != NONE) {
                loc = rms[loc].exits_to_room[dir[i].code];
                textWrap(rms[loc].desc);
                textWrap(rms[loc].longdesc);
                if (rms[loc].encRate > 0.0) {
                    vector<Enemy> enemies = check_encounter(rms[loc]);
                    if (enemies.size() > 0) {
                        do_combat(inv, enemies);
                    }
                }
                return true;
            }
            else{
                textWrap("No exit that way.");
                return true;
            }
        }
    }
    
    int NOUN_MATCH = NONE;
    int VERB_ACTION = NONE;
    int NPC_MATCH = NONE;
    bool is_npc;

    for (int i = 0; i < VERBS; i++) {

        if (wds[0] == vbs[i].text) {
            VERB_ACTION = vbs[i].code;
            break;
        }
    }

    if (wds.size() > 1 && wds[1] != "") {
        for (int i = 0; i < NOUNS; i++) {
            if (wds[1] == nns[i].word) {
                NOUN_MATCH = nns[i].code;
                is_npc = false;
                break;
            }
        }
        for (int i = 0; i < NPCS; i++){
            if (wds[1] == ns[i].word) {
                NPC_MATCH = ns[i].code;
                is_npc = true;
                break;
            }
        }
    }

    //Consider switch statement or further decomposition?
    switch (VERB_ACTION) {
        case LOOK:
            look_around(loc, rms, dir, nns, ns);
            return true;
        case INVENTORY:
            print_inventory(inv);
            return true;
        case EXAMINE:
            if (NOUN_MATCH != NONE) { 
                do_examine(loc, inv, nns, ns, NOUN_MATCH, is_npc);
                return true;
            }
            else if (NPC_MATCH != NONE) {
                do_examine(loc, inv, nns, ns, NPC_MATCH, is_npc);
                return true;
            }
        case GET:
            do_get(loc, inv, nns, NOUN_MATCH);
            return true;
        case DROP:
            do_drop(loc, inv, nns, NOUN_MATCH);
            return true;
        case USE:
            do_use(inv, nns, NOUN_MATCH);
            return true;
        case OPEN:
            do_open(loc, NOUN_MATCH, nns, rms);
            return true;
        case CLOSE:
            do_close(loc, NOUN_MATCH, nns, rms);
            return true;
        case TALK:
            do_talk(loc, NPC_MATCH, ns);
            return true;
        case SHOP_VERB:
            do_shop(loc, NPC_MATCH, ns, inv);
            return true;
        case STATS:
            player.view_stats();
            return true;
            break;
        case EQUIP:
            if (NOUN_MATCH != NONE && is_in_inv(inv, NOUN_MATCH) && nouns[NOUN_MATCH].can_equip) {
                
                bool suc = player.do_equip(nouns[NOUN_MATCH]);
                if (suc) {
                    int index = find_item(inv, NOUN_MATCH);
                    inv[index].second -= 1;
                    if (inv[index].second <= 0) {
                        inv.erase(inv.begin()+index);
                    }
                }
                else 
                    textWrap("Equip failed.");
            }
            else
                textWrap("You can't equip that.");
            return true;
            break;
        case UNEQUIP:
            if (NOUN_MATCH != NONE && player.is_equipped_with(nouns[NOUN_MATCH])) {
                noun temp = player.do_unequip(nouns[NOUN_MATCH]);
                if (!is_in_inv(inv, NOUN_MATCH))
                    inv.push_back(pair<noun, int>(temp, 1));
                else {
                    int index = find_item(inv, NOUN_MATCH);
                    inv[index].second += 1;
                }
                textWrap("You have unequipped the " + nouns[NOUN_MATCH].word + ".");
            }
            else
                textWrap("You are not currently equipped with that.");
            return true;
            break;
        case ENGAGE:
            if (NPC_MATCH != NONE) {
                textWrap(ns[NPC_MATCH].name + " says:  \"" + ns[NPC_MATCH].on_engage + "\"");
                if (ns[NPC_MATCH].can_engage) {
                    vector<Enemy> enemy = do_engage(ns[NPC_MATCH]);
                    do_combat(inv, enemy);
                }
            }
            return true;
            break;
        case REST:
            player.rest();
            if (rms[loc].encRate > 0.0) {
                vector<Enemy> enemies = check_encounter(rms[loc]);
                if (enemies.size() > 0) {
                    do_combat(inv, enemies);
                }
            }
            return true;
        default: //NONE
            textWrap("No valid command entered.");
            return true;
    }

    return false;
}

void print_story() {
    textWrap(STORYINFO);
    printf("%s\n", border);
}



/*
 * 
 */
int main(int argc, char** argv) {
    
    fill_n(border, 80, '-');
    set_directions(directions);
    set_verbs(verbs);
    set_shop(shop_verbs);
    set_enemy_data();
    set_combat_verbs();
    set_rooms();
    set_nouns();
    set_npcs();
    set_skill_data();
    
    
    inventory.push_back(pair<noun, int>(nouns[SWORD], 1));
    inventory.push_back(pair<noun, int>(nouns[SHIELD], 1));
    inventory.push_back(pair<noun, int>(nouns[GOLD], 100));
    
    player.createClass(allSkills);
    seed_seq seed(player.name.rbegin(), player.name.rend());
    main_gen.seed(seed);
    print_story();
    display_help();
    textWrap(rooms[loc].desc);
    textWrap(rooms[loc].longdesc);
    while (!flag) {
        command.clear();
        words.clear();
        if (player.is_dead())
        {
            textWrap("You were defeated, leaving the city under the iron fist of Boss Mad.");
            break;
        }
        cout << prompt;
        getline(cin, command);
        
        transform(command.begin(), command.end(), command.begin(), 
                (int(*)(int))toupper);
        
        words = split(command, ' ');
        
        if (loc == PARK) nouns[GATE].location = PARK;
        if (loc == STATUE) nouns[GATE].location = STATUE;
        
        if (words[0] == "QUIT") {
            flag = true;
        }
        else if (words[0] == "HELP") {
            display_help();
        }
        else if (words[0] == "STORY") {
            print_story();
        }
        else if (words.size() > 2) {
                textWrap("Error! More than two words were entered, this is not supported right now!");   
        }
        else {
            parse_command(loc, words, directions, verbs, rooms, nouns, npcs, inventory);
        }
    }
    cout << "Goodbye!" << endl;
    return 0;
}