#ifndef SPRITE
#define SPRITE

#include "Component.h"
#include "SDL_surface.h"
#include<stdio.h>


class sprite : public component
{
public:
	sprite();
	sprite(SDL_Surface* thesprite);
	~sprite();
	virtual void update();
	void serialize(FILE **in);
	void render();
private:
	SDL_Surface* TheSprite;
};

#endif