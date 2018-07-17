#ifndef MINUTIAEINFORMATIONII_HH
#define MINUTIAEINFORMATIONII_HH

class MinutiaeInformationII {
private:
	int type;
	double r;
	double phi;
public:
	MinutiaeInformationII(int type, double r, double phi);
	int getType();
	double getR();
	double getPhi();
};
#endif 
