#define _CRT_SECURE_NO_WARNINGS

#include "GLsprite.h"
#include <fstream>
#include "shader.h"
#include"gameobject.h"
#include"transform.h"
#include"playerstate.h"

#include"glew.h"
#include"glut.h"

#include"Matrix2D.h"
#include"texture.h"
#include"ResourceManager.h"
#include"collisionmanager.h"
#include"message.h"
#include"body.h"
#include"textmanager.h"
#include"mesh.h"
#include"FRC.h"


extern shader basic;
extern shader debugdraw;
extern shader UIshader;
extern bool debugmode;
extern ResourceManager MyRM;
extern textmanager TManager;
extern int width;
extern int height;
extern mesh squaremesh;
extern FrameRateController MyFRC;
extern gameobject* player;
extern bool pause;

Glsprite::Glsprite() : component(T_SPRITE)
{
	framex = 1;
	framey = 1;
}

void Glsprite::update()
{
	if (this->getOwner() == player)
	{
		playerstate* currentplayerstate = static_cast<playerstate*>(player->getcomponent(T_GAMESTATE));
		updateplayersprite();
		if (currentplayerstate->hurtState == STATE_IMMUNE)
			blink = (blink + 1) % 2;
		else
			blink = 0;
	}
	else
	updateframe();
}

void Glsprite::updateframe()
{
	int maxframe = 1;
	int frameper = 1;
	if (spritetexture != NULL)
	{
		maxframe = spritetexture->gerMaxFrame();
		frameper = spritetexture->getFramePer();
	}
	if (framecount < 1000.0f/MyFRC.GetFrameTime() * looptime / maxframe)
	{
		framecount++;
		return;
	}
	framex += 1;
	if (framex > frameper)
	{
		framey += 1;
		framex -= frameper;
	}
	if (framex + frameper * (framey - 1) > maxframe)
	{
		framex = 1;
		framey = 1;
	}

	//printf("x:%d,y:%d,max:%d\n", framex, framey,maxframe);
	framecount = 0;

}

void Glsprite::serialize(FILE ** in)
{
	char imagename[256];
	memset(imagename, 0, 256 * sizeof(char));

	int a = 0;
	int b = 0;
	if (*in)
	{ 
		fscanf_s(*in, "%d %d %f %d ", &a, &b,&looptime,&isUI);
		fscanf_s(*in, "%255s", imagename, sizeof(imagename));
	}
	std::string fullpath = "";
	fullpath += imagename;

	spritetexture = MyRM.loadTexture(b, a, fullpath.c_str());

}

float Glsprite::GetTextureDir()
{
	gameobject* owner = this->getOwner();
	if (owner)
	{
		if (owner->getcomponent(T_TRANSFORM)) {
			transform* thistransform = static_cast<transform*>(owner->getcomponent(T_TRANSFORM));
			return 	thistransform->GetDir();
		}
	}
	return 0;
}

float Glsprite::GettransformZ()
{
	gameobject* owner = this->getOwner();
	if (owner)
	{
		if (owner->getcomponent(T_TRANSFORM)) {
			transform* thistransform = static_cast<transform*>(owner->getcomponent(T_TRANSFORM));
			return 	thistransform->GetZ();
		}
	}
	return 0;
}




Matrix2D Glsprite::transform2D(unsigned int shaderInt)
{
	Matrix2D trans;
	Matrix2D t;
	Matrix2D s;
	Matrix2D r;

	gameobject* owner = this->getOwner();
	if (owner)
	{ 
		Vector2D transvec;
		Vector2D scale;
		float rotate;
		if 	(owner->getcomponent(T_TRANSFORM)){
			transform* thistransform = static_cast<transform*>(owner->getcomponent(T_TRANSFORM));
			
			transvec = thistransform->GetTrans();
			scale = thistransform->GetScale();
			if(shaderInt == debugdraw.getProgram())
			{
			body* thisbody = static_cast<body*>(owner->getcomponent(T_BODY));
				if (thisbody)
				{ 
					rectangle* thisrectangle = static_cast<rectangle*>(thisbody->bodyShape);
					if (thisrectangle)
					{
						scale = { thisrectangle->halfwidth,thisrectangle->halfheight };
					}
				}
			}
			rotate = thistransform->GetRotate();
		}
		Matrix2DTranslate(&t, transvec.x, transvec.y);
		Matrix2DScale(&s, scale.x, scale.y);
		Matrix2DRotDeg(&r, rotate);

		Matrix2DConcat(&trans, &r, &s);
		Matrix2DConcat(&trans, &t, &trans);

	}
	return trans;
}

