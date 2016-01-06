#ifndef CHAOS
#define CHAOS

#include "Component.h"
#include <stdio.h>


class chaos : public component
{
public:
	chaos();
	~chaos();
	virtual void update();
	void serialize(FILE ** in);

	float dmg;
	float dmgTime;
	float life;
};

#endif
