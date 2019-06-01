#include "pch.h"
#include "DomiSolConverter.h"

void DomiSolConverter::Analysis::calculateStaffHeight(){

}

void DomiSolConverter::Analysis::calculateStaffSpace(){

}

void DomiSolConverter::Analysis::calculateStaffXY(){

}

void DomiSolConverter::Analysis::classifyNote() {


	Mat objectsRectImg = objectsImg;

	/*
	* 여러 음표가 붙은 경우 잘라서 각각의 object저장
	* TODO: 바꿔야할 임의 값 : staffHeight, staffThickness
	*/
	staffHeight = 34;
	int staffThickness = 2;
	//cout << objectXY.size() << endl;

	int objectCnt = objectXY.size();
	for (int i = 0; i < objectCnt; i++) {
		Mat object = objectsImg(objectXY[i]);
		int width = object.cols;
		int height = object.rows;
		int noteHeight = (staffHeight + staffThickness * 3) / 4;

		if (height < staffHeight*1.2) {

			// 여러 음표가 붙은 경우인가
			if ((width / noteHeight) > 2) {
				Rect tmp = objectXY[i];
				Point tl = tmp.tl();
				Point br = tmp.br();
				//cout << tl << br << endl;
				objectXY.erase(objectXY.begin() + i); // 붙은 음표는 벡터에서 제거한다.
				int num = width / (noteHeight*1.5); // 이어진 음표가 몇개인지는 어떻게 계산하지?
				int splitWidth = width / num;
				int remainder = width % num;
				int start = 0;
				int end = 0;
				//cout << "width: " << width << "num: " << num << endl;
				for (int n = 0; n < num; n++) {
					Rect splitRect;
					if (n == 0) {
						end = start + splitWidth + remainder - 1;
						splitRect = Rect(Point(start, 0), Point(end, height));
						//cout << splitRect << endl;
						Mat splited = object(splitRect);
						splitRect = boundingRect(splited);
						objectXY.insert(objectXY.begin() + i + n, Rect(Point(tl.x + start, tl.y), Point(tl.x + end, tl.y + splitRect.br().y))); // 잘린 음표들을 벡터에 추가시킨다.
						start += splitWidth + remainder;
					}
					else {
						end = start + splitWidth - 1;
						splitRect = Rect(Point(start, 0), Point(end, height));
						//cout << splitRect << endl;
						Mat splited = object(splitRect);
						splitRect = boundingRect(splited);
						objectXY.insert(objectXY.begin() + i + n, Rect(Point(tl.x + start, tl.y), Point(tl.x + end, tl.y + splitRect.br().y))); // 잘린 음표들을 벡터에 추가시킨다.
						start += splitWidth;
					}
					//imwrite("outputImage/splits/" + to_string(i) + "_" + to_string(n) + ".jpg", splited(boundingRect(splited)));
				}
				objectCnt = objectCnt + num - 1;
				imwrite("outputImage/splits/" + to_string(i) + ".jpg", object);
			}
		}
	}
	//cout << objectXY.size() << endl;


	/*
	* 추출된 오브젝트 중 음표 분류 및 머리와 꼬리 인식
	* TODO: 바꿔야할 임의값: staffHeight, staffInterval
	*/
	int staffInterval = 10; // 오선간격 10으로 가정
	for (int index = 0; index < objectXY.size(); index++) {
		//rectangle(objectsRectImg, objectXY[index].tl(), objectXY[index].br(), Scalar(255, 255, 255), 1);
		Mat object = objectsImg(objectXY[index]);
		int width = object.cols;
		int height = object.rows;
		staffHeight = 34;

		//cout << "width: " << width << "height: " << height << endl;
		// 오브젝트 길이로 1차 선별
		if (height > staffHeight*0.9 && height < staffHeight*1.2) {
			bool isLine = 0;

			// Y histogram
			vector<int> Yhist(height, 0);
			int pixelCnt = 0;
			for (int nr = 0; nr < height; nr++) {
				uchar* pixel = object.ptr<uchar>(nr); // n번째 row에 대한 주소를 저장

				for (int nc = 0; nc < width; nc++) {
					if (pixel[nc] != 0) {
						Yhist[nr]++;
						pixelCnt++;
					}
				}
			}

			// noteXY.push_back(objectXY[index]);	// noteXY에 Rect정보 추가
				//rectangle(objectsRectImg, objectXY[index].tl(), objectXY[index].br(), Scalar(255, 255, 255), 1);
			imwrite("outputImage/objects/" + to_string(index) + ".jpg", object); // 음표 검출 결과 이미지 각각 저장


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
			if (blackUp > blackDown * 1.2) { // 음표 머리가 상단에 위치 (흑화소의 분포가 1.2배 이상 차이남)
				headLocation = false;
				noteXY.push_back(objectXY[index]);	// noteXY에 Rect정보 추가
				rectangle(objectsRectImg, objectXY[index].tl(), objectXY[index].br(), Scalar(255, 255, 255), 1);

			}
			else if (blackUp *1.2 < blackDown) { // 음표 머리가 하단에 위치
				noteXY.push_back(objectXY[index]);	// noteXY에 Rect정보 추가
				rectangle(objectsRectImg, objectXY[index].tl(), objectXY[index].br(), Scalar(255, 255, 255), 1);

			}

		}

	}
	imshow("objects", objectsRectImg);
	//imwrite("outputImage/objects.jpg", objectsRectImg);
}


void DomiSolConverter::Analysis::extractNoteFeature() {

	
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

DomiSolConverter::Analysis::Analysis(Mat objectsImg, vector<Rect> objectXY) {
	this->objectsImg = objectsImg;
	this->objectXY = objectXY;

	classifyNote();
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