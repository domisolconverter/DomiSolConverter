#pragma once
#include <iostream>
#include <hash_map>
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
	hash_map<string, bool> nonNotes; // ��ǥ�� ���� ����ǥ��
	string result;	// ��ǥ ���� �Ǵ� ��� ����
public:
	Note(int flag, bool isEmptyHead, bool dot, bool isWholeNote, char scale, int octave, hash_map<string, bool> nonNotes);
	int getFlag();
	bool getIsEmptyHead();
	bool getDot();
	bool getIsWholeNote();
	char getScale();
	int getOctave();
	hash_map<string, bool> getNonNotes();
	void setResult();

};

class NonNote : public Symbol {
private:
	string nonNoteType;
public:
	NonNote(int lineNum, int barNum, int orderNum, string nonNoteType);
	string getNonNoteType();
	Symbol(lineNum, barNum, orderNum) {}
};

int main(void) {
	vector<Symbol> wholeSymbols;
	vector<Note> wholeNotes;
	vector<NonNote> wholeNonNotes;
	Note tmp;
	
	merge(wholeNotes.begin(), wholeNotes.end(), wholeNonNotes.begin(), wholeNonNotes.end(), back_inserter(wholeSymbols));

}