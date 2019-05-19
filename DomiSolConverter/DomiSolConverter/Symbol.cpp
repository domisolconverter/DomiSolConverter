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

Note::Note(int flag, bool isEmptyHead, bool dot, bool isWholeNote, char scale, int octave, hash_map<string, bool> nonNotes) {
	this->flag = flag;
	this->isEmptyHead = isEmptyHead;
	this->dot = dot;
	this->isWholeNote = isWholeNote;
	this->scale = scale;
	this->octave = octave;
	this->nonNotes = nonNotes;
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

hash_map<string, bool> Note::getNonNotes() {
	return this->nonNotes;
}


NonNote::NonNote(int lineNum, int barNum, int orderNum, string nonNoteType) {
	this->nonNoteType = nonNoteType;
}

string NonNote::getNonNoteType() {
	return this->nonNoteType;
}