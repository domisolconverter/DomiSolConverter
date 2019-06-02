#include "pch.h"
#include <windows.h>
#include "DomiSolConverter.h"
#include <fstream>
#include <ctime>
#include <algorithm>

DomiSolConverter::Postproecessing::Postproecessing(vector<Note> notes, vector<NonNote> nonNotes) {
	this->notes = notes;
	this->nonNotes = nonNotes;

	combineInfo();
	saveFile(makeCode());
}

void DomiSolConverter::Postproecessing::combineInfo() {
	vector<Note>::iterator note = notes.begin();
	vector<NonNote>::iterator nonNote = nonNotes.begin();

	for (int i = 0; i < this->notes.size(); i++) {
		this->wholeSign.push_back(&(this->notes[i]));
	}

	for (int i = 0; i < this->nonNotes.size(); i++) {
		this->wholeSign.push_back(&(this->nonNotes[i]));
	}

	sort(this->wholeSign.begin(), this->wholeSign.end(), compare);
}

bool DomiSolConverter::Postproecessing::compare(Symbol *a, Symbol *b) {
	if ((*a).getLineNum() == (*b).getLineNum()) {
		return ((*a).x < (*b).x);
	}
	else {
		return ((*a).getLineNum() < (*b).getLineNum());
	}
	return true;
}

string DomiSolConverter::Postproecessing::makeCode() {
	vector<Symbol*>::iterator iter = wholeSign.begin();

	vector<char> flat;
	vector<char> sharp;

	string code = "{\n";

	for (int i = 0; i < wholeSign.size(); i++) {
		if (wholeSign[i]->getType().compare("note") == 0) {
			Note *note = (Note*)(wholeSign[i]);
			code += this->makeNoteCode(note, &(flat), &(sharp));
		}
		if (wholeSign[i]->getType().compare("nonNote") == 0) {
			NonNote *nonNote = (NonNote*)(wholeSign[i]);
			code += this->makeNonNoteCode(nonNote, &(flat), &(sharp));
		}
		code += " ";
	}
	
	code += "\n";

	for (int i = 0; i < wholeSign.size(); i++) {
		if (wholeSign[i]->getType().compare("note") == 0) {
			Note *note = (Note*)(wholeSign[i]);
			code += this->makeNoteCode(note, &(flat), &(sharp));
		}
		if (wholeSign[i]->getType().compare("nonNote") == 0) {
			NonNote *nonNote = (NonNote*)(wholeSign[i]);
			code += this->makeNonNoteCode(nonNote, &(flat), &(sharp));
		}
		code += " ";
	}
	
	code += "\n}";

	return code;
}

string DomiSolConverter::Postproecessing::makeNonNoteCode(NonNote *nonNote, vector<char> *flat, vector<char> *sharp) {
	string code = "";

	if ((*nonNote).getNonNoteType().compare("bar") == 0) {
		code = "|";
	}
	
	else if ((*nonNote).getNonNoteType().compare("4 flat") == 0) {
		code = " \\key aes \\major\n";
		(*sharp).push_back('A');
		(*sharp).push_back('B');
		(*sharp).push_back('D');
		(*sharp).push_back('E');
	}

	else if ((*nonNote).getNonNoteType().compare("g_clef") == 0) {
		code = "\\break \\clef treble\n";
	}

	else if ((*nonNote).getNonNoteType().compare("f_clef") == 0) {
		code = "\\break \\clef bass\n";
	}

	else if ((*nonNote).getNonNoteType().compare("r4") == 0) {
		code = "r4";
	}

	else if ((*nonNote).getNonNoteType().compare("r8") == 0) {
		code = "r8";
	}

	else if ((*nonNote).getNonNoteType().compare("r16") == 0) {
		code = "r16";
	}

	else if ((*nonNote).getNonNoteType().compare("4_4") == 0) {
		code = "}\n{\n \\time 4/4\n";
	}

	else if ((*nonNote).getNonNoteType().compare("2_4") == 0) {
		code = "}\n{\n \\time 2/4\n";
	}

	else if ((*nonNote).getNonNoteType().compare("2_8") == 0) {
		code = "}\n{\n \\time 2/8\n";
	}

	else if ((*nonNote).getNonNoteType().compare("3_4") == 0) {
		code = "}\n{\n \\time 3/4\n";
	}

	else if ((*nonNote).getNonNoteType().compare("3_8") == 0) {
		code = "}\n{\n \\time 3/8\n";
	}
	else {
		return "";
	}
	return code;
}

string DomiSolConverter::Postproecessing::makeNoteCode(Note *note, vector<char> *flat, vector<char> *sharp) {
	string code = "";

	switch (note->getScale()) {
	case 'C': {
		code += 'c';
		break;
	}
	case 'D': {
		code += 'd';
		break;
	}
	case 'E': {
		code += 'e';
		break;
	}
	case 'F': {
		code += 'f';
		break;
	}
	case 'G': {
		code += 'g';
		break;
	}
	case 'A': {
		code += 'a';
		break;
	}
	case 'B': {
		code += 'b';
		break;
	}
	}

	if (find((*flat).begin(), (*flat).end(), note->getScale()) != (*flat).end()) {
		code += "is";
	}

	if (find((*sharp).begin(), (*sharp).end(), note->getScale()) != (*sharp).end()) {
		code += "es";
	}

	for (int oct = 0; oct < note->getOctave(); oct++) {
		code += '\'';
	}

	

	string bit = note->transposeNote();
	if (bit.compare(string("0")) != 0) {
		code += bit;
	}

	if (note->getDot()) {
		code += ".";
	}

	return code;
}

void DomiSolConverter::Postproecessing::saveFile(string code) {
	ofstream out("output.ly");

	out << code;
	out.close();
	system("\"C:\\Program Files (x86)\\LilyPond\\usr\\bin\\lilypond\" output.ly");
}