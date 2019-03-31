#include "pch.h"
#include "DomiSolConverter.h"


void DomiSolConverter::Analysis::calculateStaffHeight(){

}

void DomiSolConverter::Analysis::calculateStaffSpace(){

}

void DomiSolConverter::Analysis::calculateStaffXY(){
	
	// 악보 한마디(20%)가 되면 오선이라 판단
	

	// 수직 히스토그램 그려면서
	// 각 줄의 오선의 첫번째 위치와 마지막을 저장

}

void DomiSolConverter::Analysis::extractFeature() {

}

void DomiSolConverter::Analysis::calculatePitch() {

}

void DomiSolConverter::Analysis::recognizeObject() {

}

void DomiSolConverter::Analysis::recognizeGeneralSymbol() {

}

void DomiSolConverter::Analysis::recognizeText() {

}

void DomiSolConverter::Analysis::recognizeNoteSymbol() {

}

DomiSolConverter::Analysis::Analysis() {

	cout<<"Creat Analysis Objects!"<<endl;
	cout << "Access OuterClass Instance From InnerClass"<<endl;

}

vector<string> DomiSolConverter::Analysis::getNote() {
	vector<string> result;
	return result;
}

vector<string> DomiSolConverter::Analysis::getNonNote() {
	vector<string> result;
	return result;
}

vector<string> DomiSolConverter::Analysis::getText() {
	vector<string> result;
	return result;
}