#include "message.h"
#include "gameobjectmanager.h"

extern gameobjectmanager GOManager;

void message::broadcast()
{
	GOManager.broadcastMessage(this);
}
