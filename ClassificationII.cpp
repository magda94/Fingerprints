#include "ClassificationII.h"

/*******************************************

PRIVATE METHODS
*******************************************/

std::string ClassificationII::getFilename() {
	std::string temp = "";

	std::size_t index = this->filepath.find_last_of(".");
	temp = this->filepath.substr(0, index);
	return temp;
}

std::string ClassificationII::getRootFolder() {
	std::string temp = "";
	std::size_t index = this->filepath.find_last_of("/");

	temp = this->filepath.substr(0, index);
	index = temp.find_last_of("/");

	return temp;
}

std::string ClassificationII::getFingerName() {
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

void ClassificationII::getSampleInformation() {
	ifstream sampleFile;
	string sampleName = this->filename + "M2.txt";

	sampleFile.open(sampleName);
	int r, phi, type;

	while (sampleFile >> type >> r >> phi) {
		this->sample.push_back(MinutiaeInformationII(type, r, phi));
	}

	sampleFile.close();

}


int ClassificationII::compareToFinger(vector<MinutiaeInformationII> otherFinger) {
	int counter = 0;

	int windowSize = 50;

	std::cout << "FILE SIZE " << this->pattern.size() <<std::endl;
	for (int i = 0; i < this->sample.size(); i++) {
		int index = this->findTheLowest(otherFinger, this->sample.at(i));
		std::cout << "INDEX " << index << std::endl;

		int patternR = otherFinger.at(index).getR();
		int patternPhi = otherFinger.at(index).getPhi();
		int sampleR = this->sample.at(i).getR();
		int samplePhi = this->sample.at(i).getPhi();

		int deltaR = patternR - sampleR;

		if (deltaR < windowSize && patternPhi == samplePhi) {
			counter++;
		}
	}

	return counter;
}

int ClassificationII::findTheLowest(vector<MinutiaeInformationII> otherFinger, MinutiaeInformationII minutiae)
{
	double result = std::numeric_limits<double>::max();
	int index = 0;

	for (int i = 0; i < otherFinger.size(); i++) {
		if (otherFinger.at(i).getType() == minutiae.getType()) {
			
			double temp = abs(otherFinger.at(i).getR() - minutiae.getR());

			if (temp < result) {
				index = i;
				result = temp;
			}
		}
	}

	return index;
}

bool ClassificationII::isTrueGroup() {

	this->readPattern();

	int counter = this->compareToFinger(this->pattern);

	if (counter >= 12) {
		return true;
	}

	return false;
}

void ClassificationII::readPattern() {
	std::string patternPath = this->verifyPath + "M2.txt";
	std::cout << "PATTERN " << patternPath << std::endl;
	ifstream patternFile;
	patternFile.open(patternPath);
	int r, phi, type;

	while (patternFile >> type >> r >> phi) {
		std::cout << "HFJFH" << std::endl;
		this->pattern.push_back(MinutiaeInformationII(r, phi, type));
	}

	patternFile.close();
}

void ClassificationII::showResult(bool result) {
	std::cout << "\nMETHOD 2: " << std::endl;
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
ClassificationII::ClassificationII(string filepath, string verifyPath) {
	this->filepath = filepath;
	this->verifyPath = verifyPath;
}

void ClassificationII::getClassification() {
	this->filename = getFilename();
	this->fingerName = getFingerName();
	this->rootPath = getRootFolder();

	this->getSampleInformation();

	this->showResult(this->isTrueGroup());
}
