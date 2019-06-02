#pragma once
#include "pch.h"
#include <iostream>
#include <algorithm>
using namespace std;

class Symbol {
private:
	string type;
	int lineNum; // 줄번호 (몇번째 오선인지)
	int barNum; // 마디번호 (해당 줄의 몇번째 마디인지)
	int orderNum; // 순서번호 (마디 안에서의 순서)

public:
	Symbol(string type) { this->type = type; };
	void setLineNum(int lineNum);
	void setbarNum(int barNum);
	void setOrderNum(int orderNum);
	string getType();

	int getLineNum();
	int getBarNum();

	int x;
	int y;
};

class Note : public Symbol {
private:
	int flag; // 꼬리개수
	bool isEmptyHead; // 머리에 색칠 유무 (2분음표는 비어있음)
	bool dot; // 점
	bool isWholeNote; // 온음표 따로 체크
	bool isHeadUp; // 음표머리 위아래
	char scale; // 도레미파솔라시도
	int octave; // 음역대
	string result;	// 음표 종류 판단 결과 저장

public:
	Note() : Symbol("note") { this->isWholeNote = false; };
	int getFlag();
	bool getIsEmptyHead();
	bool getDot();
	bool getIsWholeNote();
	bool getIsHeadUp();
	char getScale();
	int getOctave();
	string getResult;
	void setFlag_Head(int flag, bool isEmptyHead);
	void setDot(bool dot);
	void setIsWholeNote(bool isWholeNote);
	void setisHeadUp(bool isHeadUp);
	void setScale_Octave(char scale, int octave);
	void setResult(string result);
	string transposeNote();
};

class NonNote : public Symbol {
private:
	string nonNoteType;

public:
	NonNote(string nonNoteType) : Symbol("nonNote") { this->nonNoteType = nonNoteType; }
	string getNonNoteType();
	void setNonNoteType();
};