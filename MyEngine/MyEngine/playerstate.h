#ifndef PLAYERSTATE
#define PLAYERSTATE

#include"Component.h"
#include<stdio.h>

class Glsprite;

enum DOGEState {
	STATE_DOGE,
	STATE_DOGETANK
};

enum MOVINGState {
	STATE_JUMPING,
	STATE_IDLE,
	STATE_MOVING,
	STATE_ATTACK
};

enum HURTState {
	STATE_NORMAL,
	STATE_IMMUNE
};

enum INNERState {
	IS_ENTER,
	IS_MID,
	IS_LEAVING
};

enum ENEMY {
	bird,
	birdthrow,
	birdcharge,
	lava
};

//should have an intermediate level of inheritance called state, so the 2 classes below could inherit that
//but no time to make this change
class playerstate : public component {
public:
	playerstate() :component(T_GAMESTATE) {};
	virtual void init();
	virtual void update();
	void serialize(FILE **in);
	//resetState 1=doge,2=moving,3=hurt
	void countDown(float time,bool resetAnim,int resetState/*1=doge,2=moving,3=hurt*/);

	int jumpMax;
	int jumpLeft;
	int HPMax;
	int HPLeft;
	int attackDamage;
	DOGEState dogeState = STATE_DOGE;
	MOVINGState movingState = STATE_IDLE;
	HURTState hurtState = STATE_NORMAL;
	INNERState IMovingState = IS_ENTER;
	INNERState IHurtState = IS_ENTER;
	int MovingStateTimeCounter;
	int HurtStateTimeCounter;

	Glsprite* thisSprite;


};

class enemystate: public component {
public:
	enemystate() :component(T_GAMESTATE) {};
	virtual void init();
	virtual void update();
	void serialize(FILE **in);
	//resetState 1=doge,2=moving,3=hurt
	bool countDownEnemy(float time, bool resetAnim, int resetState/*1=doge,2=moving,3=hurt*/);

	int HPMax;
	int HPLeft;
	int dmg;
	float dmgTime;
	int chargeDir;
	MOVINGState movingState = STATE_MOVING;
	HURTState hurtState = STATE_NORMAL;
	INNERState IMovingState = IS_ENTER;
	INNERState IHurtState = IS_ENTER;
	ENEMY EnemyType = bird;
	int MovingStateTimeCounter;
	int HurtStateTimeCounter;

	Glsprite* thisSprite;
};

#endif
