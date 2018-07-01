#include "ImageClass.h"

/*******************************************
PRIVATE METHODS
*******************************************/

Mat ImageClass::calculateHistogram(const Mat& image) {
	Mat histogram;
	float range[] = { 0,256 };
	const float* histogramRange = {range};
	int histogramSize = 128;
	calcHist(&image, 1, 0, Mat(), histogram, 1, &histogramSize, &histogramRange, true, false);

	return histogram;
}

//MinAndMax[0] - minimum bin, MinAndMax[1] - maximum bin
int* ImageClass::getMinAndMax(const Mat& histogram) {
	static int MinAndMax[2] = { -1,-1 };
	for (int i = 0; i < 256; i++) {
		if (histogram.at<float>(i)>0) {
			if (MinAndMax[0] == -1)
				MinAndMax[0] = i;
			MinAndMax[1] = i;
		}
	}
	return MinAndMax;
}

double* ImageClass::calculateHistogramTab(const Mat& histogram, int* MinAndMax,double* histogramTab) {
	std::cout << MinAndMax[0] << std::endl;
	for (int i = MinAndMax[0]; i <= MinAndMax[1]; i++) {
		histogramTab[i - MinAndMax[0]] = histogram.at<double>(i);
	}
	std::cout << histogramTab[100] << std::endl;
	return histogramTab;
}

double * ImageClass::smoothHistogram(double * histogramTab, int * MinAndMax)
{
	int length = (MinAndMax[1] - MinAndMax[0]) + 1;

	int iter = 0;
	while (!this->bimodalTest(histogramTab, length)) {
		double previous = 0, current = 0, next = histogramTab[0];
		for (int i = 1; i < length - 1; i++) {
			previous = current;
			current = next;
			next = histogramTab[i + 1];
			histogramTab[i] = (previous + current + next) / 3;
		}

		histogramTab[length - 1] = (current + next) / 3;
		iter++;
		if (iter > 10000) {
			std::cout << "Nie mozna obliczyc" << std::endl;
			return nullptr;
		}
	}
	return histogramTab;
}

bool ImageClass::bimodalTest(double * tab, int length)
{
	bool flag = false;
	int modes = 0;

	for (int i = 1; i < length - 1; i++) {
		if (tab[i - 1] < tab[i] && tab[i + 1] < tab[i]) {
			modes++;
			if (modes > 2)
				return false;
		}
	}
	if (modes == 2)
		return true;
	return flag;
}

int ImageClass::calculateThreshold(double * histogramTab, int * MinAndMax)
{
	int length = (MinAndMax[1] - MinAndMax[0]) + 1;
	int threshold = 0;

	for (int i = 1; i < length - 1; i++) {
		if (histogramTab[i - 1] < histogramTab[i] && histogramTab[i + 1] < histogramTab[i]) {
			threshold += i;
		}
	}

	threshold = (int)floor(threshold / 2.0) + MinAndMax[0];

	return threshold;
}

/*******************************************
PUBLIC METHODS
*******************************************/
ImageClass::ImageClass(const Mat& image, std::string filepath) {
	this->image = image;
	this->orgImage = image.clone();
	this->filepath = filepath;
}

Mat& ImageClass::getImage() {
	return this->image;
}

void ImageClass::showImage() {
	imshow("window", this->image);
}

void ImageClass::showImage(const Mat& tempImage) {
	imshow("Static image", tempImage);
}

void ImageClass::filtrImage() {
	Filter medianFilter(this->image);
	image = medianFilter.filtrMedian();
}

void ImageClass::normalizeImage() {
	Filter normalizer(this->image);
	this->image = normalizer.normalize();
}

void ImageClass::binaryImage() {
	//threshold(this->image, this->image, 0, 255, CV_THRESH_BINARY_INV | CV_THRESH_OTSU);
	Mat histogram = this->calculateHistogram(this->image);
	normalize(histogram, histogram, 0, this->image.rows, NORM_MINMAX, -1, Mat());
	int* MinAndMax = this->getMinAndMax(histogram);
	int length = (MinAndMax[1] - MinAndMax[0]) + 1;
	double* histogramTab = new double[length];
	this->calculateHistogramTab(histogram, MinAndMax, histogramTab);
	std::cout << histogramTab[0] << std::endl;
	std::cout << "BEFORE SMOOTH\n";
	histogramTab = this->smoothHistogram(histogramTab, MinAndMax);
	std::cout << "AFTER SMOOTH\n";
	double threshold = this->calculateThreshold(histogramTab, MinAndMax);
	std::cout << "THRESHOLD: " << threshold;

	Mat temp;
	cv::threshold(this->image, this->image, 5, 255, CV_THRESH_BINARY_INV );
	//cv::threshold(this->image, this->image, threshold / 3, 255, CV_THRESH_BINARY_INV | CV_THRESH_OTSU);
	//cv::threshold(this->image, this->image, threshold/5, 255, CV_THRESH_BINARY_INV);
	//imshow("BINARY TEMP" , temp);
	//imshow("BINARY", this->image);
	delete[] histogramTab;
}

void ImageClass::smoothImage() {
	Filter filter(this->image);
	Mat temp = filter.reduceHoles();
	this->image = temp;
}

void ImageClass::createMask() {
	Filter filter(this->image);
	Mat temp = filter.createMask();
	//this->image = filter.getImage();
	this->fingerMask = temp;
}

void ImageClass::skeletozation() {
	Skeleton skeleton(this->image);
	Mat temp = skeleton.makeSkeleton();
	this->image = temp;
}

void ImageClass::coreDetection(){
	CoreDetection coreDetection(this->image);
	coreDetection.detectCore();
}

void ImageClass::findMinutiae() {
	Minutiae minutiae(this->image, this->fingerMask);
	minutiae.findMinutiae();
	this->endPointsVector = minutiae.getEndPointsVector();
	this->branchPointsVector = minutiae.getBranchPointsVector();
}

void ImageClass::methodI() {
	Method* methodI = new MethodI(this->image, this->filepath);
	methodI->writeToFile(this->endPointsVector, this->branchPointsVector);
	delete methodI;
}

void ImageClass::clasifyMethodI() {
	Classification classification(this->filepath);
	classification.getClassification();
}
