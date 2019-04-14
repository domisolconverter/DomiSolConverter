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
	
	// (����)DomiSolConverter�� straitenedImg�� �ٲټ���
	string INPUTPATH = "./inputImage/staffLine.jpg";

	Mat src = imread(INPUTPATH, IMREAD_GRAYSCALE);

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

			if (pixel[nc]>230) {
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
	*/

	namedWindow("OUT01", CV_WINDOW_AUTOSIZE);
	imshow("OUT01", src);

	destroyWindow("OUT01");

}

void DomiSolConverter::Analysis::extractFeature() {

}

void DomiSolConverter::Analysis::calculatePitch() {

}

void DomiSolConverter::Analysis::recognizeObject() {

}

void DomiSolConverter::Analysis::recognizeGeneralSymbol() {

}

void DomiSolConverter::Analysis::cropTextArea() {
	// (����)recognizeText�� ROI��� �ٲٱ�
	string INPUTPATH = "./outputImage/textpart4.jpg";
	Mat src = imread(INPUTPATH, IMREAD_GRAYSCALE);
	Mat binaryImg;
	Mat opened;
	Mat closed;
	Mat result;
	int width = src.cols;
	int height = src.rows;
	
	/* ������Ʈ ����� */
	// OTSU ����ȭ
    threshold(src, binaryImg, 0, 255, THRESH_BINARY | THRESH_OTSU);
	Mat element(width*0.012, width*0.012, CV_8U, Scalar(1)); // ���� ũ�� �߿�
	// ���� ����
	morphologyEx(binaryImg, opened, MORPH_OPEN, element);
	// ���� ����
	morphologyEx(opened, closed, MORPH_CLOSE, element);
	// ħ�� ����
	erode(closed, result, Mat());
	namedWindow("Component Image");
	imshow("Component Image",result);

	/* ������Ʈ�� �ܰ��� ���� */

	vector<vector<Point>> contours;
	findContours(result, contours, CV_RETR_LIST, CV_CHAIN_APPROX_NONE);
	Mat contoursResult(result.size(), CV_8U, Scalar(255));
	drawContours(contoursResult, contours, -1, Scalar(150), 2);
	namedWindow("Draw Contour Image");
	imshow("Draw Contour Image", contoursResult);

	int cmax = width;
	vector<vector<Point>>::const_iterator it = contours.begin();
	/* �ܰ��� �� �Ǻ� �ܰ����� ���� */
	while (it != contours.end()) {
		if (it->size() > cmax) 
			it = contours.erase(it);
		else
			++it;
	}
	
	drawContours(src, contours, -1, Scalar(100,255,0), 2);
	namedWindow("Only Text Component");
	imshow("Only Text Component", src);

	/* ���� ���� �κ� ū �׸�� �ؼ� �ڸ��� */



	/* ���η� �� ������ �ܰ��� Rect�� ���� */



}

void DomiSolConverter::Analysis::recognizeText() {
	
	// (����)DomiSolConverter�� straitenedImg�� �ٲټ���
	string INPUTPATH = "./inputImage/straightenedImg.jpg";
	string OUTPUTPATH = "./outputImage/textpart";

	Mat src = imread(INPUTPATH, IMREAD_GRAYSCALE);
	int width = src.cols;
	int height = src.rows;

	// ���� �ƴ� �κ� �߶� �����ϱ�
	
	int ROISize = staffXY.size() - (staffXY.size() / 2 - 1);
	Mat tempImg;
	Rect *ROI = (Rect*)malloc(sizeof(Rect) * ROISize);

	// 1. �� ������ ���κ� crop
	for (int i = 0; i < ROISize; i++) {
		// ó��
		if (i == 0) {
			ROI[i] = Rect(0, 0, width, staffXY[i].y);
			Mat subImg = Mat(src, ROI[i]);
			imwrite(OUTPUTPATH + to_string(i) + ".jpg", subImg);
		}
		// ������
		else if (i == ROISize - 1) {
			ROI[i] = Rect(0, staffXY[2 * i - 1].y, width, this->staffHeight * 2);
			Mat subImg = Mat(src, ROI[i]);
			imwrite(OUTPUTPATH + to_string(i) + ".jpg", subImg);
		}
		// �߰�
		else {
			ROI[i] = Rect(0, staffXY[2*i-1].y, width, staffXY[2 * i].y - staffXY[2 * i - 1].y);
			Mat subImg = Mat(src, ROI[i]);
			imwrite(OUTPUTPATH + to_string(i) + ".jpg", subImg);
		}

	}

	// ���ڰ� �����ϴ� �κ� : crop
	
	// ħ�� ���� ����

	/* ħ�� ���� */
	/*
	Mat erored;
	for (int i = 0; i < ROISize; i++) {
		erode(Mat(src, ROI[i]), erored, Mat());
		imwrite(OUTPUTPATH + "_eroded" + to_string(i) + ".jpg", erored);
	}
	*/

	/* OPENING ����  */
	/*
	Mat element(3, 3, CV_8U, Scalar(1)); // 3X3 ¥�� ����
	Mat opened;
	for (int i = 0; i < ROISize; i++) {
		morphologyEx(Mat(src, ROI[i]), opened, MORPH_OPEN, element);
		imwrite(OUTPUTPATH + "_opened" + to_string(i) + ".jpg", opened);
	}
	*/

	/* CLOSED ����  */
	/*
	Mat element(3, 3, CV_8U, Scalar(1)); // 7X7 ¥�� ����
	Mat closed;
	for (int i = 0; i < ROISize; i++) {
		morphologyEx(Mat(src, ROI[i]), closed, MORPH_CLOSE, element);
		imwrite(OUTPUTPATH + "_closed" + to_string(i) + ".jpg", closed);
	}

	*/

	/* ħ�� ���� ver2 */
	/*
	Mat opened;
	Mat element(2, 2, CV_8U, Scalar(1)); // 7X7 ¥�� ����
	for (int i = 0; i < ROISize; i++) {
	erode(Mat(src, ROI[i]), opened, element);
	imwrite(OUTPUTPATH + "_eroded_ver2_" + to_string(i) + ".jpg", opened);
	}
	*/
	

	/* ��â ����  */
	/*
	Mat dilated;
	for (int i = 0; i < ROISize; i++) {
		dilate(Mat(src, ROI[i]), dilated, Mat());
		imwrite(OUTPUTPATH + "_dilated" + to_string(i) + ".jpg", dilated);
	}
	*/

	/*
	Mat eroded;
	erode(Mat(src,ROI[3]), eroded, Mat());
	namedWindow("Eroded Image1");
	imshow("Eroded Image1", eroded);
	imwrite(OUTPUTPATH + "_e1" + ".jpg", eroded);
	
	Mat dilated;
	dilate(eroded, dilated, Mat());
	namedWindow("Dilated Image");
	imshow("Dilated Image", dilated);
	imwrite(OUTPUTPATH + "_d1" + ".jpg", dilated);

	Mat tophat;
	morphologyEx(Mat(src, ROI[3]), tophat, NULL, MORPH_BLACKHAT);
	namedWindow("Tophat Image");
	imshow("Tophat Image", tophat);
	imwrite(OUTPUTPATH + "_t1" + ".jpg", tophat);
	*/
}

void DomiSolConverter::Analysis::recognizeNoteSymbol() {
	
}


DomiSolConverter::Analysis::Analysis() {
	
	calculateStaffXY();
	cropTextArea();
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