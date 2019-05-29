#include "pch.h"
#include "DomiSolConverter.h"


// 오선 한 개(5개의 줄)의 높이
void DomiSolConverter::Analysis::calculateStaffHeight(){
	int sum = 0;
	for (int i = 1; i < staffXY.size(); i+=2 ) {
		sum += staffXY[i].y - staffXY[i - 1].y;
	}
	if (staffXY.size() != 0) {
		this -> staffHeight = sum / (staffXY.size() / 2);
	}
	else {
		cout << "오선의 개수가 0개 입니다"<< endl;
	}
}

// 오선에 있는 줄 사이 간격
void DomiSolConverter::Analysis::calculateStaffSpace(){

}

void DomiSolConverter::Analysis::calculateStaffXY(){


	Mat src = this->inputCalculateStaffImg;

	int width = src.cols;
	int height = src.rows;
	int oneBar = (int)((width / 100.0) * 20);    // 140
	int colCnt = 0;
	int staffNum = 0;
	vector<int> Y;
	vector<int> X;

	//* Y축 히스토그램 그리기 *//

	// 악보 한마디(20%)가 되면 오선이라 판단

	for (int nr = 0; nr < height; nr++) {


		uchar* pixel = src.ptr<uchar>(nr);
		list<int> tempX;
		colCnt = 0;

		for (int nc = 0; nc < width; nc++) {

			if (pixel[nc]<10) {
				tempX.push_back(nc);
				colCnt++;
			}
		}
		/*
		if (colCnt != 0) {
		cout <<  "의심 " << nr <<" 값 " <<  colCnt << endl;
		}
		*/

		if (colCnt >= oneBar) {

			Y.push_back(nr);
			X.push_back(tempX.front());
			X.push_back(tempX.back());
		}

		// Y축 히스토그램 완성
		for (int nc = 0; nc < colCnt; nc++) {
			pixel[nc] = pixel[nc] / 64 * 64 + 128 / 2;
		}

	}

	// 악보 라인 보정

	for (int i = 1; i < Y.size(); i++) {
		if (Y[i] - Y[i - 1] <= 2) {
			Y.erase(Y.begin() + i - 1);
			X.erase(X.begin() + 2 * i);
			X.erase(X.begin() + 2 * i + 1);
		}
	}

	for (int i = 0; i < Y.size(); i++) {
		//cout << Y[i] << " ";
	}

	// 최소3개 악보 간격보다 2배 이상 커지면
	// 다음 오선으로 판단
	int lineCnt = 0;
	int spaceCnt = 0;
	int diff = 0;
	int diffAvg = 0;
	for (int i = 1; i < Y.size(); i++) {
		if (spaceCnt == 0) {

			diff = Y[i] - Y[i - 1];
			spaceCnt++;

		}
		else {
			// 평균 구하기
			diffAvg = diff / spaceCnt;
			// 평균의 200%보다 차이가 크지 않다면 추가
			if (diffAvg * 2.5 >= Y[i] - Y[i - 1]) {
				diff += Y[i] - Y[i - 1];
				spaceCnt++;

			}
			// 크다 그리고 만약 그게 3줄 이상이면 그 전까지를 staff 하나라고 생각하기!
			else {

				//cout << "현재 평균 " << diffAvg << endl;
				//cout << "간격 " << spaceCnt << endl;

				if (spaceCnt >= 3) {
					lineCnt++;
					//cout << lineCnt << "번 째 오선이네요 첫번째 줄 y는 " << Y[i - 1 - spaceCnt] << endl;
					//cout << lineCnt << "번 째 오선이네요 마지막 줄 y는 " << Y[i] << endl;

					this->staffXY.push_back(Point(X[(i - 1 - spaceCnt) * 2], Y[i - 1 - spaceCnt]));
					this->staffXY.push_back(Point(X[(i - 1) * 2 + 1], Y[i - 1]));

				}
				else {
					//cout << "얘는 오선이 아니에요" << endl;

				}
				spaceCnt = 0;
				diff = 0;
				diffAvg = 0;

			}

			// 배열이 끝나고 마지막 staff 체크
			if (i == Y.size() - 1 && spaceCnt >= 3) {
				lineCnt++;
				//cout << lineCnt << "번 째 오선이네요" << endl;
				this->staffXY.push_back(Point(X[(i - spaceCnt) * 2], Y[i - spaceCnt]));
				this->staffXY.push_back(Point(X[i * 2 + 1], Y[i]));
			}

		}


	}
	
	// 오선의 평균 높이 계산
	calculateStaffHeight();

	//* 오선 검출 결과 테스트 프린트 *//
	/*
	for (int i = 0; i < staffXY.size(); i++) {
		cout << "( " << this->staffXY[i].x << " , " << this->staffXY[i].y << " )" << endl;
	}
	*/

	//* 오선 ROI 테스트 프린트 *//
	
	/*
	Rect rect[10]; // (수정) 동적 할당하기
	for (int i = 0; i < staffXY.size()/2; i++) {
		rect[i] = Rect(0, staffXY[i].y, width, this->staffHeight); // (수정)평균 오선 크기 넣기
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

	Rect rect4(0, staffXY[6].y, width, staffXY[7].y - staffXY[6].y);
	Mat subimage4 = src(rect4);
	namedWindow("RECT04", CV_WINDOW_AUTOSIZE);
	imshow("RECT04", subimage4);
	

	namedWindow("OUT01", CV_WINDOW_AUTOSIZE);
	imshow("OUT01", src);

	destroyWindow("OUT01");
	*/

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
	
	// (수정)DomiSolConverter의 straitenedImg로 바꾸세요
	string INPUTPATH = "./inputImage/straightenedImg.jpg";
	string OUTPUTPATH = "./outputImage/textpart";

	Mat src = imread(INPUTPATH, IMREAD_GRAYSCALE);
	int width = src.cols;
	int height = src.rows;

	// 오선 아닌 부분 잘라서 저장하기
	
	Rect ROI;
	Mat tempImg;

	// 1. 각 오선의 윗부분 crop
	for (int i = 0; i < staffXY.size(); i++) {
		// 처음
		if (i == 0) {
			ROI = Rect(0, 0, width, staffXY[i].y);
			Mat subImg = Mat(src, ROI);
			imwrite(OUTPUTPATH + to_string(i) + ".jpg", subImg);
		}
		// 마지막
		else if (i == staffXY.size() - 1) {
			ROI = Rect(0, staffXY[i].y, width, this->staffHeight * 2);
			Mat subImg = Mat(src, ROI);
			imwrite(OUTPUTPATH + to_string(i) + ".jpg", subImg);
		}
		// 중간
		else {
			ROI = Rect(0, staffXY[i].y, width, staffXY[i+1].y - staffXY[i].y);
			Mat subImg = Mat(src, ROI);
			imwrite(OUTPUTPATH + to_string(i) + ".jpg", subImg);
			i++;
		}

	}



}

