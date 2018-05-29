#ifndef MINUTIAEINFORMATION_HH
#define MINUTIAEINFORMATION_HH

class MinutiaeInformation {
private:
	int x;
	int y;
	int type;

public:
	MinutiaeInformation(int x, int y, int type);
	int getX();
	int getY();
	int getType();
};
#endif