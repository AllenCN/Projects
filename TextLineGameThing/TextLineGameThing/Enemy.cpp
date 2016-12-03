/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "Enemy.h"



using namespace std;

Enemy::Enemy() {
    name = "Default";
    word = "DEFAULT";
    gender = 0;
    max_hp = 0;
    hp = max_hp;
    max_mp = 0;
    mp = max_mp;
    str = 0;
    dur = 0;
    force = 0;
    resist = 0;
    speed = 0;
    xpyield = 0;
    goldyield = 0;
}

Enemy::Enemy(const string &nm, const string &w, int health, int mana, int strength,
          int durability, int f, int res, int spd, int xreward, int greward) {
    bernoulli_distribution gendDis(0.5);
    name = nm;
    word = w;
    if (gendDis(gen))
        gender = 1;
    else
        gender = 0;
    max_hp = health;
    hp = max_hp;
    max_mp = mana;
    mp = max_mp;
    str = strength;
    dur = durability;
    force = f;
    resist = res;
    speed = spd;
    xpyield = xreward;
    goldyield = greward;
    
}

Enemy::Enemy(const string &nm, const string &w, int sex, int health, int mana, int strength,
          int durability, int f, int res, int spd, int xreward, int greward) {
    name = nm;
    word = w;
    gender = sex;
    max_hp = health;
    hp = max_hp;
    max_mp = mana;
    mp = max_mp;
    str = strength;
    dur = durability;
    force = f;
    resist = res;
    speed = spd;
    xpyield = xreward;
    goldyield = greward;
}

void Enemy::set_stats(const string &nm, const string &w, int sex, int health, int mana, int strength,
          int durability, int f, int res, int spd, int xreward, int greward) {
    seed_seq seed (nm.begin(), nm.end());
    gen.seed(seed);
    bernoulli_distribution gendDis(0.5);
    name = nm;
    word = w;
    max_hp = health;
    hp = max_hp;
    max_mp = mana;
    mp = max_mp;
    str = strength;
    dur = durability;
    force = f;
    resist = res;
    speed = spd;
    xpyield = xreward;
    goldyield = greward;
    if (sex == -1)
        gender = gendDis(gen);
    else
        gender = sex;
}