#include"InputManager.h"
#include"SDL_keyboard.h"
#include"SDL_events.h"


InputManager::InputManager()
{
	CurrentKeyBoard = SDL_GetKeyboardState(&KeyboardSize);
	PreviousKeyBoard = new Uint8[KeyboardSize];
}

InputManager::~InputManager()
{
	delete PreviousKeyBoard;
}
void InputManager::RefreshKeyBoard()
{
	memcpy(PreviousKeyBoard, CurrentKeyBoard, KeyboardSize);
	SDL_PumpEvents();
	CurrentKeyBoard = SDL_GetKeyboardState(NULL);
}
bool InputManager::IsPressed(int key)
{
	if (CurrentKeyBoard[key])
		return true;
	else
		return false;
}
bool InputManager::IsTriggered(int key)
{
	if ((CurrentKeyBoard[key]) && (!PreviousKeyBoard[key]))
		return true;
	else
		return false;
}
bool InputManager::IsReleased(int key)
{
	if (!(CurrentKeyBoard[key]) && (PreviousKeyBoard[key]))
		return true;
	else
		return false;
}

