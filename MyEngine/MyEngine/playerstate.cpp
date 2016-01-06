#include "playerstate.h"
#include "GLsprite.h"
#include "gameobject.h"
#include "textmanager.h"
#include "FRC.h"
#include "patrol.h"
#include "transform.h"
#include "projectile.h"
#include "gameobjectfactory.h"
#include "gameobjectmanager.h"
#include "body.h"

extern FrameRateController MyFRC;
extern float gravity;
extern float jumpspeed;
extern textmanager TManager;
extern bool gameover;
extern gameobject* player;
extern gameobjectfactory GOFactory;
extern gameobjectmanager GOManager;
extern bool godmode;

void playerstate::init()
{
	if (this->getOwner())
		thisSprite = static_cast<Glsprite*>(this->getOwner()->getcomponent(T_SPRITE));

	jumpLeft = jumpMax;
	HPLeft = HPMax;
}

void playerstate::update()
{
	if (godmode)
	{
		HPLeft = HPMax;
	}
	if (HPLeft <= 0)
	{
		gameover = true;
	}

	switch (dogeState)
	{
	case STATE_DOGE:
		switch (hurtState)
		{
		case STATE_NORMAL:
			IHurtState = IS_MID;
			break;
		case STATE_IMMUNE:
			countDown(0.5, false,3);
			break;
		default:
			hurtState = STATE_NORMAL;
			break;
		}
		switch (movingState)
		{
		case STATE_IDLE:
			IMovingState = IS_MID;
			break;
		case STATE_JUMPING:
			countDown(jumpspeed / fabs(gravity), true, 2);
			break;
		case STATE_MOVING:
			countDown(0.5,true,2);
			break;
		case STATE_ATTACK:
			countDown(0.3, true, 2);
			break;
		default:
			movingState = STATE_IDLE;
			break;
		}

	case STATE_DOGETANK:
		break;
	default:
		dogeState = STATE_DOGE;
		break;
	}
}

void playerstate::serialize(FILE ** in)
{
	if (*in)
	{
		fscanf_s(*in, "%d %d %d\n", &jumpMax, &HPMax, &attackDamage);
	}
	if (HPMax > 64)
		HPMax = 64;
	if (jumpMax > 16)
		jumpMax = 16;
}

//count down time then reset to default state: resetState 1=doge,2=moving,3=hurt
void playerstate::countDown(float time, bool resetAnim, int resetState/*1=doge,2=moving,3=hurt*/)
{
	INNERState *IState = NULL;
	int *StateTimeCounter = NULL;
	switch (resetState)
	{
	case 1:
		IState = &IMovingState;
		StateTimeCounter = &MovingStateTimeCounter;
		break;
	case 2:
		IState = &IMovingState;
		StateTimeCounter = &MovingStateTimeCounter;
		break;
	case 3:
		IState = &IHurtState;
		StateTimeCounter = &HurtStateTimeCounter;
		break;
	default:
		IState = &IMovingState;
		StateTimeCounter = &MovingStateTimeCounter;
		break;
	}
	switch (*IState)
	{
	case IS_ENTER:
		if (resetAnim)
		thisSprite->init();
		*StateTimeCounter = 1000.0f / MyFRC.GetFrameTime() * time;
		*IState = IS_MID;
		break;
	case IS_MID:
		if (*StateTimeCounter > 0)
		{
			(*StateTimeCounter)--;
		}
		else
		{
			*IState = IS_LEAVING;
		}
		break;
	case IS_LEAVING:
		*IState = IS_ENTER;
		switch (resetState)
		{
		case 1:
			break;
		case 2:
			movingState = STATE_IDLE;
			break;
		case 3:
			hurtState = STATE_NORMAL;
			break;
		default:
			break;
		}
		break;
	default:
		*IState = IS_LEAVING;
		break;
	}
}

void enemystate::init()
{
	if (this->getOwner())
		thisSprite = static_cast<Glsprite*>(this->getOwner()->getcomponent(T_SPRITE));

	HPLeft = HPMax;
}

