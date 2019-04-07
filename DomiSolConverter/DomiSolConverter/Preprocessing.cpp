#include "pch.h"
#include "DomiSolConverter.h"


int DomiSolConverter::Preprocessing::show(Mat img, string title) {
	// cout << "OpenCV Version : " << CV_VERSION << endl;
	namedWindow(title, CV_WINDOW_AUTOSIZE);

	if (img.empty())
	{
		cout << "There's no sheet file!" << endl;
		return -1;
	}

	imshow(title, img);
}

void DomiSolConverter::Preprocessing::binarization() {
	//adaptiveThreshold(~inputImg, this->binaryImg, 255, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY, 5, 10);
	//adaptiveThreshold(~inputImg, this->binaryImg, 255, ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY, 15, -2);
	threshold(~inputImg, binaryImg, 0, 255, THRESH_BINARY | THRESH_OTSU);
}

void DomiSolConverter::Preprocessing::detectEdge() {

	int scale = 1;
	int delta = 0;
	int ddepth = CV_16S;

	/*
	// reduce the noise (kernel size=3)
	Mat blurredImg;
	GaussianBlur(this->binaryImg, blurredImg, Size(5, 5), 0, 0, BORDER_DEFAULT);
	*/

	Mat grad_x, grad_y;
	Mat abs_grad_x, abs_grad_y;

	// calculate derivatives in x and y directions
	Sobel(binaryImg, grad_x, ddepth, 1, 0, 3, scale, delta, BORDER_DEFAULT);
	Sobel(binaryImg, grad_y, ddepth, 0, 1, 3, scale, delta, BORDER_DEFAULT);
	//Scharr(blurredImg, grad_x, ddepth, 1, 0, scale, delta, BORDER_DEFAULT);
	//Scharr(blurredImg, grad_y, ddepth, 0, 1, scale, delta, BORDER_DEFAULT);

	// convert results back to CV_8U
	convertScaleAbs(grad_x, abs_grad_x);
	convertScaleAbs(grad_y, abs_grad_y);

	// add sobel_x & sobel_y
	addWeighted(abs_grad_x, 0.5, abs_grad_y, 0.5, 0, edgeImg);
}

void DomiSolConverter::Preprocessing::straightenImg(){

	Mat blank(inputImg.rows, inputImg.cols, CV_8UC1, Scalar(0));
	lineImg = blank.clone();


	// probabilistic line transform
	vector<Vec4i> lines;
	HoughLinesP(edgeImg, lines, 1, CV_PI / 180, 50, 200, 10);
	for (size_t i = 0; i < lines.size(); i++) {
		Vec4i l = lines[i];
		line(lineImg, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(255, 255, 255), 1);
	}
	//imshow("lineImg", lineImg);

	// calculate the slope average of straight lines (staff line)
	double sum = 0;
	for (size_t i = 0; i < lines.size(); i++) {
		Vec4i l = lines[i];
		int dx = l[0] - l[2];
		int dy = l[1] - l[3];
		double rad = atan2((double)dx, (double)dy);
		double degree = abs(rad * 180) / CV_PI;
		sum += degree;
	}
	double average = round(sum / lines.size() * 100) / 100;
	double degree = 90 - average;

	Point center = Point(inputImg.cols / 2, inputImg.rows / 2);
	double scale = 1;
	degree = degree * -1;
	Mat rot_mat = getRotationMatrix2D(center, degree, scale);
	warpAffine(inputImg, straightenedImg, rot_mat, inputImg.size());
}

void DomiSolConverter::Preprocessing::extractStaff() {

	staffImg = straightenedBinaryImg.clone();
	int horizontalsize = staffImg.cols / 50;
	Mat horizontalStructure = getStructuringElement(MORPH_RECT, Size(horizontalsize, 1));
	erode(staffImg, staffImg, horizontalStructure, Point(-1, -1));
	dilate(staffImg, staffImg, horizontalStructure, Point(-1, -1));
}

void DomiSolConverter::Preprocessing::removeStaff() {

	objectsImg = straightenedBinaryImg.clone();
	int verticalsize = objectsImg.rows / 250;
	Mat verticalStructure = getStructuringElement(MORPH_RECT, Size(1, verticalsize));
	erode(objectsImg, objectsImg, verticalStructure, Point(-1, -1));
	dilate(objectsImg, objectsImg, verticalStructure, Point(-1, -1));

}

void DomiSolConverter::Preprocessing::extractObject() {
	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;

	// find contours
	findContours(objectsImg, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));
	//vector<vector<Point>> contours_poly(contours.size());

	for (int i = 0; i < contours.size(); i++) {
		/*Contour Approximation
		   도형 외곽선을 더 적은 꼭지점수를 갖도록 바꿀때 ex) 찢겨진 사각형 복원
		approxPolyDP(Mat(contours[i]), contours_poly[i], 0, true);
		if (fabs(contourArea(Mat(contours_poly[i])) > 10)) {
			objectXY[i] = boundingRect(Mat(contours_poly[i]));
		}
		*/
		if (fabs(contourArea(Mat(contours[i])) > 5)) {
			//boundRect[rectCnt] = boundingRect(Mat(contours[i]));
			objectXY.push_back(boundingRect(Mat(contours[i])));
		}
	}
	// sort object rectangles by x
	sort(objectXY.begin(), objectXY.end(), byX());
	//for (int i = 0; i < objectXY.size(); i++) {
	//	cout << "x: " << objectXY.at(i).x << "y: " << objectXY.at(i).y << "width: " << objectXY.at(i).width << "height: " << objectXY.at(i).height << endl;
	//}

	/*
	//Draw result
	Mat contourImg = Mat(objectsImg.rows, objectsImg.cols, CV_8U);
	//Scalar color(0, 0, 0);
	Scalar color(255, 255, 255);
	for (int i = 0; i < contours.size(); i++) {
		drawContours(contourImg, contours, i, color, 1, 8, vector<Vec4i>(), 0, Point());
	}
	for (int i = 0; i < objectXY.size(); i++) {
		printf("%d", i);
		rectangle(contourImg, objectXY[i].tl(), objectXY[i].br(), color, 1);
	}
	namedWindow("contours", CV_WINDOW_AUTOSIZE);
	imshow("contours", contourImg);
	*/
}


Mat DomiSolConverter::Preprocessing::getObjectsImg() {
	return objectsImg;
}

vector<Rect> DomiSolConverter::Preprocessing::getObjectXY() {
	return objectXY;
}

DomiSolConverter::Preprocessing::Preprocessing(Mat inputImg) {
	this->inputImg = inputImg;
	
	binarization();
	//show(binaryImg, "binaryImg");
	detectEdge();
	//show(edgeImg, "edgeImg");
	straightenImg();
	//show(straightenedImg, "straightenedImg");
	threshold(~straightenedImg, straightenedBinaryImg, 0, 255, THRESH_BINARY | THRESH_OTSU);
	//show(straightenedBinaryImg, "straightenedBinaryImg");
	extractStaff();
	//show(objectsImg, "objectsImg");
	removeStaff();
	show(objectsImg, "objectsImg");
	extractObject();
	
}