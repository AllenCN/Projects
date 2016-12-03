/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Actor.h
 * Author: Corey Allen
 *
 * Created on September 11, 2016, 12:51 AM
 */

#include <string>
#include <vector>
#include <random>
#include <iostream>
#include "nouns.h"
#include "skill.h"
#include <Windows.h>
using namespace std;

#ifndef ACTOR_H
#define ACTOR_H
enum en_ATK_TYPE {PHYSICAL, ENERGY, OTHER};

class Actor {
public:
    
    string word;
    string name;
    
    int gender; //0 - male, 1 - female, 2 - neither
    int hp;
    int max_hp;
    int mp;
    int max_mp;
    int str;
    int dur;
    int force;
    int resist;
    int speed;
    
    vector<Skill> skillList;
    vector<noun> equipped;
    
    void attackTarget(Actor &target);
    void attackTarget(Actor &target, Skill skill);
    void take_damage(int dmg);
    void heal(int hprecov, int mprecov = 0);
    void add_skill(Skill &s);
    void use_buff(Skill &s);
    int has_skill(const string &w);
    bool is_dead();
    void view_stats();
    void print_skills();
    default_random_engine gen;
    
    
};

#endif /* ACTOR_H */

