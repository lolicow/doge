#ifndef COLLISIONMANAGER
#define COLLISIONMANAGER

#include<list>
#include"Vector2D.h"
class body;


class shape
{
public:
	enum Tshape
	{
		CIRCLE,
		RECTANGLE,
		SHAPENUM
	};

	shape(Tshape type);
	virtual ~shape() {};
	virtual bool testpoint(Vector2D p) = 0;

	body* ThisBody;
	Tshape mytype;
};


class circle :public shape
{
public:
	circle();
	~circle() {};

	float Radius;
	virtual bool testpoint(Vector2D p);

};

class rectangle :public shape
{
public:
	rectangle();
	~rectangle()  {};

	float halfwidth,halfheight;
	virtual bool testpoint(Vector2D p);
};

class contact
{
public:
	contact();
	~contact();

	body* Cbodies[2];
	float penetration;
	float lesserpenetration;
	Vector2D normal;
	Vector2D lessernormal;
};

typedef bool (*CollisionTest)(shape *a, Vector2D acenter, shape*b, Vector2D bcenter, contact *c);

class collsionmanager 
{
public:
	collsionmanager();
	~collsionmanager();
	bool GenerateContacts(shape *a, Vector2D acenter, shape*b, Vector2D bcenter, contact *c);
	void RegisterCollsionTest(shape::Tshape a, shape::Tshape b, CollisionTest test);
	void clearcontact(int index);

	CollisionTest CollsionTestTable[shape::SHAPENUM][shape::SHAPENUM];
	contact	contactArray[2048];
};

#endif
