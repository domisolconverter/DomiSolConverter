#include "pch.h"
#include "DomiSolConverter.h"

DomiSolConverter::DomiSolConverter(Mat input) {

	this->inputImg = input;

	// (수정) //
	this->straightenedImg = imread("./inputImage/straightenedImg.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	threshold(this->straightenedImg, this->straightenedImg, 0, 255, THRESH_BINARY | THRESH_OTSU);
	// (수정) // 
	
	cout<<"Create DomiSolConverter Objects!"<<endl;
	Analysis A(straightenedImg);
}
