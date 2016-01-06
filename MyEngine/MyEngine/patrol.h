#ifndef PATROL
#define PATROL

#include "Component.h"
#include<stdio.h>

class patrol : public component
{
public:
	patrol();
	~patrol();
	virtual void update();
	virtual void serialize(FILE **in);
	bool active;
private:
	int patrolTime;
	float patrolSpeed;
	int count;
};

#endif