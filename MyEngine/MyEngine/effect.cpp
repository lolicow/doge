#include "effect.h"	  
#include "gameobject.h"

effect::effect() : component(T_EFFECT)
{
}

void effect::init()
{
}

void effect::update()
{
	if (this->getOwner())
	{ 
		if (countdown == 0)
		{
			this->getOwner()->toBeDeleted = true;
		}
		else if (countdown > 0)
			countdown -= 1;
	}
		
}

void effect::serialize(FILE ** in)
{
	if (*in)
	{
		fscanf_s(*in, "%d %d\n", &FSIDE,&countdown);
	}
}

void effect::setCountDown(int frametime)
{
	countdown = frametime;
}

void effect::setSide(FriendlySIDE FS)
{
	FSIDE = FS;
}
