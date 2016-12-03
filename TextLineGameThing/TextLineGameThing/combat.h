/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   combat.h
 * Author: Corey Allen
 *
 * Created on September 11, 2016, 12:14 AM
 */
#include <string>
#include <vector>
#include <random>
using namespace std;
#ifndef COMBAT_H
#define COMBAT_H
class Player;
class Enemy;

enum en_COMBAT_VERBS { ATTACK, USE_C, FLEE, REST_C, STATS_C, INVENTORY_C, 
    HELP_C, COMBAT_VERBS_MAX };
enum en_TURN_ACTION_CODE { NOT_DONE, USED, FLEE_ATTEMPT };
const int COMBAT_VERBS = COMBAT_VERBS_MAX;

word combat_verbs[COMBAT_VERBS_MAX];
struct next_act_mod {
    int s = 0;
    int d = 0;
    int f = 0;
    int r = 0;
    int sd = 0;
};
static next_act_mod next;
void set_combat_verbs()
{
    combat_verbs[ATTACK].code = ATTACK;
    combat_verbs[ATTACK].text = "ATTACK";
    combat_verbs[USE_C].code = USE_C;
    combat_verbs[USE_C].text = "USE";
    combat_verbs[FLEE].code = FLEE;
    combat_verbs[FLEE].text = "FLEE";
    combat_verbs[REST_C].code = REST_C;
    combat_verbs[REST_C].text = "REST";
    combat_verbs[STATS_C].code = STATS_C;
    combat_verbs[STATS_C].text = "STATS";
    combat_verbs[INVENTORY_C].code = INVENTORY_C;
    combat_verbs[INVENTORY_C].text = "INVENTORY";
    combat_verbs[HELP_C].code = HELP_C;
    combat_verbs[HELP_C].text = "HELP";
    
}
void display_help_C() {
    ifstream helpFile;
    helpFile.open("helpfile.txt");
    string line;
    while (!helpFile.eof()) {
        getline(helpFile, line);
        cout << line << "\n";
    }
    helpFile.close();
    cout << "---------------------------" << endl;
    
}
void do_use_combat(vector< pair<noun, int> > &inv, int &match, Player &p){
        if (inv[match].first.can_use) {
            textWrap("You use one " + inv[match].first.word + ".");
            textWrap(inv[match].first.use_sentence);
            p.heal(inv[match].first.hpUp, inv[match].first.mpUp);
            inv[match].second -= 1;
            if (inv[match].second <= 0) {
                inv.erase(inv.begin()+match);
            }
        }
    
}
void c_print_inventory(vector< pair<noun, int> > &inv) {
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
int parse_combat(vector<string> &w, vector<Enemy> &es, Player &p, 
        vector< pair<noun, int> > &inv)
{
    int VERB_MATCH = -1;
    int TARGET_MATCH = -1;
    int SKILL_MATCH = -1;
    for (int i = 0; i < COMBAT_VERBS; i++) {
        if (w[0] == combat_verbs[i].text)
        {
            VERB_MATCH = i;
            break;
        }
    }
    
    if (w.size() >= 2) {
        for (int i = 0; i < es.size(); i++) {
            if (es[i].word == w[1]) {
                TARGET_MATCH = i;
                break;
            }
        }
        for (int i = 0; i < inv.size(); i++) {
            if (TARGET_MATCH == -1 && inv[i].first.word == w[1]) {
                TARGET_MATCH = i;
                break;
            }
        }
        for (int i = 0; i < p.skillList.size(); i++)
        {
            if (SKILL_MATCH == -1 && p.skillList[i].word == w[1]) {
                SKILL_MATCH = i;
                break;
            }
        }
    }
    
    if (w.size() == 3) {
        for (int i = 0; i < es.size(); i++) {
            if (TARGET_MATCH == -1 && es[i].word == w[2]) {
                TARGET_MATCH = i;
                break;
            }
        }
        for (int i = 0; i < p.skillList.size(); i++)
        {
            if (SKILL_MATCH == -1 && p.skillList[i].word == w[2]) {
                SKILL_MATCH = i;
                break;
            }
        }
    }
    
    switch (VERB_MATCH){
        case ATTACK:
            if (es.size() == 1 || TARGET_MATCH == -1) {
                p.attackTarget(es[0]);
            }
            else {
                p.attackTarget(es[TARGET_MATCH]);
            }
            return USED;
        case USE_C:
            if (SKILL_MATCH == -1)
                do_use_combat(inv, TARGET_MATCH, p);
            else {
                if (p.skillList[SKILL_MATCH].attackType == 2)  {

                    return NOT_DONE;
                }
                if (es.size() == 1 || TARGET_MATCH == -1) {
                    if (p.mp >= p.skillList[SKILL_MATCH].mp_cost &&
                        p.hp >= p.skillList[SKILL_MATCH].hp_cost) {
                        p.attackTarget(es[0], p.skillList[SKILL_MATCH]);
                        
                    }
                    else
                    {
                        cout << "Insufficient MP for that skill!" << endl;
                        return NOT_DONE;
                    }
                
                }
                else {
                    if (p.mp >= p.skillList[SKILL_MATCH].mp_cost) {
                        p.attackTarget(es[TARGET_MATCH], p.skillList[SKILL_MATCH]);
                    }
                    else
                    {
                        cout << "Insufficient MP for that skill!" << endl;
                        return NOT_DONE;
                    }
                }
            }
            return USED;
        case FLEE:
            cout << "You attempt to escape..." << endl;
            return FLEE_ATTEMPT;
        case REST_C:
            p.rest();
            return USED;
        case STATS_C:
            p.view_stats();
            for (int i = 0; i < es.size(); i++)
                es[i].view_stats();
            return NOT_DONE;
        case INVENTORY_C:
            c_print_inventory(inv);
            return NOT_DONE;
        case HELP_C:
            display_help_C();
            return NOT_DONE;
        default:
            cout << "Not valid combat command." << endl;
            return false;
    }
}



#endif /* COMBAT_H */

