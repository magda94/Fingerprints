#ifndef METHODI_HH
#define METHODI_HH

#include "opencv2/opencv.hpp"

#include <vector>
#include <string>

#include "Method.h"


using namespace cv;
using namespace std;

class MethodI :public Method{


public:
	MethodI(const Mat& image, std::string filepath);
	//void writeToFile(const vector<Point2i> endPointsVector, const vector<Point2i> branchPointsVector);
};

#endif