#include "pch.h"
#include <windows.h>
#include "DomiSolConverter.h"
#include <fstream>

DomiSolConverter::Postproecessing::Postproecessing(vector<Note> notes, vector<NonNote> nonNotes) {
	for (int i = 0; i < 2; i++) {
		Note note = Note(1, 1, i);
		if (i % 2 == 1) {
			note.setFlag_Head(1, false);
			note.setScale_Octave('E' + i, 1);
			note.setDot(false);
		}
		else if (i % 3 == 0) {
			note.setFlag_Head(0, false);
			note.setScale_Octave('C', 2);
			note.setDot(false);
		}
		else {
			note.setFlag_Head(1, true);
			note.setScale_Octave('E' + i, 0);
			note.setDot(true);
		}
		note.setIsWholeNote(false);
		this->notes.push_back(note);
	}

	saveFile(combineInfo());
}

string DomiSolConverter::Postproecessing::combineInfo() {
	vector<Note>::iterator iter;
	string code = "{\n";

	for (iter = notes.begin(); iter != notes.end(); iter++) {
		switch ((*iter).getScale()) {
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

		for (int oct = 0; oct < (*iter).getOctave(); oct++) {
			code += '\'';
		}

		switch ((*iter).getFlag()) {
		case 0: {
			if ((*iter).getIsEmptyHead()) {
				code += '2';
			}
			else {
				code += '4';
			}
			break;
		}
		case 1: code += '8'; break;
		case 2: code += '16'; break;
		}
		code += " ";
	}
	code += "\n}";

	return code
}

void DomiSolConverter::Postproecessing::transposeKey() {

}

void DomiSolConverter::Postproecessing::saveFile(string code) {
	ofstream out("output.ly");

	out << code;
	out.close();
	system("\"C:\\Program Files (x86)\\LilyPond\\usr\\bin\\lilypond\" output.ly");
}