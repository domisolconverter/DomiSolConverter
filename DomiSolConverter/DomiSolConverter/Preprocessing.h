#pragma once
#include "pch.h"
#include "DomiSolConverter.h"

class DomisolConverter::Preprocessing {
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