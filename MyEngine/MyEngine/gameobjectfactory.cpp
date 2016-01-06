#include"sprite.h"
#include"transform.h"
#include"gameobject.h"
#include"controller.h"
#include"gameobjectfactory.h"
#include"chaos.h"
#include"gameobjectmanager.h"
#include"GLsprite.h"
#include"camera.h"
#include"body.h"
#include "PhysicsManager.h"
#include "graphicmanager.h"
#include "playerstate.h"
#include "patrol.h"
#include "effect.h"
#include "enhancement.h"
#include "projectile.h"

extern gameobjectmanager GOManager;
extern gameobject* player;
extern PhysicsManager PHYManager;
extern graphicmanager GRAPHManager;


gameobject* gameobjectfactory::loadgameobject(const char* filename)
{
	gameobject* tobecreated = NULL;
	FILE* in;
	fopen_s(&in, filename, "rb");

	if (in)
	{

		tobecreated = new gameobject();
		char componentname[256];
		memset(componentname, 0, 256 * sizeof(char));

		while (!feof(in))
		{
			fscanf_s(in, "%255s\n", componentname, sizeof(componentname));
			if (strcmp(componentname, "transform") == 0)
			{
				transform* trans = new transform();
				//trans->serialize(&in);
				tobecreated->addcomponent(trans);
			}
			if (strcmp(componentname, "sprite") == 0)
			{
				sprite* spr = new sprite();
				spr->serialize(&in);
				tobecreated->addcomponent(spr);
				GRAPHManager.addGO(tobecreated);
			}
			if (strcmp(componentname, "glsprite") == 0)
			{
				Glsprite* glspr = new Glsprite();
				glspr->serialize(&in);
				tobecreated->addcomponent(glspr);
				GRAPHManager.addGO(tobecreated);
			}
			if (strcmp(componentname, "body") == 0)
			{
				body* b = new body();
				b->serialize(&in);
				tobecreated->addcomponent(b);
				PHYManager.addGO(tobecreated);
			}
			if (strcmp(componentname, "controller") == 0)
			{
				playercontroller* contr = new playercontroller();
				contr->serialize(&in);
				tobecreated->addcomponent(contr);
			}
			if (strcmp(componentname, "patrol") == 0)
			{
				patrol* ptr = new patrol();
				ptr->serialize(&in);
				tobecreated->addcomponent(ptr);
			}
			if (strcmp(componentname, "chaos") == 0)
			{
				chaos* ch = new chaos();
				ch->serialize(&in);
				tobecreated->addcomponent(ch);
			}
			if (strcmp(componentname, "effect") == 0)
			{
				effect* ef = new effect();
				ef->serialize(&in);
				tobecreated->addcomponent(ef);
			}
			if (strcmp(componentname, "projectile") == 0)
			{
				projectile* pr= new projectile();
				pr->serialize(&in);
				tobecreated->addcomponent(pr);
			}
			if (strcmp(componentname, "camera") == 0)
			{
				camera* ca = new camera();			
				tobecreated->addcomponent(ca);
			}
			if (strcmp(componentname, "playerstate") == 0)
			{
				playerstate* ps = new playerstate();
				ps->serialize(&in);
				tobecreated->addcomponent(ps);
				//player does not have manager so need to increment counter here
				//tobecreated->refcount++;
				player = tobecreated;
			}
			if (strcmp(componentname, "enemystate") == 0)
			{
				enemystate* es = new enemystate();
				es->serialize(&in);
				tobecreated->addcomponent(es);
			}
			if (strcmp(componentname, "enhancement") == 0)
			{
				enhancement* eh = new enhancement();
				eh->serialize(&in);
				tobecreated->addcomponent(eh);
			}
		}
	}
	fclose(in);
	return tobecreated;
}

void gameobjectfactory::LoadLevel(const char* filename)
{
	FILE* in;
	fopen_s(&in, filename, "rb");

	if (in)
	{
		char objectname[256];
		fgets(objectname, sizeof(objectname), in);
		
		int count = 0;
		while (!feof(in))
		{
			memset(objectname, 0, 256 * sizeof(char));
			/*std::string fullpath;*/

			fscanf_s(in, "%255s ", objectname, sizeof(objectname));
			if (objectname[0] != ',')
			{ 
			gameobject* go = loadgameobject(objectname);

			component *c = go->getcomponent(T_TRANSFORM);
			if (c)
			{
				transform *t = static_cast<transform*>(c);
				t->serialize(&in);
			}

			count++;
			if (count == 222)
			{
				count = 0;
			}
			GOManager.addGO(go);
			}
		}
	}
	fclose(in);
}

void gameobjectfactory::unLoadLevel()
{
	GOManager.reset();
	PHYManager.reset();
	GRAPHManager.reset();
}

