#pragma once
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#ifdef _DEBUG
#define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define new DEBUG_NEW
#endif
#include <algorithm>
#include <string>
#include <vector>

using namespace std;
class Word
{
public:
	Word();
	Word(const string &c);
	Word(const string &c, const vector<string> &syn);
	Word(vector<string> &list);
	virtual ~Word();


	Word operator=(const Word &other);
	bool operator==(const string &other);
	bool operator!=(const string &other);

	//Possibly, implement operator == for Word == Word and != Word

	string to_string();

	string code;
	vector<string> synonyms;
};

