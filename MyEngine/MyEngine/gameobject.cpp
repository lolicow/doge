#include "gameobject.h"
#include "message.h"


void ClearGO(std::list<gameobject*> &golist) {
	for (auto i = golist.begin(); i != golist.end();)
	{
		if (*i != NULL)
		{
			if ((*i)->toBeDeleted)
			{
				if ((*i)->refcount <= 1)
					delete (*i);
				else
					(*i)->refcount--;
				golist.erase(i++);
			}
			else
				i++;
		}
		else
			i++;
	}
}

gameobject::gameobject()
{

}

gameobject::~gameobject()
{
	for (auto it = (GOcomponents).begin(); it != (GOcomponents).end(); ++it)
	{
		delete it->second;
		it->second = NULL;
	}
}

void gameobject::init()
{
	for (auto it = (GOcomponents).begin(); it != (GOcomponents).end(); ++it)
	{
		if (it->second != NULL)
			it->second->init();
	}
}

void gameobject::update()
{
	for (auto it = (GOcomponents).begin(); it != (GOcomponents).end(); ++it)
	{
		if (it->second != NULL)
		it->second->update();
	}
}

void gameobject::addcomponent(component* newcomponent)
{
	GOcomponents[newcomponent->getType()] = newcomponent;
	newcomponent->setOwner(this);
}

void gameobject::deletecomponent(COMPONENT_TYPE ctype)
{
	GOcomponents.erase(ctype);
}

component* gameobject::getcomponent(COMPONENT_TYPE ctype)
{
	return GOcomponents[ctype];
}

void gameobject::handleMessage(message* m)
{
	for (auto it = (GOcomponents).begin(); it != (GOcomponents).end(); ++it)
	{
		if (it->second != NULL)
			it->second->handleMessage(m);
	}
}
