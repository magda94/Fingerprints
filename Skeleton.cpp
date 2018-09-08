#include "Skeleton.h"

/*******************************************
PRIVATE METHODS
*******************************************/

bool Skeleton::check0(int x, int y) {
	bool flag = false;

	for (int i = -1; i <= 1; i++) {
		for (int j = -1; j <= 1; j++) {
			if (tab0[1 + i][1 + j] == 2)
				continue;
			//black pixels
			if (tab0[1 + i][1 + j] == 1) {
				if ((int)this->image.at<uchar>(x+i, y+j) == 0) {
					flag = true;
				}
				else {
					flag = false;
					return flag;
				}
			}
			//white pixels
			if (tab0[1 + i][1 + j] == 0) {
				if ((int)this->image.at<uchar>(x+i, y+j) == 255) {
					flag = true;
				}
				else {
					flag = false;
					return flag;
				}
			}
		}
	}

	return flag;
}

bool Skeleton::check90(int x, int y) {
	bool flag = false;

	for (int i = -1; i <= 1; i++) {
		for (int j = -1; j <= 1; j++) {
			if (tab90[1 + i][1 + j] == 2)
				continue;
			//black pixels
			if (tab90[1 + i][1 + j] == 1) {
				if ((int)this->image.at<uchar>(x+i, y+j) == 0) {
					flag = true;
				}
				else {
					flag = false;
					return flag;
				}
			}
			//white pixels
			if (tab90[1 + i][1 + j] == 0) {
				if ((int)this->image.at<uchar>(x+i, y+j) == 255) {
					flag = true;
				}
				else {
					flag = false;
					return flag;
				}
			}
		}
	}

	return flag;
}

bool Skeleton::check180(int x, int y) {
	bool flag = false;

	for (int i = -1; i <= 1; i++) {
		for (int j = -1; j <= 1; j++) {
			if (tab180[1 + i][1 + j] == 2)
				continue;
			//black pixels
			if (tab180[1 + i][1 + j] == 1) {
				if ((int)this->image.at<uchar>(x+i, y+j) == 0) {
					flag = true;
				}
				else {
					flag = false;
					return flag;
				}
			}
			//white pixels
			if (tab180[1 + i][1 + j] == 0) {
				if ((int)this->image.at<uchar>(x+i, y+j) == 255) {
					flag = true;
				}
				else {
					flag = false;
					return flag;
				}
			}
		}
	}

	return flag;
}

bool Skeleton::check270(int x, int y) {
	bool flag = false;

	for (int i = -1; i <= 1; i++) {
		for (int j = -1; j <= 1; j++) {
			if (tab270[1 + i][1 + j] == 2)
				continue;
			//black pixels
			if (tab270[1 + i][1 + j] == 1) {
				if ((int)this->image.at<uchar>(x+i, y+j) == 0) {
					flag = true;
				}
				else {
					flag = false;
					return flag;
				}
			}
			//white pixels
			if (tab270[1 + i][1 + j] == 0) {
				if ((int)this->image.at<uchar>(x+i, y+j) == 255) {
					flag = true;
				}
				else {
					flag = false;
					return flag;
				}
			}
		}
	}

	return flag;
}

/*******************************************
PUBLIC METHODS
*******************************************/

Skeleton::Skeleton(const Mat& image) {
	this->image = image;
}

Mat Skeleton::makeSkeleton() {

	bool flag1 = false;
	bool flag2 = false;
	bool flag3 = false;
	bool flag4 = false;
	bool globalFlag = false;
	Mat temp = Mat(this->image.size(), CV_8UC1);
	Mat temp2 = Mat(this->image.size(), CV_8UC1);

	//fill corner
	for (int i = 0; i < temp.cols; i++) {
		temp.at<uchar>(0, i) = (uchar)255;
		temp.at<uchar>(temp.rows - 1, i) = (uchar)255;
	}

	for (int j = 0; j < temp.rows; j++) {
		temp.at<uchar>(j,0) = (uchar)255;
		temp.at<uchar>(j, temp.cols-1) = (uchar)255;
	}

	do {
		globalFlag = false;
		for (int i = 1; i < this->image.rows - 1; i++) {
			for (int j = 1; j< this->image.cols - 1; j++) {
				if ((int)this->image.at<uchar>(i, j) == 0) {
					flag1 = this->check0(i, j);
					flag2 = this->check90(i,j);
					flag3 = this->check180(i,j);
					flag4 = this->check270(i,j);

					if (flag1 || flag2 || flag3 || flag4) {
						if ((int)this->image.at<uchar>(i, j) == 0)
							globalFlag = true;
						temp.at<uchar>(i, j) = (uchar)255;
					}
					else {
						if ((int)this->image.at<uchar>(i, j) == 255)
							globalFlag = true;
						temp.at<uchar>(i, j) = (uchar)0;
					}
				}
				else {
					temp.at<uchar>(i, j) = (uchar)255;
				}
			}
		}
		temp.copyTo(this->image);
	} while (globalFlag);

	return this->image;
}
