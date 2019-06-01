#pragma once
#include "pch.h"
#include <iostream>
#include <opencv2/opencv.hpp>
#include <vector>
#include "Symbol.h"

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
		vector<Rect> nonNoteXY;
		vector<Point> staffXY;
		int staffHeight;
		int staffSpace;
		vector<string> note;
		vector<string> nonNote;
		vector<string> text;

		void calculateStaffHeight();
		void calculateStaffSpace();
		void calculateStaffXY();
		void classifyNote();
		void extractNoteFeature();
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

	public:
		Postproecessing();
	};

	Mat inputImg;
	Mat binaryImg;
	Mat straightenedImg;
	Mat staffImg;
	Mat objectsImg;
	vector<Rect> objectXY;
	Mat resultImg;
	vector<Symbol> wholeSymbols;
	vector<Note> wholeNotes;
	vector<NonNote> wholeNonNotes;

public:
	DomiSolConverter(Mat input);
};
