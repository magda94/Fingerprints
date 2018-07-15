#include "opencv2/opencv.hpp"

#include <string>

#include "ImageClass.h"

using namespace cv;

int main(int, char**)
{
	std::string filepath = "FingerDatabase/p09/k03.bmp";

	Mat img = imread(filepath, CV_LOAD_IMAGE_GRAYSCALE);

	imshow("ORG", img);
	std::cout << "Create ImageClass\n\n";
	ImageClass imageClass = ImageClass(img, filepath);
	imageClass.showImage();
	//imageClass.normalizeImage();
	imageClass.filtrImage();
	imageClass.binaryImage();
	imageClass.smoothImage();
	imageClass.createMask();
	imageClass.skeletozation();
	imageClass.drawCore();
	//imageClass.coreDetection();
	imageClass.findMinutiae();
	imageClass.methodI();
	imageClass.methodII();
	//imageClass.clasifyMethodI();

	imageClass.showImage();

	waitKey(0);
	return 0;
}