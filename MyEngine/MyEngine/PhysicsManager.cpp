#include "PhysicsManager.h"
#include "gameobject.h"
#include "body.h"
#include "collisionmessage.h"

extern float gravity;
extern gameobject* player;


PhysicsManager::~PhysicsManager()
{
	for (auto i : PhysicsGOlistTerrain)
	{
		if (i != NULL)
		{
			if (i->refcount == 1)
				delete i;
			else
				i->refcount--;
		}
	}
	for (auto i : PhysicsGOlistNoTerrain)
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

void PhysicsManager::init()
{
	for (auto i : PhysicsGOlistTerrain)
	{
		if (i != NULL)
		{
			if (i->getcomponent(T_BODY))
			{
				static_cast<body*>(i->getcomponent(T_BODY))->init();
			}
		}
	}
	for (auto i : PhysicsGOlistNoTerrain)
	{
		if (i != NULL)
		{
			if (i->getcomponent(T_BODY))
			{
				static_cast<body*>(i->getcomponent(T_BODY))->init();
			}
		}
	}
}

void PhysicsManager::addGO(gameobject * go)
{
	go->refcount++;

	body* gobody = static_cast<body*>(go->getcomponent(T_BODY));
	if ((gobody->mass ==0) && (gobody->Btype == rigidbody))
		PhysicsGOlistTerrain.push_back(go);
	else
		PhysicsGOlistNoTerrain.push_back(go);
}

void PhysicsManager::update(float deltatime)
{	
	for (auto i : PhysicsGOlistTerrain)
	{
		if (i != NULL)
		{
			if (i->getcomponent(T_BODY))
			{
				static_cast<body*>(i->getcomponent(T_BODY))->integrate(gravity,deltatime);
			}
		}
	}
	for (auto i : PhysicsGOlistNoTerrain)
	{
		if (i != NULL)
		{
			if (i->getcomponent(T_BODY))
			{
				static_cast<body*>(i->getcomponent(T_BODY))->integrate(gravity, deltatime);
			}
		}
	}
	generateAllCollision();
	sendCollisionMessage();
}

void PhysicsManager::generateAllCollision()
{
	std::list<gameobject*>::iterator i = PhysicsGOlistNoTerrain.begin();
	for (;i != PhysicsGOlistNoTerrain.end();++i)
	{
		//collision with terrain
		std::list<gameobject*>::iterator l = PhysicsGOlistTerrain.begin();
		for (;l != PhysicsGOlistTerrain.end();++l)
		{
			contact c;
			body* ibody = static_cast<body*>((*i)->getcomponent(T_BODY));
			body* lbody = static_cast<body*>((*l)->getcomponent(T_BODY));

			if (COLManager.GenerateContacts(ibody->bodyShape, ibody->getPos(), lbody->bodyShape, lbody->getPos(), &c))
			{

				for (int k = 0;k < 1024;++k)
				{
					if (COLManager.contactArray[k].Cbodies[0] == NULL)
					{
						COLManager.contactArray[k] = c;
						break;
					}
				}
			}
		}
		//collision with noterrain
		std::list<gameobject*>::iterator j = i;
		j++;
		for (;j!= PhysicsGOlistNoTerrain.end();++j)
		{
			contact c;
			body* ibody = static_cast<body*>((*i)->getcomponent(T_BODY));
			body* jbody = static_cast<body*>((*j)->getcomponent(T_BODY));

			if (COLManager.GenerateContacts(ibody->bodyShape,ibody->getPos(),jbody->bodyShape,jbody->getPos(),&c))
			{ 

				for (int k = 0;k < 1024;++k)
				{
					if (COLManager.contactArray[k].Cbodies[0] == NULL)
					{
						COLManager.contactArray[k] = c;
						break;
					}
				}
			}
		}
	}
}

void PhysicsManager::sendCollisionMessage()
{
	for (int i = 0;i < 1024;++i)
	{
		if (COLManager.contactArray[i].Cbodies[0] != NULL)
		{
			collsionmessage cm(&COLManager.contactArray[i]);
			COLManager.contactArray[i].Cbodies[0]->handleMessage(&cm);
			COLManager.contactArray[i].Cbodies[1]->handleMessage(&cm);
			COLManager.contactArray[i].Cbodies[0] = NULL;
		}
	}
}

void PhysicsManager::frameclear()
{
	ClearGO(PhysicsGOlistTerrain);
	ClearGO(PhysicsGOlistNoTerrain);
}

void PhysicsManager::reset()
{
	for (auto i = PhysicsGOlistTerrain.begin(); i != PhysicsGOlistTerrain.end();)
	{
		if (*i != NULL)
		{
			if ((*i)->refcount <= 1)
				delete (*i);
			else
				(*i)->refcount--;
			PhysicsGOlistTerrain.erase(i++);
		}
		else
			i++;
	}
	for (auto i = PhysicsGOlistNoTerrain.begin(); i != PhysicsGOlistNoTerrain.end();)
	{
		if (*i != NULL)
		{
			if ((*i)->refcount <= 1)
				delete (*i);
			else
				(*i)->refcount--;
			PhysicsGOlistNoTerrain.erase(i++);
		}
		else
			i++;
	}
}
