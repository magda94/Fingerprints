#include "ClassificationI.h"
/*******************************************
PRIVATE METHODS
*******************************************/


std::string ClassificationI::getFilename() {
	std::string temp = "";
	
	std::size_t index = this->filepath.find_last_of(".");
	temp = this->filepath.substr(0, index);
	return temp;
}

std::string ClassificationI::getRootFolder() {
	std::string temp = "";
	std::size_t index = this->filepath.find_last_of("/");
	
	temp = this->filepath.substr(0, index );
	index = temp.find_last_of("/");

	return temp;
}

std::string ClassificationI::getFingerName() {
	std::string temp = "";
	std::size_t index = this->filepath.rfind("/");

	if (index != std::string::npos) {
		temp = this->filepath.substr(index + 1);
		std::size_t index2 = temp.rfind(".");

		if (index2 != std::string::npos) {
			temp = temp.substr(0, index2);
		}
	}
	return temp;
}

void ClassificationI::getSampleInformation() {
	ifstream sampleFile;
	string sampleName = this->filename + "M1.txt";

	sampleFile.open(sampleName);
	int x, y, type;

	while (sampleFile >> type >> x >> y) {
		this->sample.push_back(MinutiaeInformationI(x, y, type));
	}

	sampleFile.close();

}


int ClassificationI::compareToFinger(vector<MinutiaeInformationI> otherFinger) {
	int counter = 0;

	int windowSize = 9;

	for (int i = 0; i < this->sample.size(); i++) {
		int index = this->findTheLowest(otherFinger, this->sample.at(i));

		int patternX = otherFinger.at(index).getX();
		int patternY = otherFinger.at(index).getY();
		int sampleX = this->sample.at(i).getX();
		int sampleY = this->sample.at(i).getY();

		int deltaX = patternX - sampleX;
		int deltaY = patternY - sampleY;

		if (deltaX < windowSize && deltaY < windowSize) {
			counter++;
		}
	}

	return counter;
}

int ClassificationI::findTheLowest(vector<MinutiaeInformationI> otherFinger, MinutiaeInformationI minutiae)
{
	double result = std::numeric_limits<double>::max();
	int index = 0;

	for (int i = 0; i < otherFinger.size(); i++) {
		if (otherFinger.at(i).getType() == minutiae.getType()) {
			double tempX = pow(otherFinger.at(i).getX() - minutiae.getX(), 2);
			double tempY = pow(otherFinger.at(i).getX() - minutiae.getX(), 2);

			double temp = sqrt(tempX + tempY);

			if (temp < result) {
				index = i;
				result = temp;
			}
		}
	}

	return index;
}

bool ClassificationI::isTrueGroup() {
	
	this->readPattern();

	int counter = this->compareToFinger(this->pattern);

	if (counter >= 12) {
		return true;
	}

	return false;
}

void ClassificationI::readPattern() {
	std::string patternPath =  this->verifyPath + "M1.txt";
	ifstream patternFile;
	patternFile.open(patternPath);
	int x, y, type;

	while (patternFile >> type >> x >> y) {
		this->pattern.push_back(MinutiaeInformationI(x, y, type));
	}

	patternFile.close();
}

void ClassificationI::showResult(bool result) {
	std::cout << "FILE: " << this->filename << std::endl;
	std::cout << "VERIFYFILE: " << this->verifyPath << std::endl;


	if (result)
		std::cout << "THIS IS PERSON " << this->verifyPath << std::endl;
	else
		std::cout << "THIS IS NOT PERSON " << this->verifyPath << std::endl;
}

/*******************************************

PUBLIC METHODS
*******************************************/
ClassificationI::ClassificationI(string filepath, string verifyPath) {
	this->filepath = filepath;
	this->verifyPath = verifyPath;
}

void ClassificationI::getClassification() {
	this->filename = getFilename();
	this->fingerName = getFingerName();
	this->rootPath = getRootFolder();
	
	this->getSampleInformation();

	this->showResult(this->isTrueGroup());
}
