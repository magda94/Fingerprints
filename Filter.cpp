#include "Filter.h"
/*******************************************
PRIVATE METHODS
*******************************************/

int Filter::checkMedian(int x, int y) {
	int result = (int)this->image.at<uchar>(x,y);
	int size = 9;

	int* medianTab = new int[size];
	int floorVal = round(0.2 * size);

	int counter = 0;

	for (int i = -1; i <= 1; i++) {
		for (int j = -1; j <= 1; j++) {
			medianTab[counter++] = (int)this->image.at<uchar>(x + i, y + j);
		}
	}

	//sort
	for (int i = 0; i < size;i++) {
		for (int j = 0; j < size-1; j++) {
			if (medianTab[j] > medianTab[j + 1]) {
				int tempValue = medianTab[j];
				medianTab[j] = medianTab[j + 1];
				medianTab[j + 1] = tempValue;
			}
		}
	}

	int medianVal = (int)(size/2);
	//check if it is in 20%
	if (medianTab[5] >= medianTab[medianVal - floorVal] && medianTab[medianVal] <= medianTab[medianVal + floorVal]) {
		result = medianTab[medianVal];
	}

	delete[] medianTab;
	return result;
}

/*******************************************
PUBLIC METHODS
*******************************************/
Filter::Filter(const Mat& image) {
	this->image = image;
}

Mat Filter::filtrMedian() {
	Mat temp(this->image.size(), this->image.type());

	for (int i = 0; i < this->image.rows; i++) {
		temp.at<uchar>(i,0) = (uchar) 255;
		temp.at<uchar>(i, this->image.cols - 1) = (uchar)255;
	}

	for (int j = 0; j < this->image.cols; j++) {
		temp.at<uchar>(0, j) = (uchar)255;
		temp.at<uchar>(this->image.rows - 1, j) = (uchar)255;
 	}

	for (int i = 1; i < this->image.rows-1; i++) {
		for (int j = 1; j < this->image.cols-1; j++) {

			temp.at<uchar>(i, j) = (uchar)checkMedian(i, j);
		}
	}

	temp.copyTo(this->image);
	return this->image;
}

Mat Filter::reduceHoles() {
	int shape = MORPH_CROSS;
	int size = 3;
	int operation = MORPH_CLOSE;
	Mat temp;

	Mat element = getStructuringElement(shape, Size(size,size), Point(1,1));
	morphologyEx(this->image,temp, operation, element);
	this->image = temp;

	return this->image;
}