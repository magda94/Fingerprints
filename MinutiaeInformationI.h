#ifndef MINUTIAEINFORMATIONI_HH
#define MINUTIAEINFORMATIONI_HH

class MinutiaeInformationI {
private:
	int x;
	int y;
	int type;

public:
	MinutiaeInformationI(int x, int y, int type);
	const int getX();
	const int getY();
	int getType();
};
#endif
