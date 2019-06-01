#include "pch.h"
#include "Symbol.h"

string Symbol::getType() {
	return this->type;
}

void Symbol::setbarNum(int barNum) {
	this->barNum = barNum;
}

void Symbol::setLineNum(int lineNum) {
	this->lineNum = lineNum;
}

void Symbol::setOrderNum(int orderNum) {
	this->orderNum = orderNum;
}

int Symbol::getFlag() {
	return 0;
}
bool Symbol::getIsEmptyHead() {
	return true;
}
bool Symbol::getDot() {
	return true;
}
bool Symbol::getIsWholeNote() {
	return false;
}
char Symbol::getScale() {
	return 'C';
}
int Symbol::getOctave() {
	return 1;
}
string Symbol::transposeNote() {
	return "2";
}

int Note::getFlag() {
	return this->flag;
}

bool Note::getIsEmptyHead() {
	return this->isEmptyHead;
}

bool Note::getDot() {
	return this->dot;
}

bool Note::getIsWholeNote() {
	return this->isWholeNote;
}

char Note::getScale() {
	return this->scale;
}

int Note::getOctave() {
	return this->octave;
}

void Note::setFlag_Head(int flag, bool isEmptyHead) {
	this->flag = flag;
	this->isEmptyHead = isEmptyHead;
}

void Note::setDot(bool dot) {
	this->dot = dot;
}

void Note::setIsWholeNote(bool isWholeNote) {
	this->isWholeNote = isWholeNote;
}

void Note::setScale_Octave(char scale, int octave) {
	this->scale = scale;
	this->octave = octave;
}


void Note::setResult(string result) {
	this->result = result;
}



string NonNote::getNonNoteType() {
	return this->nonNoteType;
}

void NonNote::setNonNoteType() {
	this->nonNoteType = nonNoteType;
}

string Note::transposeNote() {
	if (isWholeNote) {
		return string("4");
	}
	else if (flag == 0 && isEmptyHead) {
		return string("2");
	}

	else if (flag == 0 && !isEmptyHead) {
		return string("4");
	}
	else if (flag == 1 && !isEmptyHead) {
		return string("8");
	}
	else if (flag == 2 && !isEmptyHead) {
		return string("16");
	}
	else {
		return "x";
	}
}