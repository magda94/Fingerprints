#include "opencv2/opencv.hpp"

#include <string>

#include "ImageClass.h"

using namespace cv;

void doAction(std::string searchPath, std::string fingerPath);
void mainMenu();
std::string createPath(std::string personNumber, char finger);
char getFingerCharacter();
void actionController(char action);

int main(int, char**)
{

	char choosedAction;

	do {
		mainMenu();
		std::cin >> choosedAction;
		choosedAction = toupper(choosedAction);

		actionController(choosedAction);
		waitKey(0);

	} while (choosedAction != 'Q');

	return 0;
}

void mainMenu() {
	std::cout << "----------WERYFIKACJA ODCISKOW PALCA----------" << std::endl;
	std::cout << "- W - zweryfikuj osobe                       -" << std::endl;
	std::cout << "- Q - koniec programu                        -" << std::endl;
	std::cout << "----------------------------------------------" << std::endl;
}

std::string createPath(std::string personNumber, char finger) {
	std::string path = "FingerDatabase/p";
	if (personNumber.size() == 1 && (stoi(personNumber) >= 1 && stoi(personNumber) <= 9)) {
		return path + "0" + personNumber + "/" + finger + "01";
	}
	return path + personNumber + "/" + finger + "01";
}

char getFingerCharacter() {
	char finger;
	char fingerCharacters[3] = { 'k', 'w', 's' };

	while (true) {
		std::cout << "Podaj palec (k - kciuk, w - wskazujacy, s - srodkowy): ";
		std::cin >> finger;

		for (int i = 0; i < 3; i++) {
			if (fingerCharacters[i] == finger)
				return finger;
		}
		std::cout << std::endl;
		std::cout << "PODALES ZLY PALEC " << std::endl;
		//system("pause");
		//system("cls");
	}

}

void actionController(char action) {
	std::string personNumber;
	std::string searchPath;

	switch (toupper(action)) {

	case 'W': {
		std::cout << "Podej sciezke weryfikowanego odcisku: ";
		std::cin >> searchPath;
		std::cout << "Podaj numer osoby (np. 01, 02,...): ";
		std::cin >> personNumber;

		std::string verifyPath = createPath(personNumber, getFingerCharacter());

		std::cout << "VERIFY PATH: " << verifyPath << std::endl;
		doAction(searchPath, verifyPath);

		break;
	}

	case 'Q': {
		exit(0);
	}

	default: {
		std::cout << std::endl;
		std::cout << "ZLA OPCJA " << std::endl;
		system("pause");
		system("cls");
	}

	}
	//system("pause");
	//system("cls");
}

void doAction(std::string searchPath, std::string verifyPath) {
	std::string filepath = "FingerDatabase/" + searchPath + ".bmp";

	Mat img = imread(filepath, CV_LOAD_IMAGE_GRAYSCALE);

	imshow("ORG", img);
	std::cout << "Create ImageClass\n\n";
	ImageClass imageClass = ImageClass(img, filepath, verifyPath);
	imageClass.showImage();
	imageClass.filtrImage();
	imageClass.binaryImage();
	imageClass.smoothImage();
	imageClass.createMask();
	imageClass.skeletozation();
	imageClass.drawCore();
	//imageClass.coreDetection();
	imageClass.findMinutiae();
	imageClass.methodI();
	imageClass.methodII();
	imageClass.clasifyMethodI();
	imageClass.clasifyMethodII();

	imageClass.showImage();
}