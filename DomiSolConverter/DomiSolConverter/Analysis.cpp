#include "pch.h"
#include "DomiSolConverter.h"

DomiSolConverter::Analysis::Analysis(Mat objectsImg, vector<Rect> objectXY) {
	this->objectsImg = objectsImg;
	this->objectXY = objectXY;

	calculateStaffXY();
	recognizeText();
	extractFeature();
	//calculatePitch();
}

// 오선 한 개(5개의 줄)의 높이
void DomiSolConverter::Analysis::calculateStaffHeight() {
	int sum = 0;
	for (int i = 1; i < staffXY.size(); i += 2) {
		sum += staffXY[i].y - staffXY[i - 1].y;
	}
	if (staffXY.size() != 0) {
		this->staffHeight = sum / (staffXY.size() / 2);
	}
	else {
		cout << "오선의 개수가 0개 입니다" << endl;
	}
}

// 오선에 있는 줄 사이 간격
void DomiSolConverter::Analysis::calculateStaffSpace() {
	this->staffSpace = (float)this->staffHeight / 5;
}

void DomiSolConverter::Analysis::calculateStaffXY() {

	// (수정)DomiSolConverter의 straitenedImg로 바꾸세요
	string INPUTPATH = "./inputImage/staffLine.jpg";

	Mat src = imread(INPUTPATH, IMREAD_GRAYSCALE);

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

			if (pixel[nc] > 230) {
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
	calculateStaffSpace();

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
	*/
}

void DomiSolConverter::Analysis::extractFeature() {

	Mat objectsRectImg = objectsImg;

	cvtColor(objectsRectImg, objectsRectImg, COLOR_GRAY2RGB);
	for (int index = 0; index < objectXY.size(); index++) {

		Mat object = objectsImg(objectXY[index]);
		int width = object.cols;
		int height = object.rows;
		staffHeight = 34;
		if (height > staffHeight*0.9 && height < staffHeight * 1.2) {

			//rectangle(objectsRectImg, objectXY[index].tl(), objectXY[index].br(), Scalar(0, 255, 255), 1);

			bool isLine = 0;

			/*
			** Hough transform line detect
			*/

			Mat objectEdge;
			int scale = 1;
			int delta = 0;
			int ddepth = CV_16S;

			Mat grad_x, grad_y;
			Mat abs_grad_x, abs_grad_y;


			// reduce the noise (kernel size=3)
			Mat blurredImg;
			GaussianBlur(object, object, Size(3, 3), 0, 0, BORDER_DEFAULT);


			// calculate derivatives in x and y directions
			Sobel(object, grad_x, ddepth, 1, 0, 3, scale, delta, BORDER_DEFAULT);
			Sobel(object, grad_y, ddepth, 0, 1, 3, scale, delta, BORDER_DEFAULT);
			//Scharr(blurredImg, grad_x, ddepth, 1, 0, scale, delta, BORDER_DEFAULT);
			//Scharr(blurredImg, grad_y, ddepth, 0, 1, scale, delta, BORDER_DEFAULT);

			// convert results back to CV_8U
			convertScaleAbs(grad_x, abs_grad_x);
			convertScaleAbs(grad_y, abs_grad_y);

			// add sobel_x & sobel_y
			addWeighted(abs_grad_x, 0.5, abs_grad_y, 0.5, 0, objectEdge);

			//imshow("edgeImg", objectEdge);

			Mat blank(object.rows, object.cols, CV_8UC1, Scalar(0));
			Mat lineImg = blank.clone();

			vector<Vec4i> lines;
			//cout << height << endl;
			HoughLinesP(objectEdge, lines, 1, CV_PI / 180, 1, height*0.55, 1);
			if (!lines.empty()) {
				isLine = 1;
				//cout << "line is detected" << endl;
				//cout << object << endl;
				//imwrite("outputImage/lineObjects/" + to_string(index) + ".jpg", object);
				//cout << object << endl;
				/*
				** 라인이 검출되면 histogram으로 음표인지 판단
				*/

				vector<int> Xhist(width, 0);
				vector<int> Yhist(height, 0);
				/*
				// X histogram
				for (int nr = 0; nr < height; nr++) {
					uchar* pixel = object.ptr<uchar>(nr); // n번째 row에 대한 주소를 저장

					for (int nc = 0; nc < width; nc++) {
						//cout << int(pixel[nc]) << endl;
						if (pixel[nc] != 0) {
							Xhist[nc]++;
						}
					}
				}

				for (int nc = 0; nc < width; nc++) {

					cout << Xhist[nc] << "  ";
					line(histogram, Point(nc, height), Point(nc, (height - Xhist[nc])), (0), 1);
				}
				*/
				// Y histogram
				int pixelCnt = 0;
				for (int nr = 0; nr < height; nr++) {
					uchar* pixel = object.ptr<uchar>(nr); // n번째 row에 대한 주소를 저장

					for (int nc = 0; nc < width; nc++) {
						//cout << int(pixel[nc]) << endl;
						if (pixel[nc] != 0) {
							Yhist[nr]++;
							pixelCnt++;
						}
					}
				}
				if (pixelCnt < height * width * 0.95) {
					noteXY.push_back(objectXY[index]);
					rectangle(objectsRectImg, objectXY[index].tl(), objectXY[index].br(), Scalar(0, 255, 255), 1);
				}
				/*
				for (int nr = 0; nr < height; nr++) {
					cout << Yhist[nr] << "  ";
				}
				*/
				//cout << endl;
				//cout << object << endl;

			}
			//imwrite("outputImage/objects/" + to_string(index) + ".jpg", object);
		}

	}

	imwrite("outputImage/objects.jpg", objectsRectImg);


}

void DomiSolConverter::Analysis::calculatePitch() {
	vector<Rect>::iterator note = noteXY.begin();
	Mat testImg = this->objectsImg.clone();

	for (note; note != noteXY.end(); note++) {
		Rect upHead((*note).x, (*note).y, (*note).width, this->staffSpace);
		Rect downHead((*note).x, (*note).y + (*note).height - this->staffSpace, (*note).width, this->staffSpace);

		Mat up = this->objectsImg(upHead);
		Mat down = this->objectsImg(downHead);

		Rect head;

		int upCnt = 0;
		int downCnt = 0;

		// For test
		this->staffHeight = 34;
		this->staffSpace = (float)34 / (float)4;
		vector<int> test;
		test.push_back(136);
		test.push_back(135 + staffHeight);
		test.push_back(240);
		test.push_back(240 + 33);
		test.push_back(344);
		test.push_back(344 + 33);
		test.push_back(447);
		test.push_back(447 + 33);
		vector<int>::iterator staff = test.begin();

		for (int row = -1; row < 2; row++) {
			for (int pixel = 0; pixel < (*note).width; pixel++) {
				int upVal = (int)(up.at<uchar>((upHead.height / 2) + row, pixel));
				int downVal = (int)(down.at<uchar>((downHead.height / 2) + row, pixel));
				if (upVal == 255) {
					upCnt++;
				}
				if (downVal == 255) {
					downCnt++;
				}
			}
		}
		if (upCnt > downCnt) {
			head = upHead;
		}

		else {
			head = downHead;
		}

		float staff_start = 0;
		float staff_end = 0;
		for (staff; staff != test.end(); staff++) {
			putText(testImg, to_string((*staff)), Point(30, (*staff)), FONT_HERSHEY_PLAIN, 1.0, Scalar(255, 255, 255), 1);
			int index = distance(test.begin(), staff);
			if (index % 2 == 0) {
				staff_start = (*staff);
			}
			else {
				if (index == 2) {
					staffSpace = (float)33 / (float)4;
				}
				staff_end = (*staff);

				float headY = head.y + ((float)head.height / (float)2);
				line(this->objectsImg, Point(100, staff_start), Point(700, staff_start), Scalar(255, 255, 255));
				line(this->objectsImg, Point(100, staff_start + staffSpace), Point(700, staff_start + staffSpace), Scalar(255, 255, 255));
				line(this->objectsImg, Point(100, staff_start + staffSpace * 2), Point(700, staff_start + staffSpace * 2), Scalar(255, 255, 255));
				line(this->objectsImg, Point(100, staff_start + staffSpace * 3), Point(700, staff_start + staffSpace * 3), Scalar(255, 255, 255));
				line(this->objectsImg, Point(100, staff_end), Point(700, staff_end), Scalar(255, 255, 255));

				if (headY >= staff_start - (3 * this->staffSpace) && headY <= staff_end + (3 * this->staffSpace)) {
					char tone = 'E';
					float curStaff = staff_end + (3 * this->staffSpace);
					//const int threshold = (staffSpace / 3);
					const int threshold = 3.7;
					for (curStaff; curStaff >= staff_start - (3 * this->staffSpace); curStaff -= staffSpace) {
						tone++;
						if (tone == 'H') {
							tone = 'A';
						}
						if (headY < curStaff + threshold && headY > curStaff - threshold) {
							break;
						}
						else if (headY < curStaff + staffSpace - threshold && headY > curStaff + threshold) {
							tone++;
							if (tone == 'H') {
								tone = 'A';
							}
							break;
						}
						tone++;
						if (tone == 'H') {
							tone = 'A';
						}
					}

					putText(testImg, to_string(int(headY)), Point((*note).x + ((*note).width / 2), (*note).y + (*note).height + 10), FONT_HERSHEY_PLAIN, 1.0, Scalar(255, 255, 255), 1);
					putText(this->objectsImg, string(1, tone), Point((*note).x + ((*note).width / 2), (*note).y + (*note).height + 10), FONT_HERSHEY_PLAIN, 1.0, Scalar(255, 255, 255), 1);
				}
			}
		}
	}
	imshow("XY", testImg);
	imshow("Tone", this->objectsImg);
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
			ROI = Rect(0, staffXY[i].y, width, staffXY[i + 1].y - staffXY[i].y);
			Mat subImg = Mat(src, ROI);
			imwrite(OUTPUTPATH + to_string(i) + ".jpg", subImg);
			i++;
		}

	}



}

void DomiSolConverter::Analysis::recognizeNoteSymbol() {

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