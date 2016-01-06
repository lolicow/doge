#ifndef GAMEOBJECTFACTORY
#define GAMEOBJECTFACTORY

#include "Component.h"
#include "SDL_surface.h"

class gameobjectfactory 
{
public:
	gameobject* loadgameobject(const char* filename);
	void LoadLevel(const char* filename);
	void unLoadLevel();
private:
	
};

#endif