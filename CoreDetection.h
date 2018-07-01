#ifndef COREDETECTION_HH
#define COREDETECTION_HH

#include "opencv2/opencv.hpp"

#include <cmath>
#include <iostream>
#include <fstream>

using namespace cv;

class CoreDetection {
private:
	Mat image;

	const int h_divide = 12;
	const int w_divide = 8;
	double** gradientX;
	double** gradientY;
	double** gradientXX;
	double** gradientXY;
	double** gradientYY;
	double sigma;

	double countMean();
	double countDeviation();
	void countGradientX();
	void countGradientY();
	void multiplyMatrix(double** in1, double** in2, double** out, int start_rows, int start_cols);
	void filtrGaussian(double** in, double** out, int start_rows, int start_cols);

	void writeGradient(double** in, std::string name);

public:
	CoreDetection(const Mat& image);
	~CoreDetection();
	void detectCore();
};

#endif // COREDETECTION_HH
