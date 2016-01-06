#include "patrol.h"
#include "body.h"
#include "gameobject.h"
#include "transform.h"
#include "FRC.h"

extern FrameRateController MyFRC;

patrol::patrol() : component(T_ENEMYMOVINGACTION)
{
	count = 0;
}

patrol::~patrol()
{
}

void patrol::update()
{
	if (!active)
		return;
	if (this->getOwner())
	{
		body* MyBody = static_cast<body*>((this->getOwner())->getcomponent(T_BODY));
		transform* MyTransform = static_cast<transform*>((this->getOwner())->getcomponent(T_TRANSFORM));
		if (MyBody && MyTransform)
		{
			int frame = 60;	//cheat, real frame causes bug
			count = (count + 1) % (patrolTime * frame);
			if (count < (patrolTime * frame /2))
			{ 
				if (MyBody->vel.x < patrolSpeed)
					MyBody->vel.x += 0.1;
				else
					MyBody->vel.x -= 0.1;
				MyTransform->SetDir(0);
			}
			else
			{
				if (MyBody->vel.x > -patrolSpeed)
					MyBody->vel.x -= 0.1;
				else
					MyBody->vel.x += 0.1;
				MyTransform->SetDir(1);
			}
		}
	}
}

void patrol::serialize(FILE ** in)
{
	char shapename[256];
	memset(shapename, 0, 256 * sizeof(char));
	if (*in)
	{
		fscanf_s(*in, "%d %f %d\n", &patrolTime,&patrolSpeed,&active);
	}
}
