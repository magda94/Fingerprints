#include "Method.h"
/*******************************************
PROTECTED METHODS
*******************************************/

std::string Method::getFilename()
{
	std::string temp = "";
	std::size_t index = this->filepath.rfind("/");
	
	if (index != std::string::npos) {
		temp = this->filepath.substr(index+1);
		std::size_t index2 = temp.rfind(".");

		if (index2 != std::string::npos) {
			temp = temp.substr(0, index2);
		}
	}
	return temp;
}

std::string Method::getFolder()
{
	std::string temp = "";
	std::size_t index = this->filepath.rfind("/");

	if (index != std::string::npos) {
		temp = this->filepath.substr(0, index+1);
	}

	return temp;
}

void Method::writeEndsToFile(ofstream* file, const vector<Point2i> endPointsVector) {
	for (int i = 0; i < endPointsVector.size(); i++) {
		*file << "1\t" << endPointsVector.at(i).x << "\t" << endPointsVector.at(i).y << "\t" << std::endl;
	}
}

void Method::writeBranchToFile(ofstream * file, const vector<Point2i> branchPointsVector) {
	for (int i = 0; i < branchPointsVector.size(); i++) {
		*file << "2\t" << branchPointsVector.at(i).x << "\t" << branchPointsVector.at(i).y << "\t" << std::endl;
	}
}

/*******************************************
PUBLIC METHODS
*******************************************/
void Method::writeToFile(const vector<Point2i> endPointsVector, const vector<Point2i> branchPointsVector) {
	this->filename = this->getFilename();
	std::string folder = this->getFolder();


	ofstream file;
	file.open(folder + this->filename + "M1.txt");
	this->writeEndsToFile(&file, endPointsVector);
	file << "\n\n\n";
	this->writeBranchToFile(&file, branchPointsVector);
	file.close();
}
