#ifndef CLASSIFICATION_HH
#define CLASSIFICTAION_HH

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cmath>
#include <vector>

#include "MinutiaeInformation.h"

using namespace std;

class Classification {
private:
	int patternCount = 11;
	string filename;
	string fingerName;
	string rootPath;
	string filepath;
	vector<MinutiaeInformation> sample;
	vector<double> compareResults;

	std::string getFilename();
	std::string getRootFolder();
	std::string getFingerName();
	void getSampleInformation();
	void compareToOther();
	double compareToFinger(const vector<MinutiaeInformation> otherFinger);
	void searchPatterns();

	void showResult();
public:
	Classification(string filepath);
	void getClassification();
};

#endif