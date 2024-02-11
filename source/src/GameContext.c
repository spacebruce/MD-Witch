#include "GameContext.h"

struct GameContextType GameContext;
ObjectCamera GameCamera;	// Create a camera

void GameContextInit()
{
	GameContext.Freecam = false;
	
	#if (DEBUG_MODE == true)
		SYS_showFrameLoad(false);
	#endif

	GameContext.Frame = 0;
	GameContext.Camera = &GameCamera;

	//
	GameContext.CurrentStageID = 0xFF;

	InitHitboxGroup(&GameContext.HurtsPlayer, 10);
	InitHitboxGroup(&GameContext.HurtsEnemy, 10);

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