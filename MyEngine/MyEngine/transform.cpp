#include"transform.h"
#include"gameobject.h"
#include"playerstate.h"

extern gameobject* player;

transform::transform():component(T_TRANSFORM)
{
	trans.x = 0;
	trans.y = 0;
	z = 0;
	scale.x = 1.0f;
	scale.y = 1.0f;
	rotate = 0.0f;
	direction = 0;
}
transform::~transform()
{

}
void transform::update()
{
	if (trans.y < -20)
	{ 
		if (this->getOwner() == player)
		{
			static_cast<playerstate*>(player->getcomponent(T_GAMESTATE))->HPLeft = 0;
		}
		else
		{
			this->getOwner()->toBeDeleted = true;
		}
	}
	
}
void transform::MoveTrans(float x, float y)
{
	trans.x += x;
	trans.y += y;
}

void transform::serialize(FILE ** in)
{

	if (*in)
	fscanf_s(*in, ",%f,%f,%f,%f,%f,%f,%f\n",&trans.x,&trans.y,&z,&scale.x, &scale.y,&rotate,&direction);
}


