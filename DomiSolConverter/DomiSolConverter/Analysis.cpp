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
		   ���� �ܰ����� �� ���� ���������� ������ �ٲܶ� ex) ������ �簢�� ����
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
			// i��° �簢������ i+c��° �簢���� ������
			if (objectXY[i].width > objectXY[i + c].width) {
				// i+c��° �簢���� ������������
				if (!(objectXY[i].x > objectXY[i + c].x) &&
					!((objectXY[i].x + objectXY[i].width) < (objectXY[i + c].x + objectXY[i + c].width)) &&
					!(objectXY[i].y > objectXY[i + c].y) &&
					!((objectXY[i].y + objectXY[i].height) < (objectXY[i + c].y + objectXY[i + c].height))
					) {
					objectXY.erase(objectXY.begin() + i + c);
					objectsCnt--;
				}
			}
			// i��° �簢������ i+c��° �簢���� Ŭ��
			else if (objectXY[i].width > objectXY[i + c].width) {
				// i��° �簢���� ������������
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


// ���� �� ��(5���� ��)�� ����
void DomiSolConverter::Analysis::calculateStaffHeight(){

	int sum = 0;
	for (int i = 1; i < staffXY.size(); i += 2) {
		sum += staffXY[i].y - staffXY[i - 1].y;
	}
	if (staffXY.size() != 0) {
		this->staffHeight = sum / (staffXY.size() / 2);
	}
	else {
		cout << "������ ������ 0�� �Դϴ�" << endl;
	}
}

// ������ �ִ� �� ���� ����
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

void DomiSolConverter::Analysis::classifyNote() {

	Mat objectsRectImg = objectsImg;

	/*
	* ���� ��ǥ�� ���� ��� �߶� ������ object����
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

			// ���� ��ǥ�� ���� ����ΰ�
			if ((width / staffInterval) > 2) {
				Rect tmp = objectXY[i];
				Point tl = tmp.tl();
				Point br = tmp.br();
				//cout << tl << br << endl;
				objectXY.erase(objectXY.begin() + i); // ���� ��ǥ�� ���Ϳ��� �����Ѵ�.
				int num = width / (staffInterval*1.5); // �̾��� ��ǥ�� ������� ��� �������?
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
						objectXY.insert(objectXY.begin() + i + n, Rect(Point(tl.x + start, tl.y + splitRect.tl().y), Point(tl.x + end, tl.y + splitRect.br().y))); // �߸� ��ǥ���� ���Ϳ� �߰���Ų��.
						start += splitWidth + addRemainder;
						//imwrite("outputImage/splits/" + to_string(i) + "_" + to_string(n) + ".jpg", splited(boundingRect(splited)));
					}
					else {
						end = start + splitWidth + (remainder - addRemainder*(num-1)) - 1;
						splitRect = Rect(Point(start, 0), Point(end, height));
						//cout << splitRect << endl;
						Mat splited = object(splitRect);
						splitRect = boundingRect(splited);
						objectXY.insert(objectXY.begin() + i + n, Rect(Point(tl.x + start, tl.y + splitRect.tl().y), Point(tl.x + end, tl.y + splitRect.br().y))); // �߸� ��ǥ���� ���Ϳ� �߰���Ų��.
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
	* ����� ������Ʈ �� ��ǥ �з� & �Ӹ��� ���� �ν�
	* TODO: ��ĥ�� vector<Note>�� flag, isEmptyHead ����
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
		// ������Ʈ ����&�ʺ�� 1�� ����
		if (height > staffHeight*0.9 && width>staffInterval*0.8) {

			// Y histogram
			vector<int> Yhist(height, 0);
			int pixelCnt = 0;
			int blackLeft = 0;
			int blackRight = 0;
			for (int nr = 0; nr < height; nr++) {
				uchar* pixel = object.ptr<uchar>(nr); // n��° row�� ���� �ּҸ� ����

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

			if (middleCnt < width*(height/5)*0.4) { // ����ǥ, �������ڸ�ǥ ���� �͵� ����

				/* ���η� ���� �߶� �� �Ʒ� ��ȭ�� ������ ���� ��ǥ ��ġ�� �Ǵ��Ѵ�.*/
				bool headLocation = true; // true�̸� ��ǥ�Ӹ� �ϴܿ� ��ġ. false�̸� ��ǥ�Ӹ� ��ܿ� ��ġ
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
				//if (blackUp > blackDown) { // ��ǥ �Ӹ��� ��ܿ� ��ġ
					isHeadUp = true;
					// ��ȭ�� ������ ���� ���� �̻��� ��� �Ӹ��� �Ǵ�. 
					// --> Ÿ���� ���� ���� (2*pi*���ι�����*���ι�����). �� ������ ũ�� �Ӹ��� �Ǵ��Ѵ�.
					//cout << staffInterval / 2 * staffInterval*1.25 / 2 * 3.14 << endl;
					if (blackUp > headMax) {
						isEmptyHead = false;
						cout << index << "th object: Up Head is full" << endl;
					}
					else {
						isEmptyHead = true;
						cout << index << "th object: Up Head is empty" << endl;
					}

					// ���� �ִ��� �ν�
					int checkpoint;
					if (width > staffInterval * 1.25) {
						checkpoint = staffInterval / 3;
					}
					else {
						checkpoint = width / 3;
					}
					bool pre = false;
					for (int nr = height - 1; nr > height / 2; nr--) {
						uchar* pixel = object.ptr<uchar>(nr); // n��° row�� ���� �ּҸ� ����
						if (pre == false && pixel[checkpoint] != 0) {
							flag += 1;
							pre = true;
						}
						else if (pre == true && pixel[checkpoint] == 0) {
							pre = false;
						}
					}
					cout << index << "th object: flag: " << flag << endl;

					//imwrite("outputImage/objects/" + to_string(index) + ".jpg", object); // ��ǥ ���� ��� �̹��� ���� ����
					//rectangle(objectsRectImg, objectXY[index].tl(), objectXY[index].br(), Scalar(255, 255, 255), 1);
				}
				else {
				//else if (blackUp < blackDown) { // ��ǥ �Ӹ��� �ϴܿ� ��ġ
					isHeadUp = false;

					// ��ȭ�� ������ ���� ���� �̻��� ��� �Ӹ��� �� ���ִ�. 
					// --> Ÿ���� ���� ���� (2*pi*���ι�����*���ι�����). �� ������ ũ�� �Ӹ��� ���ִ�.
					//cout << staffInterval / 2 * staffInterval*1.25 / 2 * 3.14 << endl;
					if (blackDown > headMax) {
						isEmptyHead = false;
						cout << index << "th object: Down Head is full" << endl;
					}
					else {
						isEmptyHead = true;
						cout << index << "th object: Down Head is empty" << endl;
					}

					// ���� �ִ��� �ν�
					int checkpoint;
					if (width > staffInterval * 1.25) {
						checkpoint = width - staffInterval/3;
					}
					else {
						checkpoint = width / 3;
					}
					bool pre = false;
					for (int nr = 0; nr < height/2; nr++) {
						uchar* pixel = object.ptr<uchar>(nr); // n��° row�� ���� �ּҸ� ����
						if (pre == false && pixel[checkpoint] != 0) {
							flag += 1;
							pre = true;
						}
						else if (pre == true && pixel[checkpoint] == 0) {
							pre = false;
						}
					}
					cout << index << "th object: flag: " << flag << endl;
					//imwrite("outputImage/objects/" + to_string(index) + ".jpg", object); // ��ǥ ���� ��� �̹��� ���� ����
					//rectangle(objectsRectImg, objectXY[index].tl(), objectXY[index].br(), Scalar(255, 255, 255), 1);
				}
			}

		}
		if (isNonNote) {
			nonNoteXY.push_back(objectXY[index]); //nonNoteXY�� ����ǥ Rect���� �߰�
		}
		else {
			noteXY.push_back(objectXY[index]);	// noteXY�� ��ǥ Rect���� �߰�
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

		Mat src = this->straightenedImg.clone();
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
				ROI[i] = Rect(0, staffXY[2 * i - 1].y, width, height - staffXY[2 * i - 1].y);
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


	//Mat dotImg = this->straightenedImg.clone();
	//// Y histogram
	//vector<int> Yhist(objectsImg.cols, 0);
	//vector<uchar*> pixelRows;
	//for (int nr = 0; nr < objectsImg.cols; nr++) {
	//	pixelRows.push_back(objectsImg.ptr<uchar>(nr)); // n��° row�� ���� �ּҸ� ����
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
	* ��ǥ���� ���� ��&�ӽ�ǥ �ν�
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
		if (isHeadUp) { // ��ǥ�Ӹ��� ���
			accidentalArea = Rect(Point(noteXY[i].tl().x - staffInterval, noteXY[i].tl().y - staffInterval), Point(noteXY[i].tl().x, noteXY[i].tl().y + staffInterval * 2));
			dotArea = Rect(Point(noteXY[i].br().x, noteXY[i].tl().y - (staffInterval*0.25)), Point(noteXY[i].br().x + staffInterval, noteXY[i].tl().y + (staffInterval*1.25)));
		}
		else { // ��ǥ�Ӹ��� �ϴ� 
			accidentalArea = Rect(Point(noteXY[i].tl().x - staffInterval, noteXY[i].br().y - staffInterval*2), Point(noteXY[i].tl().x, noteXY[i].br().y + staffInterval));
			dotArea = Rect(Point(noteXY[i].br().x, noteXY[i].br().y - (staffInterval*1.25)), Point(noteXY[i].br().x + staffInterval, noteXY[i].br().y + (staffInterval*0.25)));
		}
		accidentalAreas.push_back(accidentalArea);
		dotAreas.push_back(dotArea);
		//rectangle(areaImg, accidentalArea.tl(), accidentalArea.br(), Scalar(0, 0, 0), 2);
		//rectangle(areaImg, dotArea.tl(), dotArea.br(), Scalar(0, 0, 0), 2);
	}

	// ����� ����ǥ�� �� �ӽ�ǥ(��, �÷�, ���ڸ�ǥ)�� ���� ��ǥ�� ���� ������ Ȯ��
	for (int i = 0; i < nonNoteInfo.size(); i++) {
		String nonNoteType = nonNoteInfo[i].getNonNoteType();
		if (nonNoteType == "sharp" || nonNoteType == "flat" || nonNoteType == "natural") {
			
			for (int j = 0; j < accidentalAreas.size(); j++) { // ��ǥ���� �ӽ�ǥ�� ���� �� �ִ� ���� ���� �ش� ����ǥ�� �����ϴ��� �˻�
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

	// nonNoteXY ������Ʈ�� �� ����ŭ ���� ������Ʈ���� ���� ���ɼ��� �ִ� �Ǵ��Ͽ� �񱳸� ���� ����
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
			// ��ǥ�Ӹ� ������ Ư�� ������ ������ �����Ǵ� ������Ʈ�� ������ ������ �Ǵ�
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

// ���� ������ �̹����� ������ �𼭸� �κ��� �Ͼ������ �ٲٱ�
void DomiSolConverter::Analysis::colorConers() {

	Mat input = this->straightenedBinaryImgforStaff;
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

void DomiSolConverter::Analysis::setObjectsImg(Mat objectsImg) {
	this->objectsImg = objectsImg;
}

void DomiSolConverter::Analysis::setObjectXY(vector<Rect> objectXY) {
	this->objectXY = objectXY;
}