#include "pch.h"
#include "DomiSolConverter.h"


int DomiSolConverter::Analysis::show(Mat img, string title) {
	// cout << "OpenCV Version : " << CV_VERSION << endl;
	namedWindow(title, CV_WINDOW_AUTOSIZE);

	if (img.empty())
	{
		cout << "There's no sheet file!" << endl;
		return -1;
	}

	imshow(title, img);
}

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


	Mat src = this->inputCalculateStaffImg.clone();

	int width = src.cols;
	int height = src.rows;
	int oneBar = (int)((width / 100.0) * 10);    
	int missingLine = (int)((width / 100.0) * 2);
	int colCnt = 0;
	int staffNum = 0;

	vector<int> Y;
	vector<int> X;

	//* Y�� ������׷� �׸��� *//

	// �Ͼ����� (10%)�� �ǰ�
	// ������ ������
	// �����̶� �Ǵ�

	// cout << missingLine << endl;

	for (int nr = 0; nr < height; nr++) {


		uchar* pixel = src.ptr<uchar>(nr);
		list<int> tempX;
		colCnt = 0;
		int space = 0;

		// Run Length Coding
		// �Ͼ����� ������� Ȯ��
		for (int nc = 0; nc < width; nc++) {

			if (pixel[nc]==255) {
				if (tempX.size() != 0) {
					space = nc - tempX.back();
					if (space < missingLine) {
						tempX.push_back(nc);
						colCnt++;
					}
					else {
						break;
					}
				}
				else {
					tempX.push_back(nc);
					colCnt++;
				}
			}
		}
				
		// �� ���� �̻�����
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

	// * ���κ��� * //
	for (int i = 1; i < Y.size();) {
		if (Y[i] - Y[i - 1] <= 2) {
			Y.erase(Y.begin() + i);
			X.erase(X.begin() + 2 * i, X.begin() + 2 * i + 2);
		}
		else {
			i++;
		}
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
		
	for (int i = 0; i < staffXY.size(); i++) {
		cout << "( " << this->staffXY[i].x << " , " << this->staffXY[i].y << " )" << endl;
	}
	
	
	//* ���� ROI �׽�Ʈ ����Ʈ *//
	/*

	Rect ROI;

	if (staffXY.size() >= 2) {
		for (int i = 0; i < staffXY.size(); i+=2) {
			// ó��
			ROI = Rect(staffXY[i].x, staffXY[i].y, width - staffXY[i].x, staffXY[i + 1].y - staffXY[i].y);
			Mat subImg = src(ROI);
			show(subImg, to_string(i/2) + "�� ° ����");

		}
	}
	*/
	// show(src, "�����ν��� �̹���");
	
}

void DomiSolConverter::Analysis::extractFeature() {
	//Draw result
	Mat objectsRectImg = objectsImg;
	//Scalar color(0, 0, 0);
	Scalar color(255, 255, 255);
	for (int i = 0; i < objectXY.size(); i++) {
		rectangle(objectsRectImg, objectXY[i].tl(), objectXY[i].br(), color, 1);
	}
	// namedWindow("objects", CV_WINDOW_AUTOSIZE);
	// imshow("objects", objectsRectImg);
}

void DomiSolConverter::Analysis::calculatePitch() {

}

void DomiSolConverter::Analysis::recognizeObject() {

}

void DomiSolConverter::Analysis::recognizeGeneralSymbol() {

}

void DomiSolConverter::Analysis::cropTextArea(Rect *ROI) {


	Mat input = this->straightenedImg.clone();
	Mat binaryImg;
	Mat opened;
	Mat closed;
	Mat result;
	int width = input.cols;
	int height = input.rows;
	int ROISize = staffXY.size() - (staffXY.size() / 2 - 1);

	for (int i = 0; i < ROISize; i++) {

		Mat src = Mat(input, ROI[i]);
		
		/* ������Ʈ ����� */
		// OTSU ����ȭ
		threshold(src, result, 0, 255, THRESH_BINARY | THRESH_OTSU);
		Mat element(width*0.012, height*0.012, CV_8U, Scalar(1)); // ���� ũ�� �߿�
		// ���� ����
		morphologyEx(result, result, MORPH_OPEN, element);
		// ���� ����
		morphologyEx(result, result, MORPH_CLOSE, element);
		// ��â ����
		//dilate(result, result, Mat());
		// ħ�� ����
		erode(result, result, Mat());
		//namedWindow("Component Image" + to_string(i));
		//imshow("Component Image" + to_string(i), result);

		/* ������Ʈ�� �ܰ��� ���� */

		vector<vector<Point>> contours;
		findContours(result, contours, CV_RETR_LIST, CV_CHAIN_APPROX_NONE);
		Mat contoursResult(result.size(), CV_8U, Scalar(255));
		//drawContours(contoursResult, contours, -1, Scalar(150), 2);
		//namedWindow("Draw Contour Image" + to_string(i));
		//imshow("Draw Contour Image" + to_string(i), contoursResult);


		

		int cmax = width;
		vector<vector<Point>>::const_iterator it = contours.begin();

		/* �ܰ��� �� �Ǻ� �ܰ����� ���� */
		while (it != contours.end()) {
			if (it->size() > cmax)
				it = contours.erase(it);
			else
				++it;
		}

		//drawContours(src, contours, -1, Scalar(100, 255, 0), 2);
		//namedWindow("Only Text Component" + to_string(i));
		//imshow("Only Text Component" + to_string(i), src);

		/* ���� ���� �κ� ū ��ǥ ���ϱ� */
		
		it = contours.begin();
		int xmin = width;
		int ymin = height;
		int xmax = 0;
		int ymax = 0;

		for (int i = 0; i < contours.size(); i++) {
			for (int j = 0; j < it->size(); j++) {
				if (contours[i][j].x < xmin) {
					xmin = contours[i][j].x;
				}
				if (contours[i][j].y < ymin) {
					ymin = contours[i][j].y;
				}
				if (contours[i][j].x > xmax) {
					xmax = contours[i][j].x;
				}
				if (contours[i][j].y > ymax) {
					ymax = contours[i][j].y;
				}
			}
			++it;
		}

		xmin = xmin*0.9;
		xmax = xmax*1.1;
		ymin = ymin*0.9;
		ymax = ymax*1.1;
		
		// ������ �̹����� �Ѿ������ ���
		if (xmax > src.cols) {
			xmax = src.cols;

		}
		if (ymax > src.rows) {
			ymax = src.rows;

		}

		// �ؽ�Ʈ ������ �������� ���� ���
		if (xmin == width || xmax == 0 || ymin == height || ymax == 0) {
			xmin = 0;
			xmax = src.cols;
			ymin = 0;
			ymax = src.rows;
		}
		
		ROI[i] = Rect(xmin, ymin, xmax - xmin, ymax - ymin);
		Mat subImg = Mat(src, ROI[i]);		
		//imwrite("./outputImage/Onlytextpart" + to_string(i) + ".jpg", subImg);
		//namedWindow("Onlytextpart" + to_string(i), WINDOW_AUTOSIZE);
		//imshow("Onlytextpart" + to_string(i), subImg);
		
	}
	

}

void DomiSolConverter::Analysis::recognizeText() {

	Mat src = this->straightenedBinaryImg;
	int width = src.cols;
	int height = src.rows;

	// ���� �ƴ� �κ� �߶� �����ϱ�
	
	int ROISize = staffXY.size() - (staffXY.size() / 2 - 1);
	// �뷫���� �ؽ�Ʈ ���� ROI �迭 �����Ҵ�
	Rect *ROI = (Rect*)malloc(sizeof(Rect) * ROISize);

	// 1. �� ������ ���κ� crop
	for (int i = 0; i < ROISize; i++) {
		// ó��
		if (i == 0) {
			ROI[i] = Rect(0, 0, width, staffXY[i].y);
		}
		// ������
		else if (i == ROISize - 1) {
			ROI[i] = Rect(0, staffXY[2 * i - 1].y, width, height - staffXY[2*i-1].y);
		}
		// �߰�
		else {
			ROI[i] = Rect(0, staffXY[2 * i - 1].y, width, staffXY[2 * i].y - staffXY[2 * i - 1].y);
		}

	}

	// ���ڰ� �����ϴ� �κ� �ڸ� �� ħ�� ���� ����
	
	 cropTextArea(ROI);

}

void DomiSolConverter::Analysis::recognizeNoteSymbol() {
	
}

// ���� ������ �̹����� ������ �𼭸� �κ��� �Ͼ������ �ٲٱ�
void DomiSolConverter::Analysis::colorConers() {

	Mat input = this->straightenedBinaryImg;
	int width = input.cols;
	int height = input.rows;
	int bar = (int)((width / 100.0) * 20);

	// ���� �� ���� �𼭸��� ���ؼ� Run Length Coding ����
	// ���������� ĥ�ϱ�
	
	for (int nr = 0; nr < height; nr++) {

		uchar* pixel = input.ptr<uchar>(nr);

		for (int nc = 0; nc < width; nc++) {
			if (pixel[nc] == 255) {
				pixel[nc] = 0;
			}
			else {
				break;
			}
		}

	}

	for (int nr = 0; nr < height; nr++) {

		uchar* pixel = input.ptr<uchar>(nr);

		for (int nc = width-1; nc >= 0; nc--) {
			if (pixel[nc] == 255) {
				pixel[nc] = 0;
			}
			else {
				break;
			}
		}

	}
	
	this->inputCalculateStaffImg = input.clone();

}


DomiSolConverter::Analysis::Analysis(Mat straightenedImg, Mat straightenedBinaryImg, Mat objectsImg, vector<Rect> objectXY) {

	// ���� ����
	this->straightenedImg = straightenedImg;
	this->straightenedBinaryImg = straightenedBinaryImg;
	colorConers();
	//show(this->inputCalculateStaffImg, "�����ν��� �̹���");
	calculateStaffXY();

	// ���� �ν�
	recognizeText();

	this->objectsImg = objectsImg;
	this->objectXY = objectXY;
	extractFeature();
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

void DomiSolConverter::Analysis::setObjectsImg(Mat objectsImg) {
	this->objectsImg = objectsImg;
}

void DomiSolConverter::Analysis::setObjectXY(vector<Rect> objectXY) {
	this->objectXY = objectXY;
}