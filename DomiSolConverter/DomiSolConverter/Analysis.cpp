#include "pch.h"
#include "DomiSolConverter.h"

void DomiSolConverter::Analysis::calculateStaffHeight() {

}

void DomiSolConverter::Analysis::calculateStaffSpace() {

}

void DomiSolConverter::Analysis::calculateStaffXY() {

}

void DomiSolConverter::Analysis::extractFeature() {
	//Draw result
	Mat objectsRectImg = objectsImg;
	//Scalar color(0, 0, 0);
	Scalar color(255, 255, 255);
	for (int i = 0; i < objectXY.size(); i++) {
		rectangle(objectsRectImg, objectXY[i].tl(), objectXY[i].br(), color, 1);
		Mat extractOject = ~objectsRectImg(Rect(objectXY[i]));
		// assume that it's work
		string fname = "test_input\\object" + to_string(i) + ".jpg";
		imwrite(fname, extractOject);
	}
}

void DomiSolConverter::Analysis::calculatePitch() {

}

void DomiSolConverter::Analysis::recognizeObject() {

}

void DomiSolConverter::Analysis::recognizeGeneralSymbol() {
	char output[100];
	FILE *p = _popen("python label_image.py symbols", "r");

	if (p != NULL) {
		while (fgets(output, sizeof(output), p) != NULL) {
			if (strlen(output) > 0) {
				this->nonNote.push_back(output);
			}
		}
	}
}

void DomiSolConverter::Analysis::recognizeText() {

}

void DomiSolConverter::Analysis::recognizeNoteSymbol() {

}

DomiSolConverter::Analysis::Analysis(Mat objectsImg, vector<Rect> objectXY) {
	this->objectsImg = objectsImg;
	this->objectXY = objectXY;

	//extractFeature();
	recognizeGeneralSymbol();
}

vector<string> DomiSolConverter::Analysis::getNote() {
	vector<string> result;
	return result;
}

vector<string> DomiSolConverter::Analysis::getNonNote() {
	vector<string> result;
	return result;
}

vector<string> DomiSolConverter::Analysis::getText() {
	vector<string> result;
	return result;
}

void DomiSolConverter::Analysis::setObjectsImg(Mat objectsImg) {
	this->objectsImg = objectsImg;
}

void DomiSolConverter::Analysis::setObjectXY(vector<Rect> objectXY) {
	this->objectXY = objectXY;
}