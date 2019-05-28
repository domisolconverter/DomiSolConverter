#include "pch.h"
#include "DomiSolConverter.h"

DomiSolConverter::DomiSolConverter(Mat input) {

	this->inputImg = input; 
	this->straightenedImg = imread("./inputImage/straightenedImg.jpg", CV_LOAD_IMAGE_GRAYSCALE); // (수정)
	cout<<"Create DomiSolConverter Objects!"<<endl;
	Analysis A(straightenedImg);
}
