#pragma once
#include "NPC.h"
#include "Word.h"
#include <fstream>
#include <iostream>
class ShopNPC :
	public NPC
{
public:
	ShopNPC();
	ShopNPC(vector<string> data);
	virtual ~ShopNPC();

	void do_shop(vector<pair<string,int>> &inv, int goldIndex = 0);

	string shop_continue;
	string shop_end;
	vector<pair<string, int>> menu;

private:
	vector<string> shop_tokens(string line, char sep = ',');
	int is_sold(string itemName);
	int in_inventory(string itemName, vector<pair<string, int>> &inv);
	void print_inventory_shop(vector<pair<string, int>> &inv);
	void print_menu();
	void print_help(const string &helpFile = "helpfile.txt");
	void textWrapper(string str, size_t width = 80);
};