void enemystate::update()
{
	patrol* thisPatrol = static_cast<patrol*>(getOwner()->getcomponent(T_ENEMYMOVINGACTION));
	Glsprite* thisSprite = static_cast<Glsprite*>(getOwner()->getcomponent(T_SPRITE));
	body* thisBody = static_cast<body*>(getOwner()->getcomponent(T_BODY));
	if (HPLeft <= 0)
	{
		if (EnemyType != lava)
		{
			if (thisSprite)
				thisSprite->ReLoadSprite(4, 13, "birddead.bmp");
			if (thisPatrol)
				thisPatrol->active = false;
			dmg = 0;
			return;
		}
	}

	switch (hurtState)
	{
	case STATE_NORMAL:
		IHurtState = IS_MID;
		break;
	case STATE_IMMUNE:
		countDownEnemy(0.25, false, 3);
		break;
	default:
		hurtState = STATE_NORMAL;
		break;
	}

	transform* playerTrans = static_cast<transform*>(player->getcomponent(T_TRANSFORM));
	transform* thisTrans = static_cast<transform*>(getOwner()->getcomponent(T_TRANSFORM));
	float squareDistance = Vector2DSquareDistance(&(playerTrans->GetTrans()), &(thisTrans->GetTrans()));
	Vector2D distance;
	Vector2D NormalDistance;
	Vector2DSub(&distance, &(playerTrans->GetTrans()), &(thisTrans->GetTrans()));
	Vector2DNormalize(&NormalDistance, &distance);
	Vector2DScale(&distance, &NormalDistance, 1.5);

	switch (EnemyType)
	{
	case bird:
		break;
	case birdthrow:
		if (squareDistance <= 8)
		{
			movingState = STATE_ATTACK;
			if (thisPatrol)
				thisPatrol->active = false;
			if (thisSprite)
			{ 
				thisSprite->ReLoadSprite(4, 11, "birdthrow.bmp");
				thisSprite->setLoopTime(1.0f);
			}
			thisTrans->SetDir((playerTrans->GetTrans().x <= thisTrans->GetTrans().x));

			//throw a ball
			if ((thisSprite->getFrameX() == 3) && (thisSprite->getFrameY() == 3) && (thisSprite->getFrameCount() == 0))
			{
				gameobject* ball = GOFactory.loadgameobject("ball.txt");
				GOManager.addGO(ball);
				transform* effecttrans = static_cast<transform*>(ball->getcomponent(T_TRANSFORM));
				projectile* ballProjectile = static_cast<projectile*>(ball->getcomponent(T_EFFECT));
				ballProjectile->dmg = dmg;
				if (effecttrans)
				{
					effecttrans->SetScale(0.04, 0.04);
					effecttrans->SetZ(-10.0);
					effecttrans->SetDir(thisTrans->GetDir());
					if (thisTrans->GetDir())
					{
						effecttrans->SetTrans(thisTrans->GetTrans().x - 0.2, thisTrans->GetTrans().y + 0.025);
						ballProjectile->vel = distance;
					}
					else
					{ 
						effecttrans->SetTrans(thisTrans->GetTrans().x + 0.2, thisTrans->GetTrans().y + 0.025);
						ballProjectile->vel = distance;
					}
				}
				ball->init();
			}


		}
		else
		{
			movingState = STATE_MOVING;
			if (thisPatrol)
				thisPatrol->active = false;
			if (thisSprite)
			{ 
				thisSprite->ReLoadSprite(4, 8, "birdrest.bmp");
				thisSprite->setLoopTime(0.8f);
			}
		}
		break;
	case birdcharge:
		switch (movingState)
		{
		case STATE_MOVING:
			if ((squareDistance <= 25) && fabs(playerTrans->GetTrans().y - thisTrans->GetTrans().y) < 0.3)
			{
				movingState = STATE_ATTACK;
				IMovingState = IS_ENTER;
				thisSprite->ReLoadSprite(4, 6, "birdcharge.bmp");

				if (playerTrans->GetTrans().x >= thisTrans->GetTrans().x)
				{
					chargeDir = 1;
					thisTrans->SetDir(0);
				}
				else
				{
					chargeDir = 0;
					thisTrans->SetDir(1);
				}
				dmg += 5;
			}
			break;
		case STATE_ATTACK:
			if(countDownEnemy(3.0f, true, 2))
			{ 
				thisSprite->ReLoadSprite(4, 7, "birdtired.bmp");
				thisBody->vel.x = 0.0f;
				dmg -= 15;
			}
			else
			{
				if (chargeDir)
				{
					thisBody->force.x += 2.5f;
				}
				else
					thisBody->force.x -= 2.5f;
			}
			break;
		case STATE_IDLE:
			if (countDownEnemy(2.0f, true, 2))
			{ 
				thisSprite->ReLoadSprite(4, 6, "birdrdy.bmp");
				movingState = STATE_MOVING;
				dmg += 10;
			}
			break;
		default:
			movingState = STATE_IDLE;
			break;
		}
		break;
	case lava:
		break;
	default:
		break;
	}

}

void enemystate::serialize(FILE ** in)
{
	if (*in)
	{
		fscanf_s(*in, "%d %f %d %d", &dmg, &dmgTime, &HPMax,&EnemyType);
	}
}

//count down time then reset to default state: resetState 2=moving,3=hurt
bool enemystate::countDownEnemy(float time, bool resetAnim, int resetState)
{
	INNERState *IState = NULL;
	int* StateTimeCounter = NULL;
	switch (resetState)
	{
	case 2:
		IState = &IMovingState;
		StateTimeCounter = &MovingStateTimeCounter;
		break;
	case 3:
		IState = &IHurtState;
		StateTimeCounter = &HurtStateTimeCounter;
		break;
	default:
		IState = &IMovingState;
		StateTimeCounter = &MovingStateTimeCounter;
		break;
	}
	switch (*IState)
	{
	case IS_ENTER:
		if (resetAnim)
			thisSprite->init();
		*StateTimeCounter = 1000.0f / MyFRC.GetFrameTime() * time;
		*IState = IS_MID;
		break;
	case IS_MID:
		if (*StateTimeCounter > 0)
		{
			(*StateTimeCounter)--;
		}
		else
		{
			*IState = IS_LEAVING;
		}
		break;
	case IS_LEAVING:
		*IState = IS_ENTER;
		switch (resetState)
		{
		case 1:
			break;
		case 2:
			movingState = STATE_IDLE;
			return true;
			break;
		case 3:
			hurtState = STATE_NORMAL;
			break;
		default:
			break;
		}
		break;
	default:
		*IState = IS_LEAVING;
		break;
	}
	return false;
}
