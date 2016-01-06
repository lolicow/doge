#ifndef BODY
#define BODY

#include"Component.h"
#include<stdio.h>
#include"Vector2D.h"


class shape;
class transform;

enum bodytype {
	rigidbody = 0,
	effectbody = 1,
	projectilebody = 2
};

class body :public component
{
public:
	body();
	~body(); 
	void integrate(float gravity,float deltatime);
	void serialize(FILE **in);
	void init();
	void debugdrawing();
	void updateTransform();
	virtual void handleMessage(message* m);
	Vector2D getPos() { return pos; };

	transform* thistrans;
	shape* bodyShape;
	Vector2D pos;
	Vector2D posPrev;
	Vector2D vel;
	Vector2D acc;
	Vector2D force;
	float mass, invmass;
	bodytype Btype;
};

#endif
