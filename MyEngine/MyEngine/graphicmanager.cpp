#include"graphicmanager.h"
#include"gameobject.h"
#include"GLsprite.h"


graphicmanager::graphicmanager()
{

}
graphicmanager::~graphicmanager()
{
	for (auto i : RenderGOlist)
	{
		if (i != NULL)
		{
			if (i->refcount == 1)
				delete i;
			else
				i->refcount--;
		}
	}
}

void graphicmanager::addGO(gameobject * go)
{
	go->refcount++;
	RenderGOlist.push_back(go);
}

void graphicmanager::update()
{

}

void graphicmanager::render()
{
	for (auto i : RenderGOlist)
	{
		if (i != NULL)
		{
			if (i->getcomponent(T_SPRITE))
			{
				Glsprite* iSprite = static_cast<Glsprite*>(i->getcomponent(T_SPRITE));
				iSprite->render(iSprite->checkUI());
			}
		}
	}
}

void graphicmanager::broadcastMessage(message* m)
{
	for (auto i : RenderGOlist)
	{
		if (i != NULL)
			i->handleMessage(m);
	}
}

void graphicmanager::frameclear()
{
	ClearGO(RenderGOlist);
}

void graphicmanager::reset()
{
	for (auto i = RenderGOlist.begin(); i != RenderGOlist.end();)
	{
		if (*i != NULL)
		{
			if ((*i)->refcount <= 1)
				delete (*i);
			else
				(*i)->refcount--;
			RenderGOlist.erase(i++);
		}
		else
			i++;
	}
}
