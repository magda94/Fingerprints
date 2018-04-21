#ifndef FILTER_HH
#define FILTER_HH

#include "opencv2/opencv.hpp"
using namespace cv;

class Filter {
private:
	Mat image;
	int checkMedian(int x, int y);
public:
	Filter(const Mat& image);
	Mat filtrMedian();
	Mat reduceHoles();
	Mat normalize();
};

#endif
