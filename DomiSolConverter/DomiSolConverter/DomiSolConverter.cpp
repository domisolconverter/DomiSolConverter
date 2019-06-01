#include "pch.h"
#include "DomiSolConverter.h"

DomiSolConverter::DomiSolConverter(Mat input) {

	this->inputImg = input;
	Preprocessing P = Preprocessing(this->inputImg);
	this->objectsImg = P.getObjectsImg();
	this->objectXY = P.getObjectXY();

	//adaptiveThreshold(~inputImg, this->binaryImg, 255, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY, 5, -10);
	//imshow("MEAN", this->binaryImg);
	//adaptiveThreshold(~inputImg, this->binaryImg, 255, ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY, 15, -2);
	//imshow("Gaussian", this->binaryImg);
	//threshold(~inputImg, binaryImg, 0, 255, THRESH_BINARY | THRESH_OTSU);
	//imshow("OTSU", this->binaryImg);

	Analysis A = Analysis(objectsImg, objectXY);
}
