#include"controller.h"
#include"gameobject.h"
#include"InputManager.h"
#include"transform.h"
#include"SDL_scancode.h"
#include"body.h"
#include"gameobjectfactory.h"
#include"gameobjectmanager.h"
#include"FRC.h"


extern InputManager MyIM;
extern gameobject* player;
extern FrameRateController MyFRC;
extern float gravity;
extern float jumpspeed;
extern gameobjectfactory GOFactory;
extern gameobjectmanager GOManager;


playercontroller::playercontroller() : component(T_CONTROLLER)
{

}

playercontroller::~playercontroller() 
{

}

void playercontroller::init()
{
	if (this->getOwner() == player)
	{
		currentplayerstate = static_cast<playerstate*>(player->getcomponent(T_GAMESTATE));
	}
}

void playercontroller::update()
{
	
	if (this->getOwner() == player)
	{
		transform* MyTransform = static_cast<transform*>((this->getOwner())->getcomponent(T_TRANSFORM));
		body* MyBody = static_cast<body*>((this->getOwner())->getcomponent(T_BODY));
		if (MyBody && MyTransform)
		{
			if (MyIM.IsPressed(SDL_SCANCODE_LEFT))
			{
				if (currentplayerstate->movingState != STATE_ATTACK)
				{
					if (MyBody->vel.x > 0)
						MyBody->force.x += -25.0f;
					else
						MyBody->force.x += -15.0f;
					MyTransform->SetDir(0);
				}

				if (currentplayerstate->movingState == STATE_IDLE)
				{
					currentplayerstate->IMovingState = IS_ENTER;
					currentplayerstate->movingState = STATE_MOVING;
				}
				else if (currentplayerstate->movingState == STATE_MOVING)
					currentplayerstate->MovingStateTimeCounter++;
			}
			if (MyIM.IsPressed(SDL_SCANCODE_RIGHT))
			{
				if (currentplayerstate->movingState != STATE_ATTACK)
				{
					if (MyBody->vel.x < 0)
						MyBody->force.x += 25.0f;
					else
						MyBody->force.x += 15.0f;
					MyTransform->SetDir(1);
				}

				if (currentplayerstate->movingState == STATE_IDLE)
				{
					currentplayerstate->IMovingState = IS_ENTER;
					currentplayerstate->movingState = STATE_MOVING;
				}
				else if (currentplayerstate->movingState == STATE_MOVING)
					currentplayerstate->MovingStateTimeCounter++;
			}
			if (MyIM.IsTriggered(SDL_SCANCODE_UP))
			{	
				if (currentplayerstate->dogeState == STATE_DOGE)
				{ 
					if (currentplayerstate->movingState != STATE_ATTACK)
					{ 
						if (currentplayerstate->jumpLeft > 0)
						{ 
							MyBody->vel.y = jumpspeed;
							currentplayerstate->jumpLeft--;

							currentplayerstate->IMovingState = IS_ENTER;
							currentplayerstate->movingState = STATE_JUMPING;
						}
					}
				}
			}
			if (MyIM.IsPressed(SDL_SCANCODE_DOWN))
			{
	
			}
			if (MyIM.IsTriggered(SDL_SCANCODE_SPACE))
			{
				if (currentplayerstate->attackDamage > 0)
				{
					if ((currentplayerstate->movingState != STATE_ATTACK) /*&& (currentplayerstate->movingState != STATE_JUMPING)*/)
					{
						if (currentplayerstate->movingState != STATE_JUMPING)
							MyBody->vel.x = 0;

						currentplayerstate->IMovingState = IS_ENTER;
						currentplayerstate->movingState = STATE_ATTACK;
		

						//create effect
						gameobject* attackeffect = GOFactory.loadgameobject("claw.txt");
						GOManager.addGO(attackeffect);
						transform* effecttrans = static_cast<transform*>(attackeffect->getcomponent(T_TRANSFORM));
						if (effecttrans)
						{
							effecttrans->SetScale(0.15, 0.15);
							effecttrans->SetZ(-10.0);
							effecttrans->SetDir(MyTransform->GetDir());
							if (MyTransform->GetDir())
							{
								effecttrans->SetTrans(MyTransform->GetTrans().x + 0.4, MyTransform->GetTrans().y);
							}
							else
								effecttrans->SetTrans(MyTransform->GetTrans().x - 0.4, MyTransform->GetTrans().y);
						}
						attackeffect->init();

					}
				}//currentplayerstate->attackDamage > 0
			}
		}
	}
}

void playercontroller::serialize(FILE ** in)
{

}