void Glsprite::render(bool UI)
{

	// Enable depth test
	//glEnable(GL_DEPTH_TEST);
	////// Accept fragment if it closer to the camera than the former one
	//glDepthFunc(GL_ALWAYS);
	if (!UI)
	{
		if (!blink)
		{ 
		//enable alpha blend
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		
		//draw(debugdraw.getProgram());
		draw(basic.getProgram());
		}
	}
	else
	{ 
		if (!blink)
		{
			//enable alpha blend
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			//draw(debugdraw.getProgram());
			draw(UIshader.getProgram());
		}
	}
	if (debugmode)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		draw(debugdraw.getProgram());
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		gameobject* owner = this->getOwner();
		if (owner)
		{
			component* thisbody = owner->getcomponent(T_BODY);
			if (thisbody)
			{
				body* mybody = static_cast<body*>(thisbody);
				//message debugmessage(M_DEBUGDRAW);
				//thisbody->handleMessage(&debugmessage);
				glUseProgram(debugdraw.getProgram());
				glColor3f(1.0, 1.0, 1.0);
				glBegin(GL_LINES);
				glVertex3f(0, 0, -10.0);
				glVertex3f(50 * (mybody->pos.x - mybody->posPrev.x), 50 * (mybody->pos.y - mybody->posPrev.y), -10.0);
				//glVertex3f(mybody->force.x, mybody->force.y + mybody->mass * gravity, -10.0);
				glEnd();
				glUseProgram(0);
			}
		}
	}
	
}

void Glsprite::draw(unsigned int shaderInt)
{
	Matrix2D trans = transform2D(shaderInt);

	glUseProgram(shaderInt);

	int program = shaderInt;
	int loc;
	if (spritetexture != NULL)
	{
		glActiveTexture(GL_TEXTURE0 + 1);
		glBindTexture(GL_TEXTURE_2D, spritetexture->getTextureID());
		//glBindTexture(GL_TEXTURE_2D, TManager.gettex());
		loc = glGetUniformLocation(program, "Texture");
		glUniform1i(loc, 1);
	}
	float dir = GetTextureDir();
	loc = glGetUniformLocation(program, "direction");
	glUniform1f(loc, dir);

	float Z = GettransformZ();
	loc = glGetUniformLocation(program, "z");
	glUniform1f(loc, Z);

	float w = width;
	float h = height;
	loc = glGetUniformLocation(program, "width");
	glUniform1f(loc, w);

	loc = glGetUniformLocation(program, "height");
	glUniform1f(loc, h);

	loc = glGetUniformLocation(program, "transform");
	glUniformMatrix3fv(loc, 1, GL_TRUE, &trans.m[0][0]);

	float frameunit = 1.0f / spritetexture->getFramePer();
	loc = glGetUniformLocation(program, "frameunit");
	glUniform1f(loc, frameunit);

	float framexf = (framex - 1) * frameunit;
	float frameyf = (framey - 1) * frameunit;
	loc = glGetUniformLocation(program, "framex");
	glUniform1f(loc, framexf);
	loc = glGetUniformLocation(program, "framey");
	glUniform1f(loc, frameyf);

	//paused
	loc = glGetUniformLocation(program, "pause");
	glUniform1i(loc, pause);

	//bind vertex array object
	glBindVertexArray(squaremesh.getVao());
	// Draw the triangle !
	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glDrawArrays(GL_TRIANGLES, 0, 6); // Starting from vertex 0; 6 vertices total -> 1 triangle

	glBindVertexArray(0);
	glActiveTexture(GL_TEXTURE0 + 1);
	glBindTexture(GL_TEXTURE_2D, 0);

	glUseProgram(0);
	//draw lines and vertex
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	//glDrawArrays(GL_TRIANGLES, 0, 6);
}

//hard coding game logic :(  
void Glsprite::updateplayersprite()
{
	playerstate* currentplayerstate = static_cast<playerstate*>(player->getcomponent(T_GAMESTATE));
	if (currentplayerstate->dogeState == STATE_DOGE)
	{
		switch (currentplayerstate->movingState)
		{
		case STATE_IDLE:
			//if (currentplayerstate->IState == IS_ENTER)
			//init();
			spritetexture = MyRM.loadTexture(3, 9, "dogerun.bmp");
			updateframe();
			break;
		case STATE_MOVING:
			spritetexture = MyRM.loadTexture(3, 9, "dogerun.bmp");
			updateframe();
			break;
		case STATE_JUMPING:
			spritetexture = MyRM.loadTexture(3, 6, "dogejump.bmp");
			updateframe();
			break;
		case STATE_ATTACK:
			spritetexture = MyRM.loadTexture(4, 12, "dogeattack.bmp");
			updateframe();
			break;
		default:
			break;
		}
	}
}

void Glsprite::ReLoadSprite(int framep, int maxf, const char * name)
{
	if (name == "")
	{
		blink = true;
	}
	else
	{ 
	spritetexture = MyRM.loadTexture(framep, maxf, name);
	blink = false;
	}
}
