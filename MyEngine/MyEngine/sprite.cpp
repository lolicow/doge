#include"sprite.h"
#include"transform.h"
#include"gameobject.h"
#include"ResourceManager.h"

extern SDL_Surface* WindowSurface;
extern ResourceManager MyRM;

sprite::sprite() : component(T_SPRITE)
{
	
}

sprite::sprite(SDL_Surface* thesprite): component(T_SPRITE)
{
	TheSprite = thesprite;
}

sprite::~sprite()
{

}

void sprite::update()
{

}

void sprite::serialize(FILE ** in)
{
	char imagename[256];
	memset(imagename, 0, 256 * sizeof(char));

	fscanf_s(*in, "%255s", imagename, sizeof(imagename));

	std::string fullpath = "";
	fullpath += imagename;

	TheSprite = MyRM.LoadBMP(fullpath.c_str());
}

void sprite::render()
{
	//if (this->getOwner())
	//{
	//	transform* MyTransform = static_cast<transform*>((this->getOwner())->getcomponent(T_TRANSFORM));
	//	if (MyTransform)
	//	{
	//		SDL_BlitSurface(TheSprite, NULL, WindowSurface, MyTransform->GetTrans());
	//	}
	//}
}
