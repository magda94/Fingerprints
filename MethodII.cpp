#include "MethodII.h"

/*******************************************
PRIVATE METHODS
*******************************************/
std::string MethodII::getFingerFolder() {
	std::string temp = "";
	std::size_t index = this->filepath.find("/");

	if (index != std::string::npos) {
		temp = this->filepath.substr(index, index + 1);


		temp = temp.substr(1, 3);
	}

	return temp;
}

void MethodII::getXYCoordinates(std::string fingerToFind, int * tabXY) {
	ifstream newFile;
	newFile.open("FingerDatabase/movedCores.data");

	std::string name;
	int x, y;

	bool flag = false;

	while (newFile >> name >> x >> y) {
		if (name == fingerToFind) {
			flag = true;
			break;
		}
	}
	
	if (!flag) {
		std::cout << "There is no finger" << std::endl;
	}

	tabXY[0] = x;
	tabXY[1] = y;
}

/*******************************************
PUBLIC METHODS
*******************************************/

MethodII::MethodII(const Mat& image, std::string filepath) {
	this->image = image;
	this->filepath = filepath;
}

void MethodII::writeToFile(const vector<Point2i> endPointsVector, const vector<Point2i> branchPointsVector) {
	std::string fingerToFind = this->getFingerFolder() + "/" + this->getFilename();
	int* tabXY = new int(2);

	this->getXYCoordinates(fingerToFind, tabXY);

	if (tabXY[0] == -1 && tabXY[1] == -1) {
		std::cout << "There is no core point. Cannot do methodII" << std::endl;
		goto endOfFunction;
	}
	
	//etykieta end of function
	endOfFunction:
	delete[] tabXY;
}
