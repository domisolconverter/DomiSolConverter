#pragma once
#include "pch.h"
#include <iostream>
#include <algorithm>
using namespace std;

class Symbol {
private:
	string type;
	int lineNum; // �ٹ�ȣ (���° ��������)
	int barNum; // �����ȣ (�ش� ���� ���° ��������)
	int orderNum; // ������ȣ (���� �ȿ����� ����)

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
	int flag; // ��������
	bool isEmptyHead; // �Ӹ��� ��ĥ ���� (2����ǥ�� �������)
	bool dot; // ��
	bool isWholeNote; // ����ǥ ���� üũ
	bool isHeadUp; // ��ǥ�Ӹ� ���Ʒ�
	char scale; // �������ļֶ�õ�
	int octave; // ������
	string result;	// ��ǥ ���� �Ǵ� ��� ����

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