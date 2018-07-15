#include "MethodII.h"

/*******************************************
PRIVATE METHODS
*******************************************/


/*******************************************
PUBLIC METHODS
*******************************************/

MethodII::MethodII(const Mat& image, std::string filepath) {
	this->image = image;
	this->filepath = filepath;
}

void MethodII::writeToFile(const vector<Point2i> endPointsVector, const vector<Point2i> branchPointsVector) {
	std::cout << "IN METHOD" << std::endl;
}
