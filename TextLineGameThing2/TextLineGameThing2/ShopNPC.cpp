#include "ShopNPC.h"



ShopNPC::ShopNPC()
{
}

ShopNPC::ShopNPC(vector<string> data) {
	assert(data.size() >= 10);
	type = stoi(data[0]);
	trigger = data[1];
	name = data[2];
	bio = data[3];
	greeting = data[4];
	on_engage = data[5];
	shop_init = data[6];
	shop_continue = data[7];
	shop_end = data[8];

	for (unsigned int i = 9; i < data.size(); i++) {
		vector<string> tmp = shop_tokens(data[i]);
		menu.push_back(make_pair(tmp[0], stoi(tmp[1])));
	}

}

ShopNPC::~ShopNPC()
{
}

void ShopNPC::do_shop(vector<pair<string, int>> &inv, int goldIndex) {
	string command;
	vector<string> words;
	bool firstTime = true;
	print_menu();
	while (true) {
		command.clear();
		words.clear();
		if (!firstTime)
		{
			textWrapper(name + ":\"" + shop_continue + "\"");
		}
		cout << ">";
		getline(cin, command);
		transform(command.begin(), command.end(), command.begin(), (int(*)(int))toupper);
		words = shop_tokens(command, ' ');
		if (words.size() > 2) {
			cout << "Use one or two words only." << endl;
			continue;
		}
		if (words.empty()) {
			continue;
		}
		if (words.size() == 1) {
			if (words[0] == "MENU") {
				print_menu();
				continue;
			}
			if (words[0] == "INVENTORY" || words[0] == "BAG") {
				print_inventory_shop(inv);
				continue;
			}
			if (words[0] == "EXIT" || words[0] == "QUIT" || words[0] == "DONE") {
				textWrapper(name + ": \"" + shop_end + "\"");
				break;
			}
			if (words[0] == "HELP") {
				print_help();
				continue;
			}
			cout << "Invalid command entered." << endl;
			continue;
		}
		if (words.size() == 2) {
			if (words[0] == "BUY" || words[0] == "PURCHASE") {
				if (is_sold(words[1]) != -1) {
					inv[goldIndex].second -= menu[is_sold(words[1])].second;
					int check = in_inventory(words[1], inv);
					if (check != -1)
					{
						inv[check].second += 1;
					}
					else {
						inv.push_back(make_pair(words[1], 1));
					}
					cout << "You buy one " << words[1] << "." << endl;
				}
				else {
					cout << "That is not sold here." << endl;
				}
				firstTime = false;
				continue;
			}
			if (words[0] == "SELL") {
				if (in_inventory(words[1], inv) != -1 && is_sold(words[1]) != -1) {
					inv[goldIndex].second += menu[is_sold(words[1])].second / 2;
					int index = in_inventory(words[1], inv);
					inv[index].second -= 1;
					if (inv[index].second == 0)
						inv.erase(inv.begin() + index);

					cout << "You sell the " << words[1] << "." << endl;
				}
				else {
					cout << "You can't sell that here." << endl;
				}
				
				firstTime = false;
				continue;
			}
			cout << "Invalid command entered." << endl;
			continue;
		}
	}
}

vector<string> ShopNPC::shop_tokens(string line, char sep) {
	std::vector<std::string> tokens;
	std::size_t start = 0, end = 0;
	while ((end = line.find(sep, start)) != std::string::npos) {
		if (end != start) {
			tokens.push_back(line.substr(start, end - start));
		}
		start = end + 1;
	}
	if (end != start) {
		tokens.push_back(line.substr(start));
	}
	return tokens;
}

void ShopNPC::print_help(const string &helpFile) {
	ifstream help;
	help.open(helpFile);
	if (help.is_open()) {
		string line;
		while (help.good()) {
			line.clear();
			getline(help, line);
			cout << line << endl;
		}
		help.close();
		cout << endl;
	}
	else {
		cout << "help file not found" << endl;
	}
}

int ShopNPC::is_sold(string itemName) {
	for (unsigned int i = 0; i < menu.size(); i++) {
		if (menu[i].first == itemName)
			return i;
	}
	return -1;
}

int ShopNPC::in_inventory(string itemName, vector<pair<string, int>> &inv) {
	for (unsigned int i = 0; i < inv.size(); i++) {
		if (inv[i].first == itemName)
			return i;
	}
	return -1;
}

void ShopNPC::print_inventory_shop(vector<pair<string, int>> &inv) {
	cout << "ITEM \t QUANTITY" << endl;
	cout << "-------------------" << endl;
	cout << inv[0].first << " \t " << inv[0].second << endl;
	for (unsigned int i = 1; i < inv.size(); i++) {
		if (inv[i].second > 1)
			cout << inv[i].first << "\t" << inv[i].second << endl;
		else
			cout << inv[i].first << endl;
	}
	cout << "-------------------" << endl;
}

void ShopNPC::print_menu() {
	cout << name << "'s shop Menu" << endl;
	cout << "ITEM \t COST" << endl;
	cout << "-------------------" << endl;
	for (unsigned int i = 0; i < menu.size(); i++) {
		cout << menu[i].first << "\t" << menu[i].second << endl;

	}
	cout << "-------------------" << endl;
}

void ShopNPC::textWrapper(string str, size_t width) {
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