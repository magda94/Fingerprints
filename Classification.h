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

class Classification {
private:
	int patternCount = 10;
	string filename;
	string fingerName;
	string rootPath;
	string filepath;
	vector<MinutiaeInformationI> sample;
	vector<double> compareResults;
	int numberOfGroup;

	int check;

	std::string getFilename();
	std::string getRootFolder();
	std::string getFingerName();
	void getSampleInformation();
	void compareToOther();
	double compareToFinger(const vector<MinutiaeInformationI> otherFinger);
	double findTheLowest(vector<MinutiaeInformationI> otherFinger, MinutiaeInformationI minutiae);
	void searchPatterns();
	void findTrueGroup();

	void showResult();
public:
	Classification(string filepath);
	void getClassification();
};

#endif