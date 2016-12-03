#include "Item.h"

Item::Item()
{
}

Item::Item(vector<string> line, vector<string> data) {
	assert(data.size() == 6 || data.size() == 7);

	for (unsigned int i = 0; i < line.size(); i++) {
		transform(line[i].begin(), line[i].end(), line[i].begin(), (int(*)(int))toupper);
	}
	//code should be very first in line
	code = line[0];
	line.erase(line.begin());
	//synonyms are the rest in line
	synonyms = line;

	type = stoi(data[0]);
	shortdesc = data[1];
	longdesc = data[2];
	hpUp = stoi(data[3]);
	mpUp = stoi(data[4]);
	use_sentence = data[5];

	if (data.size() == 7) {
		isKeyItem = (stoi(data[6]) != 0);
	}
	
}

Item::~Item()
{
}
