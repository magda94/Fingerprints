#include "opencv2/opencv.hpp"
using namespace cv;

int main(int, char**)
{
	Mat img = imread("FingerDatabase/p01/k01.bmp", CV_LOAD_IMAGE_COLOR);

	imshow("window",img);
	waitKey(0);
	return 0;
}