#pragma once
#include "pch.h"
#include <iostream>
//#include <hash_map>
#include <algorithm>
using namespace std;

class Symbol {
private:
	int lineNum; // 줄번호 (몇번째 오선인지)
	int barNum; // 마디번호 (해당 줄의 몇번째 마디인지)
	int orderNum; // 순서번호 (마디 안에서의 순서)

public:
	Symbol();
	void setLineNum(int lineNum);
	void setbarNum(int barNum);
	void setOrderNum(int orderNum);

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
	//hash_map<string, bool> nonNotes; // 음표에 붙은 비음표들
	string result;	// 음표 종류 판단 결과 저장

public:
	Note(): Symbol() {}
	int getFlag();
	bool getIsEmptyHead();
	bool getDot();
	bool getIsWholeNote();
	bool getIsHeadUp();
	char getScale();
	int getOctave();
	//hash_map<string, bool> getNonNotes();
	string getResult;
	void setFlag_Head(int flag, bool isEmptyHead);
	void setDot(bool dot);
	void setIsWholeNote(bool isWholeNote);
	void setisHeadUp(bool isHeadUp);
	void setScale_Octave(char scale, int octave);
	//void setNonNotes(hash_map<string, bool> nonNotes);
	void setResult(string result);
	int transposeNote();

};

class NonNote : public Symbol {
private:
	string nonNoteType;

public:
	NonNote(string nonNoteType) : Symbol() { this->nonNoteType = nonNoteType; }
	string getNonNoteType();
	void setNonNoteType();
};