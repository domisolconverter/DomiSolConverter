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

	// (수정)DomiSolConverter의 straitenedImg로 바꾸세요
	string INPUTPATH = "./inputImage/staffLine.jpg";
	
	Mat src = imread(INPUTPATH, IMREAD_GRAYSCALE);

	//cout<<"Creat Analysis Objects!"<<endl;
	//cout << "Access OuterClass Instance From InnerClass"<<endl;

	// Y축 히스토그램 그리기 //

	// 악보 한마디(20%)가 되면 오선이라 판단

	int width = src.cols;
	int height = src.rows;
	int oneBar = (int)((width / 100.0) * 20);    // 140
	int colCnt = 0;
	int lineCnt = 0;

	for (int nr = 0; nr < height; nr++) {
		
		
		uchar* pixel = src.ptr<uchar>(nr);
		list<int> X;
		colCnt = 0;

		for (int nc = 0; nc < width; nc++) {
			
			if (pixel[nc]>230) {
				X.push_back(nc);
				colCnt++;
			}
		}
		/*
		if (colCnt != 0) {
			cout <<  "의심 " << nr <<" 값 " <<  colCnt << endl;
		}
		*/

		if (colCnt >= oneBar) {
			lineCnt++;
			cout << lineCnt << ":" <<nr << " ";
			// 오선의 첫번째 줄
			if (lineCnt % 5 == 1) {
				this->staffXY.push_back(Point(X.front(), nr));
			}
			// 오선의 마지막번째 줄
			else if (lineCnt % 5 == 0 && lineCnt > 0) {
				this->staffXY.push_back(Point(X.back(), nr));
			}
		}

		// Y축 히스토그램 완성
		for (int nc = 0; nc < colCnt; nc++) {
			pixel[nc] = pixel[nc] / 64 * 64 + 128 / 2;
		}

	}


	for (int i = 0; i < staffXY.size(); i++) {
		cout << "( " << this->staffXY[i].x << " , " << this->staffXY[i].y <<" )" << endl;
	}

	
	Rect rect1(0, staffXY[0].y, width, staffXY[1].y - staffXY[0].y);
	Mat subimage1 = src(rect1);
	namedWindow("RECT01", CV_WINDOW_AUTOSIZE);
	imshow("RECT01", subimage1);
	
	Rect rect2(0, staffXY[2].y, width, staffXY[3].y - staffXY[2].y);
	Mat subimage2 = src(rect2);
	namedWindow("RECT02", CV_WINDOW_AUTOSIZE);
	imshow("RECT02", subimage2);

	Rect rect3(0, staffXY[4].y, width, staffXY[5].y - staffXY[4].y);
	Mat subimage3 = src(rect3);
	namedWindow("RECT03", CV_WINDOW_AUTOSIZE);
	imshow("RECT03", subimage3);

	
	Rect rect4(0, staffXY[6].y, width, staffXY[3].y - staffXY[2].y);
	Mat subimage4 = src(rect4);
	namedWindow("RECT04", CV_WINDOW_AUTOSIZE);
	imshow("RECT04", subimage4);
	


	namedWindow("OUT01", CV_WINDOW_AUTOSIZE);
	imshow("OUT01", src);

	destroyWindow("OUT01");

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