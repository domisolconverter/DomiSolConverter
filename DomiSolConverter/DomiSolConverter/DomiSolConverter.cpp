#include "pch.h"
#include "DomiSolConverter.h"

DomiSolConverter::DomiSolConverter(Mat input) {


	this->inputImg = input; 
	Preprocessing P = Preprocessing(this->inputImg);
	this->objectsImg = P.getObjectsImg();
	this->objectXY = P.getObjectXY();

	Analysis A = Analysis(objectsImg, objectXY);

	Postproecessing pp = Postproecessing(notes, nonNotes);
}
