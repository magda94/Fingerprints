#ifndef FILTER_HH
#define FILTER_HH

#include "opencv2/opencv.hpp"
#include <vector>

using namespace cv;
using namespace std;

class Filter {
private:
	Mat image;
	Mat tempFingerMask;
	vector<Point2i> buforList;
	int counter;
	int checkMedian(int x, int y);
	bool checkNumberOfBlack(Mat image, int x, int y);
	Mat chooseBiggest(Mat image);
	void addNeighbours(int x, int y);
public:
	Filter(const Mat& image);
	Mat filtrMedian();
	Mat reduceHoles();
	Mat normalize();
	Mat createMask();
};

#endif
