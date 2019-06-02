#pragma once
#include "pch.h"
#include <iostream>
//#include <hash_map>
#include <algorithm>
using namespace std;

class Symbol {
private:
	int lineNum; // �ٹ�ȣ (���° ��������)
	int barNum; // �����ȣ (�ش� ���� ���° ��������)
	int orderNum; // ������ȣ (���� �ȿ����� ����)

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
	int flag; // ��������
	bool isEmptyHead; // �Ӹ��� ��ĥ ���� (2����ǥ�� �������)
	bool dot; // ��
	bool isWholeNote; // ����ǥ ���� üũ
	bool isHeadUp; // ��ǥ�Ӹ� ���Ʒ�
	char scale; // �������ļֶ�õ�
	int octave; // ������
	//hash_map<string, bool> nonNotes; // ��ǥ�� ���� ����ǥ��
	string result;	// ��ǥ ���� �Ǵ� ��� ����

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