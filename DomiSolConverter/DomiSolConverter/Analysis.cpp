#include "pch.h"
#include "DomiSolConverter.h"
#include <Windows.h>

DomiSolConverter::Analysis::Analysis(Mat straightenedImg, Mat straightenedBinaryImgforStaff, Mat straightenedBinaryImgforObject, vector<Note> note, vector<NonNote> nonNote, vector<string> text) {
	
	this->nonNoteInfo = nonNote;
	this->noteInfo = note;
	this->straightenedImg = straightenedImg;
	this->straightenedBinaryImgforStaff = straightenedBinaryImgforStaff;
	this->straightenedBinaryImgforObject = straightenedBinaryImgforObject;

	//show(straightenedBinaryImg, "binaryImg");
	colorConers();
	calculateStaffXY();
	calculateStaffSpace();

	cout << staffHeight << endl;
	//extractStaff();
	removeStaff();
	extractObject();

	classifyNote();
	calculatePitch();
	recognizeText();
	storeNonNote();
	recognizeGeneralSymbol();
	recognizeNoteSymbol();
}

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

void DomiSolConverter::Analysis::extractStaff() {

	staffImg = straightenedBinaryImgforObject.clone();
	int horizontalsize = staffImg.cols / 50;
	Mat horizontalStructure = getStructuringElement(MORPH_RECT, Size(horizontalsize, 1));
	erode(staffImg, staffImg, horizontalStructure, Point(-1, -1));
	dilate(staffImg, staffImg, horizontalStructure, Point(-1, -1));
}

void DomiSolConverter::Analysis::removeStaff() {

	objectsImg = straightenedBinaryImgforObject.clone();
	//int verticalsize = objectsImg.rows / 250;
	int verticalsize = staffHeight / 8;
	Mat verticalStructure = getStructuringElement(MORPH_RECT, Size(1, verticalsize));
	erode(objectsImg, objectsImg, verticalStructure, Point(-1, -1));
	dilate(objectsImg, objectsImg, verticalStructure, Point(-1, -1));
}

void DomiSolConverter::Analysis::extractObject() {
	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;

	// morphology
	Mat closingStructure = getStructuringElement(MORPH_ELLIPSE, Size(2, 4));
	morphologyEx(objectsImg, objectsImg, MORPH_CLOSE, closingStructure);

	// find contours
	findContours(objectsImg, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));
	//vector<vector<Point>> contours_poly(contours.size());

	for (int i = 0; i < contours.size(); i++) {
		/*Contour Approximation
		   도형 외곽선을 더 적은 꼭지점수를 갖도록 바꿀때 ex) 찢겨진 사각형 복원
		approxPolyDP(Mat(contours[i]), contours_poly[i], 0, true);
		if (fabs(contourArea(Mat(contours_poly[i])) > 10)) {
			objectXY.push_back(boundingRect(Mat(contours_poly[i])));
		}
		*/
		if (fabs(contourArea(Mat(contours[i])) > 5)) {
			//boundRect[rectCnt] = boundingRect(Mat(contours[i]));
			objectXY.push_back(boundingRect(Mat(contours[i])));
		}
	}
	// sort object rectangles by x
	sort(objectXY.begin(), objectXY.end(), byX());
	for (int i = 0; i < objectXY.size(); i++) {
		//cout << "x: " << objectXY.at(i).x << "y: " << objectXY.at(i).y << "width: " << objectXY.at(i).width << "height: " << objectXY.at(i).height << endl;
	}

	int objectsCnt = objectXY.size();
	for (int i = 0; i < objectsCnt; i++) {
		//cout << i << "th   " << objectXY[i] << endl;
		int c = 1;
		while (1) {
			if (i + c > objectsCnt - 1) {
				break;
			}
			//cout << "c: " << c << "  " << objectXY[i+c] << endl;
			// i번째 사각형보다 i+c번째 사각형이 작을때
			if (objectXY[i].width > objectXY[i + c].width) {
				// i+c번째 사각형이 내포돼있을때
				if (!(objectXY[i].x > objectXY[i + c].x) &&
					!((objectXY[i].x + objectXY[i].width) < (objectXY[i + c].x + objectXY[i + c].width)) &&
					!(objectXY[i].y > objectXY[i + c].y) &&
					!((objectXY[i].y + objectXY[i].height) < (objectXY[i + c].y + objectXY[i + c].height))
					) {
					objectXY.erase(objectXY.begin() + i + c);
					objectsCnt--;
				}
			}
			// i번째 사각형보다 i+c번째 사각형이 클때
			else if (objectXY[i].width > objectXY[i + c].width) {
				// i번째 사각형이 내포돼있을때
				if (!(objectXY[i].x < objectXY[i + c].x) &&
					!((objectXY[i].x + objectXY[i].width) > (objectXY[i + c].x + objectXY[i + c].width)) &&
					!(objectXY[i].y < objectXY[i + c].y) &&
					!((objectXY[i].y + objectXY[i].height) > (objectXY[i + c].y + objectXY[i + c].height))
					) {
					objectXY.erase(objectXY.begin() + i);
					objectsCnt--;
				}
			}
			c++;
		}
	}

	//draw result
	Mat contourimg = Mat(objectsImg.rows, objectsImg.cols, CV_8U);
	//scalar color(0, 0, 0);
	Scalar color(255, 255, 255);
	for (int i = 0; i < contours.size(); i++) {
		drawContours(contourimg, contours, i, color, 1, 8, vector<Vec4i>(), 0, Point());
	}
	for (int i = 0; i < objectXY.size(); i++) {
		//printf("%d", i);
		rectangle(contourimg, objectXY[i].tl(), objectXY[i].br(), color, 1);
		
		//putText(contourimg, to_string(i), objectXY[i].tl(), 0.3, 0.3, Scalar::all(255));
	}
	namedWindow("contours", CV_WINDOW_AUTOSIZE);
	imshow("contours", contourimg);

}


