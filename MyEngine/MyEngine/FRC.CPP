#include"FRC.h"
#include"SDL_timer.h"

FrameRateController::FrameRateController(int Frame)
{
	ExpectedFrameTime = 1000 / Frame;
	FrameTime = 10;
	TickStart = 0;
	TickEnd = 0;
}
FrameRateController::~FrameRateController()
{
}

Uint32 FrameRateController::GetFrameTime()
{
	return FrameTime;
}
void FrameRateController::FrameStart()
{
	TickStart = SDL_GetTicks();
}
void FrameRateController::FrameEnd()
{
	TickEnd = SDL_GetTicks();
	while ((TickEnd - TickStart) < ExpectedFrameTime)
	{
		TickEnd = SDL_GetTicks();
	}
	FrameTime = TickEnd - TickStart;
}