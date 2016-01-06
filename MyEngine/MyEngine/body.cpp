#include "body.h"
#include "gameobject.h"
#include "transform.h"
#include "collisionmanager.h"
#include "collisionmessage.h"
#include "textmessage.h"
#include <math.h>
#include"glew.h"
#include"shader.h"
#include"playerstate.h"
#include"chaos.h"
#include"projectile.h"
#include"enhancement.h"
#include"textmanager.h"
#include<string>
#include"GLsprite.h"

#include "gameobjectfactory.h"
#include "gameobjectmanager.h"

extern gameobjectfactory GOFactory;
extern gameobjectmanager GOManager;

extern float maxspeed;
extern float playermaxspeed;
extern shader debugdraw;
extern float gravity;
extern gameobject* player;
extern textmanager TManager;
extern int chaos;
extern bool win;
extern bool godmode;

body::body():component(T_BODY)
{

}

body::~body()
{
	if (bodyShape)
	delete bodyShape;
}

void body::integrate(float gravity, float deltatime)
{
	posPrev = pos;

	force.y += gravity*mass;
	acc.x = force.x * invmass;
	acc.y = force.y * invmass;
	
	vel.x += acc.x * deltatime;
	if (getOwner() == player)
	{ 
	if (vel.x > playermaxspeed)
		vel.x *= 0.80;
	if (vel.x < -playermaxspeed)
		vel.x *= 0.80;
	}
	vel.y += acc.y * deltatime;
	//if (vel.y > playermaxspeed)
	//	vel.y = playermaxspeed;
	//if (vel.y < -playermaxspeed)
	//	vel.y = -playermaxspeed;

	pos.x += deltatime * vel.x;
	pos.y += deltatime * vel.y;

	//reset force
	force.y = 0;
	force.x = 0;

	updateTransform();
}

void body::serialize(FILE ** in)
{
	char shapename[256];
	memset(shapename, 0, 256 * sizeof(char));
	if (*in)
	{ 
		fscanf_s(*in, "%f %d ",&mass,&Btype);
		fscanf_s(*in, "%255s ", shapename, sizeof(shapename));

	
	if (!bodyShape)
	{
		if (strcmp(shapename, "circle") == 0)
		{
			bodyShape = new circle();
			circle* c = dynamic_cast<circle*>(bodyShape);
			if (c)
			{
				fscanf_s(*in, "%f\n", &c->Radius);
			}
			bodyShape->ThisBody = this;
		}
		else if (strcmp(shapename, "rectangle") == 0)
		{
			bodyShape = new rectangle();
			rectangle* r = dynamic_cast<rectangle*>(bodyShape);
			if (r)
			{
				fscanf_s(*in, "%f %f\n", &r->halfwidth,&r->halfheight);
			}
			bodyShape->ThisBody = this;
		}
	}
	}
	if (mass != 0.0f)
		invmass = 1.0f / mass;
	else
		invmass = 0;

}

void body::init()
{
	if (this->getOwner())
	{
		component *trans = this->getOwner()->getcomponent(T_TRANSFORM);
		if (trans)
		{
			thistrans = static_cast<transform*>(trans);
			pos = thistrans->GetTrans();
		}
	}
}

//not using this function
void body::debugdrawing()
{
	glUseProgram(debugdraw.getProgram());
	glColor3f(1.0, 1.0, 1.0);
	glBegin(GL_LINES);
	glVertex3f(0, 0, -10.0);
	glVertex3f(50*(pos.x - posPrev.x), 50 * (pos.y - posPrev.y), -10.0);
	//glVertex3f(force.x, force.y + mass * gravity, -10.0);
	glEnd();
	glUseProgram(0);
}

void body::updateTransform()
{
	if (thistrans)
	{
		thistrans->SetTrans(pos.x, pos.y);
	}

}

