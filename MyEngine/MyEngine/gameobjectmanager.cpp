#include"gameobjectmanager.h"
#include"gameobject.h"
#include"GLsprite.h"


gameobjectmanager::gameobjectmanager()
{

}
gameobjectmanager::~gameobjectmanager()
{
	for (auto i : GOlist)
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

void gameobjectmanager::addGO(gameobject * go)
{
	go->refcount++;
	GOlist.push_back(go);
}

void gameobjectmanager::init()
{
	for (auto i : GOlist)
	{
		if (i != NULL)
			i->init();
	}
}

void gameobjectmanager::update()
{
	for (auto i : GOlist)
	{
		if (i != NULL)
			i->update();
	}
}


void gameobjectmanager::broadcastMessage(message* m)
{
	for (auto i : GOlist)
	{
		if (i != NULL)
			i->handleMessage(m);
	}
}

void gameobjectmanager::frameclear()
{
	ClearGO(GOlist);
}

void gameobjectmanager::reset()
{
	for (auto i = GOlist.begin(); i != GOlist.end();)
	{
		if (*i != NULL)
		{
			if ((*i)->refcount <= 1)
				delete (*i);
			else
				(*i)->refcount--;
			GOlist.erase(i++);
		}
		else
			i++;
	}
}
