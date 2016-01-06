#ifndef TEXTMESSAGE
#define TEXTMESSAGE

#include "message.h"

class textmessage :public message {
public:
	textmessage() :message(M_RENDERTEXT) {};
	void setColor(float r, float g, float b) { color[0] = r;color[1] = g;color[2] = b; };
	std::string thistext;
	float color[3];
};

#endif
