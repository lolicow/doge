#ifndef GLSPRITE
#define GLSPRITE

#include "Component.h"
#include<stdio.h>
#include"texture.h"

class Matrix2D;


class Glsprite :public component
{
public:
	Glsprite();
	virtual void update();
	void updateframe();
	void serialize(FILE ** in);
	float GetTextureDir();
	float GettransformZ();
	Matrix2D transform2D(unsigned int shaderInt);
	void render(bool UI);
	void draw(unsigned int shaderInt);
	void setUI() { isUI = !isUI; };
	bool checkUI() { return isUI; };
	void updateplayersprite();
	void init() { framex = 1;framey = 1;framecount = 0; };
	void ReLoadSprite(int framep, int maxf, const char * name);
	int getFrameX(){ return framex; };
	int getFrameY(){ return framey; };
	int getFrameCount() { return framecount; };
	void setLoopTime(float f) { looptime = f; };
	void setFramex(int x) { framex = x; };
	void setFramey(int y) { framey = y; };
private:
	int framex;
	int framey;
	int framecount = 0;
	texture* spritetexture;
	bool isUI = false;
	int blink = 0;
	float looptime;
};

#endif
