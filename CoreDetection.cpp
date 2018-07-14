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
	int width = this->image.cols / w_divide;

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
			//std::cout <<"Start_rows+i: "<<start_rows+i<< " Start_cols+j: "<<start_cols+j<< " \tI: " << i << " J: " << j << std::endl;
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
			out[start_rows + i][start_cols + j] = result.at<float>(i, j);
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

void CoreDetection::divideMatrix(double ** in1, double ** in2, double ** out, int start_rows, int start_cols){
	int height = this->image.rows / h_divide;
	int width = this->image.cols / w_divide;

	Mat A = Mat(height, width, CV_32FC1);
	Mat B = Mat(height, width, CV_32FC1);

	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			A.at<float>(i, j) = in1[i + start_rows][j + start_cols];
			B.at<float>(i, j) = in2[i + start_rows][j + start_cols];
		}
	}

	Mat result;

	cv::divide(A, B, result);

	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			out[start_rows + i][start_cols + j] = result.at<float>(i, j);
		}
	}
}

void CoreDetection::powMatrix(double ** in, double ** out, int number, int start_rows, int start_cols) {
	int height = this->image.rows / h_divide;
	int width = this->image.cols / w_divide;

	Mat A = Mat(height, width, CV_32FC1);

	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			A.at<float>(i, j) = in[i + start_rows][j + start_cols];
		}
	}

	Mat result;

	cv::pow(A, number, result);

	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			out[start_rows + i][start_cols + j] = result.at<float>(i, j);
		}
	}
}

void CoreDetection::sqrtMatrix(double ** in, double ** out, int start_rows, int start_cols){
	int height = this->image.rows / h_divide;
	int width = this->image.cols / w_divide;

	Mat A = Mat(height, width, CV_32FC1);

	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			A.at<float>(i, j) = in[start_rows + i][start_cols + j];
		}
	}

	Mat result;

	cv::sqrt(A, result);

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

void CoreDetection::countSineComponent(int blockSize) {
	//allocate
	double** resultMatrix = new double*[this->image.rows];
	double** gradientXYPow = new double*[this->image.rows];
	double** addResult = new double*[this->image.rows];
	double** sqrtResult = new double*[this->image.rows];

	for (int i = 0; i < this->image.rows; i++) {
		resultMatrix[i] = new double[this->image.cols];
		gradientXYPow[i] = new double[this->image.cols];
		addResult[i] = new double[this->image.cols];
		sqrtResult[i] = new double[this->image.cols];
	}

	for (int i = 0; i < h_divide; i++) {
		for (int j = 0; j < w_divide; j++) {
			//std::cout << "\n\nNEW ITERATION\n\n";

			this->subtractMatrix(this->gradientXX, this->gradientYY, resultMatrix, blockSize*i, blockSize*j);
		

			this->powMatrix(resultMatrix, resultMatrix, 2, blockSize*i, blockSize*j);
			this->powMatrix(this->gradientXY, gradientXYPow, 2, blockSize*i, blockSize*j);

			this->addMatrix(gradientXYPow, resultMatrix, addResult, blockSize*i, blockSize*j);
			this->sqrtMatrix(addResult, sqrtResult, blockSize*i, blockSize*j);

			this->divideMatrix(this->gradientXY, sqrtResult, this->sineComponent, blockSize*i, blockSize*j);

			/*std::cout << "RESULTMATRIX[10][10]=" << resultMatrix[10 + blockSize * i][10 + blockSize * j] << std::endl;
			std::cout << "GRADIENTXYPOW[10][10]=" << gradientXYPow[10 + blockSize * i][10 + blockSize * j] << std::endl;*/
			/*std::cout << "RESULT[10][10]=" << addResult[10 + blockSize * i][10 + blockSize * j] << std::endl;
			std::cout << "SQRTRESULT[10][10]=" << sqrtResult[10 + blockSize * i][10 + blockSize * j] << std::endl;
			std::cout << "SINECOMPONENT[10][10]= " << this->sineComponent[10 + blockSize * i][10 + blockSize * j] << std::endl;*/
		}
	}

	//deallocate
	for (int i = 0; i < this->image.rows; i++) {
		delete[] resultMatrix[i];
		delete[] gradientXYPow[i];
		delete[] addResult[i];
		delete[] sqrtResult[i];
	}

	delete[] resultMatrix;
	delete[] gradientXYPow;
	delete[] addResult;
	delete[] sqrtResult;
}

