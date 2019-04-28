#include "pch.h"
#include "DomiSolConverter.h"

DomiSolConverter::DomiSolConverter(Mat input) {

	this->inputImg = input;
	/*
	Preprocessing P = Preprocessing(this->inputImg);
	this->objectsImg = P.getObjectsImg();
	this->objectXY = P.getObjectXY();
	
	Analysis A = Analysis(objectsImg, objectXY);
	*/
	int width = inputImg.cols;
	int height = inputImg.rows;
	Rect tmp = Rect(0, 0, width, height);
	Analysis A = Analysis(inputImg, objectXY);
}
