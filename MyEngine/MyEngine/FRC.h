#ifndef FRAMERATECONTROLLER
#define FRAMERATECONTROLLER

#include"SDL.h"

class FrameRateController {
public:
	FrameRateController(int Frame);
	~FrameRateController();
	Uint32 GetFrameTime();
	void FrameStart();
	void FrameEnd();
private:
	Uint32 TickStart;
	Uint32 TickEnd;
	Uint32 FrameTime;
	Uint32 ExpectedFrameTime;
};


#endif