void CoreDetection::countCosineComponent(int blockSize){
	//allocate
	double** substractResult = new double*[this->image.rows];
	double** gradientXYPow = new double*[this->image.rows];
	double** substractPow = new double*[this->image.rows];
	double** addResult = new double*[this->image.rows];
	double** sqrtResult = new double*[this->image.rows];

	for (int i = 0; i < this->image.rows; i++) {
		substractResult[i] = new double[this->image.cols];
		gradientXYPow[i] = new double[this->image.cols];
		substractPow[i] = new double[this->image.cols];
		addResult[i] = new double[this->image.cols];
		sqrtResult[i] = new double[this->image.cols];
	}

	for (int i = 0; i < h_divide; i++) {
		for (int j = 0; j < w_divide; j++) {

			this->subtractMatrix(this->gradientXX, this->gradientYY, substractResult, blockSize*i, blockSize*j);

			this->powMatrix(this->gradientXY, gradientXYPow, 2, blockSize*i, blockSize*j);
			this->powMatrix(substractResult, substractPow, 2, blockSize*i, blockSize*j);

			this->addMatrix(gradientXYPow, substractPow, addResult, blockSize*i, blockSize*j);

			this->sqrtMatrix(addResult, sqrtResult, blockSize*i, blockSize*j);

			this->divideMatrix(substractResult, sqrtResult, this->cosineComponent, blockSize*i, blockSize*j);

		}
	}

	//deallocate
	for (int i = 0; i < this->image.rows; i++) {
		delete[] substractResult[i];
		delete[] gradientXYPow[i];
		delete[] substractPow[i];
		delete[] addResult[i];
		delete[] sqrtResult[i];
	}

	delete[] substractResult;
	delete[] gradientXYPow;
	delete[] substractPow;
	delete[] addResult;
	delete[] sqrtResult;
}

double CoreDetection::countAtan(double x, double y){
	if (x > 0) {
		return atan(y / x);
	}
	else if (x < 0 && y >= 0) {
		return M_PI + atan(y / x);
	}
	else if (x < 0 && y < 0) {
		return (-M_PI) + atan(y / x);
	}
	else if (x == 0 && y > 0) {
		return M_PI / 2;
	}
	else if (x == 0 && y < 0) {
		return (-M_PI / 2);
	}
	else if (x == 0 && y == 0) {
		return 0;
	}
}

void CoreDetection::countFieldOrientation(){
	for (int i = 0; i < this->image.rows; i++) {
		for (int j = 0; j < this->image.cols; j++) {
			this->fieldOrientation[i][j] = M_PI/2 + this->countAtan(this->sineComponent[i][j], this->cosineComponent[i][j]) / 2;
		}
	}
}

void CoreDetection::countPoinCareTable() {
	for (int i = 1; i < this->image.rows - 1; i++) {
		for (int j = 1; j < this->image.cols - 1; j++) {
			this->poinCare[i][j] = this->countPoinCare(i, j);
		}
	}
}

double CoreDetection::countPoinCare(int x, int y) {
	double result = 0;
	double delta = 0;
	/*Point2i point0 = Point2i(x,y);
	Point2i point1 = Point2i(x-1, y);
	Point2i point2 = Point2i(x - 1, y + 1);
	Point2i point3 = Point2i(x, y + 1);*/
	Point2i point0 = Point2i(x, y);
	Point2i point1 = Point2i(x, y - 1);
	Point2i point2 = Point2i(x + 1, y - 1);
	Point2i point3 = Point2i(x + 1, y);

	delta = this->fieldOrientation[point1.x][point1.y] - this->fieldOrientation[point0.x][point0.y];
	result = this->checkDeltaForK(delta);

	delta = this->fieldOrientation[point2.x][point2.y] - this->fieldOrientation[point1.x][point1.y];
	result += this->checkDeltaForK(delta);

	delta = this->fieldOrientation[point3.x][point3.y] - this->fieldOrientation[point2.x][point2.y];
	result += this->checkDeltaForK(delta);

	delta = this->fieldOrientation[point0.x][point0.y] - this->fieldOrientation[point3.x][point3.y];
	result = this->checkDeltaForK(delta);

	result = 1 / (2 * M_PI) * result;
	return result;
}

double CoreDetection::checkDeltaForK(double delta) {
	if (abs(delta) < (M_PI / 2)) {
		return delta;
	}
	else if (delta <= (-M_PI / 2)) {
		return delta + M_PI;
	}
	else if (delta >= (M_PI / 2)) {
		return M_PI - delta;
	}
}


