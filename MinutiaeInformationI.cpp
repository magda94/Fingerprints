#include "MinutiaeInformationI.h"

MinutiaeInformationI::MinutiaeInformationI(int x, int y, int type) {
	this->x = x;
	this->y = y;
	this->type = type;
}

const int MinutiaeInformationI::getX() {
	return this->x;
}

const int MinutiaeInformationI::getY() {
	return this->y;
}

int MinutiaeInformationI::getType() {
	return this->type;
}
