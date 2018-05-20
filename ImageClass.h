#ifndef IMAGECLASS_HH
#define IMAGECLASS_HH

#include "opencv2/opencv.hpp"

#include <string>
#include <vector>

#include "Skeleton.h"
#include "Filter.h"
#include "Minutiae.h"
#include "MethodI.h"

using namespace cv;

class ImageClass {
private:
	Mat image;
	Mat fingerMask;
	std::string filepath;
	vector<Point2i> endPointsVector;
	vector<Point2i> branchPointsVector;

	Mat calculateHistogram(const Mat& image);
	int* getMinAndMax(const Mat& histogram);
	double* calculateHistogramTab(const Mat& histogram, int* MinAndMax,double*  histogramTab);
	double* smoothHistogram(double* histogramTab, int* MinAndMax);
	bool bimodalTest(double* tab, int length);
	int calculateThreshold(double* histogramTab, int* MinAndMax);
public:
	ImageClass(const Mat& image, std::string file);
	Mat& getImage();
	void filtrImage();
	void showImage();
	static void showImage(const Mat& image);
	void normalizeImage();
	void binaryImage();
	void smoothImage();
	void createMask();
	void skeletozation();
	void findMinutiae();
	void methodI();
};

#endif

