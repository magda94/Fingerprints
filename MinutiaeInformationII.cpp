#include "MinutiaeInformationII.h"

MinutiaeInformationII::MinutiaeInformationII(int type, double r, double phi) {
	this->type = type;
	this->r = r;
	this->phi = phi;
}

int MinutiaeInformationII::getType() {
	return this->type;
}

double MinutiaeInformationII::getR() {
	return this->r;
}

double MinutiaeInformationII::getPhi() {
	return this->phi;
}