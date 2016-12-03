#pragma once
#include "Noun.h"
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#ifdef _DEBUG
#define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define new DEBUG_NEW
#endif
class Equip : public Noun
{
public:
	Equip();
	Equip(vector<string> line1, vector<string> data);
	virtual ~Equip();
	enum equipLocs {HEAD, TORSO, ARMS, LEGS};
	int equipWhere;
	int strChange;
	int durChange;
	int forChange;
	int resChange;
	int spdChange;
	vector<string> addEffects;
};

