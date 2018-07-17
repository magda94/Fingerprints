#ifndef METHODII_HH
#define METHODII_HH

#include <vector>
#include <cmath>

#include "Method.h"
#include "MinutiaeInformationII.h"

using namespace cv;
using namespace std;

# define M_PI           3.14159265358979323846  /* pi */

class MethodII : public Method {
private:
	vector<MinutiaeInformationII> minutiaeInformation;

	std::string getFingerFolder();
	void getXYCoordinates(std::string fingerToFind, int* tabXY);
	void readFromFile(std::string fingerToFind, int* tabXY);
	double countR(int x, int y);
	double countPhi(int x, int y);
	void coordinatesToFile(std::string fingerToFind);
public:
	MethodII(const Mat& image, std::string filepath);
	void writeToFile(const vector<Point2i> endPointsVector, const vector<Point2i> branchPointsVector);
};

#endif 
