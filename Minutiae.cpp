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

bool Minutiae::checkIfInEdges(Point2i point) {
	int x = point.x;
	int y = point.y;
	//std::cout << "LEFT: " << fingerEdge[x][0] << " RIGHT: " << fingerEdge[x][1] << std::endl;
	if (this->fingerEdge[x][0] < y && this->fingerEdge[x][1] > y) {
		return true;
	}
	return false;
}

vector<Point2i> Minutiae::roundPoints(const vector<Point2i> pointsVector) {
	vector<Point2i> newPointsVector;
	for (int i = 0; i < pointsVector.size(); i++) {
		Point2i searchPoint = pointsVector[i];
		bool flag = false;
		for (int j = 0; j < newPointsVector.size(); j++) {
			Point2i tempPoint = newPointsVector[j];
			double distance = sqrt(pow(searchPoint.x - tempPoint.x,2) + pow(searchPoint.y - tempPoint.y,2));
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

void Minutiae::findEdges() {
	for (int i = 0; i < this->fingerMask.rows; i++) {
		bool flag = false;
		int j = 0;
		//left
		while (!flag && j<this->fingerMask.cols) {
			if ((int)this->fingerMask.at<uchar>(i, j) == 0) {
				flag = true;
				this->fingerEdge[i][0] = j;
			}
			j++;
		}
		//there is no black pixels
		if (!flag) {
			this->fingerEdge[i][0] = -1;
		}
		//right
		j = this->fingerMask.cols - 1;
		flag = false;
		while (!flag && j >= 0) {
			if ((int)this->fingerMask.at<uchar>(i, j) == 0) {
				flag = true;
				this->fingerEdge[i][1] = j;
			}
			j--;
		}
		if (!flag) {
			this->fingerEdge[i][1] = -1;
		}
	}

}

vector<Point2i> Minutiae::reduceMinutiaes(const vector<Point2i> pointsVector) {
	vector<Point2i> newPointsVector;
	for (int i = 0; i < pointsVector.size(); i++) {
		bool flag = checkIfInEdges(pointsVector[i]);
		if (flag) {
			newPointsVector.push_back(pointsVector[i]);
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
Minutiae::Minutiae(const Mat& image, const Mat& fingerMask) {
	this->image = image;
	this->fingerMask = fingerMask;

	this->fingerEdge = new int*[this->image.rows];
	for (int i = 0; i < this->image.rows; i++) {
		this->fingerEdge[i] = new int[2];
	}
}

Minutiae::~Minutiae() {
	std::cout << "DESTRUKTOR" << std::endl;
	for (int i = 0; i < this->image.rows; i++)
		delete[] this->fingerEdge[i];
	delete[] this->fingerEdge;
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

	this->findEdges();

	endPointsVector = this->reduceMinutiaes(endPointsVector);
	branchPointsVector = this->reduceMinutiaes(branchPointsVector);

	std::cout << "ENDS SIZE: " << endPointsVector.size() << std::endl;
	std::cout << "BRANCH SIZE: " << branchPointsVector.size() << std::endl;

	this->printMinutiae(endPointsVector, branchPointsVector);
}