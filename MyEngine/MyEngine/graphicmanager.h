#ifndef GRAPHICMANAGER
#define GRAPHICMANAGER

#include<list>

class gameobject;
class message;

class graphicmanager {
public:
	graphicmanager();
	~graphicmanager();
	void addGO(gameobject* go);
	void update();
	void render();
	void broadcastMessage(message* m);
	void frameclear();
	void reset();
private:
	std::list<gameobject*> RenderGOlist;
};

#endif
