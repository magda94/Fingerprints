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

	//according to Point(y,x) y->x and x->y, change the place
	tabXY[0] = y;
	tabXY[1] = x;
}

void MethodII::readFromFile(std::string fingerToFind, int* tabXY) {
	ifstream file;
	std::string fingerToFindPath = "FingerDatabase/" + fingerToFind + "M1.txt";

	file.open(fingerToFindPath);

	int type, x, y;

	while (file >> type >> x >> y) {
		int newX = x - tabXY[0];
		int newY = y - tabXY[1];

		double r = this->countR(newX, newY);
		if (r != 0) {
			double phi = this->countPhi(newX, newY);
			MinutiaeInformationII minutiaeInfo(type, r, phi);
			this->minutiaeInformation.push_back(minutiaeInfo);
		}
	}

	file.close();
}

double MethodII::countR(int x, int y) {
	double xPow = pow(x, 2);
	double yPow = pow(y, 2);
	double r = sqrt(xPow + yPow);
	return r;
}


//x->rows y->cols (change place) in function just changed
double MethodII::countPhi(int y, int x) {
	if (x > 0 && y >= 0) {
		return atan(y / x);
	}
	else if (x > 0 && y < 0) {
		return atan(y / x) + 2 * M_PI;
	}
	else if (x < 0) {
		return atan(y / x) + M_PI;
	}
	else if (x == 0 && y > 0) {
		return M_PI / 2;
	}
	else if (x == 0 && y < 0) {
		return (3 * M_PI) / 2;
	}
}

void MethodII::coordinatesToFile(std::string fingerToFind) {
	ofstream file;
	std::string fileToWritePath = "FingerDatabase/" + fingerToFind + "M2.txt";
	file.open(fileToWritePath);

	bool flag = false;
	bool isSpaceAdded = false;

	for (int i = 0; i < this->minutiaeInformation.size(); i++) {
		if (minutiaeInformation.at(i).getType() == 2 && flag == false) {
			flag = true;
		}

		if (flag == true && isSpaceAdded == false) {
			isSpaceAdded = true;
			file << "\n\n\n";
		}

		file << minutiaeInformation.at(i).getType() << "\t" << minutiaeInformation.at(i).getR() << "\t" << minutiaeInformation.at(i).getPhi() << std::endl;
	}

	file.close();
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
		std::cout << "NIE MA RDZENIA. NIE MOZNA UZYC METODY 2!" << std::endl;
		goto endOfFunction;
	}

	this->readFromFile(fingerToFind, tabXY);

	this->coordinatesToFile(fingerToFind);
	
	//etykieta end of function
	endOfFunction:
	delete[] tabXY;
}
