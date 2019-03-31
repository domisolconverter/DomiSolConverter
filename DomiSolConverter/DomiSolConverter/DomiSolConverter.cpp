#include "pch.h"
#include "DomiSolConverter.h"

DomiSolConverter::DomiSolConverter(Mat input) {

	this->inputImg = input; 
	this->straightenedImg = input; // 나중에 삭제
	cout<<"Create DomiSolConverter Objects!"<<endl;
	Analysis A;
}
