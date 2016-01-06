#include "uimanager.h"
#include "transform.h"
#include "gameobject.h"
#include "GLsprite.h"
#include "gameobjectfactory.h"
#include "gameobjectmanager.h"
#include "playerstate.h"

extern gameobjectfactory GOFactory;
extern gameobjectmanager GOManager;
extern gameobject* player;
extern int chaos;
extern bool win;
extern int width;
extern int height;

UImanager::UImanager()
{

}

UImanager::~UImanager()
{
}

void UImanager::init()
{
	portrait = GOFactory.loadgameobject("dogetroll.txt");
	GOManager.addGO(portrait);
	setTransform(portrait, 30.0f, 30.0f, 30.0f, 870.0f, 0);

	gratz = GOFactory.loadgameobject("dogeclap.txt");
	GOManager.addGO(gratz);
	setTransform(gratz, 100.0f, 100.0f, width / 2.0f, 700.0f, 0);
	Glsprite* gratzSprite = static_cast<Glsprite*>(gratz->getcomponent(T_SPRITE));
	gratzSprite->ReLoadSprite(1, 1, "");

	uilavabase = GOFactory.loadgameobject("uilavabase.txt");
	GOManager.addGO(uilavabase);
	setTransform(uilavabase, width/2.0f, 50.0f, width/2.0f, 50.0f, 0);
	Glsprite* uilavabaseSprite = static_cast<Glsprite*>(uilavabase->getcomponent(T_SPRITE));
	uilavabaseSprite->ReLoadSprite(1, 1, "");

	for (int i = 0;i < 64;++i)
	{
		hpBar[i] = GOFactory.loadgameobject("bar.txt");
		GOManager.addGO(hpBar[i]);
		setTransform(hpBar[i], 15.0f, 15.0f, 75.0f + i*30.0f, 885.0f, 1);
		Glsprite* hpBarSprite = static_cast<Glsprite*>(hpBar[i]->getcomponent(T_SPRITE));
		hpBarSprite->ReLoadSprite(1, 1, "");
	}
	for (int j = 0;j < 16;++j)
	{
		jumpBar[j] = GOFactory.loadgameobject("bar.txt");
		GOManager.addGO(jumpBar[j]);
		setTransform(jumpBar[j], 37.5f,15.0f, 97.5f + j*75.0f, 855.0f, 1);
		Glsprite* jumpBarSprite = static_cast<Glsprite*>(jumpBar[j]->getcomponent(T_SPRITE));
		jumpBarSprite->ReLoadSprite(1, 1, "");
	}

	for (int i = 0;i < 4;++i)
	{
		if (i%2 == 0)
			uilava[i] = GOFactory.loadgameobject("uilava.txt");
		else
			uilava[i] = GOFactory.loadgameobject("uilava2.txt");
		GOManager.addGO(uilava[i]);
		setTransform(uilava[i], width / 8.0f, 50.0f, width / 8.0f + width / 4.0f * i, 125.0f, 1);
		Glsprite* uilavaSprite = static_cast<Glsprite*>(uilava[i]->getcomponent(T_SPRITE));
		uilavaSprite->ReLoadSprite(2, 4, "");
	}

	Pstate = static_cast<playerstate*>(player->getcomponent(T_GAMESTATE));
}

void UImanager::update()
{
	if (win)
	{
		Glsprite* gratzSprite = static_cast<Glsprite*>(gratz->getcomponent(T_SPRITE));
		gratzSprite->ReLoadSprite(2, 2, "dogeclap.bmp");
	}

	Glsprite* portraitSprite = static_cast<Glsprite*>(portrait->getcomponent(T_SPRITE));
	if (Pstate->HPLeft <= Pstate->HPMax * 0.3f)
	{
		portraitSprite->ReLoadSprite(2, 3, "dogesweat.bmp");
	}
	else if (Pstate->HPLeft <= Pstate->HPMax * 0.6f)
	{
		portraitSprite->ReLoadSprite(2, 4, "dogetroll.bmp");
	}
	else
	{
		portraitSprite->ReLoadSprite(2, 4, "dogesun.bmp");
	}

	if (Pstate->HPLeft < 0)
		Pstate->HPLeft = 0;
	for (int i = 0;i < Pstate->HPLeft;++i)
	{
		Glsprite* hpBarSprite = static_cast<Glsprite*>(hpBar[i]->getcomponent(T_SPRITE));
		hpBarSprite->ReLoadSprite(1, 1, "hp.bmp");
	}
	for (int i = Pstate->HPLeft;i < Pstate->HPMax;++i)
	{
		Glsprite* hpBarSprite = static_cast<Glsprite*>(hpBar[i]->getcomponent(T_SPRITE));
		hpBarSprite->ReLoadSprite(1, 1, "hplost.bmp");
	}
	for (int i = Pstate->HPMax;i < 64;++i)
	{
		Glsprite* hpBarSprite = static_cast<Glsprite*>(hpBar[i]->getcomponent(T_SPRITE));
		hpBarSprite->ReLoadSprite(1, 1, "");
	}
	for (int i = 0;i < Pstate->jumpLeft;++i)
	{
		Glsprite* jumpBarSprite = static_cast<Glsprite*>(jumpBar[i]->getcomponent(T_SPRITE));
		jumpBarSprite->ReLoadSprite(1, 1, "jumpbar.bmp");
	}
	for (int i = Pstate->jumpLeft;i < 16;++i)
	{
		Glsprite* jumpBarSprite = static_cast<Glsprite*>(jumpBar[i]->getcomponent(T_SPRITE));
		jumpBarSprite->ReLoadSprite(1, 1, "");
	}

	Glsprite* uilavabaseSprite = static_cast<Glsprite*>(uilavabase->getcomponent(T_SPRITE));
	if (!chaos)
		uilavabaseSprite->ReLoadSprite(1, 1, "");
	else
	{
		uilavabaseSprite->ReLoadSprite(1, 1, "uilavabase.bmp");
		setTransform(uilavabase, width / 2.0f, chaos, width / 2.0f, chaos, 0);
		chaoscount++;
		if (chaoscount > 45)
		{
			chaos++;
			chaoscount = 0;
		}
		if (chaos > 120.0)
		{
			if (Pstate->hurtState != STATE_IMMUNE)
			{ 
				Pstate->hurtState = STATE_IMMUNE;
				Pstate->IHurtState = IS_ENTER;
				Pstate->HPLeft -= 1;
			}
		}

	}
	for (int i = 0;i < 4;++i)
	{
		Glsprite* uilavaSprite = static_cast<Glsprite*>(uilava[i]->getcomponent(T_SPRITE));
		if (!chaos)
			uilavaSprite->ReLoadSprite(2, 4, "");
		else
		{
			if (i % 2 == 0)
				uilavaSprite->ReLoadSprite(2, 4, "uilava.bmp");
			else
				uilavaSprite->ReLoadSprite(2, 4, "uilava2.bmp");
			setTransform(uilava[i], width / 8.0f, 50.0f, width / 8.0f + width / 4.0f * i, chaos * 2 - 15, 1);
		}		
	}

}

void setTransform(gameobject* GO, float scalex,float scaley,float posx,float posy,float dir)
{
	transform* trans = static_cast<transform*>(GO->getcomponent(T_TRANSFORM));
	if (trans)
	{
		trans->SetScale(scalex, scaley);
		trans->SetDir(dir);

		trans->SetTrans(posx, posy);
	}
	//GO->init();
}
