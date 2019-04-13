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
		Mat edgeImg;
		Mat lineImg;

		void binarization();
		void detectEdge();
		void straightenImg();
		void removeStaff();
		void extractObject();

	public:
		Preprocessing();
	};

	class Analysis {
	private:
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
		void cropTextArea();
		void recognizeText();
		void recognizeNoteSymbol();

	public:
		Analysis();
		vector<string> getNote();
		vector<string> getNonNote();
		vector<string> getText();
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
	Mat straightenedImg;
	Mat staffImg;
	vector<Point> objectXY;
	Mat resultImg;

public:
	DomiSolConverter(Mat input);
};
