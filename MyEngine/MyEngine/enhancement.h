#ifndef ENHANCEMENT
#define ENHANCEMENT

#include"Component.h"
#include<stdio.h>

enum ENHANCETYPE
{
	ADDJUMP,
	ADDHP,
	RESETJUMP,
	RESETHP,
	ADDATTACK,
	ENTERCHAOS,
	WIN
};

class enhancement :public component {
public:
	enhancement() :component(T_ENHANCEMENT) {};
	virtual void update();
	void serialize(FILE ** in);
	ENHANCETYPE getType() { return etype; };
	int IncreasedBy() { return Increase; };
private:
	ENHANCETYPE etype;
	int Increase;
};

#endif
