/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   nouns.h
 * Author: Corey Allen
 *
 * Created on September 10, 2016, 11:16 PM
 */
#include <string>

using namespace std;

#ifndef NOUNS_H
#define NOUNS_H



enum en_NOUNS { SWORD, SHIELD, SENZU, OINTMENT, POTION, GOLD, ODD_STATUE, BOX, GATE, NOUNS_MAX };
typedef struct noun {
    string word;
    string desc;
    string longdesc = "";
    int code = -1;
    int location = -1; 
    bool can_carry = true;
    bool can_equip = false;
    int where = -1;
    int strUp = 0;
    int durUp = 0;
    int forUp = 0;
    int resUp = 0;
    int spdUp = 0;
    bool can_use = false;
    string use_sentence;
    int hpUp = 0;
    int mpUp = 0;
} noun;



#endif /* NOUNS_H */

