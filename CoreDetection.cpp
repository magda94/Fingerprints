#include "CoreDetection.h"
/*******************************************
PRIVATE METHODS
*******************************************/

double CoreDetection::countMean() {
	double mean = 0.0;
	
	for (int i = 0; i< this->image.rows; i++) {
		for (int j = 0; j< this->image.cols; j++) {
			mean += (double)this->image.at<uchar>(i, j);
		}
	}

	mean = mean / (this->image.rows * this->image.cols);

	return mean;
}

double CoreDetection::countDeviation() {
	double sigma = 0.0;
	int N = this->image.cols * this->image.rows;

	double mean = this->countMean();

	for (int i = 0; i < this->image.rows; i++) {
		for (int j = 0; j < this->image.cols; j++) {
			sigma += pow((double)this->image.at<uchar>(i, j) - mean,2);
		}
	}

	sigma = sqrt(sigma/(N-1));

	return sigma;
}
 
void CoreDetection::countGradientX() {

	//border
	for (int i = 0; i < this->image.rows; i++) {
		gradientX[i][0] = 0;
		gradientX[i][this->image.cols - 1] = 0;
	}
	for (int j = 0; j < this->image.cols; j++) {
		gradientX[0][j] = 0;
		gradientX[this->image.rows - 1][j] = 0;
	}

	for (int i = 1; i < this->image.rows-1; i++) {
		for (int j = 1; j < this->image.cols - 1; j++) {
			gradientX[i][j] = (double)this->image.at<uchar>(i, j + 1) - (double)this->image.at<uchar>(i, j - 1);
		}
	}
}

void CoreDetection::countGradientY(){
	//border
	for (int i = 0; i < this->image.rows; i++) {
		gradientY[i][0] = 0;
		gradientY[i][this->image.cols - 1] = 0;
	}
	for (int j = 0; j < this->image.cols; j++) {
		gradientY[0][j] = 0;
		gradientY[this->image.rows - 1][j] = 0;
	}

	for (int j = 1; j < this->image.cols-1; j++) {
		for (int i = 1; i < this->image.rows - 1; i++) {
			gradientY[i][j] = (double)this->image.at<uchar>(i+1, j) - (double)this->image.at<uchar>(i-1, j);
		}
	}
}

void CoreDetection::addMatrix(double ** in1, double ** in2, double ** out, int start_rows, int start_cols) {
	int height = this->image.rows / h_divide;
	int width = this->image.rows / w_divide;

	Mat A = Mat(height, width, CV_32FC1);
	Mat B = Mat(height, width, CV_32FC1);

	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			A.at<float>(i, j) = in1[start_rows + i][start_cols + j];
			B.at<float>(i, j) = in2[start_rows + i][start_cols + j];
		}
	}

	Mat result = A + B;

	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			out[start_rows + i][start_cols + j] = result.at<float>(i, j);
		}
	}
}

void CoreDetection::subtractMatrix(double ** in1, double ** in2, double ** out, int start_rows, int start_cols) {
	int height = this->image.rows / h_divide;
	int width = this->image.cols / w_divide;

	Mat A = Mat(height, width, CV_32FC1);
	Mat B = Mat(height, width, CV_32FC1);

	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			A.at<float>(i, j) = in1[start_rows + i][start_cols + j];
			B.at<float>(i, j) = in2[start_rows + i][start_cols + j];
		}
	}

	Mat result = A - B;

	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			//std::cout <<"Start_rows+i: "<<start_rows+i<< " Start_cols+j: "<<start_cols+j<< " \tI: " << i << " J: " << j << std::endl;
			out[i][j] = result.at<float>(i, j);
		}
	}
}

void CoreDetection::multiplyMatrix(double ** in1, double ** in2, double ** out, int start_rows, int start_cols){

	int height = this->image.rows / h_divide;
	int width = this->image.cols / w_divide;

	Mat A = Mat(height, width, CV_32FC1);
	Mat B = Mat(height, width, CV_32FC1);

	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			A.at<float>(i, j) = in1[start_rows + i][start_cols + j];
			B.at<float>(i, j) = in2[start_rows + i][start_cols + j];
		}
	}

	Mat result = A * B;

	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			out[start_rows + i][start_cols + j] = result.at<float>(i, j);
		}
	}

}

void CoreDetection::filtrGaussian(double ** in, double ** out, int start_rows, int start_cols){
	int height = this->image.rows / h_divide;
	int width = this->image.cols / w_divide;

	Mat A = Mat(height, width, CV_32FC1);
	
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			A.at<float>(i, j) = in[start_rows + i][start_cols + j];
		}
	}

	Mat result;


	cv::GaussianBlur(A, result, Size(3,3), 0.5);

	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			out[start_rows + i][start_cols + j] = result.at<float>(i, j);
		}
	}
}

