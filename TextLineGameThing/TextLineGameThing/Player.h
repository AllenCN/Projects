/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Player.h
 * Author: Corey Allen
 *
 * Created on September 11, 2016, 10:50 PM
 */

#include "Actor.h"
#include <iostream>
#include <limits>

using namespace std;
#ifndef PLAYER_H
#define PLAYER_H

class Player : public Actor {
    
public:
    const int MAX_LEVEL = 10;
    
    string classname;    
    long xp;
    long lvlUp_xp;
    int base_xp;
    int level;
    vector< pair<Skill, int> > learnset;

    
    Player();

    void createClass(vector<Skill> &all);
    void construct_learnset(vector<Skill> &all, vector<int> skillCodes, vector<int> lvls);
    void level_up();
    bool do_equip(noun n);
    noun do_unequip(noun n);
    void rest();
    void view_stats();
    void victory(long exp);
    bool is_equipped_with(noun n);
    void print_equip();
    
    const string parts[4] = {"HEAD", "TORSO", "ARMS", "LEGS"};
    int clcode;
};

#endif /* PLAYER_H */

