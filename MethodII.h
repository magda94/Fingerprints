#ifndef METHODII_HH
#define METHODII_HH

#include "Method.h"

using namespace cv;
using namespace std;

class MethodII : public Method {
public:
	MethodII(const Mat& image, std::string filepath);
	void writeToFile(const vector<Point2i> endPointsVector, const vector<Point2i> branchPointsVector);
};

#endif 
