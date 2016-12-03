/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "Actor.h"

bool Actor::is_dead() {
    return hp <= 0;
}

void Actor::attackTarget(Actor& target) {
    uniform_real_distribution<double> dmg(0.8,1);
    cout << name << " attacks " << target.name << "!" << endl;
    double proportion = (double)speed/target.speed;
    
    bernoulli_distribution atkSuc(max(0.5, min(1.0, proportion)));
    if (atkSuc(gen)) {
        int damage = (str * str)/(target.dur) * dmg(gen);
        cout << target.name << " took " << damage << " damage!" << endl;
        target.take_damage(damage);
    }
    else {
        cout << name << " missed!" << endl;
    }
}

void Actor::attackTarget(Actor& target, Skill skill) {
    uniform_real_distribution<double> dmg(0.8,1);
    cout << name << " attacks " << target.name << " with " << skill.name << "!" << endl;
    mp -= skill.mp_cost;
    hp -= skill.hp_cost;
    int normStr = str;
    int normDur = dur;
    int normFor = force;
    int normRes = resist;
    int normSpd = speed;
    str += skill.str_mod;
    dur += skill.dur_mod;
    force += skill.force_mod;
    resist += skill.resist_mod;
    speed += skill.speed_mod;
    double proportion = (double)speed/target.speed;
    bernoulli_distribution atkSuc(max(0.5, min(1.0, proportion)));
    if (atkSuc(gen)) {
        if (skill.attackType == 0) //physical 
        {
            int damage = (str * str * skill.base_damage)/(target.dur) * dmg(gen);
            cout << target.name << " took " << damage << " damage!" << endl;
            target.take_damage(damage);
        }
        else {
            int damage = (force * force * skill.base_damage)/(target.resist) * dmg(gen);
            cout << target.name << " took " << damage << " damage!" << endl;
            target.take_damage(damage);
        }
    }
    else {
        cout << name << " missed!" << endl;
    }
    str = normStr;
    dur = normDur;
    force = normFor;
    resist = normRes;
    speed = normSpd;
}

void Actor::take_damage(int dmg){
    if (hp - dmg < 0)
        hp = 0;
    else
        hp -= dmg;
}

void Actor::add_skill(Skill &s) {
    //cout << name << " has learned " << s.name << " ( " << s.word << ") !" << endl;
    skillList.push_back(s);
}

void Actor::heal(int hprecov, int mprecov){
    if (hp + hprecov > max_hp)
        hp = max_hp;
    else
        hp += hprecov;
    
    if (mprecov > 0) {
        if (mp + mprecov > max_mp)
            mp = max_mp;
        else
            mp += mprecov;
    }
}

int Actor::has_skill(const string &w) {
    for (int i = 0; i < skillList.size(); i++) {
        if (skillList[i].word == w)
        {
            return i;
        }
    }
    return -1;
}

void Actor::view_stats() {

    cout << name << "'s stats:\n-----------------" << endl;
    if (gender == 0) {
        cout << "Gender:\tMale";
    }
    else {
        cout << "Gender:\tFemale";
    }
    cout << "\n----------------" << endl;
    cout << "HP:\t" << to_string(hp) << " / " << to_string(max_hp) << endl;
    cout << "MP:\t" << to_string(mp) << " / " << to_string(max_mp) << endl;
    cout << "STR:\t" << to_string(str) << "\tDUR:\t" << to_string(dur) << endl;
    cout << "FOR:\t" << to_string(force) << "\tRESIST:\t" << to_string(resist) << endl;
    cout << "SPD:\t" << to_string(speed) << endl;

}

void Actor::print_skills() {
    cout << name << "'s skills:\n----------------" << endl;
    for (int i = 0; i < skillList.size(); i++) {
        cout << skillList[i].name << " ( " << skillList[i].word << " )\t" << 
                skillList[i].mp_cost << endl;
    }
}