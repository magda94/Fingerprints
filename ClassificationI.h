#ifndef CLASSIFICATION_HH
#define CLASSIFICTAION_HH

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cmath>
#include <vector>

#include "MinutiaeInformationI.h"

using namespace std;

class ClassificationI {
private:
	string filename;
	string fingerName;
	string rootPath;
	string filepath;
	string verifyPath;
	vector<MinutiaeInformationI> sample;
	vector<MinutiaeInformationI> pattern;
	int numberOfGroup;

	int check;

	std::string getFilename();
	std::string getRootFolder();
	std::string getFingerName();
	void getSampleInformation();
	int compareToFinger(const vector<MinutiaeInformationI> otherFinger);
	int findTheLowest(vector<MinutiaeInformationI> otherFinger, MinutiaeInformationI minutiae);
	bool isTrueGroup();
	void readPattern();

	void showResult(bool result);
public:
	ClassificationI(string filepath, string verifyPath);
	void getClassification();
};

#endif