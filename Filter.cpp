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

bool Filter::checkNumberOfBlack(Mat image, int x, int y) {
	int counter = 0;
	int threshold = 15;

	for (int i = -4; i <= 4; i++) {
		for (int j = -4; j <= 4; j++) {
			if ((int)image.at<uchar>(x + i, y + j) == 0) {
				counter++;
			}
		}
	}

	if (counter > threshold)
		return true;
	return false;
}

void Filter::addNeighbours(int x, int y) {
	for (int i = -1; i <= 1; i++) {
		for (int j = -1; j <= 1; j++) {
			if (i == 0 && j == 0)
				continue;
			if ((int)tempFingerMask.at<uchar>(x + i, y + j) == 0) {
				counter++;
				//std::cout << "POINT: X: " << x + i << " Y: " << y + j <<"  COUNTER: "<<counter<< std::endl;
				tempFingerMask.at<uchar>(x + i, y + j) = 127;
				buforList.push_back(Point2i(x+i,y+j));
			}
		}
	}
}

Mat Filter::chooseBiggest(Mat fingerMask) {
	Mat newFingerMask;
	vector<Point2i> biggest;
	tempFingerMask = fingerMask.clone();
	newFingerMask = fingerMask.clone();

	//clean image
	for (int i = 0; i < fingerMask.rows; i++) {
		for (int j = 0; j < fingerMask.cols; j++) {
			newFingerMask.at<uchar>(i, j) = (uchar)255;
		}
	}

	bool flag = false;
	for (int i = 0; i < tempFingerMask.rows; i++) {
		for (int j = 0; j < tempFingerMask.cols; j++) {
			if (!flag) {
				if ((int)tempFingerMask.at<uchar>(i, j) == 0) {
					counter++;
					tempFingerMask.at<uchar>(i, j) = 127;
					this->addNeighbours(i, j);
					while (!buforList.empty()) {
						Point2i next = buforList.back();
						biggest.push_back(next);
						buforList.pop_back();
						this->addNeighbours(next.x, next.y);
					}
					std::cout << "\nCOUNTER: " << counter << std::endl;
					if (counter > 10000) {
						flag = true;
						while (!biggest.empty()) {
							Point2i tempPoint = biggest.back();
							biggest.pop_back();
							newFingerMask.at<uchar>(tempPoint.x, tempPoint.y) = (uchar)0;
						}
					}
				}
			}
		}
	}
	return newFingerMask;
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

Mat Filter::normalize() {
	int max = (int)this->image.at<uchar>(0, 0);
	int min = (int)this->image.at<uchar>(0, 0);

	for (int i = 0; i < this->image.rows; i++) {
		for (int j = 0; j < this->image.cols; j++) {
			if ((int)this->image.at<uchar>(i, j) < min) {
				min = (int)this->image.at<uchar>(i, j);
			}
			if ((int)this->image.at<uchar>(i, j) > max) {
				max = (int)this->image.at<uchar>(i, j);
			}
		}
	}

	for (int i = 0; i < this->image.rows; i++) {
		for (int j = 0; j < this->image.cols; j++) {
			this->image.at<uchar>(i, j) = (uchar)(255 * ((int)this->image.at<uchar>(i,j)-min)/(max-min));
		}
	}

	return this->image;
}

Mat Filter::createMask() {
	Mat temp;
	Mat fingerMask;
	temp= this->image.clone();
	fingerMask= this->image.clone();

	int n = 0;
	bool flag;
	while (n < 5) {
		for (int i = 4; i < temp.rows - 4; i++) {
			for (int j = 4; j < temp.cols - 4; j++) {
				flag = this->checkNumberOfBlack(temp, i, j);
				
				for (int k = -4; k <= 4; k++) {
					for (int l = -4; l <= 4; l++) {
						if (flag == true) {
							fingerMask.at<uchar>(i + k, j + l) = (uchar)0;
						}
						else {
							fingerMask.at<uchar>(i + k, j + l) = (uchar)255;
						}
					}
				}
			}
		}
		fingerMask.copyTo(temp);
		n++;
	}
	imshow("ORG FINGERMASK", fingerMask);
	fingerMask = this->chooseBiggest(fingerMask);
	imshow("FINGER MASK", fingerMask);
	return fingerMask;
}