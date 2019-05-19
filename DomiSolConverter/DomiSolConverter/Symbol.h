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
	Symbol(int lineNum, int barNum, int orderNum);
	int getLineNum();
	int getBarNum();
	int getOrderNum();
};

class Note : public Symbol {
private:
	int flag; // ��������
	bool isEmptyHead; // �Ӹ��� ��ĥ ���� (2����ǥ�� �������)
	bool dot; // ��
	bool isWholeNote; // ����ǥ ���� üũ
	char scale; // �������ļֶ�õ�
	int octave; // ������
	//hash_map<string, bool> nonNotes; // ��ǥ�� ���� ����ǥ��
	string result;	// ��ǥ ���� �Ǵ� ��� ����
public:
	Note(int lineNum, int barNum, int orderNum): Symbol(lineNum, barNum, orderNum) {}
	int getFlag();
	bool getIsEmptyHead();
	bool getDot();
	bool getIsWholeNote();
	char getScale();
	int getOctave();
	//hash_map<string, bool> getNonNotes();
	string getResult;
	void setFlag_Head(int flag, bool isEmptyHead);
	void setDot(bool dot);
	void setIsWholeNote(bool isWholeNote);
	void setScale_Octave(char scale, int octave);
	//void setNonNotes(hash_map<string, bool> nonNotes);
	void setResult(string result);
};

class NonNote : public Symbol {
private:
	string nonNoteType;
public:
	NonNote(int lineNum, int barNum, int orderNum, string nonNoteType) : Symbol(lineNum, barNum, orderNum) { this->nonNoteType = nonNoteType; }
	string getNonNoteType();
	void setNonNoteType();
	
};