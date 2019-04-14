// For test main function

#include "pch.h"
#include "DomiSolConverter.h"
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

# define INPUTFOLDER "./inputImage/"
# define OUTPUTFOLDER "./outputImage/"

// Change test case
string INPUTFILE = "case2.jpg";
string OUTPUTFILE = "";

string xml = "output.xml";
CascadeClassifier classifier;


int main()
{
	// Set path of input image and output image
	string INPUTPATH = INPUTFOLDER + INPUTFILE;
	string OUTPUTPATH = OUTPUTFOLDER + OUTPUTFILE;
	
	vector<Rect> sharps;

	cout << "OpenCV Version : " << CV_VERSION << endl;
	Mat img;
	namedWindow("CASE01", CV_WINDOW_AUTOSIZE);


	img = imread(INPUTPATH, CV_LOAD_IMAGE_GRAYSCALE);
	if (img.empty())
	{
		cout << "[!] You can NOT open the test image!" << endl;
		return -1;
	}

	
	//imwrite("test.jpg", img);
	imshow("CASE01", img);

	waitKey(0);
	destroyWindow("CASE01");
	return 0;
}