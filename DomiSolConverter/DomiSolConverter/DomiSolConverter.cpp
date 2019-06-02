#include "pch.h"
#include "DomiSolConverter.h"

DomiSolConverter::DomiSolConverter(Mat input) {

	this->inputImg = input;

	Preprocessing P = Preprocessing(this->inputImg);
	this->straightenedImg = P.getStraightenedImg();
	this->straightenedBinaryImgforStaff = P.getStraightenedBinaryImgforStaff();
	this->straightenedBinaryImgforObject = P.getStraightenedBinaryImgforObject();

	Analysis A = Analysis(straightenedImg, straightenedBinaryImgforStaff, straightenedBinaryImgforObject, this->wholeNotes, this->wholeNonNotes, this->text);
}
