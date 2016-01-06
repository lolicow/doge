#ifndef GAMECOMPONENT
#define GAMECOMPONENT
#include <unordered_map>
#include"Component.h"

class message;

class gameobject
{
public:
	gameobject();
	~gameobject();
	void init();
	void update();
	void addcomponent(component* newcomponent);
	void deletecomponent(COMPONENT_TYPE ctype);
	component* getcomponent(COMPONENT_TYPE ctype);
	void handleMessage(message* m);

	int refcount = 0;
	bool toBeDeleted = false;
private:
	std::unordered_map<COMPONENT_TYPE,component *> GOcomponents;
	
};

void ClearGO(std::list<gameobject*> &golist);

#endif