void body::handleMessage(message * m)
{
	//if (m->MType == M_DEBUGDRAW)
	//{
	//	debugdrawing();
	//}
	if (m->MType == M_COLLISION)
	{
		collsionmessage* cm = static_cast<collsionmessage*>(m);
		playerstate* currentplayerstate = static_cast<playerstate*>(player->getcomponent(T_GAMESTATE));
		body* currentplayerbody = static_cast<body*>(player->getcomponent(T_BODY));


		body* whatsBeenHit = cm->c->Cbodies[1];
		if (this == cm->c->Cbodies[1])
		{
			whatsBeenHit = cm->c->Cbodies[0];
		}


		if (thistrans)
		{
			//player logic
			if (this->getOwner() == player)
			{
				//reset jumpleft to jumpmax
				if (((this == cm->c->Cbodies[0]) && (cm->c->normal.y > 0)) || ((this == cm->c->Cbodies[1]) && (cm->c->normal.y < 0)))
				{
					if ((cm->c->Cbodies[0]->Btype == rigidbody) && (cm->c->Cbodies[1]->Btype == rigidbody))
					{
						if (fabs(cm->c->lesserpenetration)>0.02)
							currentplayerstate->jumpLeft = currentplayerstate->jumpMax;
					}
				}

				body* whatCollidePlayer = cm->c->Cbodies[1];
				if (this == cm->c->Cbodies[1])
				{
					whatCollidePlayer = cm->c->Cbodies[0];
				}

				if (whatCollidePlayer->getOwner())
				{
					enemystate* enemy = static_cast<enemystate*>(whatCollidePlayer->getOwner()->getcomponent(T_GAMESTATE));
					if (enemy)
					{
						if (enemy->EnemyType != birdthrow)
						{
							if (enemy->dmg > 0)
							{ 
								if (currentplayerstate->hurtState == STATE_NORMAL)
								{
									if (this->pos.x >= whatCollidePlayer->pos.x)
										force.x += 30;
									else
										force.x -= 30;
									currentplayerstate->hurtState = STATE_IMMUNE;
									currentplayerstate->IHurtState = IS_ENTER;
									currentplayerstate->HPLeft -= enemy->dmg;
								}
							}
						}
					}
				}


			}//player logic	 (jump and hit directly by enemies)

			//physics
			if ((mass != 0) && (cm->c->Cbodies[0]->Btype == rigidbody) && (cm->c->Cbodies[1]->Btype == rigidbody))
			{
				float forcex = cm->c->normal.x * pow(fabs(cm->c->penetration), 2) * 20000.0f;
				float forcey = cm->c->normal.y * pow(fabs(cm->c->penetration), 2) * 20000.0f;
				if (forcex > 100)
					forcex = 100;
				else if (forcex < -100)
					forcex = -100;
				if (forcey > 100)
					forcey = 100;
				else if (forcey < -100)
					forcey = -100;
				if (this == cm->c->Cbodies[0])
				{ 
					if (cm->c->Cbodies[1]->mass == 0)
					{
						pos.x += cm->c->normal.x * fabs(cm->c->penetration);
						pos.y += cm->c->normal.y * fabs(cm->c->penetration);
					}
					else
					{ 
						force.x += forcex;
						force.y += forcey;
					}
					//set vel to 0 on the collision direction
					if (cm->c->normal.x * vel.x < 0)
						vel.x = 0;
					if (cm->c->normal.y * vel.y < 0)
						vel.y = 0;
				}
				else
				{
					if (cm->c->Cbodies[0]->mass == 0)
					{
						pos.x -= cm->c->normal.x * fabs(cm->c->penetration);
						pos.y -= cm->c->normal.y * fabs(cm->c->penetration);
					}
					else
					{
						force.x -= forcex;
						force.y -= forcey;
					}

					if (cm->c->normal.x * vel.x > 0)
						vel.x = 0;
					if (cm->c->normal.y * vel.y > 0)
						vel.y = 0;
				}
				//FRICTION

				if (fabs(vel.x) > 0.001)
					force.x -= fabs(vel.x) / vel.x * pow(fabs(vel.x), 0.7) * 1.5f;
				//NO FRICTION FOR Y!
				//if (fabs(vel.y) > 0.001)
				//	force.y -= fabs(vel.y) / vel.y * pow(fabs(vel.y), 0.4) * 1.5f;
			}//physics

		if (Btype == effectbody)//this gameobject is effect or enhancement(has no real body)
		{
			effect* thiseffect = static_cast<effect*>(getOwner()->getcomponent(T_EFFECT));
			if (thiseffect)
			{ 
				//player attack effect
				if (thiseffect->getFside() == playerSide)
				{

					if (whatsBeenHit->getOwner() != player)
					{
						enemystate* enemy = static_cast<enemystate*>(whatsBeenHit->getOwner()->getcomponent(T_GAMESTATE));
						Glsprite* enemysprite = static_cast<Glsprite*>(whatsBeenHit->getOwner()->getcomponent(T_SPRITE));
						if (enemy)
						{
							if (enemy->hurtState == STATE_NORMAL)
							{
								if (currentplayerbody->pos.x >= whatsBeenHit->pos.x)
									whatsBeenHit->force.x -= 50;
								else
									whatsBeenHit->force.x += 50;
								enemy->hurtState = STATE_IMMUNE;
								enemy->IHurtState = IS_ENTER;
								enemy->HPLeft -= currentplayerstate->attackDamage;

								enemysprite->init();
								//thiseffect->getOwner()->toBeDeleted = true;
							}
						}
					}
				}//player attackeffect
				else
				{
					if (whatsBeenHit->getOwner() == player)
					{
						effect* enemyeffect = static_cast<effect*>(getOwner()->getcomponent(T_EFFECT));
						if (enemyeffect)
						{
							if (enemyeffect->dmg > 0)
							{
								if (currentplayerstate->hurtState == STATE_NORMAL)
								{
									if (this->pos.x >= currentplayerbody->pos.x)
										currentplayerbody->force.x -= 50;
									else
										currentplayerbody->force.x += 50;
									currentplayerstate->hurtState = STATE_IMMUNE;
									currentplayerstate->IHurtState = IS_ENTER;
									currentplayerstate->HPLeft -= enemyeffect->dmg;
								}
							}
						}
					}
				}
			}//effect

			enhancement*  thisEnhance = static_cast<enhancement*>(getOwner()->getcomponent(T_ENHANCEMENT));
			if (thisEnhance)
			{
				body* whatCollideEnhance = cm->c->Cbodies[1];
				if (this == cm->c->Cbodies[1])
				{
					whatCollideEnhance = cm->c->Cbodies[0];
				}
				if (whatCollideEnhance->getOwner() == player)
				{ 
					textmessage tm;
					switch (thisEnhance->getType())
					{
					case ADDJUMP:		
						currentplayerstate->jumpMax += thisEnhance->IncreasedBy();
						thisEnhance->getOwner()->toBeDeleted = true;
						tm.thistext = "YOU CAN JUMP " + std::to_string(currentplayerstate->jumpMax)+ "TIMES NOW ";
						currentplayerstate->jumpLeft = currentplayerstate->jumpMax;
						tm.setColor(1, 1, 0);
						TManager.handleMessage(&tm);
						break;
					case ADDHP:
						currentplayerstate->HPMax += thisEnhance->IncreasedBy();
						currentplayerstate->HPLeft = currentplayerstate->HPMax;
						thisEnhance->getOwner()->toBeDeleted = true;
						tm.thistext = "MAX HP IS INCREASED TO " + std::to_string(currentplayerstate->HPMax);
						tm.setColor(0, 1, 0);
						TManager.handleMessage(&tm);
						break;
					case ADDATTACK:
						currentplayerstate->attackDamage += thisEnhance->IncreasedBy();
						thisEnhance->getOwner()->toBeDeleted = true;
						tm.thistext = "PRESS SPACE TO ATTACK! DAMAGE:" + std::to_string(currentplayerstate->attackDamage);
						tm.setColor(1, 0, 0);
						TManager.handleMessage(&tm);
						break;
					case RESETJUMP:
						currentplayerstate->jumpLeft = currentplayerstate->jumpMax;
						break;
					case RESETHP:
						currentplayerstate->HPLeft = currentplayerstate->HPMax;
						break;
					case ENTERCHAOS:
						chaos = 1;
						currentplayerbody->pos.x = -18;
						currentplayerbody->pos.y = 11;
						tm.thistext = "There's little time!RUN!!!!!!!!!!";
						tm.setColor(0, 0, 0);
						TManager.handleMessage(&tm);
						break;
					case WIN:
						win = true;
						godmode = true;
						break;
					default:
						break;
					}	
				}
			}
	
		}//effect ,this gameobject is effect or enhancement(has no real body)

		if (Btype == projectilebody)//projectile
		{
			projectile* thisprojectile = static_cast<projectile*>(getOwner()->getcomponent(T_EFFECT));
			if (thisprojectile)
			{
				if (thisprojectile->getFside() == playerSide)
				{

				}
				else
				{
					if (whatsBeenHit->getOwner())
					{
						gameobject* Hiteffect = GOFactory.loadgameobject(thisprojectile->HitEffect);
						GOManager.addGO(Hiteffect);
						transform* effecttrans = static_cast<transform*>(Hiteffect->getcomponent(T_TRANSFORM));
						effect* Heffect = static_cast<effect*>(Hiteffect->getcomponent(T_EFFECT));
						Heffect->dmg = thisprojectile->dmg;
						if (effecttrans)
						{
							effecttrans->SetScale(0.06, 0.06);
							effecttrans->SetZ(-10.0);
							effecttrans->SetDir(thistrans->GetDir());

							effecttrans->SetTrans(thistrans->GetTrans().x, thistrans->GetTrans().y);
						}
						Hiteffect->init();

						getOwner()->toBeDeleted = true;
					}
				}
			}
		}//projectile


		}// this trans
		
	}

}

