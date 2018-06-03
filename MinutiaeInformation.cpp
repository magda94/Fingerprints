#include "MinutiaeInformation.h"

MinutiaeInformation::MinutiaeInformation(int x, int y, int type) {
	this->x = x;
	this->y = y;
	this->type = type;
}

const int MinutiaeInformation::getX() {
	return this->x;
}

const int MinutiaeInformation::getY() {
	return this->y;
}

int MinutiaeInformation::getType() {
	return this->type;
}
