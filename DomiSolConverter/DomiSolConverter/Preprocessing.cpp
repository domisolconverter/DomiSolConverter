#include "pch.h"
#include "DomiSolConverter.h"

DomiSolConverter::Preprocessing::Preprocessing(Mat inputImg) {
	this->inputImg = inputImg;


	//binarization();
	adaptiveThreshold(~inputImg, this->binaryImg, 255, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY, 7, -10);
	show(binaryImg, "binaryImg");
	detectEdge();
	//show(edgeImg, "edgeImg");
	straightenImg();
	//show(straightenedImg, "straightenedImg");
	threshold(~straightenedImg, straightenedBinaryImgforObject, 0, 255, THRESH_BINARY | THRESH_OTSU);
	adaptiveThreshold(~straightenedImg, straightenedBinaryImgforStaff, 255, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY, 7, -10);
}

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
	//adaptiveThreshold(~inputImg, this->binaryImg, 255, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY, 7, -10);
	//adaptiveThreshold(~inputImg, this->binaryImg, 255, ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY, 15, -2);
	//threshold(~inputImg, binaryImg, 0, 255, THRESH_BINARY | THRESH_OTSU);
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

Mat DomiSolConverter::Preprocessing::getStraightenedImg() {
	return straightenedImg;
}

Mat DomiSolConverter::Preprocessing::getStraightenedBinaryImgforStaff() {
	return straightenedBinaryImgforStaff;
}

Mat DomiSolConverter::Preprocessing::getStraightenedBinaryImgforObject() {
	return straightenedBinaryImgforObject;
}