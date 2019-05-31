#include "pch.h"
#include "DomiSolConverter.h"

DomiSolConverter::DomiSolConverter(Mat input) {

	this->inputImg = input;

	Preprocessing P = Preprocessing(this->inputImg);
	this->objectsImg = P.getObjectsImg();
	this->objectXY = P.getObjectXY();
	this->straightenedBinaryImg = P.getStraightenedBinaryImg();
	Analysis A = Analysis(straightenedBinaryImg, objectsImg, objectXY);
	//A.setObjectsImg(this->objectsImg);
	//A.setObjectXY(this->objectsImg);

}
