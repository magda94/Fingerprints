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
			if (distance < 7) {
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

vector<Point2i> Minutiae::findTrueEnds(const vector<Point2i> endPointsVector) {
	vector<Point2i> newPointsVector;
	
	for (int i = 0; i < endPointsVector.size(); i++) {
		bool flag = this->checkIfTrueEnd(endPointsVector.at(i).x, endPointsVector.at(i).y);
		if (flag) {
			//std::cout << "TRUE" << std::endl;
			newPointsVector.push_back(endPointsVector.at(i));
		}
	}
	
	return newPointsVector;
}

vector<Point2i> Minutiae::reduceHolesEndings(const vector<Point2i> endPointsVector)
{
	vector<Point2i> newPointsVector;
	for (int i = 0; i < endPointsVector.size(); i++) {
		bool flag = this->checkIfHole(endPointsVector.at(i).x, endPointsVector.at(i).y);
		if (flag) {
			newPointsVector.push_back(endPointsVector.at(i));
		}
	}
	return newPointsVector;
}

bool Minutiae::checkIfTrueEnd(int x, int y) {
	int windowSize = 21;
	bool flag = false;
	//allocate
	int** windowTab = new int*[windowSize];
	for (int i = 0; i < windowSize; i++) {
		windowTab[i] = new int[windowSize];
	}
	//initialize
	for (int i = 0; i < windowSize; i++) {
		for (int j = 0; j < windowSize; j++) {
			windowTab[i][j] = 0;
		}
	}

	int center = (windowSize - 1) / 2;
	windowTab[center][center] = -1;

	this->checkNeighborhoodInWindow(x, y, center, center, windowSize, windowTab);

	flag = this->checkEndWindowTable(windowSize, windowTab);

	//print table
	/*std::cout << "\n\n\nNEW TABLE" << std::endl;
	for (int i = 0; i < windowSize; i++) {
		for (int j = 0; j < windowSize; j++) {
			std::cout << windowTab[i][j] << "\t";
		}
		std::cout << std::endl;
	}*/

	//delocate
	for (int i = 0; i < windowSize; i++) {
		delete windowTab[i];
	}
	delete windowTab;
	return flag;
}

bool Minutiae::checkEndWindowTable(int windowSize, int** windowTable) {
	bool flag = false;

	int counter = 0;


	//up and bottom
	for (int j = 0; j < windowSize; j++) {
		if (windowTable[0][j] == 1) {
			counter++;
		}
		if (windowTable[windowSize - 1][j] == 1) {
			counter++;
		}
	}

	//left and right
	for (int i = 0; i < windowSize; i++) {
		if (windowTable[i][0] == 1) {
			counter++;
		}
		if (windowTable[i][windowSize - 1] == 1) {
			counter++;
		}
	}

	/*std::cout << "\n\n\nNEW TABLE" << std::endl;
	for (int i = 0; i < windowSize; i++) {
		for (int j = 0; j < windowSize; j++) {
			std::cout << windowTable[i][j] << "\t";
		}
		std::cout << std::endl;
	}
	std::cout << "COUNTER:" << counter << std::endl;*/
	if (counter == 1)
		flag = true;

	return flag;
}

bool Minutiae::checkIfHole(int x, int y)
{
	bool flag = true;
	int windowSize = 1;
	Point2i linePlace;
	Point2i oppositePlace;
	
	//search where is line
	for (int i = -1; i <= 1; i++) {
		for (int j = -1; j <= 1; j++) {
			if (i == 0 && j == 0)
				continue;
			if ((int)this->image.at<uchar>(x + i, y + j) == 0) {
				linePlace.x = x + i;
				linePlace.y = y + j;
			}
		}
	}

	//set opopsite
	if (linePlace.x < x) {
		oppositePlace.x = x + 1;
	}
	else if (linePlace.x > x) {
		oppositePlace.x = x - 1;
	}
	else {
		oppositePlace.x = x;
	}

	if (linePlace.y < y) {
		oppositePlace.y = y + 1;
	}
	else if (linePlace.y > y) {
		oppositePlace.y = y - 1;
	}
	else {
		oppositePlace.y = y;
	}

	//std::cout << "LINEPLACE: " << linePlace << std::endl;
	//std::cout << "OPPOSITEPLACE:" << oppositePlace << std::endl;
	//check if is black pixel
	for (int i = -windowSize; i <= windowSize; i++) {
		for (int j = -windowSize; j <= windowSize; j++) {
			if (oppositePlace.x+i== linePlace.x && oppositePlace.y+j == linePlace.y)
				continue;
			if (oppositePlace.x+i == x && oppositePlace.y+j == y)
				continue;
			if ((int)this->image.at<uchar>(oppositePlace.x + i, oppositePlace.y + j) == 0) {
				flag = false;
			}
		}
	}
	return flag;
}

void Minutiae::checkNeighborhoodInWindow(int orgX, int orgY, int x, int y, int windowSize, int** windowTab) {
	for (int i = -1; i <= 1; i++) {
		for (int j = -1; j <= 1; j++) {
			if (i == 0 && j == 0) {
				continue;
			}
			if (orgX < 0 || orgX >= this->image.rows || orgY < 0 || orgY >= this->image.cols) {
				continue;
			}
			if (x + i == windowSize || y + j == windowSize || x + i < 0 || y + j < 0) {
				continue;
			}
			if ((int)this->image.at<uchar>(orgX + i, orgY + j) == 0 && windowTab[x+i][y+j] == 0) {
				int newOrgX = orgX + i;
				int newOrgY = orgY + j;
				windowTab[x + i][y + j] = 1;
				//std::cout << "IN" << std::endl;
				if(x+i < windowSize || y+j < windowSize || x+i >= 0 || y+j >= 0)
					checkNeighborhoodInWindow(newOrgX, newOrgY, x+i, y+j, windowSize, windowTab);
			}
		}
	}
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

	/*for (int i = 0; i < branchPointsVector.size(); i++) {
		int x = branchPointsVector[i].x;
		int y = branchPointsVector[i].y;
		circle(temp, Point(y, x), 5, Scalar(255, 0, 0));
	}*/

	imshow("TEMP", temp);
}

void Minutiae::setEndPointsVector(vector<Point2i> endPointsVector) {
	this->endPointsVectorFinal = endPointsVector;
}

void Minutiae::setBranchPointsVector(vector<Point2i> branchPointsVector) {
	this->branchPointsVectorFinal = branchPointsVector;
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

	//branchPointsVector = this->roundPoints(branchPointsVector);

	endPointsVector = this->reduceHolesEndings(endPointsVector);

	endPointsVector = this->findTrueEnds(endPointsVector);

	std::cout << "ENDS SIZE: " << endPointsVector.size() << std::endl;
	std::cout << "BRANCH SIZE: " << branchPointsVector.size() << std::endl;

	this->findEdges();

	endPointsVector = this->reduceMinutiaes(endPointsVector);
	branchPointsVector = this->reduceMinutiaes(branchPointsVector);

	endPointsVector = this->roundPoints(endPointsVector);

	std::cout << "ENDS SIZE: " << endPointsVector.size() << std::endl;
	std::cout << "BRANCH SIZE: " << branchPointsVector.size() << std::endl;

	this->printMinutiae(endPointsVector, branchPointsVector);

	this->setEndPointsVector(endPointsVector);
	this->setBranchPointsVector(branchPointsVector);
}

vector<Point2i> Minutiae::getEndPointsVector()
{
	return this->endPointsVectorFinal;
}

vector<Point2i> Minutiae::getBranchPointsVector()
{
	return this->branchPointsVectorFinal;
}

