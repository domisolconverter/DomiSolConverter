#include "pch.h"
#include "DomiSolConverter.h"


void DomiSolConverter::Analysis::calculateStaffHeight(){

}

void DomiSolConverter::Analysis::calculateStaffSpace(){

}

void DomiSolConverter::Analysis::calculateStaffXY(){
	
	// �Ǻ� �Ѹ���(20%)�� �Ǹ� �����̶� �Ǵ�
	

	// ���� ������׷� �׷��鼭
	// �� ���� ������ ù��° ��ġ�� �������� ����

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

	// (����)DomiSolConverter�� straitenedImg�� �ٲټ���
	string INPUTPATH = "./inputImage/staffLine.jpg";
	
	Mat src = imread(INPUTPATH, IMREAD_GRAYSCALE);

	//cout<<"Creat Analysis Objects!"<<endl;
	//cout << "Access OuterClass Instance From InnerClass"<<endl;

	// Y�� ������׷� �׸��� //

	// �Ǻ� �Ѹ���(20%)�� �Ǹ� �����̶� �Ǵ�

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
			cout <<  "�ǽ� " << nr <<" �� " <<  colCnt << endl;
		}
		*/

		if (colCnt >= oneBar) {
			lineCnt++;
			cout << lineCnt << ":" <<nr << " ";
			// ������ ù��° ��
			if (lineCnt % 5 == 1) {
				this->staffXY.push_back(Point(X.front(), nr));
			}
			// ������ ��������° ��
			else if (lineCnt % 5 == 0 && lineCnt > 0) {
				this->staffXY.push_back(Point(X.back(), nr));
			}
		}

		// Y�� ������׷� �ϼ�
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