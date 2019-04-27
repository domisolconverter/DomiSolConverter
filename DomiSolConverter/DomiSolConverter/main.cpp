// For test main function

#include "pch.h"
#include "DomiSolConverter.h"

# define INPUTFOLDER "./inputImage/"
# define OUTPUTFOLDER "./outputImage/"

// Change test case
string INPUTFILE = "case1.jpg";
string OUTPUTFILE = "";


int main()
{
	// Set path of input image and output image
	string INPUTPATH = INPUTFOLDER + INPUTFILE;
	string OUTPUTPATH = OUTPUTFOLDER + OUTPUTFILE;


	cout << "OpenCV Version : " << CV_VERSION << endl;
	Mat img;
	namedWindow("CASE01", CV_WINDOW_AUTOSIZE);

	img = imread(INPUTPATH, IMREAD_GRAYSCALE);
	if (img.empty())
	{
		cout << "[!] You can NOT open the test image!" << endl;
		return -1;
	}
	imshow("CASE01", img);
	
	
	DomiSolConverter domisolconverter(img);
	
	waitKey(0);
	destroyWindow("CASE01");
	return 0;
}