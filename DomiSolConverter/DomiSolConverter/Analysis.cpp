#include "pch.h"
#include "DomiSolConverter.h"


// ���� �� ��(5���� ��)�� ����
void DomiSolConverter::Analysis::calculateStaffHeight(){
	int sum = 0;
	for (int i = 1; i < staffXY.size(); i+=2 ) {
		sum += staffXY[i].y - staffXY[i - 1].y;
	}
	if (staffXY.size() != 0) {
		this -> staffHeight = sum / (staffXY.size() / 2);
	}
	else {
		cout << "������ ������ 0�� �Դϴ�"<< endl;
	}
}

// ������ �ִ� �� ���� ����
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

	//* Y�� ������׷� �׸��� *//

	// �Ǻ� �Ѹ���(20%)�� �Ǹ� �����̶� �Ǵ�

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
		cout <<  "�ǽ� " << nr <<" �� " <<  colCnt << endl;
		}
		*/

		if (colCnt >= oneBar) {

			Y.push_back(nr);
			X.push_back(tempX.front());
			X.push_back(tempX.back());
		}

		// Y�� ������׷� �ϼ�
		for (int nc = 0; nc < colCnt; nc++) {
			pixel[nc] = pixel[nc] / 64 * 64 + 128 / 2;
		}

	}

	// �Ǻ� ���� ����

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

	// �ּ�3�� �Ǻ� ���ݺ��� 2�� �̻� Ŀ����
	// ���� �������� �Ǵ�
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
			// ��� ���ϱ�
			diffAvg = diff / spaceCnt;
			// ����� 200%���� ���̰� ũ�� �ʴٸ� �߰�
			if (diffAvg * 2.5 >= Y[i] - Y[i - 1]) {
				diff += Y[i] - Y[i - 1];
				spaceCnt++;

			}
			// ũ�� �׸��� ���� �װ� 3�� �̻��̸� �� �������� staff �ϳ���� �����ϱ�!
			else {

				//cout << "���� ��� " << diffAvg << endl;
				//cout << "���� " << spaceCnt << endl;

				if (spaceCnt >= 3) {
					lineCnt++;
					//cout << lineCnt << "�� ° �����̳׿� ù��° �� y�� " << Y[i - 1 - spaceCnt] << endl;
					//cout << lineCnt << "�� ° �����̳׿� ������ �� y�� " << Y[i] << endl;

					this->staffXY.push_back(Point(X[(i - 1 - spaceCnt) * 2], Y[i - 1 - spaceCnt]));
					this->staffXY.push_back(Point(X[(i - 1) * 2 + 1], Y[i - 1]));

				}
				else {
					//cout << "��� ������ �ƴϿ���" << endl;

				}
				spaceCnt = 0;
				diff = 0;
				diffAvg = 0;

			}

			// �迭�� ������ ������ staff üũ
			if (i == Y.size() - 1 && spaceCnt >= 3) {
				lineCnt++;
				//cout << lineCnt << "�� ° �����̳׿�" << endl;
				this->staffXY.push_back(Point(X[(i - spaceCnt) * 2], Y[i - spaceCnt]));
				this->staffXY.push_back(Point(X[i * 2 + 1], Y[i]));
			}

		}


	}
	
	// ������ ��� ���� ���
	calculateStaffHeight();

	//* ���� ���� ��� �׽�Ʈ ����Ʈ *//
	/*
	for (int i = 0; i < staffXY.size(); i++) {
		cout << "( " << this->staffXY[i].x << " , " << this->staffXY[i].y << " )" << endl;
	}
	*/

	//* ���� ROI �׽�Ʈ ����Ʈ *//
	
	/*
	Rect rect[10]; // (����) ���� �Ҵ��ϱ�
	for (int i = 0; i < staffXY.size()/2; i++) {
		rect[i] = Rect(0, staffXY[i].y, width, this->staffHeight); // (����)��� ���� ũ�� �ֱ�
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
	
	// (����)DomiSolConverter�� straitenedImg�� �ٲټ���
	string INPUTPATH = "./inputImage/straightenedImg.jpg";
	string OUTPUTPATH = "./outputImage/textpart";

	Mat src = imread(INPUTPATH, IMREAD_GRAYSCALE);
	int width = src.cols;
	int height = src.rows;

	// ���� �ƴ� �κ� �߶� �����ϱ�
	
	Rect ROI;
	Mat tempImg;

	// 1. �� ������ ���κ� crop
	for (int i = 0; i < staffXY.size(); i++) {
		// ó��
		if (i == 0) {
			ROI = Rect(0, 0, width, staffXY[i].y);
			Mat subImg = Mat(src, ROI);
			imwrite(OUTPUTPATH + to_string(i) + ".jpg", subImg);
		}
		// ������
		else if (i == staffXY.size() - 1) {
			ROI = Rect(0, staffXY[i].y, width, this->staffHeight * 2);
			Mat subImg = Mat(src, ROI);
			imwrite(OUTPUTPATH + to_string(i) + ".jpg", subImg);
		}
		// �߰�
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

// ���� ������ �̹����� ������ �𼭸� �κ��� �Ͼ������ �ٲٱ�
void DomiSolConverter::Analysis::colorConers() {

	Mat input = this->straightenedImg;
	int width = input.cols;
	int height = input.rows;
	int bar = (int)((width / 100.0) * 20);

	// ���� �� ���� �𼭸��� ���ؼ� Run Length Coding ����
	// �Ͼ������ ĥ�ϱ�
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

	// ��� //
	
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