void CoreDetection::countSineComponent(int start_rows, int start_cols) {
	//allocate
	double** subtractResult = new double*[this->image.rows/h_divide];
	for (int i = 0; i < this->image.rows / h_divide; i++) {
		subtractResult[i] = new double[this->image.cols / w_divide];
	}

	this->subtractMatrix(this->gradientXX, this->gradientYY, subtractResult, start_rows, start_cols);

	/*std::cout << "GradientXX[10][10]=" << this->gradientXX[10+start_rows][10+start_cols] << std::endl;
	std::cout << "GradientYY[10][10]=" << this->gradientYY[10+start_rows][10+start_cols] << std::endl;
	std::cout << "RESULT[10][10]=" << subtractResult[10][10] << std::endl;*/
	//this->addMatrix();

	//deallocate
	for (int i = 0; i < this->image.rows / h_divide; i++) {
		delete[] subtractResult[i];
	}
	delete[] subtractResult;
}

void CoreDetection::writeGradient(double ** in, std::string name)
{
	std::ofstream file;
	file.open(name + ".txt");

	for (int i = 0; i < this->image.rows; i++) {
		for (int j = 0; j < this->image.cols; j++) {
			file << in[i][j] << "\t";
		}
		file << std::endl;
	}
}

/*******************************************
PUBLIC METHODS
*******************************************/
CoreDetection::CoreDetection(const Mat & image){
	this->image = image;

	gradientX = new double*[this->image.rows];
	gradientY = new double*[this->image.rows];
	gradientXX = new double*[this->image.rows];
	gradientXY = new double*[this->image.rows];
	gradientYY = new double*[this->image.rows];
	for (int i = 0; i < this->image.rows; i++) {
		gradientX[i] = new double[this->image.cols];
		gradientY[i] = new double[this->image.cols];
		gradientXY[i] = new double[this->image.cols];
		gradientXX[i] = new double[this->image.cols];
		gradientYY[i] = new double[this->image.cols];
	}
}

CoreDetection::~CoreDetection(){

	for (int i = 0; i < this->image.rows; i++) {
		delete[] gradientX[i];
		delete[] gradientY[i];
		delete[] gradientXY[i];
		delete[] gradientYY[i];
		delete[] gradientXX[i];
	}

	delete[] gradientX;
	delete[] gradientY;
	delete[] gradientXX;
	delete[] gradientXY;
	delete[] gradientYY;
}

void CoreDetection::detectCore(){
	imshow("DETECTION", this->image);
	this->sigma = this->countDeviation();
	this->countGradientX();
	this->countGradientY();

	int blockSize = this->image.rows / this->h_divide;
	
	//for each block, count Gradients
	for (int i = 0; i < h_divide; i++) {
		for (int j = 0; j < w_divide; j++) {
			this->multiplyMatrix(this->gradientX, this->gradientX, this->gradientXX, blockSize*i, blockSize*j);//gradientXX
			this->multiplyMatrix(this->gradientX, this->gradientY, this->gradientXY, blockSize*i, blockSize*j);//gradientXY
			this->multiplyMatrix(this->gradientY, this->gradientY, this->gradientYY, blockSize*i, blockSize*j);//gradientYY
		}
	}

	//for each block, filter gradients
	for (int i = 0; i < h_divide; i++) {
		for (int j = 0; j < w_divide; j++) {
			this->filtrGaussian(this->gradientXX, this->gradientXX, blockSize*i, blockSize*j);//gradientXX
			this->filtrGaussian(this->gradientXY, this->gradientXY, blockSize*i, blockSize*j);//gradientXY
			this->filtrGaussian(this->gradientYY, this->gradientYY, blockSize*i, blockSize*j);//gradientYY
		}
	}

	//for each block, count sineComponent
	for (int i = 0; i < h_divide; i++) {
		for (int j = 0; j < w_divide; j++) {
			std::cout << "\n\nNEW ITERATION\n\n";
			this->countSineComponent(blockSize*i, blockSize*j);
		}
	}

	this->writeGradient(this->gradientX, "gradientX");
	this->writeGradient(this->gradientY, "gradientY");
	this->writeGradient(this->gradientXX, "gradientXX");
	this->writeGradient(this->gradientXY, "gradientXY");
	this->writeGradient(this->gradientYY, "gradientYY");
}
