/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "Player.h"

Player::Player() {
    name = "Default";
    gender = 0;
    classname = "None";
    max_hp = 0;
    hp = max_hp;
    max_mp = 0;
    mp = max_mp;
    str = 0;
    dur = 0;
    force = 0;
    resist = 0;
    speed = 0;
    level = 1;
    xp = 0;
    lvlUp_xp = 0;
    equipped.resize(4);
}

void Player::createClass(vector<Skill> &all) {
    int gend = 0;
    int classType = 1;
    uniform_int_distribution<int> range(50, 150);
    string line;
    cout << "CHARACTER GENERATION" << endl;
    cout << "--------------------" << endl;
    
    cout << "Enter character's name:\n>";
    getline(cin, name);
    seed_seq seed (name.begin(), name.end());
    gen.seed(seed);
    cout << "Your character's gender? 0 = male, 1 = female\n>";
    getline(cin, line);
    gend = stoi(line);
    if (gend >= 2) {
        gender = 1;
    }
    else if (gend < 0) {
        gender = 0;
    }
    else {
        gender = gend;
    }
    
    cout << "Your character's class? 1 = brawler, 2 = shooter, 3 = speedster\n>";
    getline(cin, line);
    classType = stoi(line);
    vector<int> lv;
    vector<int> codes;
    lv.push_back(2);
    lv.push_back(4);
    switch (classType) {
        case 2: //shooter
            classname = "Shooter";
            clcode = 2;
            max_hp = 30;
            hp = max_hp;
            max_mp = 50;
            mp = max_mp;
            str = 9;
            dur = 9;
            force = 13;
            resist = 12;
            speed = 7;
            lvlUp_xp = 850 + range(gen);
            base_xp = lvlUp_xp;
            codes.push_back(4);
            codes.push_back(7);
            construct_learnset(all, codes, lv);
            break;
        case 3: //speed
            classname = "Speedster";
            clcode = 3;
            max_hp = 40;
            hp = max_hp;
            max_mp = 40;
            mp = max_mp;
            str = 8;
            dur = 8;
            force = 8;
            resist = 8;
            speed = 18;
            lvlUp_xp = 500 + range(gen);
            base_xp = lvlUp_xp;
            codes.push_back(5);
            codes.push_back(8);
            construct_learnset(all, codes, lv);
            break;
        default: //brawler
            classname = "Brawler";
            clcode = 1;
            max_hp = 50;
            hp = max_hp;
            max_mp = 30;
            mp = max_mp;
            str = 13;
            dur = 12;
            force = 8;
            resist = 9;
            speed = 8;
            base_xp = 700 + range(gen);
            lvlUp_xp = base_xp;
            codes.push_back(3);
            codes.push_back(6);
            construct_learnset(all, codes, lv);
            break;
    }
    skillList.push_back(all[clcode-1]);
    
}

void Player::level_up() {
    uniform_int_distribution<int> bigPBoost(10,20);
    uniform_int_distribution<int> smlPBoost(5, 10);
    uniform_int_distribution<int> bigBoost(2,4);
    uniform_int_distribution<int> midBoost(1,3);
    uniform_int_distribution<int> smlBoost(0,2);
    if (xp >= lvlUp_xp) {
        while (xp >= lvlUp_xp) {
            cout << "Level up! " << name << " is now level " << to_string(++level) << "!" << endl;
            if (level < MAX_LEVEL)
                lvlUp_xp = level * level * base_xp;
            else
                lvlUp_xp = (numeric_limits<long>::max)();
        }
        //check learnset
        if (learnset.size() > 0 && level >= learnset.front().second) {
            add_skill(learnset.front().first);
            cout << name << " has learned " << learnset.front().first.name << 
                    " ( " << learnset.front().first.word << " )!" << endl;
            learnset.erase(learnset.begin());
        }
        //increase all stats randomly
        switch (clcode) {
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
            default: //brawler
                max_hp += bigPBoost(gen);
                max_mp += smlPBoost(gen);
                str += bigBoost(gen);
                dur += bigBoost(gen);
                force += smlBoost(gen);
                resist += midBoost(gen);
                speed += smlBoost(gen);
              break;  
        }
        
        hp = max_hp;
        mp = max_mp;
    }
}

bool Player::do_equip(noun n) {
    if (equipped[n.where].code == -1) {
        equipped[n.where] = n;
        str += n.strUp;
        dur += n.durUp;
        force += n.forUp;
        resist += n.resUp;
        speed += n.spdUp;
        cout << "You have equipped the " << n.word << "." << endl;
        return true;
    }
    else{
        cout << "You already have something equipped there." << endl;
        return false;
    }
}

noun Player::do_unequip(noun n) {
    noun returnval = equipped[n.where];
    equipped[n.where].code = -1;
    equipped[n.where].word = "";
    str -= n.strUp;
    dur -= n.durUp;
    force -= n.forUp;
    resist -= n.resUp;
    speed -= n.spdUp;
    return returnval;
    
}

bool Player::is_equipped_with(noun n) {
    for (int i = 0; i < equipped.size(); i++)
    {
        if (equipped[i].code == n.code) {
            return true;
        }
    }
    return false;
}

void Player::rest() {
    cout << name << " rests to recover." << endl;
    heal(max_hp/4, max_mp/4);
}

void Player::view_stats() {
    cout << name << "'s stats:\n-----------------" << endl;
    cout << "Class:\t" << classname << endl;
    if (gender == 0) {
        cout << "Gender:\tMale";
    }
    else {
        cout << "Gender:\tFemale";
    }
    cout << "\n----------------" << endl;
    cout << "Level " << to_string(level) << endl;
    cout << "EXP:\t" << to_string(xp) << endl;
    cout << "To next level:\t" << to_string(lvlUp_xp);
    cout << "\n----------------" << endl;
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
        cout << parts[i] << "\t" << equipped[i].word << endl;
    }
}

void Player::victory(long exp) {
    xp += exp;
    level_up();
}

void Player::construct_learnset(vector<Skill> &all, 
        vector<int> skillCodes, vector<int> lvls) {
    for (int i = 0; i < skillCodes.size(); i++) {
        learnset.push_back(pair<Skill, int>(all[skillCodes[i]], lvls[i]));
    }
}