void CoreDetection::findMax() {
	double max = -10;
	int max_x = 0;
	int max_y = 0;
	for (int i = 1; i < this->image.rows - 1; i++) {
		for (int j = 1; j < this->image.cols; j++) {
			if (this->poinCare[i][j] > max) {
				max = this->poinCare[i][j];
				max_x = i;
				max_y = j;
			}
		}
	}

	std::cout << "MAX_X: " << max_x << " MAX_Y: " << max_y << " = " << max << std::endl;
	int count = 0;
	vector<Point2i> corePoints;
	for (int i = 1; i < this->image.rows - 1; i++) {
		for (int j = 1; j < this->image.cols - 1; j++) {
			if (this->poinCare[i][j] == max) {
				count++;
				corePoints.push_back(Point2i(i,j));
			}
		}
	}

	std::cout << "COUNT CORE : " << count << std::endl;

	Mat temp;
	temp = this->image.clone();
	cvtColor(temp, temp, CV_GRAY2BGR);

	for (int i = 0; i < corePoints.size(); i++) {
		circle(temp, Point(corePoints.at(i).y, corePoints.at(i).x), 5, Scalar(0, 255, 0));
	}
	/*circle(temp, Point(max_y, max_x), 5, Scalar(0, 255, 0));
	circle(temp, Point(max_x, max_y), 5, Scalar(0, 255, 0));*/

	imshow("CORE", temp);
}

void CoreDetection::findPoinCare(){
	std::vector<Point2i> corePoints;
	for (int i = 1; i < this->image.rows - 1; i++) {
		for (int j = 1; j < this->image.cols - 1; j++) {
			bool flag = this->checkIfCore(i, j);
			if (flag) {
				corePoints.push_back(Point2i(i, j));
			}
		}
	}

	std::cout << "FOUND CORES: " << corePoints.size() << std::endl;
}

bool CoreDetection::checkIfCore(int x, int y) {
	if (this->poinCare[x][y] <0.55 && this->poinCare[x][y] > 0.40) {
		return true;
	}
	if (this->poinCare[x][y] == 0.5) {
		return true;
	}
	if (this->poinCare[x][y] == -0.5) {
		std::cout << "DELTA\n" << std::endl;
	}
	if (this->poinCare[x][y] == 1) {
		std::cout << "DOUBLE DELTA" << std::endl;
	}
	return false;
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
	sineComponent = new double*[this->image.rows];
	cosineComponent = new double*[this->image.rows];
	fieldOrientation = new double*[this->image.rows];
	poinCare = new double*[this->image.rows];

	for (int i = 0; i < this->image.rows; i++) {
		gradientX[i] = new double[this->image.cols];
		gradientY[i] = new double[this->image.cols];
		gradientXY[i] = new double[this->image.cols];
		gradientXX[i] = new double[this->image.cols];
		gradientYY[i] = new double[this->image.cols];
		sineComponent[i] = new double[this->image.cols];
		cosineComponent[i] = new double[this->image.cols];
		fieldOrientation[i] = new double[this->image.cols];
		poinCare[i] = new double[this->image.cols];
	}
}

CoreDetection::~CoreDetection(){

	for (int i = 0; i < this->image.rows; i++) {
		delete[] gradientX[i];
		delete[] gradientY[i];
		delete[] gradientXY[i];
		delete[] gradientYY[i];
		delete[] gradientXX[i];
		delete[] sineComponent[i];
		delete[] cosineComponent[i];
		delete[] fieldOrientation[i];
		delete[] poinCare[i];
	}

	delete[] gradientX;
	delete[] gradientY;
	delete[] gradientXX;
	delete[] gradientXY;
	delete[] gradientYY;
	delete[] sineComponent;
	delete[] cosineComponent;
	delete[] fieldOrientation;
	delete[] poinCare;
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

	this->countSineComponent(blockSize);
	this->countCosineComponent(blockSize);

	//for each block, filter components
	/*for (int i = 0; i < h_divide; i++) {
		for (int j = 0; j < w_divide; j++) {
			this->filtrGaussian(this->sineComponent, this->sineComponent, blockSize*i, blockSize*j);//gradientXX
			this->filtrGaussian(this->cosineComponent, this->cosineComponent, blockSize*i, blockSize*j);//gradientXY
		}
	}*/

	this->countFieldOrientation();

	this->countPoinCareTable();

	this->findPoinCare();
	this->findMax();

	this->writeGradient(this->gradientX, "gradientX");
	this->writeGradient(this->gradientY, "gradientY");
	this->writeGradient(this->gradientXX, "gradientXX");
	this->writeGradient(this->gradientXY, "gradientXY");
	this->writeGradient(this->gradientYY, "gradientYY");
	this->writeGradient(this->sineComponent, "sineComponent");
	this->writeGradient(this->cosineComponent, "cosineComponent");
	this->writeGradient(this->fieldOrientation, "fieldOrientation");
	this->writeGradient(this->poinCare, "poinCare");
}
