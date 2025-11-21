#include "Common.h"
#include "Init.h"
#include "MapControl.h"
#include "GameState.h"
#include "Event.h"

#undef main


int main()
{
	if (LogInit() != 0)
	{
		SDL_Log("Open log file failed\n");
		return -1;
	}

	if (WindowRdrInit())
	{
		return -1;
	}
	if (FontInit())
	{
		return -1;
	}
	RectInit();
	GameStateInit();
	RandInit();
	
	EnterMainEventLoop();

	DeInit();
	SDL_Log("Enter y to leave");
	char c;
	while (1)
	{
		c = getchar();
		if (c == 'y')
		{
			break;
		}
	}
	return 0;
}