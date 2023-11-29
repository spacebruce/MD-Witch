#include "GameContext.h"

struct GameContextType GameContext;

void GameContextInit()
{
	#if (DEBUG_MODE == true)
		SYS_showFrameLoad(false);
	#endif

	GameContext.Frame = 0;

	if(IS_PAL_SYSTEM)
	{
		GameContext.Framerate = 50;
		GameContext.Speedup = FIX16(1.2);
	}
	else
	{
		GameContext.Framerate = 60;
		GameContext.Speedup = FIX16(1);
	}
}

void GameContextTick()
{
	++GameContext.Frame;
	if(!GameContext.Paused)
	{
		++GameContext.StageFrame;
	}
}