#include "pch.h"
#include "DomiSolConverter.h"

void DomiSolConverter::Analysis::calculateStaffHeight(){

}

void DomiSolConverter::Analysis::calculateStaffSpace(){

}

void DomiSolConverter::Analysis::calculateStaffXY(){

}

void DomiSolConverter::Analysis::extractFeature() {
	
	Mat objectsRectImg = objectsImg;

	cvtColor(objectsRectImg, objectsRectImg, COLOR_GRAY2RGB);
	for (int index = 0; index < objectXY.size(); index++) {

		Mat object = objectsImg(objectXY[index]);
		int width = object.cols;
		int height = object.rows;
		staffHeight = 34;
		if (height > staffHeight*0.9) {

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
			cout << height << endl;
			HoughLinesP(objectEdge, lines, 1, CV_PI / 180, 1, height*0.55, 1);
			if (!lines.empty()) {
				isLine = 1;
				cout << "line is detected" << endl;
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
	imshow("objects", objectsRectImg);
	imwrite("outputImage/objects.jpg", objectsRectImg);
	
}

void DomiSolConverter::Analysis::calculatePitch() {
	//잠깐만용><

	Mat objectsRectImg = objectsImg;

	cvtColor(objectsRectImg, objectsRectImg, COLOR_GRAY2RGB);
	for (int index = 0; index < objectXY.size(); index++) {

		Mat object = objectsImg(objectXY[index]);
		int width = object.cols;
		int height = object.rows;
		staffHeight = 34;
		if (height > staffHeight*0.9) {

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
			cout << height << endl;
			HoughLinesP(objectEdge, lines, 1, CV_PI / 180, 1, height*0.5, 1);
			if (!lines.empty()) {
				isLine = 1;
				cout << "line is detected" << endl;
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
				for (int nr = 0; nr < height; nr++) {
					uchar* pixel = object.ptr<uchar>(nr); // n번째 row에 대한 주소를 저장

					for (int nc = 0; nc < width; nc++) {
						//cout << int(pixel[nc]) << endl;
						if (pixel[nc] != 0) {
							Yhist[nr]++;
						}
					}
				}
				int headHeight = height * 0.3;
				int headPixelCnt = 0;
				bool isHead = 0;
				// 좌측하단 머리
				for (int i = height; i > height - headHeight; i--) {
					headPixelCnt += Yhist[i - 1];
				}
				if (headPixelCnt > (width*headHeight)*0.6) {
					isHead = 1;
					rectangle(objectsRectImg, objectXY[index].tl(), objectXY[index].br(), Scalar(0, 255, 255), 1);
				}
				if (isHead == 0) {
					int headPixelCnt = 0;
					for (int i = 0; i < headHeight; i++) {
						headPixelCnt += Yhist[i];
					}
					if (headPixelCnt > (width*headHeight)*0.7) {
						isHead = 1;
						//rectangle(objectsRectImg, objectXY[index].tl(), objectXY[index].br(), Scalar(0, 255, 255), 1);
					}
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
	imshow("objects", objectsRectImg);
	imwrite("outputImage/objects.jpg", objectsRectImg);
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