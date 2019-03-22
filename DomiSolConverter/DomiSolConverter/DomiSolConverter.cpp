#include <iostream>
#include <opencv2/opencv.hpp>
#include "pch.h"

using namespace std;
//using namespace cv;

int main()
{
	cout << "OpenCV Version : " << CV_VERSION << endl;
	cv::Mat img;
	cv::namedWindow("EXAMPLE01", CV_WINDOW_AUTOSIZE);

	img = cv::imread("test.jpg", CV_LOAD_IMAGE_COLOR);
	if (img.empty())
	{
		cout << "[!] You can NOT see the cat!" << endl;
		return -1;
	}
	imshow("EXAMPLE01", img);
	cv::waitKey(0);
	cv::destroyWindow("EXAMPLE01");
	return 0;
}
