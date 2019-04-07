#include "pch.h"
#include "DomiSolConverter.h"

void DomiSolConverter::Analysis::calculateStaffHeight(){

}

void DomiSolConverter::Analysis::calculateStaffSpace(){

}

void DomiSolConverter::Analysis::calculateStaffXY(){

}

void DomiSolConverter::Analysis::extractFeature() {
	//Draw result
	Mat objectsRectImg = objectsImg;
	//Scalar color(0, 0, 0);
	Scalar color(255, 255, 255);
	for (int i = 0; i < objectXY.size(); i++) {
		rectangle(objectsRectImg, objectXY[i].tl(), objectXY[i].br(), color, 1);
	}
	namedWindow("objects", CV_WINDOW_AUTOSIZE);
	imshow("objects", objectsRectImg);
}

void DomiSolConverter::Analysis::calculatePitch() {

}

void DomiSolConverter::Analysis::recognizeObject() {

}

void DomiSolConverter::Analysis::recognizeGeneralSymbol() {

}

void DomiSolConverter::Analysis::recognizeText() {

}

void DomiSolConverter::Analysis::recognizeNoteSymbol() {

}

DomiSolConverter::Analysis::Analysis(Mat objectsImg, vector<Rect> objectXY) {
	this->objectsImg = objectsImg;
	this->objectXY = objectXY;

	extractFeature();
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