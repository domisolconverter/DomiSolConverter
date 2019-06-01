#pragma once
#include "pch.h"
#include <iostream>
#include <opencv2/opencv.hpp>
#include "Symbol.h"
#include <vector>
#include <sstream>
#include <codecvt>
#include <fstream>
#include <io.h>
#include <fcntl.h>
#include <string>
#include <codecvt>



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
		Mat getStraightenedImg();
		Mat getStraightenedBinaryImg();
		vector<Rect> getObjectXY();
	};
	
	class Analysis {
	private:


		Mat straightenedImg;
		Mat straightenedBinaryImg;
		Mat inputCalculateStaffImg;
		Mat objectsImg;
		vector<Rect> objectXY;
		vector<Rect> noteXY;
		vector<Point> staffXY;
		int staffHeight;
		float staffSpace;
		vector<Note> noteInfo;
		vector<NonNote> nonNoteInfo;
		// wstring을 string으로 형변환 필요 !!!!!!!!!!!!
		vector<wstring> text;

		void calculateStaffHeight();
		void calculateStaffSpace();
		void calculateStaffXY();
		void extractFeature();
		void calculatePitch();
		void recognizeObject();
		void recognizeGeneralSymbol();
		void cropTextArea(Rect*);
		void recognizeText();
		void recognizeNoteSymbol();
		void colorConers();
		int show(Mat img, string title);

	public:

		Analysis(Mat straightenedImg, Mat straightenedBinaryImg, Mat objectsImg, vector<Rect> objectXY);

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
	Mat straightenedImg;
	Mat straightenedBinaryImg;
	Mat staffImg;
	Mat objectsImg;
	vector<Rect> objectXY;
	Mat resultImg;

public:
	DomiSolConverter(Mat input);
};
