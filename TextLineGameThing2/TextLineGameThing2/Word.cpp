#include "Word.h"



Word::Word()
{
	code = "";
}

Word::Word(const string &c) {
	code = c;
	transform(code.begin(), code.end(), code.begin(),
		(int(*)(int))toupper);
}

Word::Word(const string &c, const vector<string> &syn) {
	code = c;
	synonyms = syn;
	transform(code.begin(), code.end(), code.begin(),
		(int(*)(int))toupper);
	for (unsigned int i = 0; i < synonyms.size(); i++) {
		transform(synonyms[i].begin(), synonyms[i].end(), synonyms[i].begin(), (int(*)(int))toupper);
	}
}

Word Word::operator=(const Word &other)
{
	Word w(other.code, other.synonyms);
	return w;
}

Word::Word(vector<string> &line) {
	for (unsigned int i = 0; i < line.size(); i++) {
		transform(line[i].begin(), line[i].end(), line[i].begin(), (int(*)(int))toupper);
	}
	//code should be very first in line
	code = line[0];
	line.erase(line.begin());
	//synonyms are the rest in line
	synonyms = line;
}

bool Word::operator==(const string &other) {
	if (code == other) {
		return true;
	}
	else {
		return find(synonyms.begin(), synonyms.end(), other) != synonyms.end();
	}
}

bool Word::operator!=(const string &other) {
	return !operator==(other);
}

string Word::to_string() {
	string synoString = "SYNONYMS: ";
	for (unsigned int i = 0; i < synonyms.size(); i++) {
		synoString += synonyms[i] + ", ";
	}
	return "CODE: " + code + "\n" + synoString;
}

Word::~Word()
{
}
