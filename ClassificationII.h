#ifndef CLASSIFICATION_II_H
#define CLASSIFICATION_II_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cmath>
#include <vector>

#include "MinutiaeInformationII.h"

using namespace std;

class ClassificationII {
private:
	string filename;
	string fingerName;
	string rootPath;
	string filepath;
	string verifyPath;
	vector<MinutiaeInformationII> sample;
	vector<MinutiaeInformationII> pattern;

	std::string getFilename();
	std::string getRootFolder();
	std::string getFingerName();
	void getSampleInformation();
	int compareToFinger(const vector<MinutiaeInformationII> otherFinger);
	int findTheLowest(vector<MinutiaeInformationII> otherFinger, MinutiaeInformationII minutiae);
	bool isTrueGroup();
	void readPattern();

	void showResult(bool result);
public:
	ClassificationII(string filepath, string verifyPath);
	void getClassification();
};

#endif