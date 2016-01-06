#ifndef EFFECT
#define EFFECT

#include  "Component.h"
#include <stdio.h>

enum FriendlySIDE{
	playerSide,
	enemySide,
};

class effect :public component
{
public:
	effect();
	virtual void init();
	virtual void update();
	virtual void serialize(FILE ** in);
	void setCountDown(int frametime);
	void setSide(FriendlySIDE FS);
	int getCountDown() { return countdown; };
	FriendlySIDE getFside() { return FSIDE; };
	int dmg;
protected:
	int countdown;
	FriendlySIDE FSIDE;
};

#endif
