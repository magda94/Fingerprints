#include "opencv2/opencv.hpp"

#include "ImageClass.h"

using namespace cv;

int main(int, char**)
{
	Mat img = imread("FingerDatabase/p01/s01.bmp", CV_LOAD_IMAGE_GRAYSCALE);

	std::cout << "Create ImageClass\n\n";
	ImageClass imageClass = ImageClass(img);
	imageClass.showImage();
	imageClass.filtrImage();
	imageClass.binaryImage();

	waitKey(0);
	return 0;
}