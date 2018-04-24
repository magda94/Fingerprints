#ifndef MINUTIAE_HH
#define MINUTIAE_HH

#include "opencv2/opencv.hpp"
#include <vector>

using namespace cv;
using namespace std;

class Minutiae {
private:
	Mat image;
	Mat fingerMask;
	int** fingerEdge;
	bool checkIfEnd(int x, int y);
	bool checkIfBrach(int x, int y);
	bool checkIfInEdges(Point2i point);
	vector<Point2i> roundPoints(const vector<Point2i> pointsVector);
	void findEdges();
	vector<Point2i> reduceMinutiaes(const vector<Point2i> pointsVector);
	void printMinutiae(const vector<Point2i> endPointsVector, const vector<Point2i> branchPointsVector);
public:
	Minutiae(const Mat& image, const Mat& fingerMask);
	~Minutiae();
	void findMinutiae();
};

#endif