// 오선 한 개(5개의 줄)의 높이
void DomiSolConverter::Analysis::calculateStaffHeight(){

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

	Mat src = this->inputCalculateStaffImg.clone();

	int width = src.cols;
	int height = src.rows;
	int oneBar = (int)((width / 100.0) * 10);    
	int missingLine = (int)((width / 100.0) * 2);
	int colCnt = 0;
	int staffNum = 0;

	vector<int> Y;
	vector<int> X;

	//* Y축 히스토그램 그리기 *//

	// 하얀줄이 (10%)가 되고
	// 끊기지 않으면
	// 오선이라 판단

	// cout << missingLine << endl;

	for (int nr = 0; nr < height; nr++) {


		uchar* pixel = src.ptr<uchar>(nr);
		list<int> tempX;
		colCnt = 0;
		int space = 0;

		// Run Length Coding
		// 하얀줄이 끊겼는지 확인
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
				
		// 한 마디 이상인지
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

	// * 라인보정 * //
	for (int i = 1; i < Y.size();) {
		if (Y[i] - Y[i - 1] <= 2) {
			Y.erase(Y.begin() + i);
			X.erase(X.begin() + 2 * i, X.begin() + 2 * i + 2);
		}
		else {
			i++;
		}
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
		
	for (int i = 0; i < staffXY.size(); i++) {
		cout << "( " << this->staffXY[i].x << " , " << this->staffXY[i].y << " )" << endl;
	}
	
	
	//* 오선 ROI 테스트 프린트 *//
	/*

	Rect ROI;

	if (staffXY.size() >= 2) {
		for (int i = 0; i < staffXY.size(); i+=2) {
			// 처음
			ROI = Rect(staffXY[i].x, staffXY[i].y, width - staffXY[i].x, staffXY[i + 1].y - staffXY[i].y);
			Mat subImg = src(ROI);
			show(subImg, to_string(i/2) + "번 째 오선");

		}
	}
	*/
	// show(src, "오선인식한 이미지");
}

void DomiSolConverter::Analysis::classifyNote() {

	Mat objectsRectImg = objectsImg;

	/*
	* 여러 음표가 붙은 경우 잘라서 각각의 object저장
	*/
	int staffThickness;
	if (staffHeight > 30) staffThickness = 2;
	else staffThickness = 1;
	int staffInterval = (staffHeight + staffThickness * 3) / 4;
	//cout << objectXY.size() << endl;

	int objectCnt = objectXY.size();
	for (int i = 0; i < objectCnt; i++) {
		Mat object = objectsImg(objectXY[i]);
		int width = object.cols;
		int height = object.rows;

		//if (height < staffHeight*1.2) {

			// 여러 음표가 붙은 경우인가
			if ((width / staffInterval) > 2) {
				Rect tmp = objectXY[i];
				Point tl = tmp.tl();
				Point br = tmp.br();
				//cout << tl << br << endl;
				objectXY.erase(objectXY.begin() + i); // 붙은 음표는 벡터에서 제거한다.
				int num = width / (staffInterval*1.5); // 이어진 음표가 몇개인지는 어떻게 계산하지?
				int splitWidth = width / num;
				int remainder = width % num;
				int addRemainder = remainder / num;
				int start = 0;
				int end = 0;
				//cout << "width: " << width << "num: " << num << endl;
				for (int n = 0; n < num; n++) {
					Rect splitRect;
					if (n != num-1) {
						end = start + splitWidth + addRemainder - 1;
						splitRect = Rect(Point(start, 0), Point(end, height));
						//cout << splitRect << endl;
						Mat splited = object(splitRect);
						splitRect = boundingRect(splited);
						objectXY.insert(objectXY.begin() + i + n, Rect(Point(tl.x + start, tl.y + splitRect.tl().y), Point(tl.x + end, tl.y + splitRect.br().y))); // 잘린 음표들을 벡터에 추가시킨다.
						start += splitWidth + addRemainder;
						//imwrite("outputImage/splits/" + to_string(i) + "_" + to_string(n) + ".jpg", splited(boundingRect(splited)));
					}
					else {
						end = start + splitWidth + (remainder - addRemainder*(num-1)) - 1;
						splitRect = Rect(Point(start, 0), Point(end, height));
						//cout << splitRect << endl;
						Mat splited = object(splitRect);
						splitRect = boundingRect(splited);
						objectXY.insert(objectXY.begin() + i + n, Rect(Point(tl.x + start, tl.y + splitRect.tl().y), Point(tl.x + end, tl.y + splitRect.br().y))); // 잘린 음표들을 벡터에 추가시킨다.
						start += splitWidth;
						//imwrite("outputImage/splits/" + to_string(i) + "_" + to_string(n) + ".jpg", splited(boundingRect(splited)));
					}
				}
				objectCnt = objectCnt + num - 1;
				//imwrite("outputImage/splits/" + to_string(i) + ".jpg", object);
			}
		//}
	}
	//cout << objectXY.size() << endl;


	/*
	* 추출된 오브젝트 중 음표 분류 & 머리와 꼬리 인식
	* TODO: 합칠때 vector<Note>에 flag, isEmptyHead 저장
	*/
	int headMax = staffInterval / 2 * staffInterval / 2 * 2.9;
	//cout << objectsImg(objectXY[90]) << endl;
	for (int index = 0; index < objectXY.size(); index++) {
		//rectangle(objectsRectImg, objectXY[index].tl(), objectXY[index].br(), Scalar(255, 255, 255), 1);
		Mat object = objectsImg(objectXY[index]);
		int width = object.cols;
		int height = object.rows;
		int flag = 0;
		bool isEmptyHead = false;
		bool isNonNote = true;
		bool isHeadUp;

		//cout << "width: " << width << "height: " << height << endl;
		// 오브젝트 길이&너비로 1차 선별
		if (height > staffHeight*0.9 && width>staffInterval*0.8) {

			// Y histogram
			vector<int> Yhist(height, 0);
			int pixelCnt = 0;
			int blackLeft = 0;
			int blackRight = 0;
			for (int nr = 0; nr < height; nr++) {
				uchar* pixel = object.ptr<uchar>(nr); // n번째 row에 대한 주소를 저장

				for (int nc = 0; nc < width / 2; nc++) {
					if (pixel[nc] != 0) {
						Yhist[nr]++;
						pixelCnt++;
						blackLeft++;
					}
				}
				for (int nc = ceil(double(width) / 2); nc < width; nc++) {
					if (pixel[nc] != 0) {
						Yhist[nr]++;
						pixelCnt++;
						blackRight++;
					}
				}
			}

			int middleCnt = 0;
			for (int y = height/5*2; y < height/5*3; y++) {
				middleCnt += Yhist[y];
			}

			if (middleCnt < width*(height/5)*0.4) { // 박자표, 높은음자리표 같은 것들 제외

				/* 가로로 반을 잘라서 위 아래 흑화소 분포를 비교해 음표 위치를 판단한다.*/
				bool headLocation = true; // true이면 음표머리 하단에 위치. false이면 음표머리 상단에 위치
				int blackUp = 0;
				int blackDown = 0;
				for (int y = 0; y < staffInterval; y++) {
					blackUp += Yhist[y];
				}
				for (int y = Yhist.size() - staffInterval; y < Yhist.size(); y++) {
					blackDown += Yhist[y];
				}
				isNonNote = false;

				if(blackLeft > blackRight) {
				//if (blackUp > blackDown) { // 음표 머리가 상단에 위치
					isHeadUp = true;
					// 흑화소 개수가 일정 범위 이상일 경우 머리로 판단. 
					// --> 타원의 넓이 공식 (2*pi*가로반지름*세로반지름). 이 값보다 크면 머리로 판단한다.
					//cout << staffInterval / 2 * staffInterval*1.25 / 2 * 3.14 << endl;
					if (blackUp > headMax) {
						isEmptyHead = false;
						cout << index << "th object: Up Head is full" << endl;
					}
					else {
						isEmptyHead = true;
						cout << index << "th object: Up Head is empty" << endl;
					}

					// 꼬리 있는지 인식
					int checkpoint;
					if (width > staffInterval * 1.25) {
						checkpoint = staffInterval / 3;
					}
					else {
						checkpoint = width / 3;
					}
					bool pre = false;
					for (int nr = height - 1; nr > height / 2; nr--) {
						uchar* pixel = object.ptr<uchar>(nr); // n번째 row에 대한 주소를 저장
						if (pre == false && pixel[checkpoint] != 0) {
							flag += 1;
							pre = true;
						}
						else if (pre == true && pixel[checkpoint] == 0) {
							pre = false;
						}
					}
					cout << index << "th object: flag: " << flag << endl;

					//imwrite("outputImage/objects/" + to_string(index) + ".jpg", object); // 음표 검출 결과 이미지 각각 저장
					//rectangle(objectsRectImg, objectXY[index].tl(), objectXY[index].br(), Scalar(255, 255, 255), 1);
				}
				else {
				//else if (blackUp < blackDown) { // 음표 머리가 하단에 위치
					isHeadUp = false;

					// 흑화소 개수가 일정 범위 이상일 경우 머리가 꽉 차있다. 
					// --> 타원의 넓이 공식 (2*pi*가로반지름*세로반지름). 이 값보다 크면 머리가 차있다.
					//cout << staffInterval / 2 * staffInterval*1.25 / 2 * 3.14 << endl;
					if (blackDown > headMax) {
						isEmptyHead = false;
						cout << index << "th object: Down Head is full" << endl;
					}
					else {
						isEmptyHead = true;
						cout << index << "th object: Down Head is empty" << endl;
					}

					// 꼬리 있는지 인식
					int checkpoint;
					if (width > staffInterval * 1.25) {
						checkpoint = width - staffInterval/3;
					}
					else {
						checkpoint = width / 3;
					}
					bool pre = false;
					for (int nr = 0; nr < height/2; nr++) {
						uchar* pixel = object.ptr<uchar>(nr); // n번째 row에 대한 주소를 저장
						if (pre == false && pixel[checkpoint] != 0) {
							flag += 1;
							pre = true;
						}
						else if (pre == true && pixel[checkpoint] == 0) {
							pre = false;
						}
					}
					cout << index << "th object: flag: " << flag << endl;
					//imwrite("outputImage/objects/" + to_string(index) + ".jpg", object); // 음표 검출 결과 이미지 각각 저장
					//rectangle(objectsRectImg, objectXY[index].tl(), objectXY[index].br(), Scalar(255, 255, 255), 1);
				}
			}

		}
		if (isNonNote) {
			nonNoteXY.push_back(objectXY[index]); //nonNoteXY에 비음표 Rect정보 추가
		}
		else {
			noteXY.push_back(objectXY[index]);	// noteXY에 음표 Rect정보 추가
			Note n = Note();
			n.setFlag_Head(flag, isEmptyHead);
			n.setisHeadUp(isHeadUp);
			n.x = objectXY[index].x + (objectXY[index].width / 2);
			n.y = objectXY[index].y + (objectXY[index].height / 2);
			this->noteInfo.push_back(n);
		}
		//putText(objectsRectImg, to_string(index), objectXY[index].tl(), 0.3, 0.3, Scalar::all(255));
	}
	imshow("objects", objectsRectImg);
	//imwrite("outputImage/objects.jpg", objectsRectImg);
}


void DomiSolConverter::Analysis::storeNonNote() {
	//Draw result
	Mat objectsRectImg = objectsImg.clone();
	//Scalar color(0, 0, 0);
	Scalar color(255, 255, 255);
	for (int i = 0; i < this->nonNoteXY.size(); i++) {
		rectangle(objectsRectImg, nonNoteXY[i].tl(), nonNoteXY[i].br(), color, 1);
		Mat extractOject = ~objectsRectImg(Rect(nonNoteXY[i]));
		// assume that it's work
		string fname = "symbols\\object" + to_string(i) + ".jpg";
		imwrite(fname, extractOject);
	}
	imshow("nonNote obj", objectsRectImg);
}

void DomiSolConverter::Analysis::calculatePitch() {
	vector<Rect>::iterator note = noteXY.begin();
	Mat toneImg = this->objectsImg.clone();

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
		this->staffSpace = (float)33 / (float)4;

		vector<Point>::iterator staff = staffXY.begin();
		int maxCnt = 0;

		float headY = 0;
		int headCnt = 0;

		for (int row = upHead.height / 4; row <= (upHead.height / 4) * 3; row++) {
			int upOneLine = 0;
			int downOneLine = 0;
			for (int pixel = 0; pixel < (*note).width; pixel++) {
				int upVal = (int)(up.at<uchar>(row, pixel));
				int downVal = (int)(down.at<uchar>(row, pixel));
				if (upVal == 255) {
					upOneLine++;
				}
				if (downVal == 255) {
					downOneLine++;
				}
			}
			upCnt += upOneLine;
			downCnt += downOneLine;

			if (maxCnt < upOneLine) {
				maxCnt = upOneLine;
			}

			if (maxCnt < downOneLine) {
				maxCnt = downOneLine;
			}
		}

		if (upCnt > downCnt) {
			head = upHead;
		}

		else {
			head = downHead;
		}

		headY = head.y + (head.height / 2);

		float staff_start = 0;
		float staff_end = 0;
		int lineNum = -1;

		for (staff; staff != staffXY.end(); staff++) {
			int index = distance(staffXY.begin(), staff);
			if (index % 2 == 0) {
				staff_start = (*staff).y;
				lineNum++;
			}
			else {
				staff_end = (*staff).y;
				/*
				line(this->objectsImg, Point(100, staff_start), Point(700, staff_start), Scalar(255, 255, 255));
				line(this->objectsImg, Point(100, staff_start + staffSpace), Point(700, staff_start + staffSpace), Scalar(255, 255, 255));
				line(this->objectsImg, Point(100, staff_start + staffSpace * 2), Point(700, staff_start + staffSpace * 2), Scalar(255, 255, 255));
				line(this->objectsImg, Point(100, staff_start + staffSpace * 3), Point(700, staff_start + staffSpace * 3), Scalar(255, 255, 255));
				line(this->objectsImg, Point(100, staff_end), Point(700, staff_end), Scalar(255, 255, 255));
				*/

				if (headY >= staff_start - (3 * this->staffSpace) && headY <= staff_end + (3 * this->staffSpace)) {
					int octave = 0;

					char tone = 'E';
					float curStaff = staff_end + (3 * this->staffSpace);
					const int threshold = (staffSpace / 3);
					for (curStaff; curStaff >= staff_start - (3 * this->staffSpace); curStaff -= staffSpace) {
						tone++;
						if (tone == 'H') {
							tone = 'A';
						}
						else if (tone == 'C') {
							octave++;
						}
						if (headY < curStaff + threshold && headY > curStaff - threshold) {
							break;
						}
						else if (headY > curStaff - staffSpace + threshold && headY < curStaff - threshold) {
							tone++;
							if (tone == 'H') {
								tone = 'A';
							}
							else if (tone == 'C') {
								octave++;
							}
							break;
						}
						tone++;
						if (tone == 'H') {
							tone = 'A';
						}
						else if (tone == 'C') {
							octave++;
						}
					}
					putText(toneImg, string(1, tone), Point((*note).x + ((*note).width / 2), (*note).y + (*note).height + 10), FONT_HERSHEY_PLAIN, 1.0, Scalar(255, 255, 255), 1);
					vector<Note>::iterator iter = noteInfo.begin();
					for (iter; iter != noteInfo.end(); iter++) {
						(*iter).setScale_Octave(tone, octave);
						(*iter).setLineNum(lineNum);
					}
				}
			}
		}
	}
	imshow("Tone", toneImg);
}

void DomiSolConverter::Analysis::recognizeObject() {

}

void DomiSolConverter::Analysis::recognizeGeneralSymbol() {
	char output[100];
	FILE *p = _popen("python label_image.py symbols", "r");
	
	if (p != NULL) {
		int idx = 0;
		while (fgets(output, sizeof(output), p) != NULL) {
			if (strlen(output) > 0) {
				for (int i = 0; i < 100; i++) {
					if (output[i] == '\n') {
						output[i] = '\0';
					}
				}
				cout << output << "\n";

				NonNote nn(output);
				nn.x = objectXY[idx].x + (objectXY[idx].width / 2);
				nn.y = objectXY[idx].y + (objectXY[idx].height / 2);
				this->nonNoteInfo.push_back(nn);
			}
			idx++;
		}
	}
	system("del *.jpg");
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

		/* 컴포넌트 만들기 */
		// OTSU 이진화
		threshold(src, result, 0, 255, THRESH_BINARY | THRESH_OTSU);
		Mat element(width*0.012, height*0.012, CV_8U, Scalar(1)); // 필터 크기 중요
		// 열림 연산
		morphologyEx(result, result, MORPH_OPEN, element);
		// 닫힘 연산
		morphologyEx(result, result, MORPH_CLOSE, element);
		// 팽창 연산
		//dilate(result, result, Mat());
		// 침식 연산
		erode(result, result, Mat());
		//namedWindow("Component Image" + to_string(i));
		//imshow("Component Image" + to_string(i), result);

		/* 컴포넌트의 외곽선 검출 */

		vector<vector<Point>> contours;
		findContours(result, contours, CV_RETR_LIST, CV_CHAIN_APPROX_NONE);
		Mat contoursResult(result.size(), CV_8U, Scalar(255));
		//drawContours(contoursResult, contours, -1, Scalar(150), 2);
		//namedWindow("Draw Contour Image" + to_string(i));
		//imshow("Draw Contour Image" + to_string(i), contoursResult);




		int cmax = width;
		vector<vector<Point>>::const_iterator it = contours.begin();

		/* 외곽선 중 악보 외곽선은 제거 */
		while (it != contours.end()) {
			if (it->size() > cmax)
				it = contours.erase(it);
			else
				++it;
		}

		//drawContours(src, contours, -1, Scalar(100, 255, 0), 2);
		//namedWindow("Only Text Component" + to_string(i));
		//imshow("Only Text Component" + to_string(i), src);

		/* 문자 영역 부분 큰 좌표 정하기 */

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

		// 여백이 이미지를 넘어버리는 경우
		if (xmax > src.cols) {
			xmax = src.cols;

		}
		if (ymax > src.rows) {
			ymax = src.rows;

		}

		// 텍스트 영역을 추출하지 못한 경우
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

		Mat src = this->straightenedImg.clone();
		int width = src.cols;
		int height = src.rows;

		// 오선 아닌 부분 잘라서 저장하기
		int ROISize = staffXY.size() - (staffXY.size() / 2 - 1);
		// 대략적인 텍스트 영역 ROI 배열 동적할당
		Rect *ROI = (Rect*)malloc(sizeof(Rect) * ROISize);

		// 1. 각 오선의 윗부분 crop
		for (int i = 0; i < ROISize; i++) {
			// 처음
			if (i == 0) {
				ROI[i] = Rect(0, 0, width, staffXY[i].y);
			}
			// 마지막
			else if (i == ROISize - 1) {
				ROI[i] = Rect(0, staffXY[2 * i - 1].y, width, height - staffXY[2 * i - 1].y);
			}
			// 중간
			else {
				ROI[i] = Rect(0, staffXY[2 * i - 1].y, width, staffXY[2 * i].y - staffXY[2 * i - 1].y);
			}

		}

		// 문자가 존재하는 부분 자른 뒤 침식 연산 진행

		cropTextArea(ROI);
	
	
}

void DomiSolConverter::Analysis::recognizeNoteSymbol() {


	//Mat dotImg = this->straightenedImg.clone();
	//// Y histogram
	//vector<int> Yhist(objectsImg.cols, 0);
	//vector<uchar*> pixelRows;
	//for (int nr = 0; nr < objectsImg.cols; nr++) {
	//	pixelRows.push_back(objectsImg.ptr<uchar>(nr)); // n번째 row에 대한 주소를 저장
	//}

	//vector<Note>::iterator iter = noteInfo.begin();
	//for (iter; iter != noteInfo.end(); iter++) {
	//	(*iter).~~
	//}

	//for (int i = 0; i < noteXY.size(); i++) {
	//	Rect dotArea = Rect(Point(noteXY[i].br().x, noteXY[i].tl().y - (staffInterval / 3)), Point(noteXY[i].br().x + staffInterval/3*2, noteXY[i].br().y + (staffInterval / 3)));
	//	rectangle(dotImg, dotArea.tl(), dotArea.br(), Scalar(0, 0, 0), 2);
	//	Rect dotDetect = boundingRect(objectsImg(dotArea));
	//	if (dotDetect.area() != 0) {
	//		cout << "dot detected" << endl;
	//		//rectangle(dotImg, dotArea.tl() + dotDetect.tl(), dotArea.br() + dotDetect.br(), Scalar(0, 0, 0), 2);
	//	}
	//}
	//show(dotImg, "dotImg");

	/*
	* 음표옆에 붙은 점&임시표 인식
	*/
	Mat areaImg = this->straightenedImg.clone();
	int staffThickness = 1;
	int staffInterval = (staffHeight + staffThickness * 3) / 4;
	
	vector<Rect> accidentalAreas;
	vector<Rect> dotAreas;
	for (int i=0; i < noteInfo.size(); i++) {
		bool isHeadUp = noteInfo[i].getIsHeadUp();
		Rect accidentalArea;
		Rect dotArea;
		if (isHeadUp) { // 음표머리가 상단
			accidentalArea = Rect(Point(noteXY[i].tl().x - staffInterval, noteXY[i].tl().y - staffInterval), Point(noteXY[i].tl().x, noteXY[i].tl().y + staffInterval * 2));
			dotArea = Rect(Point(noteXY[i].br().x, noteXY[i].tl().y - (staffInterval*0.25)), Point(noteXY[i].br().x + staffInterval, noteXY[i].tl().y + (staffInterval*1.25)));
		}
		else { // 음표머리가 하단 
			accidentalArea = Rect(Point(noteXY[i].tl().x - staffInterval, noteXY[i].br().y - staffInterval*2), Point(noteXY[i].tl().x, noteXY[i].br().y + staffInterval));
			dotArea = Rect(Point(noteXY[i].br().x, noteXY[i].br().y - (staffInterval*1.25)), Point(noteXY[i].br().x + staffInterval, noteXY[i].br().y + (staffInterval*0.25)));
		}
		accidentalAreas.push_back(accidentalArea);
		dotAreas.push_back(dotArea);
		//rectangle(areaImg, accidentalArea.tl(), accidentalArea.br(), Scalar(0, 0, 0), 2);
		//rectangle(areaImg, dotArea.tl(), dotArea.br(), Scalar(0, 0, 0), 2);
	}

	// 저장된 비음표들 중 임시표(샵, 플랫, 제자리표)에 대해 음표에 붙은 것인지 확인
	for (int i = 0; i < nonNoteInfo.size(); i++) {
		String nonNoteType = nonNoteInfo[i].getNonNoteType();
		if (nonNoteType == "sharp" || nonNoteType == "flat" || nonNoteType == "natural") {
			
			for (int j = 0; j < accidentalAreas.size(); j++) { // 음표별로 임시표가 붙을 수 있는 영역 내에 해당 비음표가 존재하는지 검사
				if (accidentalAreas[j].tl().x < nonNoteInfo[i].x && nonNoteInfo[i].x < accidentalAreas[j].br().x &&
					accidentalAreas[j].tl().y < nonNoteInfo[i].y && nonNoteInfo[i].y < accidentalAreas[j].br().y) {
					vector<string> tmpNonNotes = noteInfo[j].getNonNotes();
					tmpNonNotes.push_back(nonNoteType);
					noteInfo[j].setNonNotes(tmpNonNotes);
					break;
				}
			}

		}
	}

	// nonNoteXY 오브젝트들 중 점만큼 작은 오브젝트들을 점일 가능성이 있다 판단하여 비교를 위해 저장
	vector<Rect> dotPossible;
	for (int i = 0; i < nonNoteXY.size(); i++) {
		if (nonNoteXY[i].width < staffInterval && nonNoteXY[i].height < staffInterval) {
			dotPossible.push_back(nonNoteXY[i]);
			//rectangle(areaImg, nonNoteXY[i].tl(), nonNoteXY[i].br(), Scalar(0, 0, 0), 2);
		}
	}

	vector<Note>::iterator iter = noteInfo.begin();
	for (int i = 0; i < dotAreas.size(); i++, iter++) {
		bool isDot = false;
		for (int d = 0; d < dotPossible.size(); d++) {
			int centerX = (dotPossible[d].tl().x + dotPossible[d].br().x) / 2;
			int centerY = (dotPossible[d].tl().y + dotPossible[d].br().y) / 2;
			// 음표머리 오른쪽 특정 영역에 점으로 추정되는 오브젝트가 있으면 점으로 판단
			if (dotAreas[i].tl().x < centerX && centerX < dotAreas[i].br().x &&
				dotAreas[i].tl().y < centerY && centerY < dotAreas[i].br().y ) {
				isDot = true;
				cout << "Dot detected" << endl;
				rectangle(areaImg, dotPossible[d].tl(), dotPossible[d].br(), Scalar(0, 0, 0), 5);
				break;
			}
		}
		noteInfo[i].setDot(isDot);
	}
	show(areaImg, "areaImg");
}

// 기울기 보정한 이미지의 검은색 모서리 부분을 하얀색으로 바꾸기
void DomiSolConverter::Analysis::colorConers() {

	Mat input = this->straightenedBinaryImgforStaff;
	int width = input.cols;
	int height = input.rows;
	int bar = (int)((width / 100.0) * 20);

	// 양쪽 네 곳의 모서리에 대해서 Run Length Coding 진행
	// 검은색으로 칠하기
	
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

void DomiSolConverter::Analysis::setObjectsImg(Mat objectsImg) {
	this->objectsImg = objectsImg;
}

void DomiSolConverter::Analysis::setObjectXY(vector<Rect> objectXY) {
	this->objectXY = objectXY;
}