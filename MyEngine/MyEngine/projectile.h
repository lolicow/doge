#ifndef PROJECTILE
#define PROJECTIME

#include  "effect.h"
#include <stdio.h>
#include "Vector2D.h"

class projectile :public effect
{
public:
	projectile();
	virtual void init();
	virtual void serialize(FILE ** in);

	Vector2D vel;
	char HitEffect[32];
};

#endif
