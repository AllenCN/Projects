/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   npc.h
 * Author: Corey Allen
 *
 * Created on September 11, 2016, 12:51 AM
 */
#include <string>
using namespace std;
#ifndef NPC_H
#define NPC_H
enum en_NPCS { BOSS, SHOPKEEPER, TRAINER, NPCS_MAX };
const int NPCS = NPCS_MAX;
struct npc {
     string name;
     string word;
     string desc = "";
     string greeting = "";
     string on_engage = "";
     string shop_init = "";
     string shop_continue = "";
     string shop_end = "";
     int code;
     int location;
     bool is_shopkeeper = false;
     vector< pair<noun, int> > items;
     bool can_engage = false;
     int enemyID = NONE;
 };

 npc npcs[NPCS];
 
 void set_npcs() {
     
     npcs[BOSS].name = "Boss Mad";
     npcs[BOSS].word = "BOSS";
     npcs[BOSS].desc = "The main villain of this game. He's as imposing as you'd expect.";
     npcs[BOSS].greeting = "A newcomer, eh? If you're here to beat me, you'd better turn away now, runt!";
     npcs[BOSS].on_engage = "So you think you can beat me? Prepare to have that thought ruthlessly beaten out of you!";
     npcs[BOSS].code = BOSS;
     npcs[BOSS].location = SQUARE;
     npcs[BOSS].can_engage = true;
     //npcs[BOSS].enemyID = BOSS_MAD;
     
     npcs[SHOPKEEPER].name = "Shopkeeper Ray";
     npcs[SHOPKEEPER].word = "SHOPKEEPER";
     npcs[SHOPKEEPER].desc = "A man whose prospects have been in the shitter for a while. He'll help you out... for a price!";
     npcs[SHOPKEEPER].greeting = "You want somethin'?";
     npcs[SHOPKEEPER].code = SHOPKEEPER;
     npcs[SHOPKEEPER].location = SHOP;
     npcs[SHOPKEEPER].is_shopkeeper = true;
     npcs[SHOPKEEPER].shop_init = "Welcome! I'll sell you whatever you need to take down that bastard!";
     npcs[SHOPKEEPER].shop_continue = "Anything else?";
     npcs[SHOPKEEPER].shop_end = "Thank you, and give Boss Mad hell from me!";
     npcs[SHOPKEEPER].on_engage = "I don't fight, you dick!";
     npcs[SHOPKEEPER].items.push_back(pair<noun, int>(nouns[SENZU], 30));
     npcs[SHOPKEEPER].items.push_back(pair<noun, int>(nouns[OINTMENT], 10));
     npcs[SHOPKEEPER].items.push_back(pair<noun, int>(nouns[POTION], 10));
     
     npcs[TRAINER].name = "Trainer Dave";
     npcs[TRAINER].word = "TRAINER";
     npcs[TRAINER].desc = "An old sage who can make you stronger if you're willing.";
     npcs[TRAINER].greeting = "Do you feel you're not strong enough to take on Boss Mad? Train with me.";
     npcs[TRAINER].code = TRAINER;
     npcs[TRAINER].location = DOJO;
     npcs[TRAINER].can_engage = true;
     npcs[TRAINER].on_engage = "You wish to train with me? Very well, but I warn you, I am pretty sink-or-swim...";
     //npcs[TRAINER].enemyID = TRAINER_DAVE;
 }   
 
#endif /* NPC_H */

