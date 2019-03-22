#pragma once
#include "pch.h"
#include "DomiSolConverter.h"

class DomisolConverter::Analysis {
private:
	vector<Point> staffXY;
	int staffHeight;
	int staffSpace;
	vector<string> note;
	vector<string> nonNote;
	vector<string> text;

	void extractFeature();
	void calculatePitch();
	void recognizeObject();
	void recognizeGeneralSymbol();
	void recognizeText();
	void recognizeNoteSymbol();

public:
	Analysis();
	vector<string> getNote();
	vector<string> getNonNote();
	vector<string> getText();
};