#ifndef FILTER_HH
#define FILTER_HH

#include "opencv2/opencv.hpp"
#include <vector>
#include <cmath>

using namespace cv;
using namespace std;

class Filter {
private:
	Mat image;
	Mat tempFingerMask;
	vector<Point2i> buforList;
	int counter;

	int heightOffset;
	int widthOffset;

	int checkMedian(int x, int y);
	bool checkNumberOfBlack(Mat image, int x, int y);
	Mat chooseBiggest(Mat image);
	void addNeighbours(int x, int y);
	double getMean();
	double getVariance(double mean);
	double getValueNormalized(int imageValue, double mean, double mean0, double variance, double variance0);
	Mat centerMask(Mat image);
	void countCoordinates(Mat image, int* tab);
public:
	Filter(const Mat& image);
	Mat filtrMedian();
	Mat reduceHoles();
	Mat normalize();
	Mat normalize2();//filter with histogram 
	Mat createMask();
	Mat getImage();
	int getHeightOffset();
	int getWidthOffset();
};

#endif
