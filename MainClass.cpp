#include "opencv2/opencv.hpp"

#include <string>

#include "ImageClass.h"

using namespace cv;

int main(int, char**)
{
	std::string filepath = "FingerDatabase/p01/k01.bmp";

	Mat img = imread(filepath, CV_LOAD_IMAGE_GRAYSCALE);
	//Mat img = imread("FingerDatabase/lena.png", CV_LOAD_IMAGE_GRAYSCALE);

	imshow("ORG", img);
	std::cout << "Create ImageClass\n\n";
	ImageClass imageClass = ImageClass(img, filepath);
	imageClass.showImage();
	imageClass.filtrImage();
	imageClass.normalizeImage();
	imageClass.binaryImage();
	imageClass.smoothImage();
	imageClass.createMask();
	imageClass.skeletozation();
	imageClass.findMinutiae();
	imageClass.methodI();

	imageClass.showImage();

	waitKey(0);
	return 0;
}