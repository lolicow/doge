#include "collisionmanager.h"
#include "Math2D.h"
#include "body.h"
#include "PhysicsManager.h"

extern PhysicsManager PHYManager;

shape::shape(Tshape type)
{
	mytype = type;
}

bool shape::testpoint(Vector2D p)
{
	return false;
}

circle::circle():shape(CIRCLE)
{

}

bool circle::testpoint(Vector2D p)
{
	Vector2D center = ThisBody->pos;

	return StaticPointToStaticCircle(&p, &center, Radius);
}

rectangle::rectangle():shape(RECTANGLE)
{
}

bool rectangle::testpoint(Vector2D p)
{
	Vector2D center = ThisBody->pos;
	return StaticPointToStaticRect(&p, &center, halfwidth * 2, halfheight * 2);
}

contact::contact()
{
	Cbodies[0] = NULL;
	Cbodies[1] = NULL;
}

contact::~contact()
{
}

bool circletocircle(shape* a, Vector2D acenter, shape* b, Vector2D bcenter, contact *c)
{
	circle* acircle = static_cast<circle*>(a);
	circle* bcircle = static_cast<circle*>(b);

	Vector2D positionDelta;
	Vector2DSub(&positionDelta, &acenter, &bcenter);
	float disSquared = Vector2DSquareLength(&positionDelta);
	float combinedRadius = acircle->Radius + bcircle->Radius;

	if (disSquared < combinedRadius*combinedRadius)
	{
		Vector2D normal;
		float penetration;
		if (disSquared == 0.0f)
		{
			penetration = acircle->Radius > bcircle->Radius? acircle->Radius: bcircle->Radius;
			if (positionDelta.x > 0)
				normal.x = 1;
			else
				normal.x = -1;
			normal.y = 0;
		}
		else
		{
			float dis = Vector2DLength(&positionDelta);
			penetration = combinedRadius - dis;
			Vector2DNormalize(&normal,&positionDelta);
		}
		if (c)
		{ 
			c->Cbodies[0] = a->ThisBody;
			c->Cbodies[1] = b->ThisBody;
			c->penetration = penetration;
			c->normal = normal;
		}
		return 1;
	}
	else
		return 0;

}

bool circletorectangle(shape* a, Vector2D acenter, shape* b, Vector2D bcenter, contact *c)
{
	circle* acircle = static_cast<circle*>(a);
	rectangle* brect = static_cast<rectangle*>(b);

	Vector2D ClosestPoint;
	if (acenter.x > bcenter.x + brect->halfwidth)
		ClosestPoint.x = bcenter.x + brect->halfwidth;
	else if (acenter.x < bcenter.x - brect->halfwidth)
		ClosestPoint.x = bcenter.x - brect->halfwidth;
	else
		ClosestPoint.x = acenter.x;

	if (acenter.y > bcenter.y + brect->halfheight)
		ClosestPoint.y = bcenter.y + brect->halfheight;
	else if (acenter.y < bcenter.y - brect->halfheight)
		ClosestPoint.y = bcenter.y - brect->halfheight;
	else
		ClosestPoint.y = acenter.y;

	Vector2D positionDelta;
	Vector2DSub(&positionDelta, &ClosestPoint, &acenter);
	float dis = Vector2DLength(&positionDelta);
	if (dis < acircle->Radius)
	{
		c->Cbodies[0] = a->ThisBody;
		c->Cbodies[1] = b->ThisBody;
		if (dis == 0.0f)
		{
			float dx;
			float dy;
			int sign = 1;

			if (acenter.x - (bcenter.x - brect->halfwidth) >(bcenter.x + brect->halfwidth) - acenter.x)
			{
				dx = acenter.x - (bcenter.x - brect->halfwidth);
				sign = -1;
			}
			else
				dx = (bcenter.x + brect->halfwidth) - acenter.x;
			if (acenter.y - (bcenter.y - brect->halfheight) >(bcenter.y + brect->halfheight) - acenter.y)
			{
				float dy = acenter.y - (bcenter.y - brect->halfheight);
				sign = -1;
			}
			else
				dy = (bcenter.y + brect->halfheight) - acenter.y;

			if (dx < dy)
			{
				c->penetration = dx;
				c->normal.x = sign;
				c->normal.y = 0;
			}
			else
			{
				c->penetration = dy;
				c->normal.y = sign;
				c->normal.x = 0;
			}
		}
		else
		{
			c->penetration = acircle->Radius - dis;
			Vector2DNormalize(&(c->normal), &positionDelta);
		}

		return 1;
	}
	else
		return 0;
}

bool rectangletocircle(shape* a, Vector2D acenter, shape* b, Vector2D bcenter, contact *c)
{
	bool temp = circletorectangle(b, bcenter, a, acenter, c);
	c->normal.x *= -1;
	c->normal.y *= -1;
	return temp;
}

bool rectangletorectangle(shape* a, Vector2D acenter, shape* b, Vector2D bcenter, contact *c)
{
	rectangle* arect = static_cast<rectangle*>(a);
	rectangle* brect = static_cast<rectangle*>(b);

	Vector2D centerdelta;
	Vector2DSub(&centerdelta, &acenter, &bcenter);

	float dx = fabs(centerdelta.x) - arect->halfwidth - brect->halfwidth;
	float dy = fabs(centerdelta.y) - arect->halfheight - brect->halfheight;

	if (dx < 0.001)
	{
		if (dy <  0.001)
		{
			c->Cbodies[0] = a->ThisBody;
			c->Cbodies[1] = b->ThisBody;

			if (fabs(dx) < fabs(dy))
			{
				c->penetration = dx;
				c->lesserpenetration = dy;
				if (centerdelta.x > 0)
					c->normal.x = 1;
				else
					c->normal.x = -1;
				if (centerdelta.y > 0)
					c->lessernormal.y = 1;
				else
					c->lessernormal.y = -1;
				c->normal.y = 0;
			}
			else
			{
				c->penetration = dy;
				c->lesserpenetration = dx;
				if (centerdelta.y > 0)
					c->normal.y = 1;
				else
					c->normal.y = -1;
				if (centerdelta.x > 0)
					c->lessernormal.x = 1;
				else
					c->lessernormal.x = -1;
				c->normal.x = 0;
			}

			return 1;
		}
		else
			return 0;
	}
	else
		return 0;
}

collsionmanager::collsionmanager()
{
	//Register collision tests for all the shape types
	RegisterCollsionTest(shape::CIRCLE, shape::CIRCLE, circletocircle);
	RegisterCollsionTest(shape::RECTANGLE, shape::RECTANGLE, rectangletorectangle);
	RegisterCollsionTest(shape::CIRCLE, shape::RECTANGLE, circletorectangle);
	RegisterCollsionTest(shape::RECTANGLE, shape::CIRCLE, rectangletocircle);
}

collsionmanager::~collsionmanager()
{

}

bool collsionmanager::GenerateContacts(shape * a, Vector2D acenter, shape * b, Vector2D bcenter, contact * c)
{
	return (*CollsionTestTable[a->mytype][b->mytype])(a,acenter,b,bcenter,c);
}

void collsionmanager::RegisterCollsionTest(shape::Tshape a, shape::Tshape b, CollisionTest test)
{
	CollsionTestTable[a][b] = test;
}


void collsionmanager::clearcontact(int index)
{
	contactArray[index].Cbodies[0] = NULL;
}


