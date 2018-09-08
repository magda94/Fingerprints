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

double Filter::getMean()
{
	double mean = 0.0;

	for (int i = 0; i < this->image.rows; i++) {
		for (int j = 0; j < this->image.cols; j++) {
			mean += (int)this->image.at<uchar>(i, j);
		}
	}

	mean = mean / (this->image.rows * this->image.cols);

	return mean;
}

double Filter::getVariance(double mean)
{
	double variance = 0.0;

	std::cout << "VALUE image: " << (double)this->image.at<uchar>(1, 0) << std::endl;

	for (int i = 0; i < this->image.rows; i++) {
		for (int j = 0; j < this->image.cols; j++) {
			variance += pow((double)this->image.at<uchar>(i, j) - mean, 2);
		}
	}

	variance = variance / (this->image.rows * this->image.cols);

	return variance;
}

double Filter::getValueNormalized(int imageValue, double mean, double mean0, double variance, double variance0)
{
	double newValue = 0.0;

	if (imageValue > mean) {
		newValue = mean0 + sqrt((variance0 * pow(imageValue-mean,2))/variance);
	}
	else {
		newValue = mean0 - sqrt((variance0 * pow(imageValue - mean, 2)) / variance);
	}

	return newValue;
}

Mat Filter::centerMask(Mat maskImage) {

	int mask_height_center, mask_width_center;
	int image_height_center, image_width_center;

	image_height_center = floor(maskImage.rows / 2);
	image_width_center = floor(maskImage.cols / 2);

	int mask_beg_height, mask_end_height;
	int mask_beg_width, mask_end_width;

	int* tab = new int(4);

	this->countCoordinates(this->image, tab);

	mask_beg_height = tab[0];
	mask_end_height = tab[1];
	mask_beg_width = tab[2];
	mask_end_width = tab[3];

	mask_height_center = floor((mask_end_height + mask_beg_height) / 2);
	mask_width_center = floor((mask_end_width + mask_beg_width) / 2);

	Mat temp;
	Mat this_image_temp;
	temp = maskImage.clone();
	this_image_temp = this->image.clone();

	int move_height = image_height_center - mask_height_center;
	int move_width = image_width_center - mask_width_center;

	this->heightOffset = move_height;
	this->widthOffset = move_width;

	std::cout << "HOFFSET " << this->heightOffset << " WOFFSET " << this->widthOffset << std::endl;

	Mat trans_mat = (Mat_<double>(2, 3) << 1, 0, move_width, 0, 1, move_height);
	bitwise_not(temp, temp);
	bitwise_not(this_image_temp, this_image_temp);

	warpAffine(temp, temp, trans_mat, image.size());
	warpAffine(this_image_temp, this_image_temp, trans_mat, image.size());

	bitwise_not(temp, temp);
	bitwise_not(this_image_temp, this_image_temp);

	delete[] tab;

	this->image = this_image_temp.clone();
	return temp;
}

void Filter::countCoordinates(Mat image, int * tab) {

	//mask beggining height
	bool flag = 0;
	for (int i = 1; i < image.rows-1; i++) {
		for (int j = 1; j < image.cols-1; j++) {
			if ((int)image.at<uchar>(i, j) == 0) {
				tab[0] = i;
				flag = true;
				break;
			}
		}
		if (flag == true) {
			break;
		}
	}

	//mask height_end
	flag = false;
	for (int i = image.rows - 2; i >= 1; i--) {
		for (int j = 1; j < image.cols-1; j++) {
			if ((int)image.at<uchar>(i, j) == 0) {
				tab[1] = i;
				flag = true;
				break;
			}
		}
		if (flag == true) {
			break;
		}
	}

	//width_beggining
	flag = false;
	for (int j = 1; j < image.cols-1; j++) {
		for (int i = 1; i < image.rows-1; i++) {
			if ((int)image.at<uchar>(i, j) == 0) {
				tab[2] = j;
				flag = true;
				break;
			}
		}
		if (flag == true) {
			break;
		}
	}

	//width_end
	flag = false;
	for (int j = image.cols - 2; j >= 1; j--) {
		for (int i = 1; i < image.rows-1; i++) {
			if ((int)image.at<uchar>(i, j) == 0) {
				tab[3] = j;
				flag = true;
				break;
			}
		}
		if (flag == true) {
			break;
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
		temp.at<uchar>(i,0) = (uchar) 0;
		temp.at<uchar>(i, this->image.cols - 1) = (uchar)0;
	}

	for (int j = 0; j < this->image.cols; j++) {
		temp.at<uchar>(0, j) = (uchar)0;
		temp.at<uchar>(this->image.rows - 1, j) = (uchar)0;
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
	int operation = MORPH_CLOSE; //open-> close, close->open (for white pixel)
	Mat temp;
	Mat temp2;

	bitwise_not(this->image, this->image);
	imshow("BEFORE REDUCE", this->image);
	Mat element = getStructuringElement(shape, Size(size,size), Point(1,1));
	morphologyEx(this->image, temp, operation, element);
	this->image = temp.clone();
	bitwise_not(this->image, this->image);

	imshow("REDUCE HOLES",this->image);
	return this->image;
}

Mat Filter::normalize()
{
	double mean = this->getMean();
	double variance = this->getVariance(mean);

	double mean0 = mean;
	double variance0 =  variance;

	for (int i = 0; i < this->image.rows; i++) {
		for (int j = 0; j < this->image.cols; j++) {
			this->image.at<uchar>(i,j) = this->getValueNormalized((int)this->image.at<uchar>(i, j), mean, mean0, variance, variance0);
		}
	}


	std::cout << "MEAN: " << mean << std::endl;
	std::cout << "VARIANCE: " << variance << std::endl;

	//imshow("FUNCTION NORMALIZED IMAGE",this->image);
	return this->image;
}

Mat Filter::normalize2() {
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

	fingerMask = this->chooseBiggest(fingerMask);
	fingerMask = this->centerMask(fingerMask);

	return fingerMask;
}

Mat Filter::getImage()
{
	return this->image;
}

int Filter::getHeightOffset() {
	return heightOffset;
}

int Filter::getWidthOffset() {
	return widthOffset;
}
