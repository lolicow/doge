#include<cstdio>
#include<SDL.h>
#include<string>
#include<sstream>
#include"ResourceManager.h"
#include"InputManager.h"
#include"FRC.h"
#include"gameobject.h"
#include"gameobjectmanager.h"
#include"sprite.h"
#include"transform.h"
#include"controller.h"
#include"chaos.h"
#include"gameobjectfactory.h"
#include"GLsprite.h"
#include"glew.h"
#include"shader.h"
#include"PhysicsManager.h"
#include"graphicmanager.h"
#include"uimanager.h"
#include"textmanager.h"
#include"mesh.h"
#include"body.h"


//globals
ResourceManager MyRM;
InputManager MyIM;
int framer = 60;
FrameRateController MyFRC(framer);
SDL_Window* pWindow;
SDL_Surface* WindowSurface;
gameobjectmanager GOManager;
PhysicsManager PHYManager;
graphicmanager GRAPHManager;
UImanager UIManager;
textmanager TManager;
gameobjectfactory GOFactory;
gameobject* player;
shader basic;
shader UIshader;
shader Tshader;
shader debugdraw;
bool debugmode = false;
bool pause = false;
bool gameover = false;
bool godmode = false;
bool win = false;
int chaos = 0;
int width = 1600;
int height = 900;
float gravity = -3.0;
float maxspeed = 10.0;
float playermaxspeed = 1.25;
float jumpspeed = 2.1f;
mesh squaremesh;


int main(int argc, char *argv[])
{
	bool Terminated = false;
	SDL_Init(SDL_INIT_EVERYTHING);
	pWindow = SDL_CreateWindow("Doge's Adventure", 0, 0, width, height, SDL_WINDOW_OPENGL | SDL_WINDOW_FULLSCREEN);
	
	if (pWindow == NULL) {
		// In the case that the window could not be made...
		printf("Could not create window: %s\n", SDL_GetError());
		return 1;
	}
	SDL_GLContext OpenGLWindow = SDL_GL_CreateContext(pWindow);

	MyRM.InitResourceManager();
	MyIM.RefreshKeyBoard();
	//using sdlsurface to render the window
	//WindowSurface = SDL_GetWindowSurface(pWindow);
	glutInitDisplayMode(GLUT_RGBA);
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		getchar();
		return -1;
	}


	//shader part
	squaremesh.initsquare();
	glLineWidth(2.0);
	basic.LoadShaders("2DBASIC.VERT", "2DBASIC.FRAG");
	debugdraw.LoadShaders("2DDEBUG.VERT", "2DDEBUG.FRAG");
	UIshader.LoadShaders("2DUI.VERT", "2DUI.FRAG");
	Tshader.LoadShaders("2DTEXT.VERT", "2DTEXT.FRAG");

	//load level
	GOFactory.LoadLevel("level1.csv");
	printf("Load Level1\n\n");
	UIManager.init();
	GOManager.init();
	PHYManager.init();

	//load font
	TManager.init("arialbi.ttf");
	printf("Load arialbi.ttf\n\n");

	while (!Terminated)
	{
		SDL_Event event;
		MyFRC.FrameStart();
		MyIM.RefreshKeyBoard();

		if (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
				Terminated = true;
		}

		//SDL_FillRect(WindowSurface, NULL, 0x000000);
		if (MyIM.IsTriggered(SDL_SCANCODE_R))
		{
			gameover = false;
			pause = false;
			godmode = false;
			chaos = 0;
			win = false;

			GOFactory.unLoadLevel();
 			GOFactory.LoadLevel("level1.csv");
			UIManager.init();
			GOManager.init();
			PHYManager.init();
			TManager.initHelp();
		}
		if (MyIM.IsTriggered(SDL_SCANCODE_S))
		{
			gameover = false;
			pause = false;
			godmode = false;
			chaos = 0;

			playerstate* currentPlayerState = static_cast<playerstate*>(player->getcomponent(T_GAMESTATE));
			currentPlayerState->HPLeft = currentPlayerState->HPMax;

			//static_cast<transform*>(player->getcomponent(T_TRANSFORM))->SetTrans(0.0, -0.6);
			static_cast<body*>(player->getcomponent(T_BODY))->pos.x = 0.1;
			static_cast<body*>(player->getcomponent(T_BODY))->pos.y = -1.3;
		}

		if (MyIM.IsTriggered(SDL_SCANCODE_X))
		{
			debugmode = (!debugmode);
		}
		if (MyIM.IsTriggered(SDL_SCANCODE_P))
		{
			pause = (!pause);
		}
		//if (MyIM.IsTriggered(SDL_SCANCODE_C))
		//{
		//	chaos = (!chaos);
		//}
		if (MyIM.IsTriggered(SDL_SCANCODE_G))
		{
			godmode = (!godmode);
			//win = (!win);
		}

		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		TManager.renderAllUIText();
		if (!pause && !gameover)
		{
			if (MyFRC.GetFrameTime() <= 50.0f && !win)
			PHYManager.update(MyFRC.GetFrameTime() / 1000.0f);
			GOManager.update();
			UIManager.update();
		}
		else
		{
			
		}
		GRAPHManager.render();

		TManager.renderAllUIText();

		//render the fps on top right
		int framerate = 0;
		if (MyFRC.GetFrameTime() != 0)
		{
			framerate = 1000 / MyFRC.GetFrameTime();
	
			std::string stringframerate;
			std::stringstream ss;
			ss << framerate;

			float white[3] = { 1,1,1 };
			stringframerate = ss.str() + "fps";
			const char* ccframerate = stringframerate.c_str();
			TManager.rendertext(ccframerate, width - 100, 850, 0.5, 0.5,white, 1.0);
		}
		
		PHYManager.frameclear();
		GOManager.frameclear();
		GRAPHManager.frameclear();
		SDL_GL_SwapWindow(pWindow);
		/*SDL_UpdateWindowSurface(pWindow);*/
		
		MyFRC.FrameEnd();
		//printf("frame time is %d\n", MyFRC.GetFrameTime());		
	}
	
	SDL_DestroyWindow(pWindow);
	SDL_Quit();
	return 0;
}