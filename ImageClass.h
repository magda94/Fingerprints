#ifndef IMAGECLASS_HH
#define IMAGECLASS_HH

#include "opencv2/opencv.hpp"

#include <string>
#include <vector>
#include <iostream>
#include <fstream>

#include "Skeleton.h"
#include "Filter.h"
#include "Minutiae.h"
#include "MethodI.h"
#include "MethodII.h"
#include "ClassificationI.h"
#include "CoreDetection.h"

using namespace cv;

class ImageClass {
private:
	Mat image;
	Mat orgImage;
	Mat fingerMask;
	std::string filepath;
	std::string verifyPath;
	vector<Point2i> endPointsVector;
	vector<Point2i> branchPointsVector;
	int heightOffset;
	int widthOffset;

	Mat calculateHistogram(const Mat& image);
	int* getMinAndMax(const Mat& histogram);
	double* calculateHistogramTab(const Mat& histogram, int* MinAndMax,double*  histogramTab);
	double* smoothHistogram(double* histogramTab, int* MinAndMax);
	bool bimodalTest(double* tab, int length);
	int calculateThreshold(double* histogramTab, int* MinAndMax);
	void moveCore();
	std::string getFilename();
	std::string getFolder();
public:
	ImageClass(const Mat& image, std::string filepath, std::string verifyPath);
	Mat& getImage();
	void filtrImage();
	void showImage();
	static void showImage(const Mat& image);
	void normalizeImage();
	void binaryImage();
	void smoothImage();
	void createMask();
	void skeletozation();
	void drawCore();
	void coreDetection();
	void findMinutiae();
	void methodI();
	void methodII();
	void clasifyMethodI();
};

#endif

