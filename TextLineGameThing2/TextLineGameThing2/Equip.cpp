#include "Equip.h"

Equip::Equip()
{
	code = "NOTHING";
}

Equip::Equip(vector<string> line, vector<string> data) {
	assert(data.size() >= 9);
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
	equipWhere = stoi(data[3]);
	strChange = stoi(data[4]);
	durChange = stoi(data[5]);
	forChange = stoi(data[6]);
	resChange = stoi(data[7]);
	spdChange = stoi(data[8]);

	if (data.size() > 9) {
		for (unsigned int i = 9; i < data.size(); i++) {
			addEffects.push_back(data[i]);
		}
	}
	
}

Equip::~Equip()
{
}
