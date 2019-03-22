#pragma once
#include "pch.h"
#include "DomiSolConverter.h"

class DomisolConverter::Preprocessing {
private:
	void combineInfo();
	void transposeKey();
	void saveFile();

public:
	Preprocessing();
};