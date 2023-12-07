#include "GameContext.h"

struct GameContextType GameContext;
ObjectCamera GameCamera;	// Create a camera

void GameContextInit()
{
	#if (DEBUG_MODE == true)
		SYS_showFrameLoad(false);
	#endif

	GameContext.Frame = 0;
	GameContext.Camera = &GameCamera;

	//
	GameContext.CurrentStageID = 0xFF;

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