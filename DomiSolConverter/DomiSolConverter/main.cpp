// For test main function

#include "pch.h"
#include "DomiSolConverter.h"

# define INPUTFOLDER "./inputImage/"
# define OUTPUTFOLDER "./outputImage/"

// Change test case
string INPUTFILE = "noStaff.jpg";
string OUTPUTFILE = "";


int main()
{
	// Set path of input image and output image
	string INPUTPATH = INPUTFOLDER + INPUTFILE;
	string OUTPUTPATH = OUTPUTFOLDER + OUTPUTFILE;

	cout << "OpenCV Version : " << CV_VERSION << endl;
	Mat img;
	namedWindow("CASE01", CV_WINDOW_AUTOSIZE);

	img = imread(INPUTPATH, CV_LOAD_IMAGE_COLOR);
	if (img.empty())
	{
		cout << "[!] You can NOT open the test image!" << endl;
		return -1;
	}
	imshow("CASE01", img);
	waitKey(0);
	destroyWindow("CASE01");
	return 0;
}