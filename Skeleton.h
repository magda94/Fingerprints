#ifndef SKELETON_HH
#define SKELETON_HH

#include "opencv2/opencv.hpp"
using namespace cv;

class Skeleton {
private:
	Mat image;
	int tab0[3][3] = { {2,0,2}, {2,1,2}, {1,1,1} };
	int tab90[3][3] = { {1,2,2}, {1,1,0}, {1,2,2} };
	int tab180[3][3] = { {1,1,1}, {2,1,2}, {2,0,2} };
	int tab270[3][3] = { {2,2,1}, {0,1,1}, {2,2,1} };

	bool check0(int x, int y);
	bool check90(int x, int y);
	bool check180(int x, int y);
	bool check270(int x, int y);
public:
	Skeleton(const Mat& image);
	Mat makeSkeleton();
};

#endif