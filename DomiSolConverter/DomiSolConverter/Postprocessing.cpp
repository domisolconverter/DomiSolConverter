#include "pch.h"
#include <windows.h>
#include "DomiSolConverter.h"
#include <fstream>
#include <ctime>
#include <algorithm>

DomiSolConverter::Postproecessing::Postproecessing(vector<Note> notes, vector<NonNote> nonNotes) {
	srand((unsigned int)time(0));

	this->nonNotes.push_back(NonNote("4 flat"));
	this->nonNotes.push_back(NonNote("4 rest"));
	this->nonNotes.push_back(NonNote("g clef"));
	this->nonNotes.push_back(NonNote("slur"));
	this->nonNotes.push_back(NonNote("slur"));
	this->nonNotes.push_back(NonNote("bar"));
	this->nonNotes.push_back(NonNote("bar"));
	this->nonNotes.push_back(NonNote("bar"));
	this->nonNotes.push_back(NonNote("4_4"));

	this->nonNotes[2].x = 1;
	this->nonNotes[1].x = 50;
	this->nonNotes[0].x = 2;
	this->nonNotes[8].x = 0;
	this->nonNotes[8].y = 3;
	this->nonNotes[2].y = 3;
	this->nonNotes[0].y = 3;
	this->nonNotes[1].y = 3;

	//slur
	this->nonNotes[3].x = 3;
	this->nonNotes[3].y = 6;
	this->nonNotes[4].x = 15;
	this->nonNotes[4].y = 18;

	this->nonNotes[5].x = 14;
	this->nonNotes[5].y = 3;
	this->nonNotes[6].x = 25;
	this->nonNotes[6].y = 3;
	this->nonNotes[7].x = 37;
	this->nonNotes[7].y = 18;

	Note n[13];

	n[0] = Note();

	n[0].setDot(true);
	n[0].setFlag_Head(0, false);
	n[0].setScale_Octave('G', 1);

	n[1]= Note();

	n[1].setDot(false);
	n[1].setFlag_Head(1, false);
	n[1].setScale_Octave('A', 1);

	n[2] = Note();

	n[2].setDot(false);
	n[2].setFlag_Head(0, false);
	n[2].setScale_Octave('B', 1);

	n[3] = Note();

	n[3].setDot(false);
	n[3].setFlag_Head(0, false);
	n[3].setScale_Octave('G', 1);

	n[4] = Note();

	n[4].setDot(true);
	n[4].setFlag_Head(0, false);
	n[4].setScale_Octave('C', 2);

	n[5] = Note();

	n[5].setDot(false);
	n[5].setFlag_Head(1, false);
	n[5].setScale_Octave('D', 2);

	n[6] = Note();

	n[6].setDot(false);
	n[6].setFlag_Head(0, false);
	n[6].setScale_Octave('E', 2);

	n[7] = Note();

	n[7].setDot(false);
	n[7].setFlag_Head(0, false);
	n[7].setScale_Octave('C', 2);

	n[8] = Note();

	n[8].setDot(false);
	n[8].setFlag_Head(0, false);
	n[8].setScale_Octave('B', 1);

	n[9] = Note();

	n[9].setDot(false);
	n[9].setFlag_Head(0, false);
	n[9].setScale_Octave('A', 1);

	n[10] = Note();

	n[10].setDot(false);
	n[10].setFlag_Head(0, false);
	n[10].setScale_Octave('G', 1);

	n[11] = Note();

	n[11].setDot(false);
	n[11].setFlag_Head(0, false);
	n[11].setScale_Octave('A', 1);

	n[12] = Note();

	n[12].setDot(true);
	n[12].setFlag_Head(0, true);
	n[12].setScale_Octave('B', 1);

	int arr[13] = { 4, 6, 1, 8, 11, 0, 3, 12, 5, 7, 2, 9, 10 };

	for (int i = 0; i < 13; i++) {
		n[arr[i]].x = (arr[i]+1) * 3;
		n[arr[i]].y = 3;
		this->notes.push_back(n[arr[i]]);
	}

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

	else if ((*nonNote).getNonNoteType().compare("g clef") == 0) {
		code = " \\clef treble\n";
	}

	else if ((*nonNote).getNonNoteType().compare("f clef") == 0) {
		code = " \\clef bass\n";
	}

	else if ((*nonNote).getNonNoteType().compare("4 rest") == 0) {
		code = "r4";
	}

	else if ((*nonNote).getNonNoteType().compare("4_4") == 0) {
		code = " \\time 4/4\n";
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