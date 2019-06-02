#include "pch.h"
#include <windows.h>
#include "DomiSolConverter.h"
#include <fstream>
#include <ctime>
#include <sstream>
#include <string>
#include <algorithm>

DomiSolConverter::Postproecessing::Postproecessing(string inputPath, vector<Note> notes, vector<NonNote> nonNotes) {
	this->notes = notes;
	this->nonNotes = nonNotes;

	combineInfo();
	saveFile(inputPath, makeCode());
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

	Signature signature = Signature();

	string code = "{\n";

	for (int i = 0; i < wholeSign.size(); i++) {
		if (wholeSign[i]->getType().compare("note") == 0) {
			Note *note = (Note*)(wholeSign[i]);
			code += this->makeNoteCode(note, &(signature));
		}
		if (wholeSign[i]->getType().compare("nonNote") == 0) {
			NonNote *nonNote = (NonNote*)(wholeSign[i]);
			if (!signature.isSignatureEnd((*nonNote))) {
				code += this->makeNonNoteCode(nonNote, &(signature));
			}
			else {
				code += signature.getCode();
				signature.clearElement();
			}
		}
		code += " ";
	}
	
	code += "\n}";

	return code;
}

string DomiSolConverter::Postproecessing::makeNonNoteCode(NonNote *nonNote, Signature *sig) {
	string code = "";

	if ((*nonNote).getNonNoteType().compare("bar") == 0) {
		code = "|";
	}
	
	else if ((*nonNote).getNonNoteType().compare("flat") == 0 || (*nonNote).getNonNoteType().compare("sharp") == 0) {
		sig->putElement((*nonNote));
	}

	else if ((*nonNote).getNonNoteType().compare("g clef") == 0) {
		code = "\\break \\clef treble\n";
	}

	else if ((*nonNote).getNonNoteType().compare("f clef") == 0) {
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

	else if ((*nonNote).getNonNoteType().compare("4 4") == 0) {
		code = "}\n{\n \\time 4/4\n";
	}

	else if ((*nonNote).getNonNoteType().compare("2 4") == 0) {
		code = "}\n{\n \\time 2/4\n";
	}

	else if ((*nonNote).getNonNoteType().compare("2 8") == 0) {
		code = "}\n{\n \\time 2/8\n";
	}

	else if ((*nonNote).getNonNoteType().compare("3 4") == 0) {
		code = "}\n{\n \\time 3/4\n";
	}

	else if ((*nonNote).getNonNoteType().compare("3 8") == 0) {
		code = "}\n{\n \\time 3/8\n";
	}
	else {
		return "";
	}
	return code;
}

string DomiSolConverter::Postproecessing::makeNoteCode(Note *note, Signature *sig) {
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

	vector<string> speical = sig->getScale();

	if (find(speical.begin(), speical.end(), string(1, note->getScale())) != speical.end()) {
		if (sig->getType().compare("flat") == 0) {
			code += "is";
		}
		else {
			code += "es";
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

void DomiSolConverter::Postproecessing::saveFile(string inputPath, string code) {
	string resultFile = getResultName(inputPath);
	ofstream out(resultFile);

	out << code;
	out.close();
	system("\"C:\\Program Files (x86)\\LilyPond\\usr\\bin\\lilypond\" output.ly");
}

string DomiSolConverter::Postproecessing::getResultName(string inputPath) {
	vector<string> path;
	stringstream ss(inputPath);
	string temp;

	while (getline(ss, temp, '/')) {
		path.push_back(temp);
	}
	string result = path.back();
	path.pop_back();
	result = "result_" + result.substr(0, result.size() - 4);
	return result;
}