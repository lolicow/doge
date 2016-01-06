#include "camera.h"
#include"glew.h"
#include "shader.h"
#include "Vector2D.h"
#include "transform.h"
#include "gameobject.h"

extern shader basic;
extern shader debugdraw;
extern gameobject* player;
extern int chaos;

void camera::loadcamera()
{

	Vector2D cameracoordvec2 = static_cast<transform*>(this->getOwner()->getcomponent(T_TRANSFORM))->GetTrans();
	float cameracoord[2] = { cameracoordvec2.x ,cameracoordvec2.y };
	
	glUseProgram(basic.getProgram());
	int loc = glGetUniformLocation(basic.getProgram(), "cameratransform");
	glUniform2fv(loc, 1, &cameracoord[0]);
	glUseProgram(0);

	glUseProgram(debugdraw.getProgram());
	loc = glGetUniformLocation(debugdraw.getProgram(), "cameratransform");
	glUniform2fv(loc, 1, &cameracoord[0]);
	glUseProgram(0);
}

void camera::update()
{
	if (this->getOwner())
	{
		if (this->getOwner()->getcomponent(T_TRANSFORM))
		{
			transform* cameratrans = static_cast<transform*>(this->getOwner()->getcomponent(T_TRANSFORM));
			transform* playertrans = static_cast<transform*>(player->getcomponent(T_TRANSFORM));
			float randx = 0.0f;
			float randy = 0.0f;
			if (chaos)
			{ 
			randx = ((rand() % 100) - 50) / 4000.0f;
			randy = ((rand() % 100) - 50) / 4000.0f;
			cameratrans->SetTrans(playertrans->GetTrans().x * 0.2 + cameratrans->GetTrans().x * 0.8 + randx, randy + playertrans->GetTrans().y * 0.2 + 0.5 * 0.2 + cameratrans->GetTrans().y * 0.8);
			}
			else
			//if 	((fabs(cameratrans->GetTrans().x - playertrans->GetTrans().x) > 0.01) || (fabs(cameratrans->GetTrans().y - playertrans->GetTrans().y -1.0) > 0.01) )
			cameratrans->SetTrans(playertrans->GetTrans().x * 0.2 + cameratrans->GetTrans().x * 0.8 + randx, randy + playertrans->GetTrans().y * 0.2 + 1.0 * 0.2 + cameratrans->GetTrans().y * 0.8);
			if (cameratrans->GetTrans().y < -15)
			{
				cameratrans->SetTrans(cameratrans->GetTrans().x, -15);
			}
		}
	}
	loadcamera();
}
