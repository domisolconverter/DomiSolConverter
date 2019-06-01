#include "pch.h"
#include "DomiSolConverter.h"

DomiSolConverter::DomiSolConverter(Mat input) {

	this->inputImg = input;

	Preprocessing P = Preprocessing(this->inputImg);
	this->objectsImg = P.getObjectsImg();
	this->objectXY = P.getObjectXY();
	this->straightenedImg = P.getStraightenedImg();
	this->straightenedBinaryImg = P.getStraightenedBinaryImg();

	Analysis A = Analysis(straightenedImg, straightenedBinaryImg, objectsImg, objectXY, this->wholeNotes, this->wholeNonNotes, this->text);
}
