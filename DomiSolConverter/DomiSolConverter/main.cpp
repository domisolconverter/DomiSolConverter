// For test main function

#include "pch.h"
#include "DomiSolConverter.h"

# define INPUTFOLDER "./inputImage/"
# define OUTPUTFOLDER "./outputImage/"

// Change test case
string INPUTFILE = "case9.jpg";
string OUTPUTFILE = "";


int main()
{
	// Set path of input image and output image
	string INPUTPATH = INPUTFOLDER + INPUTFILE;
	string OUTPUTPATH = OUTPUTFOLDER + OUTPUTFILE;


	cout << "OpenCV Version : " << CV_VERSION << endl;
	Mat img;
	//namedWindow("CASE01", CV_WINDOW_AUTOSIZE);

	img = imread(INPUTPATH, IMREAD_GRAYSCALE);
	if (img.empty())
	{
		cout << "[!] You can NOT open the test image!" << endl;
		return -1;
	}

	// Image Resizing
	double height = img.rows;
	double width = img.cols;
	resize(img, img, Size(int(width * (1000 / height)), 1000), 0, 0, CV_INTER_NN);

	DomiSolConverter domisolconverter(img);
	
	waitKey(0);
	destroyWindow("CASE01");
	return 0;
}