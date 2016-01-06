#ifndef UIMANAGER
#define UIMANAGER

class gameobject;
class playerstate;

class UImanager {
public:
	UImanager();
	~UImanager();
	void init();
	void update();


	gameobject* portrait;
	gameobject* hpBar[64];
	gameobject* jumpBar[16];

	gameobject* uilava[4];
	gameobject* uilavabase;
	gameobject* gratz;

	playerstate* Pstate;
	int chaoscount = 0;
};

void setTransform(gameobject* GO, float scalex, float scaley, float posx, float posy, float dir);
#endif