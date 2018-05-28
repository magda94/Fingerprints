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
	vector<Point2i> endPointsVectorFinal;
	vector<Point2i> branchPointsVectorFinal;
	int** fingerEdge;


	bool checkIfEnd(int x, int y);
	bool checkIfBrach(int x, int y);
	bool checkIfInEdges(Point2i point);
	vector<Point2i> roundPoints(const vector<Point2i> pointsVector);
	void findEdges();
	vector<Point2i> reduceMinutiaes(const vector<Point2i> pointsVector);
	vector<Point2i> findTrueEnds(const vector<Point2i> endPointsVector);
	vector<Point2i> reduceHolesEndings(const vector<Point2i> endPointsVector);
	bool checkIfTrueEnd(int x, int y);
	void checkNeighborhoodInWindow(int orgX, int orgY, int x, int y, int windowSize, int** windowTab);
	bool checkEndWindowTable(int winodwSize, int** windowTable);
	bool checkIfHole(int x, int y);
	void printMinutiae(const vector<Point2i> endPointsVector, const vector<Point2i> branchPointsVector);
	void setEndPointsVector(vector<Point2i> endPointsVector);
	void setBranchPointsVector(vector<Point2i> branchPointsVector);
public:
	Minutiae(const Mat& image, const Mat& fingerMask);
	~Minutiae();
	void findMinutiae();
	vector<Point2i> getEndPointsVector();
	vector<Point2i> getBranchPointsVector();
};

#endif