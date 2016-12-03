#pragma once
#include "Word.h"
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#ifdef _DEBUG
#define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define new DEBUG_NEW
#endif
#include <assert.h>
class Noun :
	public Word
{
public:
	Noun();
	Noun(vector<string> line1, vector<string> data);
	virtual ~Noun();

	int type;

	string shortdesc;
	string longdesc;
	
};

