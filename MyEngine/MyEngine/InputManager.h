
#ifndef INPUTMANAGER
#define INPUTMANAGER

#include"SDL_stdinc.h"
#include"string.h"

class InputManager {
public:
	InputManager();
	~InputManager();
	void RefreshKeyBoard();
	bool IsPressed(int key);
	bool IsTriggered(int key);
	bool IsReleased(int key);
private:
	const Uint8* CurrentKeyBoard;
	Uint8* PreviousKeyBoard;
	int KeyboardSize;
};

#endif
