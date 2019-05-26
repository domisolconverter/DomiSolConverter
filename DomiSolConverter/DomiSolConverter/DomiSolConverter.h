#pragma once
#include "pch.h"
#include <iostream>
#include <opencv2/opencv.hpp>
#include "Symbol.h"
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
		vector<Rect> getObjectXY();
	};

	class Analysis {
	private:
		Mat objectsImg;
		vector<Rect> objectXY;
		vector<Rect> noteXY;
		vector<Point> staffXY;
		int staffHeight;
		float staffSpace;
		vector<string> text;
		vector<Note> notes;
		vector<NonNote> nonNotes;

		void calculateStaffHeight();
		void calculateStaffSpace();
		void calculateStaffXY();
		void extractFeature();
		void calculatePitch();
		void recognizeObject();
		void recognizeGeneralSymbol();
		void recognizeText();
		void recognizeNoteSymbol();

	public:
		Analysis(Mat objectsImg, vector<Rect> objectXY);
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
		vector<Note> notes;
		vector<NonNote> nonNotes;

	public:
		Postproecessing(vector<Note> notes, vector<NonNote> nonNotes);
	};

	Mat inputImg;
	Mat binaryImg;
	Mat straightenedImg;
	Mat staffImg;
	Mat objectsImg;
	vector<Rect> objectXY;
	Mat resultImg;
	vector<Note> notes;
	vector<NonNote> nonNotes;

public:
	DomiSolConverter(Mat input);
};
