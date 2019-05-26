#include "pch.h"
#include "DomiSolConverter.h"

DomiSolConverter::Analysis::Analysis(Mat objectsImg, vector<Rect> objectXY) {
	this->objectsImg = objectsImg;
	this->objectXY = objectXY;

	extractFeature();
	recognizeGeneralSymbol();
}

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
		string fname = "symbols\\object" + to_string(i) + ".jpg";
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
		int idx = 0;
		while (fgets(output, sizeof(output), p) != NULL) {
			if (strlen(output) > 0) {
				for (int i = 0; i < 100; i++) {
					if (output[i] == '\n') {
						output[i] = '\0';
					}
				}
				cout << output << "\n";
				NonNote nn(1, 1, 1, output, objectXY[idx].x + (objectXY[idx].width / 2), objectXY[idx].y + (objectXY[idx].height / 2));
				this->nonNote.push_back(nn);
			}
			idx++;
		}
	}
}

void DomiSolConverter::Analysis::recognizeText() {

}

void DomiSolConverter::Analysis::recognizeNoteSymbol() {

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