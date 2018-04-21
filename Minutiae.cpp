#include "Minutiae.h"
/*******************************************
PRIVATE METHODS
*******************************************/
bool Minutiae::checkIfEnd(int x, int y) {
	int counter = 0;
	for (int i = -1; i <= 1; i++) {
		for (int j = -1; j <= 1; j++) {
			if (i == 0 && j == 0)
				continue;
			if ((int)this->image.at<uchar>(x + i, y + j) == 0) {
				counter++;
			}
		}
	}
	if (counter == 1) {
		return true;
	}
	return false;
}

bool Minutiae::checkIfBrach(int x, int y) {
	int counter = 0;
	for (int i = -1; i <= 1; i++) {
		for (int j = -1; j <= 1; j++) {
			if (i == 0 && j == 0)
				continue;
			if ((int)this->image.at<uchar>(x + i, y + j) == 0) {
				counter++;
			}
		}
	}
	if (counter == 3) {
		return true;
	}
	return false;
}

vector<Point2i> Minutiae::roundPoints(const vector<Point2i> pointsVector) {
	vector<Point2i> newPointsVector;
	for (int i = 0; i < pointsVector.size(); i++) {
		Point2i searchPoint = pointsVector[i];
		/*if (newPointsVector.size() == 0) {
			newPointsVector.push_back(searchPoint);
			continue;
		}*/
		bool flag = false;
		for (int j = 0; j < newPointsVector.size(); j++) {
			Point2i tempPoint = newPointsVector[j];
			double distance = sqrt(pow(searchPoint.x - tempPoint.x,2) + pow(searchPoint.y - tempPoint.y,2));
			//std::cout << "DISTANCE : " << distance << std::endl;
			if (distance < 15) {
				flag = true;
			}
		}
		if (!flag) {
			newPointsVector.push_back(searchPoint);
		}
	}

	return newPointsVector;
}

void Minutiae::printMinutiae(const vector<Point2i> endPointsVector, const vector<Point2i> branchPointsVector) {
	Mat temp;
	temp = this->image.clone();
	cvtColor(temp, temp, CV_GRAY2BGR);

	for (int i = 0; i < endPointsVector.size(); i++) {
		int x = endPointsVector[i].x;
		int y = endPointsVector[i].y;
		circle(temp, Point(y, x), 5, Scalar(0, 0, 255));//BGR
	}

	for (int i = 0; i < branchPointsVector.size(); i++) {
		int x = branchPointsVector[i].x;
		int y = branchPointsVector[i].y;
		circle(temp, Point(y, x), 5, Scalar(255, 0, 0));
	}

	imshow("TEMP", temp);
}



/*******************************************
PUBLIC METHODS
*******************************************/
Minutiae::Minutiae(const Mat& image) {
	this->image = image;
}

void Minutiae::findMinutiae() {
	vector<Point2i> endPointsVector;
	vector<Point2i> branchPointsVector;

	for (int i = 1; i < this->image.rows - 1; i++) {
		for (int j = 1; j < this->image.cols - 1; j++) {
			if ((int)this->image.at<uchar>(i, j) == 0) {
				bool flag = this->checkIfEnd(i, j);
				if (flag) {
					endPointsVector.push_back(Point2i(i, j));
				}
				flag = this->checkIfBrach(i, j);
				if (flag) {
					branchPointsVector.push_back(Point2i(i, j));
				}
			}
		}
	}

	std::cout << "ENDS SIZE: " << endPointsVector.size()<<std::endl;
	std::cout << "BRANCH SIZE: " << branchPointsVector.size() << std::endl;

	endPointsVector = this->roundPoints(endPointsVector);
	branchPointsVector = this->roundPoints(branchPointsVector);

	std::cout << "ENDS SIZE: " << endPointsVector.size() << std::endl;
	std::cout << "BRANCH SIZE: " << branchPointsVector.size() << std::endl;

	this->printMinutiae(endPointsVector, branchPointsVector);
}