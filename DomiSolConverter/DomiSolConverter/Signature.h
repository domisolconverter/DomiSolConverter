#pragma once
#include "pch.h"
#include <iostream>
#include <vector>
#include "Symbol.h"

using namespace std;

class Signature {
private:
	bool isMajor;
	string type;
	string sign;

	vector<string> scale;
	vector<NonNote> element;

	string calculateSign();
	void calculateScale();

public:
	int sign_start;
	int sign_end;

	void clearElement();

	vector<string> getScale();
	string getType();
	void putElement(NonNote nonNote);
	bool isSignatureEnd(NonNote nonNote);
	string getCode();
	string getSign();

	Signature() { this->isMajor = true; };
};