#ifndef COMPONENT_H
#define COMPONENT_H


class gameobject;
class message;


enum COMPONENT_TYPE
{
	T_TRANSFORM,
	T_SPRITE,
	T_CONTROLLER,
	T_CHAOS, //useless
	T_CAMERA,
	T_BODY,
	T_GAMESTATE,
	T_EFFECT,
	T_ENEMYMOVINGACTION,
	T_ENHANCEMENT
};

class component
{
public:
	component(COMPONENT_TYPE ctype);
	virtual ~component() {};
	virtual void init() {};
	virtual void update() {};
	virtual void handleMessage(message* m) {};
	COMPONENT_TYPE getType() { return Ctype; };
	gameobject* getOwner() { return Owner; };
	void setOwner(gameobject* go) { Owner = go; };
private:
	gameobject* Owner;
	COMPONENT_TYPE Ctype;
};

#endif