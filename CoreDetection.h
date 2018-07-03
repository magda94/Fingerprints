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

	void addMatrix(double** in1, double** in2, double** out, int start_rows, int start_cols);
	void subtractMatrix(double** in1, double** in2, double** out, int start_rows, int start_cols);
	void multiplyMatrix(double** in1, double** in2, double** out, int start_rows, int start_cols);
	void divideMatrix(double** in1, double** in2, double** out, int start_rows, int start_cols);
	void powMatrix(double** in, double** out, int number, int start_rows, int start_cols);
	void sqrtMatrix(double** in,double** out, int start_rows, int start_cols);


	void filtrGaussian(double** in, double** out, int start_rows, int start_cols);
	void countSineComponent(int blockSize);
	void countCosineComponent(int start_rows, int start_cols);
	
	void countFieldOrientation();

	void writeGradient(double** in, std::string name);

public:
	CoreDetection(const Mat& image);
	~CoreDetection();
	void detectCore();
};

#endif // COREDETECTION_HH
