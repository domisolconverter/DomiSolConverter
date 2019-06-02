#pragma once
#include "pch.h"
#include <iostream>
#include <opencv2/opencv.hpp>
#include "Symbol.h"
#include <vector>
#include "Signature.h"

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

		void extractStaff();
		void removeStaff();
		void extractObject();
		void calculateStaffHeight();
		void calculateStaffSpace();
		void calculateStaffXY();
		void classifyNote();
		void storeNonNote();
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
		string makeCode();
		void saveFile(string code);
		vector<Note> notes;
		vector<NonNote> nonNotes;
		vector<Symbol*> wholeSign;
		string makeNoteCode(Note *note, Signature *sig);
		string makeNonNoteCode(NonNote *nonNote, Signature *sig);
		static bool compare(Symbol *a, Symbol *b);

	public:
		Postproecessing(vector<Note> notes, vector<NonNote> nonNotes);
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
	vector<Note> wholeNotes;
	vector<NonNote> wholeNonNotes;
	vector<string> text;

public:
	DomiSolConverter(Mat input);
};
