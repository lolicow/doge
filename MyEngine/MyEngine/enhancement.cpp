#include "enhancement.h"

void enhancement::update()
{
}

void enhancement::serialize(FILE ** in)
{
	if (*in)
	{
		fscanf_s(*in, "%d %d\n", &etype,&Increase);
	}
}
