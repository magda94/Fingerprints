#include "Classification.h"
/*******************************************
PRIVATE METHODS
*******************************************/


std::string Classification::getFilename() {
	std::string temp = "";
	
	std::size_t index = this->filepath.find_last_of(".");
	temp = this->filepath.substr(0, index);
	return temp;
}

std::string Classification::getRootFolder() {
	std::string temp = "";
	std::size_t index = this->filepath.find_last_of("/");
	
	temp = this->filepath.substr(0, index );
	index = temp.find_last_of("/");

	return temp;
}

std::string Classification::getFingerName() {
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

void Classification::getSampleInformation() {
	ifstream sampleFile;
	string sampleName = this->filename + ".txt";

	sampleFile.open(sampleName);
	int x, y, type;

	while (sampleFile >> type >> x >> y) {
		this->sample.push_back(MinutiaeInformation(x, y, type));
	}

	sampleFile.close();

}

void Classification::compareToOther() {
	for (int i = 1; i <= this->patternCount; i++) {
		vector<MinutiaeInformation> otherFinger;
		ifstream otherFile;
		stringstream intStream;
		intStream << i;
		string otherName;
		if (i > 9) {
			otherName = this->rootPath.substr(0, this->rootPath.length() - 2) + intStream.str();
		}
		else
		{
			otherName = this->rootPath.substr(0, this->rootPath.length() - 1) + intStream.str();
		}

		otherName += "/" + this->fingerName + ".txt";
		
		otherFile.open(otherName);

		int x, y, type;

		while (otherFile >> type >> x >> y) {
			otherFinger.push_back(MinutiaeInformation(x, y, type));
		}

		this->compareResults.push_back(this->compareToFinger(otherFinger));

		otherFile.close();
	}
}

double Classification::compareToFinger(const vector<MinutiaeInformation> otherFinger) {
	double result = 0.0;

	return result;
}

void Classification::showResult() {
	std::cout << "FILEPATH: " << this->filepath << std::endl;
	std::cout << "FINGERNAME: " << this->fingerName << std::endl;
	std::cout << "FILE: " << this->filename << std::endl;
	std::cout << "FOLDER: " << this->rootPath << std::endl;

	std::cout << "\n\nRESULTS: " << std::endl;
	for (int i = 0; i < this->compareResults.size(); i++) {
		std::cout << "FINGER " << i + 1 << ": " << this->compareResults.at(i) << std::endl;
	}
}

/*******************************************
PUBLIC METHODS
*******************************************/
Classification::Classification(string filepath) {
	this->filepath = filepath;
}

void Classification::getClassification() {
	this->filename = getFilename();
	this->fingerName = getFingerName();
	this->rootPath = getRootFolder();
	
	this->getSampleInformation();
	this->compareToOther();

	this->showResult();
}
