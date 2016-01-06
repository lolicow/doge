
#ifndef PHYSICSMANAGER
#define PHYSICSMANAGER

#include<list>
#include"collisionmanager.h"

class gameobject;


class PhysicsManager {
public:
	PhysicsManager() {};
	~PhysicsManager();
	void addGO(gameobject * go);
	void init();
	void update(float deltatime);
	void generateAllCollision();
	void sendCollisionMessage();
	void frameclear();
	void reset();
private:
	std::list<gameobject*> PhysicsGOlistTerrain;
	std::list<gameobject*> PhysicsGOlistNoTerrain;
	collsionmanager COLManager;
};

#endif
