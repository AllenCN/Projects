/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   skill.h
 * Author: Corey Allen
 *
 * Created on September 12, 2016, 3:16 PM
 */
#include <string>
#include <vector>
using namespace std;

#ifndef SKILL_H
#define SKILL_H


typedef struct Skill {
    string name;
    string word;
    int attackType;
    
    int mp_cost;
    int hp_cost = 0;
    
    int str_mod = 0;
    int dur_mod = 0;
    int force_mod = 0;
    int resist_mod = 0;
    int speed_mod = 0;
    
    double base_damage;
} Skill;

//From stackoverflow.



#endif /* SKILL_H */