void DomiSolConverter::Analysis::recognizeNoteSymbol() {
	
}

// 기울기 보정한 이미지의 검은색 모서리 부분을 하얀색으로 바꾸기
void DomiSolConverter::Analysis::colorConers() {

	Mat input = this->straightenedImg;
	int width = input.cols;
	int height = input.rows;
	int bar = (int)((width / 100.0) * 20);

	// 양쪽 네 곳의 모서리에 대해서 Run Length Coding 진행
	// 하얀색으로 칠하기
	for (int nr = 0; nr < height; nr++) {

		uchar* pixel = input.ptr<uchar>(nr);

		for (int nc = 0; nc < width; nc++) {
			if (pixel[nc] < 10) {
				pixel[nc] = 255;
			}
			else {
				break;
			}
		}

	}

	for (int nr = 0; nr < height; nr++) {

		uchar* pixel = input.ptr<uchar>(nr);

		for (int nc = width-1; nc >= 0; nc--) {
			if (pixel[nc] < 10) {
				pixel[nc] = 255;
			}
			else {
				break;
			}
		}

	}

	this->inputCalculateStaffImg = input;

	// 출력 //
	
	namedWindow("input", CV_WINDOW_AUTOSIZE);
	imshow("input", this->inputCalculateStaffImg);

	destroyWindow("input");
	

}

DomiSolConverter::Analysis::Analysis(Mat straightenedImg){
	
	this->straightenedImg = straightenedImg;
	colorConers(); 
	calculateStaffXY();
	
	//recognizeText();

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