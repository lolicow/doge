#include "projectile.h"
#include "body.h"
#include "gameobject.h"

projectile::projectile()
{
}

void projectile::init()
{
	body* thisBody = static_cast<body*>(getOwner()->getcomponent(T_BODY));

	if (thisBody)
	{
		thisBody->vel = vel;
	}

}

void projectile::serialize(FILE ** in)
{
	memset(HitEffect, 0, 32 * sizeof(char));
	if (*in)
	{
		fscanf_s(*in, "%d %d ", &FSIDE, &countdown);
		fscanf_s(*in, "%255s\n", HitEffect,sizeof(HitEffect));
	}
}
