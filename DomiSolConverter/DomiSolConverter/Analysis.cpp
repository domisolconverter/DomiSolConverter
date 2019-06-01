#include "pch.h"
#include "DomiSolConverter.h"
#include "iconv.h"


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
	// namedWindow("objects", CV_WINDOW_AUTOSIZE);
	// imshow("objects", objectsRectImg);
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

		for (staff; staff != test.end(); staff++) {
			int index = distance(test.begin(), staff);
			if (index % 2 == 0) {
				staff_start = (*staff);
				lineNum++;
			}
			else {
				staff_end = (*staff);
				line(this->objectsImg, Point(100, staff_start), Point(700, staff_start), Scalar(255, 255, 255));
				line(this->objectsImg, Point(100, staff_start + staffSpace), Point(700, staff_start + staffSpace), Scalar(255, 255, 255));
				line(this->objectsImg, Point(100, staff_start + staffSpace * 2), Point(700, staff_start + staffSpace * 2), Scalar(255, 255, 255));
				line(this->objectsImg, Point(100, staff_start + staffSpace * 3), Point(700, staff_start + staffSpace * 3), Scalar(255, 255, 255));
				line(this->objectsImg, Point(100, staff_end), Point(700, staff_end), Scalar(255, 255, 255));

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

}


// UTF-8 인코딩된 파일 읽어오는 함수
wstring readFile(const char* filename)
{
	wifstream wif(filename);
	wif.imbue(locale(locale::empty(), new codecvt_utf8<wchar_t>));
	wstringstream wss;

	wss << wif.rdbuf();
	return wss.str();
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
		imwrite("./outputImage/Onlytextpart.jpg", subImg);
		//namedWindow("Onlytextpart" + to_string(i), WINDOW_AUTOSIZE);
		//imshow("Onlytextpart" + to_string(i), subImg);

		system("tesseract ./outputImage/Onlytextpart.jpg ./outputImage/text_result -l kor+eng");

		// 1. 결과 읽어와서 string vector에 저장
		_setmode(_fileno(stdout), _O_U16TEXT);
		wstring wstr = readFile("./outputImage/text_result.txt");

		wcout << wstr << endl;
		//(수정) wstring을 string 형변환 필요!!!!!!!!!!!!!!!!!!!!!!
		this->text.push_back(wstr);
	}

	//(수정) 형변환 필요
	// 글자 인식 결과 출력
	/*
	vector<wstring>::iterator iter;
	for (iter = this->text.begin(); iter != this->text.end(); ++iter) {
		wcout << *iter << endl;
	}
	*/

}



void DomiSolConverter::Analysis::recognizeText() {

		Mat src = this->straightenedBinaryImg;
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
	
}

// 기울기 보정한 이미지의 검은색 모서리 부분을 하얀색으로 바꾸기
void DomiSolConverter::Analysis::colorConers() {

	Mat input = this->straightenedBinaryImg;
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

DomiSolConverter::Analysis::Analysis(Mat straightenedImg, Mat straightenedBinaryImg, Mat objectsImg, vector<Rect> objectXY) {

	// 오선 검출
	this->straightenedImg = straightenedImg;
	this->straightenedBinaryImg = straightenedBinaryImg;
	colorConers();
	//show(this->inputCalculateStaffImg, "오선인식할 이미지");
	calculateStaffXY();

	// 글자 인식
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