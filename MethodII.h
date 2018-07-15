#ifndef METHODII_HH
#define METHODII_HH

#include "Method.h"

using namespace cv;
using namespace std;

class MethodII : public Method {
	MethodII(const Mat& image, std::string filepath);
};

#endif 
