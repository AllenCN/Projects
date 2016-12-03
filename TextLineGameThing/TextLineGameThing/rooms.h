/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   rooms.h
 * Author: Corey Allen
 *
 * Created on September 11, 2016, 12:00 AM
 */
#include <string>
#ifndef ROOMS_H
#define ROOMS_H
using namespace std;

enum en_ROOMS { SQUARE, PARK, STATUE, SHOP, DOJO, TOWER, LAB, ROOMS_MAX };

struct room {
    string desc;
    string longdesc = "";
    int exits_to_room[DIRS];
    double encRate = 0.0;
    vector<int> enemycodes;
};
const int ROOMS = ROOMS_MAX;
room rooms[ROOMS_MAX];

void set_rooms() {
    // enum en_ROOMS { SQUARE, PARK, STATUE, SHOP, DOJO, TOWER, LAB };
    rooms[SQUARE].desc = "Town Square";
    rooms[SQUARE].longdesc = "The center of the town, where Boss Mad awaits all challengers. Can you possibly defeat him!?";
    rooms[SQUARE].exits_to_room[NORTH] = DOJO;
    rooms[SQUARE].exits_to_room[EAST] = SHOP;
    rooms[SQUARE].exits_to_room[SOUTH] = PARK;
    rooms[SQUARE].exits_to_room[WEST] = TOWER;
    
    rooms[PARK].desc = "Town Park";
    rooms[PARK].longdesc = "The south park, where a statue waits to the east.";
    rooms[PARK].exits_to_room[NORTH] = SQUARE;
    rooms[PARK].exits_to_room[EAST] = NONE;
    rooms[PARK].exits_to_room[SOUTH] = NONE;
    rooms[PARK].exits_to_room[WEST] = NONE;
    
    rooms[STATUE].desc = "Statue Area";
    rooms[STATUE].longdesc = "A statue of Boss Mad. At the base, someone has spraypainted a picture of dung...";
    rooms[STATUE].encRate = 0.15;
    rooms[STATUE].enemycodes.push_back(THUG);
    rooms[STATUE].exits_to_room[NORTH] = NONE;
    rooms[STATUE].exits_to_room[EAST] = NONE;
    rooms[STATUE].exits_to_room[SOUTH] = NONE;
    rooms[STATUE].exits_to_room[WEST] = NONE;
    
    rooms[SHOP].desc = "Local Shop";
    rooms[SHOP].longdesc = "A shop, headed by someone who wants to see Boss Mad taken down. But don't expect his services to come cheap!";
    rooms[SHOP].exits_to_room[NORTH] = NONE;
    rooms[SHOP].exits_to_room[EAST] = NONE;
    rooms[SHOP].exits_to_room[SOUTH] = NONE;
    rooms[SHOP].exits_to_room[WEST] = SQUARE;
    
    rooms[DOJO].desc = "North Dojo";
    rooms[DOJO].longdesc = "A dojo run by someone who wants to see Boss Mad defeated ASAP. Talk to him to learn skills.";
    rooms[DOJO].exits_to_room[NORTH] = NONE;
    rooms[DOJO].exits_to_room[EAST] = NONE;
    rooms[DOJO].exits_to_room[SOUTH] = SQUARE;
    rooms[DOJO].exits_to_room[WEST] = NONE;
    
    rooms[TOWER].desc = "West Tower";
    rooms[TOWER].longdesc = "A tower that was subject to a horrific science accident not long ago. Since then, it's a hangout for the monsters and mutants, as well as the local thugs. Careful.";
    rooms[TOWER].encRate = 0.3;
    rooms[TOWER].enemycodes.push_back(THUG);
    rooms[TOWER].enemycodes.push_back(RAT);
    rooms[TOWER].exits_to_room[NORTH] = LAB;
    rooms[TOWER].exits_to_room[EAST] = SQUARE;
    rooms[TOWER].exits_to_room[SOUTH] = NONE;
    rooms[TOWER].exits_to_room[WEST] = NONE;
    
    rooms[LAB].desc = "Abandoned Lab";
    rooms[LAB].longdesc = "A lab that was abandoned a long time ago. Powerful monsters lurk nearby. Watch out.";
    rooms[LAB].encRate = 0.45;
    rooms[LAB].enemycodes.push_back(BEAST);
    rooms[LAB].enemycodes.push_back(RAT);
    rooms[LAB].exits_to_room[NORTH] = NONE;
    rooms[LAB].exits_to_room[EAST] = NONE;
    rooms[LAB].exits_to_room[SOUTH] = TOWER;
    rooms[LAB].exits_to_room[WEST] = NONE;
}

#endif /* ROOMS_H */

