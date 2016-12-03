/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Enemy.h
 * Author: Corey Allen
 *
 * Created on September 11, 2016, 10:47 PM
 */
#include "Actor.h"

#ifndef ENEMY_H
#define ENEMY_H

//struct enemy_data;

class Enemy : public Actor {
public:
    int xpyield;
    int goldyield;
    
    string word;

    Enemy();
    Enemy(const string &nm, const string &w, int health, int mana, int strength,
          int durability, int f, int res, int spd, int xreward, int greward);
    Enemy(const string &nm, const string &w, int sex, int health, int mana, int strength,
          int durability, int f, int res, int spd, int xreward, int greward);

    //void set_stats(enemy_data dat);
    void set_stats(const string &nm, const string &w, int sex, int health, int mana, int strength,
          int durability, int f, int res, int spd, int xreward, int greward);
    
    
};

#endif /* ENEMY_H */

