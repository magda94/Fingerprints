#include "opencv2/opencv.hpp"

#include "ImageClass.h"

using namespace cv;

int main(int, char**)
{
	Mat img = imread("FingerDatabase/p01/k01.bmp", CV_LOAD_IMAGE_GRAYSCALE);
	//Mat img = imread("FingerDatabase/lena.png", CV_LOAD_IMAGE_GRAYSCALE);

	imshow("ORG", img);
	std::cout << "Create ImageClass\n\n";
	ImageClass imageClass = ImageClass(img);
	imageClass.showImage();
	imageClass.filtrImage();
	imageClass.normalizeImage();
	imageClass.binaryImage();
	imageClass.smoothImage();
	imageClass.skeletozation();
	imageClass.findMinutiae();

	imageClass.showImage();

	waitKey(0);
	return 0;
}