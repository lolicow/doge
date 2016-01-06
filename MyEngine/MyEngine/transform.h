#ifndef TRANSFORM
#define TRANSFORM

#include"SDL_rect.h"
#include"Component.h"
#include<stdio.h>
#include"Vector2D.h"


class transform:public component
{
public:
	transform();
	~transform();
	virtual void update();
	void MoveTrans(float x,float y);
	void SetTrans(float x, float y) {
		trans.x = x;
		trans.y = y;
	};
	void SetScale(float x, float y) {
		scale.x = x;
		scale.y = y;
	};
	void SetRotate(float x) {
		rotate = x;
	};
	void SetZ(float x) {
		z = x;
	};
	void SetDir(float x) {
		direction = x;
	};
	Vector2D GetTrans() { return trans; };
	Vector2D GetScale() { return scale; };
	float GetRotate() { return rotate; };
	float GetZ() { return z; };
	float GetDir() { return direction; };
	void serialize(FILE **in);

private:
	Vector2D trans;
	Vector2D scale;
	float rotate;
	float z;
	float direction = 0;
};

#endif