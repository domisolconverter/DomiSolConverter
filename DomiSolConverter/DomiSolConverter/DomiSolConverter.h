#pragma once
#include "pch.h"
#include <iostream>
#include <opencv2/opencv.hpp>
#include <vector>

using namespace std;
using namespace cv;

class DomisolConverter {
private:
	class Preprocessing;
	class Analysis;
	class Postproecessing;

	Mat inputImg;
	Mat binaryImg;
	Mat straightenedImg;
	Mat staffImg;
	vector<Point> objectXY;
	Mat resultImg;

public:
	DomisolConverter(Mat input);
};
