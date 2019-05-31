#pragma once
#include "pch.h"
#include <iostream>
#include <opencv2/opencv.hpp>
#include <vector>

using namespace std;
using namespace cv;

class DomiSolConverter {
private:
	class Preprocessing {
	private:
		Mat inputImg;
		Mat binaryImg;
		Mat edgeImg;
		Mat lineImg;
		Mat straightenedImg;
		Mat straightenedBinaryImg;
		Mat staffImg;
		Mat objectsImg;
		vector<Rect> objectXY;

		void binarization();
		void detectEdge();
		void straightenImg();
		void extractStaff();
		void removeStaff();
		void extractObject();
		int show(Mat img, string title);

		struct byX {
			bool operator () (const Rect & a, const Rect & b) {
				return a.x < b.x;
			}
		};
	public:
		Preprocessing(Mat inputImg);
		Mat getObjectsImg();
		Mat getStraightenedBinaryImg();
		vector<Rect> getObjectXY();
	};
	
	class Analysis {
	private:

		
		Mat straightenedBinaryImg;
		Mat inputCalculateStaffImg;
		Mat objectsImg;
		vector<Rect> objectXY;

		vector<Point> staffXY;
		int staffHeight;
		int staffSpace;
		vector<string> note;
		vector<string> nonNote;
		vector<string> text;

		void calculateStaffHeight();
		void calculateStaffSpace();
		void calculateStaffXY();
		void extractFeature();
		void calculatePitch();
		void recognizeObject();
		void recognizeGeneralSymbol();
		void recognizeText();
		void recognizeNoteSymbol();
		void colorConers();
		int show(Mat img, string title);

	public:

		Analysis(Mat straightenedImg, Mat objectsImg, vector<Rect> objectXY);

		vector<string> getNote();
		vector<string> getNonNote();
		vector<string> getText();
		void setObjectsImg(Mat objectsImg);
		void setObjectXY(vector<Rect> objectXY);
	};



	class Postproecessing{
	private:
		void combineInfo();
		void transposeKey();
		void saveFile();

	public:
		Postproecessing();
	};

	Mat inputImg;
	Mat binaryImg;
	Mat straightenedBinaryImg;
	Mat staffImg;
	Mat objectsImg;
	vector<Rect> objectXY;
	Mat resultImg;

public:
	DomiSolConverter(Mat input);
};
