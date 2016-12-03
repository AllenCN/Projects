/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   global.h
 * Author: Corey Allen
 *
 * Created on September 11, 2016, 12:58 AM
 */
#include <string>
#include <sstream>
#include <cstdlib>
#include <cstdio>
#include <cstring>

#include <iostream>
#include <string>
#include <vector>
#include <cctype>
#include <algorithm>
#include <fstream>
#include <random>
using namespace std;

#ifndef GLOBAL_H
#define GLOBAL_H
const string HELPFILE = "helpfile.txt";

void textWrap(string str, size_t width = 80) {
    string tmpWord;
    string tmpLine;
    int strLength = str.size();

    for (int i = 0; i < strLength; i++) {
        if (str[i] != ' ') {
            tmpWord += str[i];
        }
        else {
            if (tmpWord.size() + tmpLine.size() >= width) {
                cout << tmpLine;
                cout << endl;
                tmpLine.clear();
            }
            tmpLine += tmpWord + ' ';
            tmpWord.clear();
        }
    }
    cout << tmpLine << tmpWord << endl;
}

void display_help() {
    ifstream helpFile;
    helpFile.open(HELPFILE);
    string line;
    while (!helpFile.eof()) {
        getline(helpFile, line);
        cout << line << "\n";
    }
    helpFile.close();
    cout << "---------------------------" << endl;
    
}

std::vector<std::string> split(const std::string &text, char sep) {
    std::vector<std::string> tokens;
    std::size_t start = 0, end = 0;
    while ((end = text.find(sep, start)) != std::string::npos) {
        if (end != start) {
          tokens.push_back(text.substr(start, end - start));
        }
        start = end + 1;
    }
    if (end != start) {
       tokens.push_back(text.substr(start));
    }
    return tokens;
}
struct Skill;
std::vector<Skill> allSkills;


#endif /* GLOBAL_H */

