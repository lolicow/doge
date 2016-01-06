#ifndef CONTROLLER
#define CONTROLLER

#include "Component.h"
#include "playerstate.h"
#include<stdio.h>

class playercontroller : public component
{
public:
	playercontroller();
	~playercontroller();
	virtual void init();
	virtual void update();
	void serialize(FILE ** in);
private:
	playerstate* currentplayerstate;
};

class othercontroller {

};

#endif
