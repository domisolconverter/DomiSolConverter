#include "pch.h"
#include "Symbol.h"

Symbol::Symbol(int lineNum, int barNum, int orderNum) {
	this->lineNum = lineNum;
	this->barNum = barNum;
	this->orderNum = orderNum;
}

int Symbol::getLineNum() {
	return this->lineNum;
}

int Symbol::getBarNum() {
	return this->barNum;
}

int Symbol::getOrderNum() {
	return this->orderNum;
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

//hash_map<string, bool> Note::getNonNotes() {
//	return this->nonNotes;
//}

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

//void Note::setNonNotes(hash_map<string, bool> nonNotes) {
//	this->nonNotes = nonNotes;
//}

void Note::setResult(string result) {
	this->result = result;
}



string NonNote::getNonNoteType() {
	return this->nonNoteType;
}

void NonNote::setNonNoteType() {
	this->nonNoteType = nonNoteType;
}
