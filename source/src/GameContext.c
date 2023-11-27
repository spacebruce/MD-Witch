#include "GameContext.h"

struct GameContextType GameContext;

void GameContextInit()
{
	#if (DEBUG_MODE == true)
		SYS_showFrameLoad(false);
	#endif

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