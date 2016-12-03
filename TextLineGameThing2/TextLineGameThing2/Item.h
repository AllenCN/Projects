#pragma once
#include "Noun.h"

class Item : public Noun
{
public:
	Item();
	Item(vector<string> line1, vector<string> data);
	virtual ~Item();

	int hpUp;
	int mpUp;
	string use_sentence;
	bool isKeyItem = false;
	

};

