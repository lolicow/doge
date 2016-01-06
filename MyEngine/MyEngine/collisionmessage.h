#ifndef COLLISIONMEESAGE
#define COLLISIONMESSAGE

#include "message.h"
class contact;

class collsionmessage :public message
{
public:
	collsionmessage(contact* con) : message(M_COLLISION) { c = con; };
	contact* c;
};

#endif

