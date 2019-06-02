#include "pch.h"
#include "DomiSolConverter.h"

DomiSolConverter::DomiSolConverter(Mat input) {

	this->inputImg = input;

	Preprocessing P = Preprocessing(this->inputImg);
	this->straightenedImg = P.getStraightenedImg();
	this->straightenedBinaryImg = P.getStraightenedBinaryImg();

	Analysis A = Analysis(straightenedImg, straightenedBinaryImg, this->wholeNotes, this->wholeNonNotes, this->text);
}
