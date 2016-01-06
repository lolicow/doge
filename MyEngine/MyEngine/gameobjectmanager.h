#ifndef GAMEOBJECTMANAGER
#define GAMEOBJECTMANAGER

#include<list>

class gameobject;
class message;

class gameobjectmanager
{
public:
	gameobjectmanager();
	~gameobjectmanager();
	void addGO(gameobject* go);
	void init();
	void update();
	void broadcastMessage(message* m);
	void frameclear();
	void reset();
private: 
	std::list<gameobject*> GOlist;
};

#endif