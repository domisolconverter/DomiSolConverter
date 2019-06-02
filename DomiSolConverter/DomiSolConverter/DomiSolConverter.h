#pragma once
#include "pch.h"
#include <iostream>
#include <opencv2/opencv.hpp>
#include "Symbol.h"
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
		Mat straightenedBinaryImgforStaff;
		Mat straightenedBinaryImgforObject;

		void binarization();
		void detectEdge();
		void straightenImg();
		int show(Mat img, string title);

	public:
		Preprocessing(Mat inputImg);
		Mat getStraightenedImg();
		Mat getStraightenedBinaryImgforStaff();
		Mat getStraightenedBinaryImgforObject();
	};
	
	class Analysis {
	private:
		Mat staffImg;
		Mat straightenedImg;
		Mat straightenedBinaryImgforStaff;
		Mat straightenedBinaryImgforObject;
		Mat inputCalculateStaffImg;
		Mat objectsImg;
		vector<Rect> objectXY;
		vector<Rect> noteXY;
		vector<Rect> nonNoteXY;
		vector<Point> staffXY;
		int staffHeight;
		float staffSpace;
		vector<Note> noteInfo;
		vector<NonNote> nonNoteInfo;
		vector<string> text;

		void extractStaff();
		void removeStaff();
		void extractObject();
		void calculateStaffHeight();
		void calculateStaffSpace();
		void calculateStaffXY();
		void classifyNote();
		void extractFeature();
		void calculatePitch();
		void recognizeObject();
		void recognizeGeneralSymbol();
		void cropTextArea(Rect*);
		void recognizeText();
		void recognizeNoteSymbol();
		void colorConers();
		int show(Mat img, string title);

		struct byX {
			bool operator () (const Rect & a, const Rect & b) {
				return a.x < b.x;
			}
		};


	public:
		Analysis(Mat straightenedImg, Mat straightenedBinaryImgforStaff, Mat straightenedBinaryImgforObject, vector<Note> note, vector<NonNote> nonNote, vector<string> text);

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
	Mat straightenedBinaryImgforStaff;
	Mat straightenedBinaryImgforObject;
	Mat staffImg;
	Mat objectsImg;
	vector<Rect> objectXY;
	Mat resultImg;
	vector<Symbol> wholeSymbols;
	vector<Note> wholeNotes;
	vector<NonNote> wholeNonNotes;
	vector<string> text;

public:
	DomiSolConverter(Mat input);
};
