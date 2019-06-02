#include "pch.h"
#include "Signature.h"

void Signature::putElement(NonNote nonNote) {
	this->element.push_back(nonNote);
	this->type = nonNote.getNonNoteType();
}

bool Signature::isSignatureEnd(NonNote nonNote) {
	if (!this->element.empty()) {
		if (this->element.begin()->getNonNoteType().compare("flat") == 0) {
			if (nonNote.getNonNoteType().compare("flat") == 0) {
				return false;
			}
			else {
				return true;
			}
		}
		else if (this->element.begin()->getNonNoteType().compare("sharp") == 0) {
			if (nonNote.getNonNoteType().compare("sharp") == 0) {
				return false;
			}
			else {
				return true;
			}
		}
	}
	return false;
}

string Signature::getSign() {
	return this->sign;
}

string Signature::calculateSign() {
	vector<string> scales;
	if (this->type.compare("sharp") == 0) {
		char tmp[6] = { 'G', 'D', 'A', 'E', 'B', 'F' };

		for (int i = 0; i < 6; i++) {
			scales.push_back(string(1, tmp[i]));
		}
	}
	else {
		string tmp[6] = { "fes", "bes", "ees", "aes", "des", "ges" };

		for (int i = 0; i < 6; i++) {
			scales.push_back(tmp[i]);
		}
	}
	this->sign = scales[this->element.size()];
	return sign;
}

void Signature::calculateScale() {
	if (this->sign.compare("G") == 0) {
		scale.push_back("F");
	}
	else if (this->sign.compare("D") == 0) {
		scale.push_back("F");
		scale.push_back("C");
	}
	else if (this->sign.compare("A") == 0) {
		scale.push_back("F");
		scale.push_back("C");
		scale.push_back("G");
	}
	else if (this->sign.compare("E") == 0) {
		scale.push_back("F");
		scale.push_back("C");
		scale.push_back("G");
		scale.push_back("D");
	}
	else if (this->sign.compare("B") == 0) {
		scale.push_back("F");
		scale.push_back("C");
		scale.push_back("G");
		scale.push_back("D");
		scale.push_back("A");
	}
	else if (this->sign.compare("F") == 0) {
		scale.push_back("F");
		scale.push_back("C");
		scale.push_back("G");
		scale.push_back("D");
		scale.push_back("A");
		scale.push_back("E");
	}

	else if (this->sign.compare("fes") == 0) {
		scale.push_back("B");
	}
	else if (this->sign.compare("bes") == 0) {
		scale.push_back("B");
		scale.push_back("E");
	}
	else if (this->sign.compare("ees") == 0) {
		scale.push_back("B");
		scale.push_back("E");
		scale.push_back("A");
	}
	else if (this->sign.compare("aes") == 0) {
		scale.push_back("B");
		scale.push_back("E");
		scale.push_back("A");
		scale.push_back("D");
	}
	else if (this->sign.compare("des") == 0) {
		scale.push_back("B");
		scale.push_back("E");
		scale.push_back("A");
		scale.push_back("D");
		scale.push_back("G");
	}
	else if (this->sign.compare("ges") == 0) {
		scale.push_back("B");
		scale.push_back("E");
		scale.push_back("A");
		scale.push_back("D");
		scale.push_back("G");
		scale.push_back("C");
	}
}

string Signature::getCode() {
	string code = "";
	code = "\\key " + calculateSign() + "\\major\n";
	this->calculateScale();
	return code;
}

void Signature::clearElement() {
	this->element.clear();
}

vector<string> Signature::getScale() {
	return this->scale;
}

string Signature::getType() {
	return this->type;
}