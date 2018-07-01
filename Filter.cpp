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

Mat Filter::centerMask(Mat image)
{
	int height_beg, height_end, width_beg, width_end;

	bool flag = false;

	//height_beggining
	for (int i = 0; i < image.rows; i++) {
		for (int j = 0; j < image.cols; j++) {
			if ((int)image.at<uchar>(i, j) == 0) {
				height_beg = i;
				flag = true;
				break;
			}
		}
		if (flag == true) {
			break;
		}
	}

	//height_end
	flag = false;
	for (int i = image.rows - 1; i >= 0; i--) {
		for (int j = 0; j < image.cols; j++) {
			if ((int)image.at<uchar>(i, j) == 0) {
				height_end = i;
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
	for (int j = 0; j < image.cols; j++) {
		for (int i = 0; j < image.rows; i++) {
			if ((int)image.at<uchar>(i, j) == 0) {
				width_beg = j;
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
	for (int j = image.cols - 1; j >= 0; j--) {
		for (int i = 0; j < image.rows; i++) {
			if ((int)image.at<uchar>(i, j) == 0) {
				width_end = j;
				flag = true;
				break;
			}
		}
		if (flag == true) {
			break;
		}
	}

	std::cout << "H_B: " << height_beg << "\tH_E: " << height_end << "\tW_B: " << width_beg << "\tW_E: " << width_end << std::endl;

	int avg_height = floor((height_beg + height_end)/2);
	int avg_width = floor((width_beg + width_end)/2);

	std::cout << " AVG_H: " << avg_height << "\tAVG_W: " << avg_width << std::endl;

	int avg_height_image = floor(image.rows/2);
	int avg_width_image = floor(image.cols/2);

	std::cout << " AVG_H_I: " << avg_height_image << "\tAVG_W_I: " << avg_width_image << std::endl;

	Mat temp_image(image.size(), image.type());
	Mat temp_image_this(this->image.size(), this->image.type());

	for (int i = 0; i < image.rows; i++) {
		for (int j = 0; j < image.cols; j++) {
			temp_image.at<uchar>(i, j) = 255;
			temp_image_this.at<uchar>(i, j) = 255;
		}
	}

	//I
	for (int i = 0; i <= avg_height; i++) {
		for (int j = 0; j <= avg_width; j++) {
			if (avg_height_image > i && avg_width_image > j) {
				temp_image.at<uchar>(avg_height_image - i, avg_width_image - j) = image.at<uchar>(avg_height - i, avg_width - j);
				temp_image_this.at<uchar>(avg_height_image - i, avg_width_image - j) = this->image.at<uchar>(avg_height - i, avg_width - j);
			}
		}
	}

	/*cvtColor(temp_image, temp_image, CV_GRAY2BGR);
	circle(temp_image, Point(avg_width_image, avg_height_image), 5, Scalar(255, 0, 0));
	circle(temp_image, Point(avg_width, avg_height), 5, Scalar(0, 0, 255));*/

	//II
	for (int i = 0; i <= avg_height; i++) {
		for (int j = 0; j <= avg_width; j++) {
			if (avg_height_image > i && avg_width_image > j) {
				temp_image.at<uchar>(avg_height_image - i, avg_width_image + j) = image.at<uchar>(avg_height - i, avg_width + j);
				temp_image_this.at<uchar>(avg_height_image - i, avg_width_image + j) = this->image.at<uchar>(avg_height - i, avg_width + j);
			}
		}
	}

	//III
	for (int i = 0; i <= avg_height; i++) {
		for (int j = 0; j <= avg_width; j++) {
			if (avg_height_image > i && avg_width_image > j) {
				temp_image.at<uchar>(avg_height_image + i, avg_width_image + j) = image.at<uchar>(avg_height + i, avg_width + j);
				temp_image_this.at<uchar>(avg_height_image + i, avg_width_image + j) = this->image.at<uchar>(avg_height + i, avg_width + j);
			}
		}
	}

	//IV
	for (int i = 0; i <= avg_height; i++) {
		for (int j = 0; j <= avg_width; j++) {
			if (avg_height_image > i && avg_width_image > j) {
				temp_image.at<uchar>(avg_height_image + i, avg_width_image - j) = image.at<uchar>(avg_height + i, avg_width - j);
				temp_image_this.at<uchar>(avg_height_image + i, avg_width_image - j) = this->image.at<uchar>(avg_height + i, avg_width - j);
			}
		}
	}


	//border
	for (int i = 0; i < image.rows; i++) {
		temp_image.at<uchar>(i, 0) = (uchar)255;
		temp_image.at<uchar>(i, image.cols-1) = (uchar)255;
	}

	for (int j = 0; j < image.cols; j++) {
		temp_image.at<uchar>(0, j) = (uchar)255;
		temp_image.at<uchar>(image.rows-1, j) = (uchar)255;
	}

	this->image = temp_image_this.clone();

	//imshow("TEMP_MASK_IMAGE", temp_image_this);

	return temp_image;
	//return image;
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

	//imshow("REDUCE HOLES",this->image);
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
	//imshow("ORG FINGERMASK", fingerMask);
	fingerMask = this->chooseBiggest(fingerMask);
	//fingerMask = this->centerMask(fingerMask);
	//imshow("FINGER MASK", fingerMask);
	return fingerMask;
}

Mat Filter::getImage()
{
	return this->image;
}