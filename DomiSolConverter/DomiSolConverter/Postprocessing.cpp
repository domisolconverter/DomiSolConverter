#include "pch.h"
#include <windows.h>
#include "DomiSolConverter.h"
#include <fstream>
#include <ctime>

DomiSolConverter::Postproecessing::Postproecessing(vector<Note> notes, vector<NonNote> nonNotes) {
	srand((unsigned int)time(0));

	for (int i = 0; i < 3; i++) {
		Note note = Note();
		char tone = 'A' + (rand() % 7);
		int octave = rand() % 2 + 1;
		int flag = rand() % 3;
		int dot = rand() % 2;

		if (flag == 0) {
			note.setFlag_Head(flag, true);
		}
		else {
			note.setFlag_Head(flag, false);
		}

		note.setScale_Octave(tone, octave);
		note.setDot(dot);
		note.setIsWholeNote(false);
		this->notes.push_back(note);
	}

	combineInfo();
	saveFile(makeCode());
}

void DomiSolConverter::Postproecessing::combineInfo() {
	vector<Note>::iterator note = notes.begin();
	vector<NonNote>::iterator nonNote = nonNotes.begin();

	for (int i = 0; i < notes.size(); i++) {
		this->wholeSign.push_back(&(notes[i]));
	}

	for (int i = 0; i < nonNotes.size(); i++) {
		this->wholeSign.push_back(&(nonNotes[i]));
	}
}

string DomiSolConverter::Postproecessing::makeCode() {
	vector<Symbol*>::iterator iter = wholeSign.begin();

	string code = "{\n";

	for (int i = 0; i < wholeSign.size(); i++) {
		if (wholeSign[i]->getType().compare("note") == 0) {
			Note *note = (Note*)(wholeSign[i]);
			code += this->makeNoteCode(note);
		}
		if (wholeSign[i]->getType().compare("nonNote") == 0) {
			NonNote *nonNote = (NonNote*)(wholeSign[i]);
			code += this->makeNonNoteCode(nonNote);
		}
		code += " ";
	}
	code += "\n}";

	return code;
}

string DomiSolConverter::Postproecessing::makeNonNoteCode(NonNote *nonNote) {
}

string DomiSolConverter::Postproecessing::makeNoteCode(Note *note) {
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