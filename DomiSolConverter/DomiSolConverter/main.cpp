// For test main function

#include "pch.h"
#include "DomiSolConverter.h"

int main(int argc, char **argv)
{
	// Set path of input image and output image

	// Change test case
	//string INPUTFILE = argv[1];
	string INPUTFILE = "./inputImage/½£¼Ó.jpg";

	cout << "OpenCV Version : " << CV_VERSION << endl;
	Mat img;
	//namedWindow("CASE01", CV_WINDOW_AUTOSIZE);

	img = imread(INPUTFILE, IMREAD_GRAYSCALE);
	if (img.empty())
	{
		cout << "[!] You can NOT open the test image!" << endl;
		return -1;
	}

	// Image Resizing
	double height = img.rows;
	double width = img.cols;
	resize(img, img, Size(700, int(height * (700 / width))), 0, 0, CV_INTER_NN);

	DomiSolConverter domisolconverter(img, INPUTFILE);
	
	waitKey(0);
	destroyWindow("CASE01");
	return 0;
}