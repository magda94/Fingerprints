#ifndef METHOD_HH
#define METHOD_HH

#include "opencv2/opencv.hpp"

#include <iostream>
#include <fstream>
#include <vector>
#include <string>

#include "Minutiae.h"


using namespace cv;
using namespace std;

class Method {
protected:
	Mat image;
	string filename;
	string filepath;

	std::string getFilename();
	std::string getFolder();
	void writeEndsToFile(ofstream* file, const vector<Point2i> endPointsVector);
	void writeBranchToFile(ofstream* file, const vector<Point2i> branchPointsVector);
public:
	void writeToFile(const vector<Point2i> endPointsVector, const vector<Point2i> branchPointsVector);
};

#endif
